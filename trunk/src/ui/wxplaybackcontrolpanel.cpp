/**************************************************************************************
 * Name:      wxplaybackcontrolpanel.cpp
 * Purpose:   Implementation of classes wxPlaybackControlPanel and wxVideoPlaybackPanel
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2008-11-22
 * Copyright: Rigoberto Calleja, Ricardo Garcia
 * License:   wxWindows license
 **************************************************************************************/

#include <memory>

#include "wxplaybackcontrolpanel.h"
#include "wxvideopanel.h"
#include "resources.h"

#include <wx/slider.h>
#include <wx/font.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include "../saya/core/avcontroller.h"
#include "../saya/core/dialogs.h"
#include "../saya/core/debuglog.h"
#include "../saya/inputmonitor.h"
#include "../saya/playbackmanager.h"

class wxBitmap;

using std::auto_ptr;

const int id_PLAYBACK_FIRSTFRAME = wxNewId();
const int id_PLAYBACK_FASTREWIND = wxNewId();
const int id_PLAYBACK_PREVFRAME = wxNewId();
const int id_PLAYBACK_PLAY = wxNewId();
const int id_PLAYBACK_NEXTFRAME = wxNewId();
const int id_PLAYBACK_FASTFORWARD = wxNewId();
const int id_PLAYBACK_LASTFRAME = wxNewId();
const int PlaybackSliderResolution = 10000;
const int PlaybackSliderPageSize = PlaybackSliderResolution / 200;

// ----------------------------
// begin wxPlaybackControlPanel
// ----------------------------

