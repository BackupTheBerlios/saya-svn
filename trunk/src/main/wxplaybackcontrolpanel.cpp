/**************************************************************************************
 * Name:      wxplaybackcontrolpanel.cpp
 * Purpose:   Implementation of classes wxPlaybackControlPanel and wxVideoPlaybackPanel
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2008-11-22
 * Copyright: Rigoberto Calleja, Ricardo Garcia
 * License:   wxWindows license
 **************************************************************************************/

#include "wxplaybackcontrolpanel.h"
#include "wxvideopanel.h"
#include "resources.h"

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

const int id_PLAYBACK_FIRSTFRAME = wxNewId();
const int id_PLAYBACK_FASTREWIND = wxNewId();
const int id_PLAYBACK_PREVFRAME = wxNewId();
const int id_PLAYBACK_PLAY = wxNewId();
const int id_PLAYBACK_NEXTFRAME = wxNewId();
const int id_PLAYBACK_FASTFORWARD = wxNewId();
const int id_PLAYBACK_LASTFRAME = wxNewId();

wxPlaybackControlPanel::wxPlaybackControlPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
wxPanel( parent, id, pos, size, style ),
m_VideoPanel(NULL)
{
	m_VBox = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* hbox1;
	hbox1 = new wxBoxSizer( wxHORIZONTAL );

	m_PlaybackSlider = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	hbox1->Add(10,0); // Adds a spacer
	hbox1->Add(m_PlaybackSlider, 1, wxEXPAND, 5 );
	hbox1->Add(10,0); // Adds a spacer

	wxBoxSizer* hbox2;
	hbox2 = new wxBoxSizer( wxHORIZONTAL );
	unsigned long defaultbuttonstyle;
	#ifdef __WIN32__
      defaultbuttonstyle = wxBU_AUTODRAW | wxBORDER_THEME;
    #else
	  defaultbuttonstyle = wxBORDER_NONE;
    #endif

    wxBitmap* imgfirstframe = syLoadImage(wxT("btn_firstframe.png"));
    wxBitmap* imgfastrewind = syLoadImage(wxT("btn_fastrewind.png"));
    wxBitmap* imgprevframe = syLoadImage(wxT("btn_prevframe.png"));
    wxBitmap* imgplay = syLoadImage(wxT("btn_play.png"));
    wxBitmap* imgnextframe = syLoadImage(wxT("btn_nextframe.png"));
    wxBitmap* imgfastforward = syLoadImage(wxT("btn_fastforward.png"));
    wxBitmap* imglastframe = syLoadImage(wxT("btn_lastframe.png"));

    wxSize defaultbuttonsize(26,26);

	m_btnFirstFrame = new wxBitmapButton( this, wxID_ANY, *imgfirstframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnFirstFrame->SetBitmapHover(*imgfirstframe);
	m_btnFastRewind = new wxBitmapButton( this, wxID_ANY, *imgfastrewind, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnPreviousFrame = new wxBitmapButton( this, wxID_ANY, *imgprevframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnPlay = new wxBitmapButton( this, wxID_ANY, *imgplay, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnNextFrame = new wxBitmapButton( this, wxID_ANY, *imgnextframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnFastForward = new wxBitmapButton( this, wxID_ANY, *imgfastforward, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
	m_btnLastFrame = new wxBitmapButton( this, wxID_ANY, *imglastframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );

	delete imglastframe;
	delete imgfastforward;
	delete imgnextframe;
	delete imgplay;
	delete imgprevframe;
	delete imgfastrewind;
	delete imgfirstframe;

	m_Shuttle = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_txtShuttle = new wxStaticText( this, wxID_ANY, wxT("[Display]"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	m_txtShuttle->Wrap( -1 );

    hbox2->Add(16,0); // Adds a spacer
	hbox2->Add(m_btnFirstFrame, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnFastRewind, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnPreviousFrame, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnPlay, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnNextFrame, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnFastForward, 0, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(m_btnLastFrame, 0, wxALIGN_CENTER_VERTICAL, 5);
    hbox2->Add(16,0); // Adds a spacer
	hbox2->Add(m_Shuttle, 1, wxALIGN_CENTER_VERTICAL, 5);
	hbox2->Add(20,0); // Adds a spacer
	hbox2->Add(m_txtShuttle, 1, wxALIGN_CENTER_VERTICAL, 5);

	m_VBox->Add( hbox1, 0, wxEXPAND, 2 );
	m_VBox->Add( hbox2, 0, wxEXPAND, 2 );
    m_VBox->Add(0,4); // Adds a spacer

	this->SetSizer(m_VBox);
	this->Layout();
	m_VBox->Fit(this);
}

wxPlaybackControlPanel::~wxPlaybackControlPanel()
{
}

wxVideoPlaybackPanel::wxVideoPlaybackPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) :
wxPlaybackControlPanel( parent, id, pos, size, style)
{
    m_VideoPanel = new wxVideoPanel(this);
    wxBoxSizer* hbox0 = NULL;
    hbox0 = new wxBoxSizer(wxHORIZONTAL);
    hbox0->Add(m_VideoPanel, 1, wxEXPAND, 5);
    m_VBox->Insert(0, hbox0, 1, wxEXPAND, 5 );
	this->Layout();
	m_VBox->Fit(this);
}

wxVideoPlaybackPanel::~wxVideoPlaybackPanel() {
}
