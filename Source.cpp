#include "stdafx.h"
#include "model.h"


#include <winsparkle.h>

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
	wxConfig* config;
	wxLocale* locale;
	wxLog* log;
	FILE* logFile;
};

wxIMPLEMENT_APP(MyApp);

#include "FlashDialog.h"

size_t curlMemBlockWriter(void *ptr, size_t size, size_t nmemb, class MemBlock* buf) {
	char* ch = (char*) calloc(1, buf->size + size * nmemb);
	if (buf->ptr != NULL) {
		memcpy(ch, buf->ptr, buf->size);
	}
	memcpy(ch + buf->size, ptr, size * nmemb);
	if (buf->ptr != NULL) {
		free(buf->ptr);
	}
	buf->ptr = ch;
	buf->size += size * nmemb;
	return size * nmemb;
}

using namespace rapidjson;

class CustomDialog;

class MyProcess: public wxProcess
{
public:
    MyProcess(CustomDialog* parent, const wxString& cmd);
	virtual ~MyProcess();

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

/*
static int CompareReleases(const FirmwareRelease** a, const FirmwareRelease** b) {
	return 0;
}
*/

class CustomDialog : public FlashDialog
{
public:
	CustomDialog(MyApp* app, const wxString& title);

public:
	FirmwareReleases releases;
	MyApp* app;

	wxString getProgress() {
		return progressText->GetValue();
	}

	void setProgress(wxString txt) {
		progressText->SetValue(txt);
		progressText->ScrollLines(100);
	}

	virtual void onActivateDialog( wxActivateEvent& event ) {
	}

	virtual void onClose( wxCloseEvent& event ) {
		if (running != NULL) {
			running->Detach();
			wxKill(running->GetPid());
			running = NULL;
		}
		Destroy();
		app->Exit();
	}
	
	virtual void onCheckUpdatesButton(wxCommandEvent& event) {
		win_sparkle_set_automatic_check_for_updates(1);
		win_sparkle_check_update_with_ui();
	}

	virtual void onFirmwareChoice( wxCommandEvent& event ) {
		if (firmwareChoice->GetSelection() == 0) {
			setProgress(_("Select firmware"));
			flashButton->Disable();
		} else {
			int sel = firmwareChoice->GetSelection();
			if (sel > 0 && sel <= releases.Count()) {
				wxString txt;
				FirmwareRelease r = releases[sel - 1];
				//wxDateTime dd;
				//wxString::const_iterator leftOver;
				//   "updated_at": "2015-06-01T20:01:35Z",
				//dd.ParseFormat(r.date, wxString("%Y-%m-%d'T'"), &leftOver);
				txt.Printf(_("%s\n\n[%s] %s\n\n%s"), r.name, r.date, r.file, r.description);
				setProgress(txt);
				flashButton->Enable();
			}
		}
	}

	wxFileName* downloadFile(wxString name, wxString url) {
		MemBlock buf;
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, (const char*) url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, _("github.com/huksley/MagnumFlasher"));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlMemBlockWriter);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (res != CURLE_OK) {
			return NULL;
		}

