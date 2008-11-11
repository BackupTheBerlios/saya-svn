/****************************************************************************
 * Name:      playcontrol.h
 * Purpose:   Declaration for Saya play control
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Created:   2008-11-10
 * Copyright: Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code found at
 *            http://zetcode.com/tutorials/wxwidgetstutorial/customwidgets/
 ***************************************************************************/

#ifndef PLAYCONTROL_H_INCLUDED
#define PLAYCONTROL_H_INCLUDED

#include <wx/wx.h>
class PlayControl : public wxPanel
{
public:
    PlayControl(wxPanel* parent, int id);
};
#endif // PLAYCONTROL_H_INCLUDED
