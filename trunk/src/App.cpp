/***************************************************************
 * Name:      SayaApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
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
#include "vidproject/projectmanager.h"

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
    if(!ProjectManager::Get()->LoadConfig())
        return false;
    return true;
}

void App::OnShowStartupDialog(wxCommandEvent& event) {
//    wxMessageBox(_("Hola mundo! :) "),_("Hello world"));
}

void App::InitManagers() {
    ProjectManager::Get();
}

bool App::OnInit()
{
    wxXmlResource::Get()->InitAllHandlers();
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)

    InitManagers();


    if(!LoadConfig()) {
        wxLogError(_("WARNING: Could not read configuration!"));
    }

    AppFrame* frame = new AppFrame(NULL, _("Saya - Swift audiovisual Authoring for You and Anyone"));
    ProjectManager::Get()->SetMainFrame(frame);
    SetTopWindow(frame);
    frame->Show();
    wxCommandEvent evt(cmd_SHOW_STARTUP_DIALOG);
    wxPostEvent(this, evt);
	return wxsOK;
}
