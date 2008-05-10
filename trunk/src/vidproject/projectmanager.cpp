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
    m_project_xml = wxEmptyString;
    m_original_xml = wxEmptyString;
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

bool ProjectManager::LoadConfig() {
    // TODO (rick#1#): Load configuration for the project manager
    wxConfig* cfg = new wxConfig (APP_NAME);
    wxString key;

    // Read last used directory
    key = _T("paths/LastProjectDir");
    if (cfg->Exists (key))
        m_LastProjectDir = cfg->Read(key,wxEmptyString);

    delete cfg;
    return true;
}

bool ProjectManager::SaveConfig() {
    // TODO (rick#1#): Save configuration for the project manager
    wxConfig* cfg = new wxConfig (APP_NAME);

    // Save last used directory
    cfg->Write (_T("paths/LastProjectDir"),m_LastProjectDir);

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
