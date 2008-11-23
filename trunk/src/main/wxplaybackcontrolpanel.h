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

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/panel.h>
    class wxSlider;
    class wxBitmapButton;
    class wxStaticText;
#else
    #include "wx_pch.h"
#endif

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

	public:
		wxPlaybackControlPanel( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~wxPlaybackControlPanel();
};

#endif //WXPLAYBACKCONTROLPANEL_H
