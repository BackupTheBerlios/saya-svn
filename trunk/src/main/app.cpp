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

#ifndef WX_PRECOMP
    #include <wx/fs_zip.h>
    #include <wx/fs_mem.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/config.h>
    #include <wx/log.h>
#endif

#include "s2wx.h"
#include "app.h"
#include "resources.h"
#include "debuglog.h"
#include "../saya/projectmanager.h"
#include "../saya/core/systring.h"
#include "../saya/configprovider.h"
#include "../saya/sayaconfig.h"

//(*AppHeaders
#include <wx/image.h>
//*)

extern int idExitApp;

IMPLEMENT_APP(App);

BEGIN_EVENT_TABLE( App, wxApp )
    EVT_MENU(idExitApp, App::OnExitApp)
END_EVENT_TABLE()


/** Our Implementation of SayaConfig with wxWidgets. @see SayaConfig */
class AppConfig : public SayaConfig {
    public:

        /** Standard constructor. */
        AppConfig(const char* application_name);

        /** Reads configuration. @see SayaConfig::Read */
        virtual syString Read(const char* key, const char* defaultvalue);

        /** Writes configuration. @see SayaConfig::Read */
        virtual bool Write(const char* key, const char* value);

        /** Checks whether a configuration key exists. @see SayaConfig::Read */
        virtual bool Exists(const char* key);

        /** Standard destructor. */
        virtual ~AppConfig();
    private:
        /** Our configuration object */
        wxConfig* m_config;
};

/** Our Configuration object provider. @see SayaConfigProvider */
class AppConfigProvider : public SayaConfigProvider {
    public:
        /** Creates a SayaConfig object */
        virtual SayaConfig* Create(const char* application_name);

        /** Virtual destructor. */
        virtual ~AppConfigProvider() {}
};

AppConfig::AppConfig(const char* application_name) : SayaConfig(application_name) {
    m_config = new wxConfig(s2wx(application_name));
}

syString AppConfig::Read(const char* key, const char* defaultvalue) {
    return wx2s(m_config->Read(s2wx(key),s2wx(defaultvalue)));
}

bool AppConfig::Write(const char* key, const char* value) {
    return m_config->Write(s2wx(key),s2wx(value));
}

bool AppConfig::Exists(const char* key) {
    return m_config->Exists(s2wx(key));
}

AppConfig::~AppConfig() {
    delete m_config;
    m_config = NULL;
}

SayaConfig* AppConfigProvider::Create(const char* application_name) {
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
    ProjectManager::Get()->SetConfigProvider(m_configprovider);
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
    m_configprovider = new AppConfigProvider;
    m_debuglog = new AppDebugLog(NULL);
    sayaDebugLogger::SetDebugLogger(m_debuglog);

    DebugLog("Debug log initialized.");

    DebugLog("Initializing Resources path...");
    syInitResourcesPaths();

    DebugLog("Initializing File system handlers...");
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxImage::AddHandler(new wxPNGHandler);
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
    wxFrame* frame = CreateMainFrame();
    SetTopWindow(frame);

    DebugLog("Initialization finished.");
	return wxsOK;
}

int App::OnExit() {
    return wxApp::OnExit();
}

App::~App() {
    delete m_configprovider;
}

void App::OnExitApp(wxCommandEvent& event) {
    if(m_debuglog) {
        DebugLog("Good bye.");
        delete m_debuglog;
    }
    m_debuglog = NULL;
}
