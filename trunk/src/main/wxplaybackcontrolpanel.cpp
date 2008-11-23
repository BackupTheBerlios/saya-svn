/**************************************************************************************
 * Name:      wxplaybackcontrolpanel.cpp
 * Purpose:   Implementation of classes wxPlaybackControlPanel and wxVideoPlaybackPanel
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2008-11-22
 * Copyright: Rigoberto Calleja, Ricardo Garcia
 * License:   wxWindows license
 **************************************************************************************/

#include "wxplaybackcontrolpanel.h"

#ifndef WX_PRECOMP
    #include <wx/slider.h>
    #include <wx/gdicmn.h>
    #include <wx/font.h>
    #include <wx/colour.h>
    #include <wx/settings.h>
    #include <wx/string.h>
    #include <wx/sizer.h>
    #include <wx/bitmap.h>
    #include <wx/image.h>
    #include <wx/icon.h>
    #include <wx/bmpbuttn.h>
    #include <wx/button.h>
    #include <wx/stattext.h>
    #include <wx/panel.h>
#endif

wxPlaybackControlPanel::wxPlaybackControlPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) :
wxPanel( parent, id, pos, size, style )
{
	wxBoxSizer* vBox;
	vBox = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* hbox1;
	hbox1 = new wxBoxSizer( wxHORIZONTAL );

	m_PlaybackSlider = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	hbox1->Add(m_PlaybackSlider, 1, wxEXPAND, 5 );

	vBox->Add( hbox1, 1, wxEXPAND, 5 );

	wxBoxSizer* hbox2;

	hbox2 = new wxBoxSizer( wxHORIZONTAL );
	unsigned long defaultbuttonstyle = defaultbuttonstyle | wxBU_EXACTFIT;

	m_btnFirstFrame = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_firstframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnFastRewind = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_fastrewind.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnPreviousFrame = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_prevframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnPlay = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_play.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnNextFrame = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_nextframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnFastForward = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_fastforward.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_btnLastFrame = new wxBitmapButton( this, wxID_ANY, wxBitmap( wxT("../resources/img/btn_lastframe.png"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, defaultbuttonstyle );
	m_Shuttle = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_txtShuttle = new wxStaticText( this, wxID_ANY, wxT("[Display]"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_txtShuttle->Wrap( -1 );

	hbox2->Add(m_btnFirstFrame, 0, 0, 5);
	hbox2->Add(m_btnFastRewind, 0, 0, 5);
	hbox2->Add(m_btnPreviousFrame, 0, 0, 5);
	hbox2->Add(m_btnPlay, 0, 0, 5);
	hbox2->Add(m_btnNextFrame, 0, 0, 5);
	hbox2->Add(m_btnFastForward, 0, 0, 5);
	hbox2->Add(m_btnLastFrame, 0, 0, 5);
	hbox2->Add(m_Shuttle, 1, 0, 5);
	hbox2->Add(m_txtShuttle, 1, 0, 5);

	vBox->Add( hbox2, 1, wxEXPAND, 5 );

	this->SetSizer( vBox );
	this->Layout();
	vBox->Fit(this);
}

wxPlaybackControlPanel::~wxPlaybackControlPanel()
{
}
