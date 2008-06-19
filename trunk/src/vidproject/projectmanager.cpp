/***************************************************************
 * Name:      projectmanager.cpp
 * Purpose:   Implementation of the Project Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "../iomgr/iocommon.h"
#include "projectmanager.h"
#include "vidproject.h"
#include <string>

// For internationalization
#include <libintl.h>
//

using namespace std;

static bool s_IsAppShuttingDown = false;
const std::string APP_NAME = "SayaVideoEditor";
const std::string APP_VENDOR = "Rick Garcia";
const std::string APP_SHOWNAME = "Saya";
const std::string APP_SHOWOFFNAME = "Saya - Swift audiovisual Authoring for You and Anyone";

bool IsAppShuttingDown() {
    return s_IsAppShuttingDown;
}

void ShutDownApp() {
    s_IsAppShuttingDown = true;
}

static ProjectManager* TheProjectManager = NULL;

ProjectManager::ProjectManager() {
    //ctor
    m_project = 0;
    m_recentfiles.clear();
    m_evthandler = NULL;
    m_logger = NULL;
    m_configprovider = NULL;
    m_clearundohistoryonsave = true;
    m_recentfilesmodcounter = 1;
    m_recentimportsmodcounter = 1;

}

void ProjectManager::SetConfigProvider(sayaConfigProvider* provider) {
    m_configprovider = provider;
}

bool ProjectManager::GetClearUndoHistoryOnSave() {
    return m_clearundohistoryonsave;
}

void ProjectManager::SetClearUndoHistoryOnSave(bool flag) {
    m_clearundohistoryonsave = flag;
}


void ProjectManager::SetEventHandler(sayaEvtHandler* handler) {
    m_evthandler = handler;
}

void ProjectManager::SetDebugLogger(sayaDebugLogger* logger) {
    m_logger = logger;
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

VidProject* ProjectManager::GetProject() {
    return m_project;
}

bool ProjectManager::HasProject() {
    return (m_project != NULL);
}

const std::string ProjectManager::GetLastProjectDir() {
    return m_lastprojectdir;
}

void ProjectManager::AddToRecentFiles(const std::string& s,bool fromthebeginning) {

    if(s.empty())
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
    m_recentfilesmodcounter = (m_recentfilesmodcounter + 1) & 0x3FFFFFFF;
}

void ProjectManager::AddToRecentImports(const std::string& s,bool fromthebeginning) {

    if(s.empty())
        return;

    if(!fromthebeginning && m_recentimports.size() >= 9) {
        return; // Queue full
    }

    // First, check if it's in the list
    size_t i;
    for(i = 0; i < m_recentimports.size(); i++) {
        if(s == m_recentimports[i]) {
            return; // Found
        }
    }

    // Finally, add it
    if(fromthebeginning) {
        m_recentimports.push_front(s); // Add to the beginning
        if(m_recentimports.size() > 9) {
            m_recentimports.pop_back();
        }
    } else {
        m_recentimports.push_back(s); // Add to the end
    }
    m_recentimportsmodcounter = (m_recentimportsmodcounter + 1) & 0x3FFFFFFF;
}


void ProjectManager::ClearRecentFiles() {
    m_recentfiles.clear();
    m_recentfilesmodcounter = (m_recentfilesmodcounter + 1) & 0x3FFFFFFF;
}

void ProjectManager::ClearRecentImports() {
    m_recentimports.clear();
    m_recentimportsmodcounter = (m_recentimportsmodcounter + 1) & 0x3FFFFFFF;
}

unsigned int ProjectManager::GetRecentFilesModCounter() {
    return m_recentfilesmodcounter;
}

unsigned int ProjectManager::GetRecentImportsModCounter() {
    return m_recentimportsmodcounter;

}



bool ProjectManager::LoadConfig() {
    // TODO (rick#1#): Load configuration for the project manager
    if(!m_configprovider) {
        return false; // ERROR!
    }
    sayaConfig* cfg = m_configprovider->Create(APP_NAME);
    std::string key;
    std::string tmpname;

    // Read last used directory
    key = "paths/LastProjectDir";
    if (cfg->Exists(key))
        m_lastprojectdir = cfg->Read(key,"");
    unsigned int i;
    for(i = 1; i <= 9; i++) {
        key = ioCommon::Printf("RecentProjects/File%u",i);
        if(m_logger != NULL) {
            m_logger->DebugLog(string("Reading key: ") + key);
        }
        if(cfg->Exists(key)) {
            tmpname = cfg->Read(key,"");
            AddToRecentFiles(tmpname,false);
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::SaveConfig() {
    // TODO (rick#1#): Save configuration for the project manager
    if(!m_configprovider) {
        return false; // ERROR!
    }
    sayaConfig* cfg = m_configprovider->Create(APP_NAME);
    std::string key;

    // Save last used directory
    cfg->Write("paths/LastProjectDir",m_lastprojectdir);

    // Save Recent Projects list

    key = "";
    unsigned int i;
    for(i = 0; i < 9; i++) {
        key = ioCommon::Printf("RecentProjects/File%u",i);
        if(m_logger) {
            m_logger->DebugLog(key.c_str());
        }
        if(i>=m_recentfiles.size()) {
            cfg->Write(key,"");
        } else {
            cfg->Write(key,m_recentfiles[i]);
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::LoadProject(const std::string filename) {
    std::string data("");
    bool result = false;
    CloseProject(true); // Close any project we have in memory
    VidProject* prj = VidProject::Load(filename,m_lasterror);
    if(prj != NULL) {
        AddToRecentFiles(filename);
        m_lastprojectdir = ioCommon::GetPathname(filename); // Extract last project directory from opened file path
        m_project = prj;
        result = true;
    } else {
        if(m_evthandler) {
            m_evthandler->ErrorMessageBox(m_lasterror.c_str(),gettext("Error loading project"));
        }
    }
    OnProjectStatusModified();
    return result;
}

const std::string ProjectManager::GetRecentProjectName(int fileno) {
    int maxfileno = m_recentfiles.size();
    if(fileno < 1)
        fileno = 1;
    if(fileno >= maxfileno)
        fileno = maxfileno;
    if(!fileno)
        return ""; //
    fileno--; // Zero based now
    return m_recentfiles[fileno];
}

const std::string ProjectManager::GetOfflineProjectTitle(const std::string& filename) {
    return VidProject::GetOfflineProjectTitle(filename);
}

const std::string ProjectManager::GetOfflineProjectTitle(const char* filename) {
    return VidProject::GetOfflineProjectTitle(std::string(filename));
}

std::string ProjectManager::GetRecentImportName(int fileno) {
    int maxfileno = m_recentimports.size();
    if(fileno < 1)
        fileno = 1;
    if(fileno >= maxfileno)
        fileno = maxfileno;
    if(!fileno)
        return ""; //
    fileno--; // Zero based now
    return m_recentimports[fileno];
}

bool ProjectManager::LoadRecentProject(int fileno) {
    std::string filename = GetRecentProjectName(fileno);
    if(filename.empty())
        return false;
    bool result = LoadProject(filename);
    OnProjectStatusModified();
    return result;
}

bool ProjectManager::SaveProject() {
    if(!m_project)
        return false;
    return m_project->Save(); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectAs(const std::string filename) {
    if(!m_project)
        return false;
    return m_project->SaveAs(filename); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectCopy(const std::string filename) {
    if(!m_project)
        return false;
    return m_project->SaveCopy(filename); // Title update is invoked by the project saving method
}

bool ProjectManager::InteractiveSaveProject() {

    if(m_project == NULL) return true; // No error, since there's no file to save
    bool result = false;
    if(m_project->IsNew()) {
        result = InteractiveSaveProjectAs();
    } else {
        result = SaveProject();
        if(!result) {
            bool answer = false;
            std::string msg = gettext("Couldn't save the file! Try with a different name?");
            std::string caption = gettext("Error Saving");
            if(m_evthandler) {
                answer = m_evthandler->YesNoMessageBox(msg.c_str(),caption.c_str(),true);
            }
            if(answer) {
                result = InteractiveSaveProjectAs();
            }
        }
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectAs() {
    if(m_project == NULL)
        return true;
    bool result = false;
    std::string filename("");
    if(m_evthandler != NULL) {
        filename = m_evthandler->ShowDialogSaveProjectAs();
    }
    if(!filename.empty()) {
        result = SaveProjectAs(filename);
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectCopy() {
    if(m_project == NULL)
        return true;
    bool result = false;
    std::string filename("");
    if(m_evthandler != NULL) {
        filename = m_evthandler->ShowDialogSaveProjectCopyAs();
    }
    if(!filename.empty()) {
        result = SaveProjectCopy(filename);
    }
    return result;
}

bool ProjectManager::CloseProject(bool force) {
    if(!m_project)
        return true;
    bool result = (force || !(m_project->IsModified()));
    // If not modified (or forced), return success.
    if(!result) {
        sayaYesNoCancel answer = sayaNo;
        if(m_evthandler != NULL) {
            m_evthandler->YesNoCancelMessageBox(gettext("Project has been modified. Save?\nChoose 'Cancel' "
            "to continue working on the project."),
            gettext("Save project?"),false);
        }
        if(answer == sayaYes) {
            result = InteractiveSaveProject();
            if(!result) {
                if(m_evthandler != NULL) {
                    m_evthandler->ErrorMessageBox(gettext("Could not save file! Project will not be closed."), gettext("Info"));
                }
            }
        } else {
            result = (answer == sayaNo); // if user chooses No (don't save), return success.
            // else (user chooses Cancel) return failure.
        }
    }
    if(result) {
        delete m_project;
        m_project = NULL;
    }
    OnProjectStatusModified();
    return result;
}

void ProjectManager::OnProjectStatusModified() {
    if(m_evthandler) {
        m_evthandler->ProcessSayaEvent(sayaevt_ProjectStatusChanged);
    }
}

ProjectManager::~ProjectManager() {
    //dtor
    SaveConfig();
    CloseProject(true);
}
