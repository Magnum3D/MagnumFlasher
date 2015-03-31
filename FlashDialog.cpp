///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>

#include "FlashDialog.h"

///////////////////////////////////////////////////////////////////////////

FlashDialog::FlashDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );
	
	logo = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("logo.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
	bSizer1->Add( logo, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	labelFirmwareChoice = new wxStaticText( this, wxID_ANY, wxT("Select firmware"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	labelFirmwareChoice->Wrap( -1 );
	bSizer2->Add( labelFirmwareChoice, 0, wxALL, 5 );
	
	wxArrayString firmwareChoiceChoices;
	firmwareChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, firmwareChoiceChoices, 0 );
	firmwareChoice->SetSelection( 0 );
	firmwareChoice->SetMinSize( wxSize( 300,-1 ) );
	
	bSizer2->Add( firmwareChoice, 0, wxALL, 5 );
	
	firmwareRefreshButton = new wxButton( this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( firmwareRefreshButton, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer2, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	progressText = new wxStaticText( this, wxID_ANY, wxT("Press Refresh..."), wxDefaultPosition, wxDefaultSize, 0 );
	progressText->Wrap( -1 );
	bSizer5->Add( progressText, 0, wxALL, 5 );
	
	progressBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	progressBar->SetValue( 0 ); 
	bSizer5->Add( progressBar, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer5->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	flashButton = new wxButton( this, wxID_ANY, wxT("Flash now"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( flashButton, 0, wxALL, 5 );
	
	closeButton = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( closeButton, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer4, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( FlashDialog::onActivateDialog ) );
	firmwareChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FlashDialog::onFirmwareChoice ), NULL, this );
	firmwareRefreshButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFirmwareRefreshButton ), NULL, this );
	flashButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFlashButton ), NULL, this );
	closeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onCloseButton ), NULL, this );
}

FlashDialog::~FlashDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( FlashDialog::onActivateDialog ) );
	firmwareChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FlashDialog::onFirmwareChoice ), NULL, this );
	firmwareRefreshButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFirmwareRefreshButton ), NULL, this );
	flashButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFlashButton ), NULL, this );
	closeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onCloseButton ), NULL, this );
	
}
