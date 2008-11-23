///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxplaybackcontrolpanel.h"

///////////////////////////////////////////////////////////////////////////

AllInOnePlaybackControlGUI::AllInOnePlaybackControlGUI( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* vBox;
	vBox = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* hbox1;
	hbox1 = new wxBoxSizer( wxHORIZONTAL );

	m_playbackslider = new wxSlider( this, ID_PLAYSLIDER, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	hbox1->Add( m_playbackslider, 1, wxEXPAND, 5 );

	vBox->Add( hbox1, 1, wxEXPAND, 5 );

	wxBoxSizer* hbox2;
	hbox2 = new wxBoxSizer( wxHORIZONTAL );

	m_firstframe = new wxBitmapButton( this, ID_FIRSTFRAME, wxBitmap( wxT("../resources/img/btn_firstframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_firstframe, 0, 0, 5 );

	m_fastrewind = new wxBitmapButton( this, ID_FASTREWIND, wxBitmap( wxT("../resources/img/btn_fastrewind.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_fastrewind, 0, 0, 5 );

	m_previousframe = new wxBitmapButton( this, ID_PREVIOUSFRAME, wxBitmap( wxT("../resources/img/btn_prevframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_previousframe, 0, 0, 5 );

	m_play = new wxBitmapButton( this, ID_PLAY, wxBitmap( wxT("../resources/img/btn_play.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_play, 0, 0, 5 );

	m_nextframe = new wxBitmapButton( this, ID_NEXTFRAME, wxBitmap( wxT("../resources/img/btn_nextframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_nextframe, 0, 0, 5 );

	m_fastforward = new wxBitmapButton( this, ID_FASTFORWARD, wxBitmap( wxT("../resources/img/btn_fastforward.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_fastforward, 0, 0, 5 );

	m_lastframe = new wxBitmapButton( this, ID_LASTFRAME, wxBitmap( wxT("../resources/img/btn_lastframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	hbox2->Add( m_lastframe, 0, 0, 5 );

	m_shuttle = new wxSlider( this, ID_SHUTTLE, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	hbox2->Add( m_shuttle, 0, 0, 5 );

	m_display = new wxStaticText( this, wxID_ANY, wxT("[Display]"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_display->Wrap( -1 );
	hbox2->Add( m_display, 1, 0, 5 );

	vBox->Add( hbox2, 1, wxEXPAND, 5 );

	this->SetSizer( vBox );
	this->Layout();
	vBox->Fit( this );
}

AllInOnePlaybackControlGUI::~AllInOnePlaybackControlGUI()
{
}
