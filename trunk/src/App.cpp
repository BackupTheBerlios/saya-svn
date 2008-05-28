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

#include <wx/fs_zip.h>
#include <wx/fs_mem.h>
#include <wx/xrc/xmlres.h>
#include <wx/config.h>

#include "App.h"
#include "Main.h"
#include "vidproject/projectmanager.h"

//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP(App);


BEGIN_EVENT_TABLE( App, wxApp )
END_EVENT_TABLE()

bool App::LoadConfig()
{
    if(!ProjectManager::Get()->LoadConfig())
        return false;
    return true;
}

void App::InitManagers() {
    ProjectManager::Get();
}

bool App::LoadXRCResources() {
    bool result = false;
    wxXmlResource* rsc = wxXmlResource::Get();
    do {
        if(!rsc->Load(_T("resources/mainmenu.xrc"))) break;
        if(!rsc->Load(_T("resources/welcome.xrc"))) break;
        result = true;
    }while(false);
    return result;
}


bool App::OnInit()
{
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxXmlResource::Get()->InitAllHandlers();
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();

	//*)
    if(!LoadXRCResources()) {
        return false;
    }

    InitManagers();


    if(!LoadConfig()) {
        wxLogError(_("WARNING: Could not read configuration!"));
    }

    AppFrame* frame = new AppFrame(NULL, _("Saya - Swift audiovisual Authoring for You and Anyone"));
    ProjectManager::Get()->SetMainFrame(frame);
    SetTopWindow(frame);
    frame->Show(); // TODO: Comment me after welcome dialog has been implemented

	return wxsOK;
}
