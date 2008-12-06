/************************************************************************************
 * Name:      wxplaybackcontrolpanel.h
 * Purpose:   Declaration for classes wxPlaybackControlPanel and wxVideoPlaybackPanel
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2008-11-22
 * Copyright: Rigoberto Calleja, Ricardo Garcia
 * License:   wxWindows license
 ************************************************************************************/
#ifndef WXPLAYBACKCONTROLPANEL_H
#define WXPLAYBACKCONTROLPANEL_H

#include <wx/panel.h>
class wxSlider;
class wxBitmapButton;
class wxStaticText;
class wxVideoPanel;
class wxBoxSizer;

extern const int id_PLAYBACK_FIRSTFRAME;
extern const int id_PLAYBACK_FASTREWIND;
extern const int id_PLAYBACK_PREVFRAME;
extern const int id_PLAYBACK_PLAY;
extern const int id_PLAYBACK_NEXTFRAME;
extern const int id_PLAYBACK_FASTFORWARD;
extern const int id_PLAYBACK_LASTFRAME;

class wxPlaybackControlPanel : public wxPanel {

	protected:
		wxSlider* m_PlaybackSlider;
		wxBitmapButton* m_btnFirstFrame;
		wxBitmapButton* m_btnFastRewind;
		wxBitmapButton* m_btnPreviousFrame;
		wxBitmapButton* m_btnPlay;
		wxBitmapButton* m_btnNextFrame;
		wxBitmapButton* m_btnFastForward;
		wxBitmapButton* m_btnLastFrame;
		wxSlider* m_Shuttle;
		wxStaticText* m_txtShuttle;
		wxBoxSizer* m_VBox;
		/** @brief Video Panel.
		 *  Initialized to NULL by the constructor.
		 * You must initialize it to something else in your derived class' constructor.
		 */
		wxPanel* m_VideoPanel;

	public:
		wxPlaybackControlPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL);
		virtual ~wxPlaybackControlPanel();
};

class wxVideoPlaybackPanel : public wxPlaybackControlPanel {

    protected:
        wxVideoPanel* GetVideoPanel();
    public:
        wxVideoPlaybackPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        virtual ~wxVideoPlaybackPanel();
};

#endif //WXPLAYBACKCONTROLPANEL_H
