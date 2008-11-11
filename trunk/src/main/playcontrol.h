/****************************************************************************
 * Name:      playcontrol.h
 * Purpose:   Declaration for Saya playback control
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Created:   2008-11-10
 * Copyright: Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code found at
 *            http://zetcode.com/tutorials/wxwidgetstutorial/customwidgets/
 *            http://zetcode.com/tutorials/wxwidgetstutorial/widgets/
 ***************************************************************************/

#ifndef PLAYCONTROL_H_INCLUDED
#define PLAYCONTROL_H_INCLUDED

#include <wx/wx.h>
class PlayControl : public wxPanel
{
private:
    wxSlider *m_slider;
    wxBitmapButton *m_stop;
    wxBitmapButton *m_play;
    wxBitmapButton *m_fastrewind;
    wxBitmapButton *m_fastforward;
public:
    wxPanel *m_parent;

    PlayControl(wxPanel* parent, int id);
    void OnPaint(wxPaintEvent& event);
};
#endif // PLAYCONTROL_H_INCLUDED
