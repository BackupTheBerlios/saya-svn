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

#include "s2wx.h"
#include "App.h"
#include "Main.h"
#include "debuglog.h"
#include "vidproject/projectmanager.h"

//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP(App);

BEGIN_EVENT_TABLE( App, wxApp )
    EVT_MENU(idExitApp, App::OnExitApp)
END_EVENT_TABLE()

AppConfig::AppConfig(std::string application_name) : SayaConfig(application_name) {
    m_config = new wxConfig(s2wx(application_name));
}

std::string AppConfig::Read(const std::string& key, const std::string& defaultvalue) {
    return wx2s(m_config->Read(s2wx(key),s2wx(defaultvalue)));
}

bool AppConfig::Write(const std::string& key, const std::string& value) {
    return m_config->Write(s2wx(key),s2wx(value));
}

bool AppConfig::Exists(const std::string& key) {
    return m_config->Exists(s2wx(key));
}

AppConfig::~AppConfig() {
    delete m_config;
    m_config = NULL;
}

SayaConfig* AppConfigProvider::Create(const std::string application_name) {
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
        if(!rsc->Load(_T("resources/pickname.xrc"))) break;
        result = true;
    }while(false);
    return result;
}


bool App::OnInit()
{
    m_debuglog = new AppDebugLog(NULL);
    sayaDebugLogger::SetDebugLogger(m_debuglog);

    DebugLog("Debug log initialized.");

    DebugLog("Initializing File system handlers...");
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    DebugLog("Initializing XML Resource handlers...");
    wxXmlResource::Get()->InitAllHandlers();
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)
    DebugLog("Initializing Image handlers...");
	wxInitAllImageHandlers();

    DebugLog("Loading resources...");
    if(!LoadXRCResources()) {
        return false;
    }

    DebugLog("Initializing Project Manager...");
    InitManagers();


    if(!LoadConfig()) {
        wxLogError(_("WARNING: Could not read configuration!"));
        DebugLog("WARNING: Could not read configuration!");
    }

    DebugLog("Creating main frame...");
    AppFrame* frame = new AppFrame(NULL, _("Saya - Unsheathe your Creativity"));
    ProjectManager::Get()->SetEventHandler(frame);
    SetTopWindow(frame);

    DebugLog("Initialization finished.");
	return wxsOK;
}

int App::OnExit() {
    return wxApp::OnExit();
}

App::~App() {
}

void App::OnExitApp(wxCommandEvent& event) {
    if(m_debuglog) {
        DebugLog("Good bye.");
        delete m_debuglog;
    }
    m_debuglog = NULL;
}