wxPlaybackControlPanel::wxPlaybackControlPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) :
wxPanel( parent, id, pos, size, style ),
m_VideoPanel(NULL)
{
	m_VBox = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* hbox1;
	hbox1 = new wxBoxSizer( wxHORIZONTAL );

	m_PlaybackSlider = new wxSlider( this, wxID_ANY, 0, 0, PlaybackSliderResolution, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_PlaybackSlider->SetPageSize(PlaybackSliderPageSize); // This avoids the ugly "skip a lot when clicked" default behavior.
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

    wxSize defaultbuttonsize(26,26);
    {
        auto_ptr<wxBitmap> imgfirstframe(syLoadImage("btn_firstframe.png"));
        auto_ptr<wxBitmap> imgfastrewind(syLoadImage("btn_fastrewind.png"));
        auto_ptr<wxBitmap> imgprevframe(syLoadImage("btn_prevframe.png"));
        auto_ptr<wxBitmap> imgplay(syLoadImage("btn_play.png"));
        auto_ptr<wxBitmap> imgnextframe(syLoadImage("btn_nextframe.png"));
        auto_ptr<wxBitmap> imgfastforward(syLoadImage("btn_fastforward.png"));
        auto_ptr<wxBitmap> imglastframe(syLoadImage("btn_lastframe.png"));

        m_btnFirstFrame = new wxBitmapButton( this, wxID_ANY, *imgfirstframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnFirstFrame->SetBitmapHover(*imgfirstframe);
        m_btnFastRewind = new wxBitmapButton( this, wxID_ANY, *imgfastrewind, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnPreviousFrame = new wxBitmapButton( this, wxID_ANY, *imgprevframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnPlay = new wxBitmapButton( this, wxID_ANY, *imgplay, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnNextFrame = new wxBitmapButton( this, wxID_ANY, *imgnextframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnFastForward = new wxBitmapButton( this, wxID_ANY, *imgfastforward, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
        m_btnLastFrame = new wxBitmapButton( this, wxID_ANY, *imglastframe, wxDefaultPosition, defaultbuttonsize, defaultbuttonstyle );
    }

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

// --------------------------
// end wxPlaybackControlPanel
// --------------------------

// --------------------------------
// begin wxVideoPlaybackPanel::Data
// --------------------------------

class wxVideoPlaybackPanel::Data : public wxEvtHandler {
    public:
        Data(wxVideoPlaybackPanel* parent);
        void InitObjectIds();
        virtual ~Data();
        int m_IdFirstFrame;
        int m_IdFastRewind;
        int m_IdPrevFrame;
        int m_IdPlay;
        int m_IdNextFrame;
        int m_IdFastForward;
        int m_IdLastFrame;
        int m_IdPlaybackSlider;
        int m_IdShuttle;

    private:
        wxVideoPlaybackPanel* m_Parent;
        void OnCommand(wxCommandEvent& event);


};

wxVideoPlaybackPanel::Data::Data(wxVideoPlaybackPanel* parent) :
m_IdFirstFrame(0),
m_IdFastRewind(0),
m_IdPrevFrame(0),
m_IdPlay(0),
m_IdNextFrame(0),
m_IdFastForward(0),
m_IdLastFrame(0),
m_IdPlaybackSlider(0),
m_IdShuttle(0),
m_Parent(parent)
{
    m_Parent->Connect(wxEVT_COMMAND_BUTTON_CLICKED,wxObjectEventFunction(&Data::OnCommand),NULL,static_cast<wxEvtHandler*>(this));
}

wxVideoPlaybackPanel::Data::~Data()
{
    m_Parent->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,wxObjectEventFunction(&Data::OnCommand),static_cast<wxEvtHandler*>(this));
}

void wxVideoPlaybackPanel::Data::InitObjectIds() {
    m_IdFirstFrame = m_Parent->m_btnFirstFrame->GetId();
    m_IdFastRewind = m_Parent->m_btnFastRewind->GetId();
    m_IdPrevFrame = m_Parent->m_btnPreviousFrame->GetId();
    m_IdPlay = m_Parent->m_btnPlay->GetId();
    m_IdNextFrame = m_Parent->m_btnNextFrame->GetId();
    m_IdFastForward = m_Parent->m_btnFastForward->GetId();
    m_IdLastFrame = m_Parent->m_btnLastFrame->GetId();
    m_IdPlaybackSlider = m_Parent->m_PlaybackSlider->GetId();
    m_IdShuttle = m_Parent->m_Shuttle->GetId();
}

void wxVideoPlaybackPanel::Data::OnCommand(wxCommandEvent& event) {
    if(!this || !m_Parent || !m_Parent->m_Player) return;
    if(event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED) {
        int id = event.GetId();
        AVPlayerEvent::PlayerEventId evtid;

        if(id == m_IdFirstFrame) {
            evtid = AVPlayerEvent::idGotoFirstFrame;
        } else if (id == m_IdFastRewind) {
            evtid = AVPlayerEvent::idFastRewind;
        } else if (id == m_IdPrevFrame) {
            evtid = AVPlayerEvent::idGotoPrevFrame;
        } else if (id == m_IdPlay) {
            if(m_Parent->m_Player->IsPlaying()) {
                evtid = AVPlayerEvent::idPause;
            } else {
                evtid = AVPlayerEvent::idPlay;
            }
        } else if (id == m_IdNextFrame) {
            evtid = AVPlayerEvent::idGotoNextFrame;
        } else if (id == m_IdFastForward) {
            evtid = AVPlayerEvent::idFastForward;
        } else if (id == m_IdLastFrame) {
            evtid = AVPlayerEvent::idGotoLastFrame;
        } else {
            evtid = AVPlayerEvent::idNone;
        }
        if(evtid && m_Parent->m_Player) {
            AVPlayerEvent myevent(evtid);
            m_Parent->m_Player->ProcessEvent(myevent);
        }
    }

}


// ------------------------------
// end wxVideoPlaybackPanel::Data
// ------------------------------

// --------------------------
// begin wxVideoPlaybackPanel
// --------------------------

wxVideoPlaybackPanel::wxVideoPlaybackPanel( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) :
wxPlaybackControlPanel( parent, id, pos, size, style),
m_Player(NULL),
m_Data(new Data(this))
{
    m_Data->InitObjectIds();
    m_VideoPanel = new wxVideoPanel(this);
    wxBoxSizer* hbox0 = NULL;
    hbox0 = new wxBoxSizer(wxHORIZONTAL);
    hbox0->Add(m_VideoPanel, 1, wxEXPAND, 5);
    m_VBox->Insert(0, hbox0, 1, wxEXPAND, 5 );
	this->Layout();
	m_VBox->Fit(this);
}

void wxVideoPlaybackPanel::SetAVPlayer(AVPlayer* player) {
    m_Player = player;
    if(m_Player) {
        m_Player->SetVideoOut(static_cast<wxVideoPanel*>(m_VideoPanel)->GetVideo());
        m_Player->Init();
    }
}

wxVideoPlaybackPanel::~wxVideoPlaybackPanel() {
    m_Player = NULL;
    delete m_Data;
    m_Data = NULL;
}

// ------------------------
// end wxVideoPlaybackPanel
// ------------------------
