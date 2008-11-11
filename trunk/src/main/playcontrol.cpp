/****************************************************************************
 * Name:      playcontrol.h
 * Purpose:   Definition for Saya play control
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Created:   2008-11-10
 * Copyright: Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code found at
 *            http://zetcode.com/tutorials/wxwidgetstutorial/customwidgets/
 ***************************************************************************/
#include "playcontrol.h"
#include <wx/wx.h>
PlayControl::PlayControl(wxPanel* parent, int id)
    : wxPanel(parent, id, wxDefaultPosition, wxSize(-1, -1), wxSUNKEN_BORDER)
{

};
