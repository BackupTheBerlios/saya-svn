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
    #include <wx/filedlg.h>
#endif

#include "../saya/core/systring.h"
#include "../saya/core/intl.h"
#include "../saya/core/config.h"
#include "../saya/core/dialogs.h"
#include "../saya/core/eventqueue.h"
#include "../saya/core/avdevice.h"
#include "../saya/playbackmanager.h"
#include "../saya/projectmanager.h"

#include "app.h"
#include "resources.h"
#include "debuglog.h"
#include "config.h"

int idsyEventpassed = wxNewId();

extern wxFrame* CreateMainFrame();

class wxMyApp : public wxApp {
    public:
        void OnSayaEvent(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxMyApp, wxApp)
    EVT_MENU(idsyEventpassed, wxMyApp::OnSayaEvent)
END_EVENT_TABLE()

void wxMyApp::OnSayaEvent(wxCommandEvent& event) {
    syEvtQueue::ProcessNextEvent();
}

IMPLEMENT_APP_NO_MAIN(wxMyApp)

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
        int wxResult;
        syDebugLog* m_DebugLog;
        void* m_TopWindow;
};

wxSayaApp::Data::Data() :
m_DebugLog(0),
m_TopWindow(0)
{
    int dummy = 0;
    wxResult = wxEntryStart(dummy, (char**)0);
}

wxSayaApp::Data::~Data() {
    PlaybackManager::Unload();
    ProjectManager::Unload();
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
        DebugLog(_("Initializing Playback Manager..."));
        PlaybackManager::Get();

        DebugLog(_("Initializing Project Manager..."));
        ProjectManager::Get();

        DebugLog(_("Initializing Resources path..."));
        syInitResourcesPaths();

        if(!ProjectManager::Get()->LoadConfig()) {
            ErrorMessageBox(_("WARNING: Could not read configuration!"));
        }


        DebugLog(_("Initializing File system handlers..."));
        wxFileSystem::AddHandler(new wxZipFSHandler);
        wxFileSystem::AddHandler(new wxMemoryFSHandler);
        wxImage::AddHandler(new wxPNGHandler);
        DebugLog(_("Initializing XML Resource handlers..."));
        wxXmlResource::Get()->InitAllHandlers();

        DebugLog(_("Initializing Image handlers..."));
        wxInitAllImageHandlers();

        DebugLog(_("Loading resources..."));
        if(!m_Data->LoadXRCResources())
            break;

        DebugLog(_("Creating main frame..."));
        CreateMainFrame();

        DebugLog(_("Initialization finished."));
        result = true;
    }while(false);
	return result;
}

/** Exits the main loop on the next iteration. */
void wxSayaApp::Exit(bool now) {
    AVDevice::ShutDownAll();
    if(wxTheApp) {
        if(now) {
            wxTheApp->Exit();
        } else {
            wxTheApp->ExitMainLoop();
        }
    }
    DebugLog(_("Good bye."));
}

void wxSayaApp::OnExit() {
    ProjectManager::Get()->SaveConfig();
    ProjectManager::Get()->CloseProject(true);
    wxTheApp->OnExit();
}

void wxSayaApp::Run() {
    Result = 0;
    wxTheApp->MainLoop();
}

int wxSayaApp::MessageBox(const syString& message, const syString& caption,unsigned int flags,void* parent) const {
    return wxMessageBox(message, caption, flags, static_cast<wxWindow*>(parent));
}

void wxSayaApp::ErrorMessageBox(const syString& message) const {
    wxLogError(wxString(message));
}

void wxSayaApp::LogStatus(const syString& message) const {
    wxLogStatus(wxString(message));
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
    int y) const
{
    syFileDialogResult result;
    if(flags & syFD_MULTIPLE) {
        wxFileDialog mydialog((wxWindow*)parent, message, default_path, default_filename, wildcard, flags, wxPoint(x, y));
        int resultcode = mydialog.ShowModal();
        if(resultcode == wxID_OK) {
            wxArrayString chosenfiles;
            mydialog.GetFilenames(chosenfiles);
            for(unsigned int i = 0, imax = chosenfiles.GetCount(); i < imax; ++i) {
                result.AddFile(syString(chosenfiles[i]));
            }
        }
    } else {
        syString resultingfilename(wxFileSelector(wxString(message), wxString(default_path), wxString(default_filename), wxString(default_extension), wxString(wildcard), flags, static_cast<wxWindow*>(parent), x, y));
        if(!resultingfilename.empty()) {
            result.AddFile(resultingfilename);
        }
    }
    return result;
}

void wxSayaApp::SetTopWindow(void* window) {
    m_Data->m_TopWindow = window;
    if(wxTheApp) {
        wxTheApp->SetTopWindow(static_cast<wxWindow*>(window));
    }
}

void* wxSayaApp::GetTopWindow() const {
    return m_Data->m_TopWindow;
}

bool wxSayaApp::IsMainLoopRunning() const {
    return wxTheApp->IsMainLoopRunning();
}

void wxSayaApp::PostEvent(syEvtHandler* handler, syEvent& event) {
    if(!handler || IsAppShuttingDown()) return;
    syEvtQueue::PostEvent(handler, event);
    wxCommandEvent tmpevent(wxEVT_COMMAND_MENU_SELECTED, idsyEventpassed);
    wxPostEvent(wxTheApp, tmpevent); // This will enable wxWidgets to wake up with Saya events just as with wxWidgets events.
}

/** Wakes up the main thread to begin event processing. */
void wxSayaApp::WakeUpIdle() {
    wxWakeUpIdle();
}

// -------------
// end wxSayaApp
// -------------
