/***************************************************************
 * Name:      SayaApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ricardo Garcia ()
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/xrc/xmlres.h>
#include <wx/config.h>

#include "App.h"
#include "Main.h"


//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP(App);

int cmd_SHOW_STARTUP_DIALOG = wxNewId();

BEGIN_EVENT_TABLE( App, wxApp )
    EVT_COMMAND(-1, cmd_SHOW_STARTUP_DIALOG, App::OnShowStartupDialog)
END_EVENT_TABLE()

bool App::LoadConfig()
{
// TODO (rick#1#): Load frame config (window position and size), layout, last open project, etc.

    return true;
}

void App::OnShowStartupDialog(wxCommandEvent& event) {
//    wxMessageBox(_("Hola mundo! :) "),_("Hello world"));
}

bool App::OnInit()
{
    wxXmlResource::Get()->InitAllHandlers();
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)

    if(!LoadConfig()) {
        wxLogError(_("WARNING: Could not read configuration!"));
    }

    AppFrame* frame = new AppFrame(0L, _("Saya - Swift audiovisual Authoring for You and Anyone"));

    frame->Show();
    wxCommandEvent evt(cmd_SHOW_STARTUP_DIALOG);
    wxPostEvent(this, evt);
	return wxsOK;
}
