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

#include "core/iocommon.h"
#include "core/avcontroller.h"
#include "projectmanager.h"
#include "vidproject.h"
#include <list>
#include <map>

#include "core/systring.h"
#include "sayaconfig.h"
#include "configprovider.h"
#include "sayadebuglogger.h"
#include "sayaevthandler.h"
#include "recentfileslist.h"
#include "presetmanager.h"


// For internationalization
#include <libintl.h>
//

using namespace std;

static bool s_IsAppShuttingDown = false;
const char* APP_NAME = "SayaVideoEditor";
const char* APP_VENDOR = "Rick Garcia";
const char* APP_SHOWNAME = "Saya";
const char* APP_SHOWOFFNAME = "SayaVE Ain't Yet Another Video Editor";

bool IsAppShuttingDown() {
    return s_IsAppShuttingDown;
}

void ShutDownApp() {
    s_IsAppShuttingDown = true;
}

static ProjectManager* TheProjectManager = NULL;

// ------------------------
// begin ProjectManagerData
//-------------------------

class ProjectManagerData {
    public:
        /** Constructor. */
        ProjectManagerData(ProjectManager* parent);

        /** Destructor. */
        ~ProjectManagerData();

        /** The currently opened project */
        VidProject* m_Project;

        /** The last used project directory */
        syString m_LastProjectDir;

        /** A pointer to the program's event handler */
        sayaEvtHandler* m_EvtHandler;

        /** A pointer to the program's config provider */
        SayaConfigProvider* m_ConfigProvider;

        ProjectManager* m_Parent;

        syString m_LastError;
};

ProjectManagerData::ProjectManagerData(ProjectManager* parent) : m_Parent(parent) {
}

ProjectManagerData::~ProjectManagerData() {
}

// ----------------------
// end ProjectManagerData
//-----------------------

// --------------------
// begin ProjectManager
//---------------------

ProjectManager::ProjectManager() {
    //ctor
    m_Data = new ProjectManagerData(this);
    m_Data->m_Project = 0;
    m_RecentFiles = new RecentFilesList(9);
    m_RecentImports = new RecentFilesList(9);
    m_Presets = new PresetManager;
    m_Data->m_EvtHandler = NULL;
    m_Data->m_ConfigProvider = NULL;
    m_ClearUndoHistoryOnSave = true;

}

ProjectManager::~ProjectManager() {
    //dtor
    SaveConfig();
    CloseProject(true);
    delete m_Presets;
    delete m_RecentImports;
    delete m_RecentFiles;
    delete m_Data;
}

void ProjectManager::SetConfigProvider(SayaConfigProvider* provider) {
    m_Data->m_ConfigProvider = provider;
}

void ProjectManager::SetEventHandler(sayaEvtHandler* handler) {
    m_Data->m_EvtHandler = handler;
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
    return m_Data->m_Project;
}

bool ProjectManager::HasProject() {
    return (m_Data->m_Project != NULL);
}

const syString ProjectManager::GetLastProjectDir() {
    return m_Data->m_LastProjectDir;;
}

