/***************************************************************
 * Name:      ui/app.cpp
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
    #include <wx/app.h>
    #include <wx/log.h>
    #include <wx/image.h>
    #include <wx/msgdlg.h>
#endif

#include "../saya/core/systring.h"
#include "../saya/core/intl.h"
#include "../saya/core/config.h"
#include "../saya/core/dialogs.h"
#include "../saya/playbackmanager.h"
#include "../saya/projectmanager.h"
#include "app.h"
#include "resources.h"
#include "debuglog.h"
#include "config.h"

extern wxFrame* CreateMainFrame();

IMPLEMENT_APP_NO_MAIN(wxApp)

const char* APP_NAME = "SayaVideoEditor";
const char* APP_VENDOR = "Rick Garcia";
const char* APP_SHOWNAME = "Saya";
const char* APP_SHOWOFFNAME = "SayaVE Ain't Yet Another Video Editor";

// ---------------------
// begin wxSayaApp::Data
// ---------------------

class wxSayaApp::Data {
    public:
        Data();
        ~Data();
        bool LoadXRCResources();
        PlaybackManager* m_PlaybackManager;
        void CreatePlaybackManager();
        int wxResult;
        syDebugLog* m_DebugLog;
};

wxSayaApp::Data::Data() :
m_PlaybackManager(0),
m_DebugLog(0)
{
    int dummy = 0;
    wxResult = wxEntryStart(dummy, (char**)0);
}

void wxSayaApp::Data::CreatePlaybackManager() {
    m_PlaybackManager = new PlaybackManager();
}

wxSayaApp::Data::~Data() {
    delete m_PlaybackManager;
    wxEntryCleanup();
}

bool wxSayaApp::Data::LoadXRCResources() {
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

// -------------------
// end wxSayaApp::Data
// -------------------

// ---------------
// begin wxSayaApp
// ---------------

wxSayaApp::wxSayaApp() :
m_Data(new Data())
{
}

wxSayaApp::~wxSayaApp() {
    delete m_Data;
}

const char* wxSayaApp::GetApplicationName() const {
    return APP_NAME;
}

const char* wxSayaApp::GetApplicationDisplayName() const {
    return APP_SHOWNAME;
}

const char* wxSayaApp::GetApplicationVendor() const {
    return APP_VENDOR;
}

const char* wxSayaApp::GetApplicationShowOffName() const {
    return APP_SHOWOFFNAME;
}

syConfig* wxSayaApp::CreateConfig() const {
    return new wxsyConfig(GetApplicationName());
}

syDebugLog* wxSayaApp::CreateDebugLog() const {
    syDebugLog* log = m_Data->m_DebugLog = new AppDebugLog;
    return log;
}

bool wxSayaApp::OnInit(int argc, const char** argv) {
    bool result = false;
    do {
        // Init Project Manager and Playback Manager.
        DebugLog(_("Initializing Resources path..."));
        syInitResourcesPaths();
        DebugLog(_("Initializing File system handlers..."));
        wxFileSystem::AddHandler(new wxZipFSHandler);
        wxFileSystem::AddHandler(new wxMemoryFSHandler);
        wxImage::AddHandler(new wxPNGHandler);
        DebugLog(_("Initializing XML Resource handlers..."));
        wxXmlResource::Get()->InitAllHandlers();

        DebugLog(_("Initializing Image handlers..."));
        wxInitAllImageHandlers();

        DebugLog("Loading resources...");
        if(!m_Data->LoadXRCResources())
            break;

        DebugLog("Initializing Project Manager...");
        ProjectManager::Get();

        if(!ProjectManager::Get()->LoadConfig()) {
            ErrorMessageBox(_("WARNING: Could not read configuration!"));
        }

        DebugLog("Initializing Playback Manager...");
        m_Data->CreatePlaybackManager();

        DebugLog("Creating main frame...");
        CreateMainFrame();

        DebugLog("Initialization finished.");
        result = true;
    }while(false);
	return result;
}

/** Exits the main loop on the next iteration. */
void wxSayaApp::Exit(bool now) {
    DebugLog("Good bye.");
    if(wxTheApp) {
        if(now) {
            wxTheApp->Exit();
        } else {
            wxTheApp->ExitMainLoop();
        }
    }
}

void wxSayaApp::OnExit() {
    wxTheApp->OnExit();
}

void wxSayaApp::Run() {
    Result = 0;
    wxTheApp->OnRun();
}

void wxSayaApp::ErrorMessageBox(const char* str) const {
    wxLogError(wxString(str,wxConvUTF8));
}

void wxSayaApp::MessageBox(const syString& message, const syString& caption) const {
    wxMessageBox(message,caption);
}

syFileDialogResult wxSayaApp::FileSelector(
    const syString& message,
    const syString& default_path,
    const syString& default_filename,
    const syString& default_extension,
    const syString& wildcard,
    int flags,
    void* parent,
    int x,
    int y)
{
    #warning TODO:Implement wxSayaApp::FileSelector
}


// -------------
// end wxSayaApp
// -------------
