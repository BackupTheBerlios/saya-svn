#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "projectmanager.h"
#include "vidproject.h"
#include <wx/wx.h>
#include <wx/ffile.h>

static bool s_IsAppShuttingDown = false;
const wxString APP_NAME = _T("SayaVideoEditor");
const wxString APP_VENDOR = _T("Rick Garcia");
const wxString APP_SHOWNAME = _T("Saya");



bool IsAppShuttingDown() {
    return s_IsAppShuttingDown;
}

void ShutdDownApp() {
    s_IsAppShuttingDown = true;
}

static ProjectManager* TheProjectManager = NULL;

ProjectManager::ProjectManager() {
    //ctor
    m_project = 0;
    m_recentfiles.clear();
    m_recentfilesmodified = true;
    m_project_xml = wxEmptyString;
    m_original_xml = wxEmptyString;
    m_MainFrame = NULL;
}

void ProjectManager::SetMainFrame(wxFrame* frame) {
    m_MainFrame = frame;
}

ProjectManager* ProjectManager::Get() {
    if(TheProjectManager == NULL && !IsAppShuttingDown()) {
        TheProjectManager = new ProjectManager();
    }
    return TheProjectManager;
}

void ProjectManager::Unload() {
    if(TheProjectManager != NULL) {
        delete TheProjectManager;
        TheProjectManager = NULL;
    }
}

const wxString ProjectManager::GetLastProjectDir() {
    return m_LastProjectDir;
}

bool ProjectManager::LoadProjectFromXml(const wxString &data) {
    bool IsOk = false;
    VidProject *prj = new VidProject;

    IsOk = prj->LoadFromXml(data);
    if(!IsOk) {
        delete prj;
    } else {
        if(m_project) {
            delete m_project;
        }
        m_project = prj;
        m_project_xml = data;
        m_original_xml = data;
    }
    return IsOk;
}

void ProjectManager::AddToRecentFiles(const wxString& s,bool fromthebeginning) {

    if(s == wxEmptyString)
        return;

    if(!fromthebeginning && m_recentfiles.size() >= 9) {
        return; // Queue full
    }

    // First, check if it's in the list
    size_t i;
    for(i = 0; i < m_recentfiles.size(); i++) {
        if(s == m_recentfiles[i]) {
            return; // Found
        }
    }

    // Finally, add it
    if(fromthebeginning) {
        m_recentfiles.push_front(s); // Add to the beginning
        if(m_recentfiles.size() > 9) {
            m_recentfiles.pop_back();
        }
    } else {
        m_recentfiles.push_back(s); // Add to the end
    }
    m_recentfilesmodified = true;
}

void ProjectManager::ClearRecentFiles() {
    m_recentfiles.clear();
    m_recentfilesmodified = true;
}

bool ProjectManager::LoadConfig() {
    // TODO (rick#1#): Load configuration for the project manager
    wxConfig* cfg = new wxConfig (APP_NAME);
    wxString key;
    wxString tmpname;

    // Read last used directory
    key = _T("paths/LastProjectDir");
    if (cfg->Exists(key))
        m_LastProjectDir = cfg->Read(key,wxEmptyString);
    key = _T("RecentProjects");
    int i;
    for(i = 1; i <= 9; i++) {
        key.Printf(_T("RecentProjects/File%d"),i);
        if(cfg->Exists(key)) {
            tmpname = cfg->Read(key,wxEmptyString);
            AddToRecentFiles(tmpname,false);
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::SaveConfig() {
    // TODO (rick#1#): Save configuration for the project manager
    wxConfig* cfg = new wxConfig (APP_NAME);
    wxString key;

    // Save last used directory
    cfg->Write (_T("paths/LastProjectDir"),m_LastProjectDir);

    // Save Recent Projects list

    size_t i;
    for(i = 0; i < 9; i++) {
        key.Printf(_T("RecentProjects/File%d"),i+1);
        if(i>=m_recentfiles.size()) {
            cfg->Write(key,wxEmptyString);
        } else {
            cfg->Write(key,m_recentfiles[i]);
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::LoadProject(const wxString filename) {
    wxString data = wxEmptyString;
    bool result = false;
    wxFFile myfile;
    do {
        if(!wxFileExists(filename)) {
            m_lasterror.Printf(_("Error: Could not find file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.Open(filename)) {
            m_lasterror.Printf(_("Error: Could not open file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.ReadAll(&data)) {
            m_lasterror.Printf(_("Error: Could not read file '%s'!"),filename.c_str());
            break;
        }
        myfile.Close();
        result = LoadProjectFromXml(data);
        if(!result) {
            m_lasterror.Printf(_("Error: File '%s' contains invalid data!"),filename.c_str());
        } else {
            wxFileName fullname;
            fullname.Assign(filename);
            m_LastProjectDir = fullname.GetPath(); // Extract last project directory from opened file path
            AddToRecentFiles(filename);
        }
    } while(false);

    if(myfile.IsOpened()) {
        myfile.Close();
    }

    return result;
}

ProjectManager::~ProjectManager() {
    //dtor
    SaveConfig();
}