		wxString temp = wxStandardPaths::Get().GetTempDir();
		wxFileName* fname = new wxFileName(wxString::Format("%s/%s", temp, name));
		wxFile* f = new wxFile(fname->GetFullPath(), wxFile::write);
		f->Write(buf.handoff());
		f->Close();
		delete f;
		return fname;
	}

	virtual void onFirmwareRefreshButton( wxCommandEvent& event ) {
		setProgress(_("Downloading releases..."));
		firmwareRefreshButton->Disable();
		CURL* curl = curl_easy_init();
		if(curl) {
			MemBlock buf;
			char errbuf[CURL_ERROR_SIZE];
			wxString url = app->config->Read("ReleaseURL", _("https://api.github.com/repos/Magnum3D/MagnumFirmware/releases"));
			curl_easy_setopt(curl, CURLOPT_URL, (const char*) url);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);
			curl_easy_setopt(curl, CURLOPT_USERAGENT, _("github.com/huksley/MagnumFlasher"));
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlMemBlockWriter);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
			CURLcode res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			if (res != CURLE_OK) {
				setProgress(wxString::Format(_("Error requesting %s: %i %s"), url, res, wxString(curl_easy_strerror(res))));
				return;
			}

			app->config->Write("ReleaseURL", url);
			parseFirmware(buf.handoff());
		}
		fillFirmware();
	}

	void parseFirmware(wxString buf) {
		releases.Clear();
		Document d;
		wxLogVerbose("JSON: %s", buf);
		const char* json = (const char*)buf.mb_str(wxConvUTF8);
		int len = _mbstrlen(json);
		d.Parse(json);
		if (d.HasParseError()) {
			const char* err = "?";
			switch (d.GetParseError()) {
			case kParseErrorDocumentEmpty: err = "The document is empty."; break;
		    case kParseErrorDocumentRootNotSingular: err = "The document root must not follow by other values."; break;
			case kParseErrorValueInvalid: err = "Invalid value."; break;
			case kParseErrorObjectMissName: err = "Missing a name for object member."; break;
			case kParseErrorObjectMissColon: err = "Missing a colon after a name of object member."; break;
			case kParseErrorObjectMissCommaOrCurlyBracket: err = "Missing a comma or '}' after an object member."; break;
			case kParseErrorArrayMissCommaOrSquareBracket: err = "Missing a comma or ']' after an array element."; break;
			case kParseErrorStringUnicodeEscapeInvalidHex: err = "Incorrect hex digit after \\u escape in string."; break;
			case kParseErrorStringUnicodeSurrogateInvalid: err = "The surrogate pair in string is invalid."; break;
			case kParseErrorStringEscapeInvalid: err = "Invalid escape character in string."; break;
			case kParseErrorStringMissQuotationMark: err = "Missing a closing quotation mark in string."; break;
			case kParseErrorStringInvalidEncoding: err = "Invalid encoding in string."; break;
			case kParseErrorNumberTooBig: err = "Number too big to be stored in double."; break;
			case kParseErrorNumberMissFraction: err = "Miss fraction part in number."; break;
			case kParseErrorNumberMissExponent: err = "Miss exponent in number."; break;
			case kParseErrorTermination: err = "Parsing was terminated."; break;
			case kParseErrorUnspecificSyntaxError: err = "Unspecific syntax error."; break;
			}
			wxLogWarning("Failed to parse JSON, code %i %s pos %i len %i:%i", (int) d.GetParseError(), wxString(err), d.GetErrorOffset(), wxString(json).Length(), len);
			wxLogWarning("JSON %s", json);
			return;
		}
		
		app->config->Write("LatestReleases", buf);
		int cap = d.Capacity();
		wxLogVerbose("Found releases: %i", cap);
		for (int i = 0; i < cap; i++) {
			wxString publishDate = wxString::FromUTF8(d[i]["published_at"].GetString());
			wxString body = wxString::FromUTF8(d[i]["body"].GetString());
			wxString name = wxString::FromUTF8(d[i]["name"].GetString());
			wxLogVerbose("Found release: %i %s", i, name);

			bool found = false;
			const Value& assets = d[i]["assets"];
			if (!assets.IsNull() && assets.IsArray()) {
				for (int j = 0; j < assets.Size(); j++) {
					const Value& asset = assets[j];
						
					wxString file = asset["name"].GetString();
					wxLogVerbose("Found asset: %i %s", i, file);
					wxString fileUrl = asset["browser_download_url"].GetString();
					int downloadCount = asset["download_count"].GetInt();
					wxString fDate = asset["created_at"].GetString();
					if (asset.HasMember("updated_at")) {
						fDate = asset["updated_at"].GetString();
					}
					if (file.EndsWith(".hex")) {
						FirmwareRelease r;
						r.name = name;
						r.file = file;
						r.description = body;
						r.downloadUrl = fileUrl;
						r.published = publishDate;
						r.downloadCount = downloadCount;
						r.date = fDate;
						wxLogVerbose("Registering firmware: %s %s", file, fileUrl);
						releases.Add(r);
					}
				}
			}

			//releases.Sort(CompareReleases);
		}
	}

	void fillFirmware() {
		firmwareChoice->Clear();
		wxString str;
		str.Printf(_("Found %i firmware releases"), releases.Count());
		firmwareChoice->Append(str);
		for (int i = 0; i < releases.Count(); i++) {
			wxString title;
			title.Printf("%s %s", releases[i].name, releases[i].file);
			firmwareChoice->Append(title);
		}
		firmwareChoice->SetSelection(0);
		setProgress(_("Select firmware"));
		firmwareRefreshButton->Enable();
	}

	virtual void flashFile( wxCommandEvent& event ) {
		if (running != NULL) {
			return;
		}

		wxFileDialog openFileDialog(this, _("Open firmware file"), "", "", "HEX files (*.hex)|*.hex", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if (openFileDialog.ShowModal() == wxID_CANCEL) {
			return;     // the user changed idea...
		}

		wxString txt;
		txt.Printf(_("Flash %s???"), openFileDialog.GetPath());
		int answer = wxMessageBox(txt, _("Confirm"), wxOK | wxCANCEL, this);
		if (answer == wxOK) {
			flashFile(openFileDialog.GetPath(), openFileDialog.GetPath());
		}
	}

	virtual void flashFile(wxString ffile, wxString fileName) {
		if (running != NULL) {
			return;
		}

		wxString cmd = app->config->Read("DefaultFlashCommand2", _("hid_bootloader_cli_upgraded -mmcu=at90usb1286 -w -v \"%s\""));
		cmd = wxString::Format(cmd, ffile);
		MyProcess * const process = new MyProcess(this, cmd);
		process->Redirect();
		this->setProgress("");
		running = process;
		setProgress(wxString::Format(_("Flashing %s"), fileName));
		long pid = wxExecute(cmd, wxEXEC_ASYNC | wxEXEC_HIDE_CONSOLE, process);
		if (!pid) {
			setProgress(wxString::Format(_("Execution of '%s' failed."), cmd));
			wxLogError(_("Execution of '%s' failed."), cmd.c_str());
			delete process;
		} else {
			setProgress(wxString::Format(_("Process launched (%ld): '%s'"), pid, cmd));
			wxLogStatus(_("Process %ld (%s) launched."), pid, cmd.c_str());
		}
	}

	void executeFlash() {
		if (running != NULL) {
			return;
		}

		int sel = firmwareChoice->GetSelection();
		if (sel > 0 && sel <= releases.Count()) {
			FirmwareRelease r = releases[sel - 1];
			progressBar->SetValue(0);
			setProgress(wxString::Format(_("Downloading %s"), r.file));
			wxFileName* fname = downloadFile(r.file, r.downloadUrl);
			if (fname == NULL) {
				setProgress(wxString::Format(_("Failed to download %s"), r.downloadUrl));
			}
			wxString ffile = fname->GetFullPath();
			flashFile(ffile, r.file);
			delete fname;
		}
	}

	void executeFinished(FlashFinishedEvent& event) {
		if (event.statusCode == 0) {
			setProgress(_("Successfully flashed."));			
		} else {
			setProgress(wxString::Format(_("Finished with err code: %i"), event.statusCode));
		}
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
			txt.Printf(_("Download && flash %s???"), r.file);
			int answer = wxMessageBox(txt, _("Confirm"), wxOK | wxCANCEL, this);
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
		if (running != NULL) {
			delete running;
		}
		this->Close();
	}

	void OnTimer(wxTimerEvent& event) {
		if (running != NULL) {
			progressBar->SetValue(progressBar->GetValue() + 1);

			wxString text = getProgress();
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

			if (text != orig) {
				setProgress(text);
			}
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


CustomDialog::CustomDialog(MyApp* app, const wxString & title): FlashDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230), wxDEFAULT_DIALOG_STYLE), m_timer(this, TIMER_ID) {
	this->app = app;

	wxString oldrel = app->config->Read("LatestReleases", "");
	if (!oldrel.IsEmpty()) {
		parseFirmware(oldrel);
		fillFirmware();
	} else {
		firmwareChoice->SetSelection(0);
		firmwareRefreshButton->Enable();
		flashButton->Disable();
	}
	progressBar->SetRange(100);
	m_timer.Start(1000);
	running = NULL;
	Bind(MY_EXECFINISHED, &CustomDialog::executeFinished, this);
	Centre();
	ShowModal();
	//Destroy(); 
}

MyProcess::MyProcess(CustomDialog* parent, const wxString& cmd): wxProcess(parent) {
	m_cmd = cmd;
    m_parent = parent;
}

MyProcess::~MyProcess() {
	this->Kill(this->GetPid());
}

void MyProcess::OnTerminate(int pid, int status) {
	m_parent->addEvent(new FlashFinishedEvent(status));
}


bool MyApp::OnInit()
{
	//logFile = fopen(wxString::Format("%s/MagnumFlasher.log", wxStandardPaths::Get().GetTempDir()), "w+");
	//log = new wxLogStderr(logFile);
	wxLog::SetLogLevel(wxLOG_Max);
	wxCmdLineParser cmd(argc, argv);
	cmd.AddLongSwitch("verbose");
	cmd.AddLongSwitch("help");
	if (cmd.Parse(true) == -1) {
		return true;
	}

	if (cmd.Found("help")) {
		cmd.Usage();
		return true;
	}

	if (cmd.Found("verbose")) {
		wxLog::SetVerbose(true);
	}

	
	config = new wxConfig(_("Magnum Flasher"), _("Magnum3D"));
	wxString appcastUrl = config->Read("AppcastURL", _("http://magnum3d.ru/downloads/appcast.xml"));
	config->Write("AppcastURL", appcastUrl);
	win_sparkle_set_appcast_url(appcastUrl);
	win_sparkle_init();

	wxFileSystem::AddHandler(new wxMemoryFSHandler);
	void wxInitAllImageHandlers();
	wxLocale::AddCatalogLookupPathPrefix(".");
	locale = new wxLocale();
	locale->Init(wxLANGUAGE_RUSSIAN);
	locale->AddCatalog("lang");
    CustomDialog *custom = new CustomDialog(this, _("Magnum3D firmware flasher"));
    //custom->Show(true);
	wxLog::SetActiveTarget(NULL);
	delete log;
	fclose(logFile);

	win_sparkle_cleanup();
    return true;
}