#include "stdafx.h"
#include "model.h"

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

#include "FlashDialog.h"

size_t curlWXStringWriter(void *ptr, size_t size, size_t nmemb, wxString* buf) {
	buf->Append(wxString::FromUTF8((char*) ptr, size * nmemb));
	return size * nmemb;
}

using namespace rapidjson;

class CustomDialog;

class MyProcess: public wxProcess
{
public:
    MyProcess(CustomDialog* parent, const wxString& cmd);

    virtual void OnTerminate(int pid, int status);

protected:
    CustomDialog *m_parent;
    wxString m_cmd;
};

class FlashFinishedEvent: public wxEvent {
public:
	int statusCode;
    FlashFinishedEvent(int status);
	virtual wxEvent* Clone() const;
};

wxDEFINE_EVENT(MY_EXECFINISHED, FlashFinishedEvent);

FlashFinishedEvent::FlashFinishedEvent(int status): wxEvent(0, MY_EXECFINISHED) {
	statusCode = status;
}

wxEvent* FlashFinishedEvent::Clone() const {
	return new FlashFinishedEvent(this->statusCode);
}

class CustomDialog : public FlashDialog
{
public:
CustomDialog(const wxString& title);

public:
	FirmwareReleases releases;

	virtual void onActivateDialog( wxActivateEvent& event ) {
	}

	virtual void onFirmwareChoice( wxCommandEvent& event ) {
		if (firmwareChoice->GetSelection() == 0) {
			progressText->SetLabel("Select firmware");
			flashButton->Disable();
		} else {
			int sel = firmwareChoice->GetSelection();
			if (sel > 0 && sel <= releases.Count()) {
				wxString txt;
				FirmwareRelease r = releases[sel - 1];
				txt.Printf("%s %s\nDownloads: %i\n%s", r.name, r.file, r.downloadCount, r.description);
				progressText->SetLabel(txt);
				flashButton->Enable();
			}
		}
	}

