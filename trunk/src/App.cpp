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

AppConfig::AppConfig(std::string application_name) : sayaConfig(application_name) {
    m_config = new wxConfig(wxString(application_name.c_str(),wxConvUTF8));
}

std::string AppConfig::Read(const std::string& key, const std::string& defaultvalue) {
    return std::string(m_config->Read(wxString(key.c_str(),wxConvUTF8),wxString(defaultvalue.c_str(),wxConvUTF8)).mb_str());
}

bool AppConfig::Write(const std::string& key, const std::string& value) {
    return m_config->Write(wxString(key.c_str(),wxConvUTF8),wxString(value.c_str(),wxConvUTF8));
}

bool AppConfig::Exists(const std::string& key) {
    return m_config->Exists(wxString(key.c_str(),wxConvUTF8));
}

AppConfig::~AppConfig() {
    delete m_config;
    m_config = NULL;
}

sayaConfig* AppConfigProvider::Create(const std::string application_name) {
    return new AppConfig(application_name);
}

bool App::LoadConfig()
{
    if(!ProjectManager::Get()->LoadConfig())
        return false;
    return true;
}

void App::InitManagers() {
    ProjectManager::Get();
    ProjectManager::Get()->SetConfigProvider(&m_configprovider);
}

bool App::LoadXRCResources() {
    bool result = false;
    wxXmlResource* rsc = wxXmlResource::Get();
    do {
        if(!rsc->Load(_T("resources/mainmenu.xrc"))) break;
        if(!rsc->Load(_T("resources/welcome.xrc"))) break;
        if(!rsc->Load(_T("resources/newproject.xrc"))) break;
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
    ProjectManager::Get()->SetEventHandler(frame);
    SetTopWindow(frame);

	return wxsOK;
}
