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
#include "debuglog.h"
#include "vidproject/projectmanager.h"

//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP(App);

BEGIN_EVENT_TABLE( App, wxApp )
    EVT_MENU(idExitApp, App::OnExitApp)
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
    ProjectManager::Get()->SetDebugLogger(this);
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
    m_debuglog = new AppDebugLog(NULL);
    m_debuglog->Log(_T("Debug log initialized."));

    m_debuglog->Log(_T("Initializing File system handlers..."));
    wxFileSystem::AddHandler(new wxZipFSHandler);
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    m_debuglog->Log(_T("Initializing XML Resource handlers..."));
    wxXmlResource::Get()->InitAllHandlers();
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)
	bool wxsOK = true;
    m_debuglog->Log(_T("Initializing Image handlers..."));
	wxInitAllImageHandlers();

    m_debuglog->Log(_T("Loading resources..."));
    if(!LoadXRCResources()) {
        return false;
    }

    m_debuglog->Log(_T("Initializing Project Manager..."));
    InitManagers();


    if(!LoadConfig()) {
        wxLogError(_("WARNING: Could not read configuration!"));
        m_debuglog->Log(_T("WARNING: Could not read configuration!"));
    }

    m_debuglog->Log(_T("Creating main frame..."));
    AppFrame* frame = new AppFrame(NULL, _("Saya - Unsheathe your Creativity"));
    ProjectManager::Get()->SetEventHandler(frame);
    SetTopWindow(frame);

    m_debuglog->Log(_T("Initialization finished."));
	return wxsOK;
}

int App::OnExit() {
    return wxApp::OnExit();
}

App::~App() {
}

void App::DebugLog(const std::string&msg) {
    if(m_debuglog) {
        m_debuglog->Log(msg.c_str());
    }
}

void App::DebugLog(const char* msg) {
    if(m_debuglog) {
        m_debuglog->Log(msg);
    }
}

void App::OnExitApp(wxCommandEvent& event) {
    if(m_debuglog) {
        m_debuglog->Log(_T("Good bye."));
        delete m_debuglog;
    }
    m_debuglog = NULL;
}
