///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __FLASHDIALOG_H__
#define __FLASHDIALOG_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/gauge.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class FlashDialog
///////////////////////////////////////////////////////////////////////////////
class FlashDialog : public wxDialog 
{
	private:
	
	protected:
		wxStaticBitmap* logo;
		wxStaticText* labelFirmwareChoice;
		wxChoice* firmwareChoice;
		wxButton* firmwareRefreshButton;
		wxStaticText* progressText;
		wxGauge* progressBar;
		wxStaticLine* m_staticline1;
		wxButton* flashButton;
		wxButton* closeButton;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onActivateDialog( wxActivateEvent& event ) { event.Skip(); }
		virtual void onFirmwareChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFirmwareRefreshButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void onFlashButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCloseButton( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		FlashDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Flash Magnum3D"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~FlashDialog();
	
};

#endif //__FLASHDIALOG_H__
