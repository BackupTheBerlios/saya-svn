/****************************************************************************
 * Name:      playcontrol.h
 * Purpose:   Definition for Saya playback control
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Created:   2008-11-10
 * Copyright: Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code found at
 *            http://zetcode.com/tutorials/wxwidgetstutorial/customwidgets/
 *            http://zetcode.com/tutorials/wxwidgetstutorial/widgets/
 ***************************************************************************/
#include <wx/wx.h>
#include "playcontrol.h"

int ID_SLIDER = 1;

PlayControl::PlayControl(wxPanel* parent, int id)
    : wxPanel(parent, id, wxDefaultPosition, wxSize(-1, -1), wxSUNKEN_BORDER)
{
    m_parent = parent;

    wxPanel *panel = new wxPanel(this, wxID_ANY);

    m_slider = new wxSlider(panel, ID_SLIDER, 0, 0, 100, wxPoint(-1, -1),
      wxSize(150, -1), wxSL_LABELS);

    m_stop = new wxBitmapButton(panel, wxID_ANY, wxBitmap(wxT("../resources/img/new_video.png"),
      wxBITMAP_TYPE_PNG), wxPoint(180, 20));

    Connect(wxEVT_PAINT, wxPaintEventHandler(PlayControl::OnPaint));
}

void PlayControl::OnPaint(wxPaintEvent& event)
{

}