	virtual void onFirmwareRefreshButton( wxCommandEvent& event ) {
		int foundCount = 0;
		progressText->SetLabel("Downloading releases...");
		firmwareRefreshButton->Disable();
		CURL* curl = curl_easy_init();
		if(curl) {
			wxString buf;
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/Magnum3D/MagnumFirmware/releases");
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, "MagnumFlasher-by-huksley");
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWXStringWriter);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
			CURLcode res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);

			releases.Clear();
			Document d;
			d.Parse(buf);
			int cap = d.Capacity();
			buf.Printf("%i", cap);
			for (int i = 0; i < cap; i++) {
				wxString publishDate = d[i]["published_at"].GetString();
				wxString body = d[i]["body"].GetString();
				wxString name = d[i]["name"].GetString();
				
				bool found = false;
				const Value& assets = d[i]["assets"];
				if (!assets.IsNull() && assets.IsArray()) {
					for (int j = 0; j < assets.Size(); j++) {
						const Value& asset = assets[j];
						
						wxString file = asset["name"].GetString();
						wxString fileUrl = asset["browser_download_url"].GetString();
						int downloadCount = asset["download_count"].GetInt();
						if (file.EndsWith(".hex")) {
							FirmwareRelease r;
							r.name = name;
							r.file = file;
							r.description = body;
							r.downloadUrl = fileUrl;
							r.published = publishDate;
							r.downloadCount = downloadCount;
							releases.Add(r);
							foundCount ++;
							break;
						}
					}
				}
			}
			
		}
		
		firmwareChoice->Clear();
		wxString str;
		str.Printf("Found %i firmware releases", foundCount);
		firmwareChoice->Append(str);
		for (int i = 0; i < releases.Count(); i++) {
			wxString title;
			title.Printf("%s %s", releases[i].name, releases[i].file);
			firmwareChoice->Append(title);
		}
		firmwareChoice->SetSelection(0);
		progressText->SetLabel("Select firmware");
		firmwareRefreshButton->Enable();
	}

	void executeFlash() {
		if (running != NULL) {
			return;
		}
		progressBar->SetValue(0);
		wxString cmd = "hid_bootloader_cli_upgraded -mmcu=at90usb1286 -w -v Magnum-PRO-B03-F4g.hex";
		MyProcess * const process = new MyProcess(this, cmd);
		process->Redirect();
		running = process;
		long pid = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_SHOW_CONSOLE, process);
		if (!pid) {
			progressText->SetLabel(wxString::Format("Execution of '%s' failed.", cmd));
			wxLogError(wxT("Execution of '%s' failed."), cmd.c_str());
			delete process;
		} else {
			progressText->SetLabel(wxString::Format("Process launched (%ld): '%s'", pid, cmd));
			wxLogStatus(wxT("Process %ld (%s) launched."), pid, cmd.c_str());
		}
	}

	void executeFinished(FlashFinishedEvent& event) {
		progressText->SetLabel(wxString::Format("Finished with err code: %i", event.statusCode));
		wxMicroSleep(10);
		flashButton->Enable();
		progressBar->SetValue(100);
		running = NULL;
	}

	virtual void onFlashButton(wxCommandEvent& event ) {
		int sel = firmwareChoice->GetSelection();
		if (sel > 0 && sel <= releases.Count()) {
			wxString txt;
			FirmwareRelease r = releases[sel - 1];
			txt.Printf("Download && flash %s???", r.file);
			int answer = wxMessageBox(txt, "Confirm", wxOK | wxCANCEL, this);
			if (answer == wxOK) {
				executeFlash();
				flashButton->Disable();
			}
		}
	}

	void addEvent(wxEvent* ev) {
		QueueEvent(ev);
	}

	virtual void onCloseButton( wxCommandEvent& event ) {
		this->Close();
	}

	void OnTimer(wxTimerEvent& event) {
		if (running != NULL) {
			progressBar->SetValue(progressBar->GetValue() + 1);

			wxString text = progressText->GetLabel();
			wxString orig = text;

			if (running->IsInputAvailable()) {
				wxInputStream& in = *running->GetInputStream();
				while(in.CanRead()) {
					char buffer[4096];
					buffer[in.Read((void*) buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
					text.Append(buffer);
				}
			}

			if (running->IsErrorAvailable()) {
				wxInputStream& in = *running->GetErrorStream();
				while(in.CanRead()) {
					char buffer[4096];
					buffer[in.Read((void*) buffer, WXSIZEOF(buffer) - 1).LastRead()] = '\0';
					text.Append(buffer);
				}
			}

			if (text != orig)
			progressText->SetLabel(text);
		}
	}
private:
    wxTimer m_timer;
	MyProcess* running;
    wxDECLARE_EVENT_TABLE();
};

#define TIMER_ID 1

wxBEGIN_EVENT_TABLE(CustomDialog, wxDialog)
    EVT_TIMER(TIMER_ID, CustomDialog::OnTimer)
wxEND_EVENT_TABLE()


CustomDialog::CustomDialog(const wxString & title): FlashDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230), wxDEFAULT_DIALOG_STYLE), m_timer(this, TIMER_ID) {
	firmwareChoice->SetSelection(0);
	firmwareRefreshButton->Enable();
	flashButton->Disable();
	progressBar->SetRange(100);
	m_timer.Start(1000);
	running = NULL;
	Bind(MY_EXECFINISHED, &CustomDialog::executeFinished, this);
	Centre();
	ShowModal();
	Destroy(); 
}

MyProcess::MyProcess(CustomDialog* parent, const wxString& cmd): wxProcess(parent) {
	m_cmd = cmd;
    m_parent = parent;
}

void MyProcess::OnTerminate(int pid, int status) {
	m_parent->addEvent(new FlashFinishedEvent(status));
}

bool MyApp::OnInit()
{
    CustomDialog *custom = new CustomDialog(wxT("Magnum3D firmware flasher"));
    custom->Show(true);
    return true;
}