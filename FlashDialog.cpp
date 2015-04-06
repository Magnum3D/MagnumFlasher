///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "FlashDialog.h"

///////////////////////////////////////////////////////////////////////////

FlashDialog::FlashDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer51->SetMinSize( wxSize( 650,-1 ) ); 
	logo = new wxStaticBitmap( this, wxID_ANY, wxBitmap( wxT("logo.bmp"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxSize( 160,-1 ), wxSIMPLE_BORDER );
	logo->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	logo->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );
	
	bSizer51->Add( logo, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	labelFirmwareChoice = new wxStaticText( this, wxID_ANY, _("Select firmware"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	labelFirmwareChoice->Wrap( -1 );
	bSizer2->Add( labelFirmwareChoice, 0, wxALL, 5 );
	
	wxArrayString firmwareChoiceChoices;
	firmwareChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, firmwareChoiceChoices, 0 );
	firmwareChoice->SetSelection( 0 );
	firmwareChoice->SetMinSize( wxSize( 300,-1 ) );
	
	bSizer2->Add( firmwareChoice, 0, wxALL, 5 );
	
	firmwareRefreshButton = new wxButton( this, wxID_ANY, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( firmwareRefreshButton, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer2, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	progressText = new wxTextCtrl( this, wxID_ANY, _("Press Refresh..."), wxDefaultPosition, wxSize( 680,125 ), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP|wxSTATIC_BORDER|wxVSCROLL );
	progressText->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	progressText->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	bSizer5->Add( progressText, 0, wxALL, 5 );
	
	progressBar = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	progressBar->SetValue( 0 ); 
	bSizer5->Add( progressBar, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer5->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	flashButton = new wxButton( this, wxID_ANY, _("Flash now"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( flashButton, 0, wxALL, 5 );
	
	closeButton = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( closeButton, 0, wxALL, 5 );
	
	m_button4 = new wxButton( this, wxID_ANY, _("File..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_button4, 0, wxALL, 5 );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Click Boot button on printer during flashing!"), wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_CENTRE );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	m_staticText3->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_staticText3->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	bSizer4->Add( m_staticText3, 0, wxALL, 5 );
	
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, _("More info"), wxT("http://magnum3d.ru/firmware/"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer4->Add( m_hyperlink1, 0, wxALL, 5 );
	
	
	bSizer5->Add( bSizer4, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	bSizer51->Add( bSizer5, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer51, 1, wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer1->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText2 = new wxStaticText( this, wxID_ANY, _("(C) 2014 - 2015 Magnum3D.ru"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText2->Wrap( -1 );
	bSizer1->Add( m_staticText2, 0, wxALL, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( FlashDialog::onActivateDialog ) );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FlashDialog::onClose ) );
	firmwareChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FlashDialog::onFirmwareChoice ), NULL, this );
	firmwareRefreshButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFirmwareRefreshButton ), NULL, this );
	flashButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFlashButton ), NULL, this );
	closeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onCloseButton ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::flashFile ), NULL, this );
	m_hyperlink1->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( FlashDialog::onLinkClick ), NULL, this );
}

FlashDialog::~FlashDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( FlashDialog::onActivateDialog ) );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( FlashDialog::onClose ) );
	firmwareChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( FlashDialog::onFirmwareChoice ), NULL, this );
	firmwareRefreshButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFirmwareRefreshButton ), NULL, this );
	flashButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onFlashButton ), NULL, this );
	closeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::onCloseButton ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlashDialog::flashFile ), NULL, this );
	m_hyperlink1->Disconnect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( FlashDialog::onLinkClick ), NULL, this );
	
}
