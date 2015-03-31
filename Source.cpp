// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};


wxIMPLEMENT_APP(MyApp);

#include "FlashDialog.h"

class CustomDialog : public FlashDialog
{
public:
CustomDialog(const wxString& title);

protected:
virtual void onActivateDialog( wxActivateEvent& event ) { event.Skip(); }

virtual void onFirmwareChoice( wxCommandEvent& event ) {
	if (firmwareChoice->GetSelection() == 0) {
		progressText->SetLabel("");
	} else
	if (firmwareChoice->GetSelection() == 1) {
		progressText->SetLabel("New firmware 2014-05-01.... changelog here");
	} else
	if (firmwareChoice->GetSelection() == 2) {
		progressText->SetLabel("Old firmware 2014-05-03.... changelog here");
	}
}

virtual void onFirmwareRefreshButton( wxCommandEvent& event ) {
	firmwareChoice->Clear();
	firmwareChoice->Append("Choose one..");
	firmwareChoice->Append("Firmware #1");
	firmwareChoice->Append("Firmware #2");
	firmwareChoice->SetSelection(0);
}

virtual void onFlashButton( wxCommandEvent& event ) {
}

virtual void onCloseButton( wxCommandEvent& event ) {
	this->Close();
}

};

CustomDialog::CustomDialog(const wxString & title): FlashDialog(NULL, -1, title, wxDefaultPosition, wxSize(250, 230), wxDEFAULT_DIALOG_STYLE)
{
  Centre();
  ShowModal();
  Destroy(); 
}

bool MyApp::OnInit()
{
    CustomDialog *custom = new CustomDialog(wxT("Magnum3D firmware flasher"));
    custom->Show(true);
    return true;
}