bool ProjectManager::LoadConfig() {
    // TODO (rick#1#): Load configuration for the project manager
    if(!m_Data->m_ConfigProvider) {
        return false; // ERROR!
    }
    SayaConfig* cfg = m_Data->m_ConfigProvider->Create(APP_NAME);
    syString key;
    syString tmpname;

    // Read last used directory
    key = "paths/LastProjectDir";
    if (cfg->Exists(key.c_str()))
        m_Data->m_LastProjectDir = cfg->Read(key.c_str(),"");
    unsigned int i;
    for(i = 1; i <= 9; i++) {
        key = ioCommon::Printf("RecentProjects/File%u",i);
        DebugLog(syString("Reading key: ") + key);
        if(cfg->Exists(key.c_str())) {
            tmpname = cfg->Read(key.c_str(),"");
            m_RecentFiles->Add(tmpname.c_str(),false);
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::SaveConfig() {
    // TODO (rick#1#): Save configuration for the project manager
    if(!m_Data->m_ConfigProvider) {
        return false; // ERROR!
    }
    SayaConfig* cfg = m_Data->m_ConfigProvider->Create(APP_NAME);
    syString key;

    // Save last used directory
    cfg->Write("paths/LastProjectDir",m_Data->m_LastProjectDir.c_str());

    // Save Recent Projects list

    key = "";
    unsigned int i;
    for(i = 1; i <= 9; ++i) {
        key = ioCommon::Printf("RecentProjects/File%u",i);
        DebugLog(key.c_str());
        if(i>m_RecentFiles->size()) {
            cfg->Write(key.c_str(),"");
        } else {
            cfg->Write(key.c_str(),m_RecentFiles->item(i).c_str());
        }
    }

    delete cfg;
    return true;
}

bool ProjectManager::LoadProject(const syString& filename) {
    syString data("");
    bool result = false;
    CloseProject(true); // Close any project we have in memory
    VidProject* prj = VidProject::Load(filename.c_str(),m_Data->m_LastError);
    if(prj != NULL) {
        m_RecentFiles->Add(filename.c_str());
        m_Data->m_LastProjectDir = ioCommon::GetPathname(filename.c_str()); // Extract last project directory from opened file path
        m_Data->m_Project = prj;
        result = true;
    } else {
        if(m_Data->m_EvtHandler) {
            m_Data->m_EvtHandler->ErrorMessageBox(m_Data->m_LastError.c_str(),gettext("Error loading project"));
        }
    }
    OnProjectStatusModified();
    return result;
}

const syString ProjectManager::GetOfflineProjectTitle(const syString& filename) {
    return VidProject::GetOfflineProjectTitle(filename.c_str());
}

const syString ProjectManager::GetOfflineProjectTitle(const char* filename) {
    return VidProject::GetOfflineProjectTitle(filename);
}

bool ProjectManager::LoadRecentProject(int fileno) {
    if(fileno <= 0 || fileno > (int)m_RecentFiles->size()) { return false; }
    syString filename = m_RecentFiles->item(fileno);
    if(filename.empty())
        return false;
    bool result = LoadProject(filename);
    OnProjectStatusModified();
    return result;
}

bool ProjectManager::SaveProject() {
    if(!m_Data->m_Project)
        return false;
    return m_Data->m_Project->Save(); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectAs(const syString& filename) {
    if(!m_Data->m_Project)
        return false;
    return m_Data->m_Project->SaveAs(filename); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectCopy(const syString& filename) {
    if(!m_Data->m_Project)
        return false;
    return m_Data->m_Project->SaveCopy(filename); // Title update is invoked by the project saving method
}

bool ProjectManager::InteractiveSaveProject() {

    if(m_Data->m_Project == NULL) return true; // No error, since there's no file to save
    bool result = false;
    if(m_Data->m_Project->IsNew()) {
        result = InteractiveSaveProjectAs();
    } else {
        result = SaveProject();
        if(!result) {
            bool answer = false;
            syString msg = gettext("Couldn't save the file! Try with a different name?");
            syString caption = gettext("Error Saving");
            if(m_Data->m_EvtHandler) {
                answer = m_Data->m_EvtHandler->YesNoMessageBox(msg.c_str(),caption.c_str(),true);
            }
            if(answer) {
                result = InteractiveSaveProjectAs();
            }
        }
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectAs() {
    if(m_Data->m_Project == NULL)
        return true;
    bool result = false;
    syString filename("");
    if(m_Data->m_EvtHandler != NULL) {
        filename = m_Data->m_EvtHandler->ShowDialogSaveProjectAs();
    }
    if(!filename.empty()) {
        result = SaveProjectAs(filename);
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectCopy() {
    if(m_Data->m_Project == NULL)
        return true;
    bool result = false;
    syString filename("");
    if(m_Data->m_EvtHandler != NULL) {
        filename = m_Data->m_EvtHandler->ShowDialogSaveProjectCopyAs();
    }
    if(!filename.empty()) {
        result = SaveProjectCopy(filename);
    }
    return result;
}

bool ProjectManager::CloseProject(bool force) {
    if(!m_Data->m_Project)
        return true;
    bool result = (force || !(m_Data->m_Project->IsModified()));
    // If not modified (or forced), return success.
    if(!result) {
        sayaYesNoCancel answer = sayaNo;
        if(m_Data->m_EvtHandler != NULL) {
            m_Data->m_EvtHandler->YesNoCancelMessageBox(gettext("Project has been modified. Save?\nChoose 'Cancel' "
            "to continue working on the project."),
            gettext("Save project?"),false);
        }
        if(answer == sayaYes) {
            result = InteractiveSaveProject();
            if(!result) {
                if(m_Data->m_EvtHandler != NULL) {
                    m_Data->m_EvtHandler->ErrorMessageBox(gettext("Could not save file! Project will not be closed."), gettext("Info"));
                }
            }
        } else {
            result = (answer == sayaNo); // if user chooses No (don't save), return success.
            // else (user chooses Cancel) return failure.
        }
    }
    if(result) {
        delete m_Data->m_Project;
        m_Data->m_Project = NULL;
    }
    OnProjectStatusModified();
    return result;
}

const char* ProjectManager::GetLastError() const {
    return m_Data->m_LastError.c_str();
}

void ProjectManager::OnProjectStatusModified() {
    if(m_Data->m_EvtHandler) {
        m_Data->m_EvtHandler->ProcessSayaEvent(sayaevt_ProjectStatusChanged);
    }
}

// ------------------
// end ProjectManager
//-------------------
