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

#include <memory>

#include "core/intl.h"
#include "core/systring.h"
#include "core/codecplugin.h"
#include "core/config.h"
#include "core/debuglog.h"
#include "core/app.h"
#include "core/dialogs.h"
#include "core/iocommon.h"
#include "core/avcontroller.h"

#include "projectmanager.h"
#include "vidproject.h"

#include "saya_events.h"
#include "recentfileslist.h"
#include "presetmanager.h"


// For internationalization
//

static ProjectManager* TheProjectManager = 0;

// --------------------------
// begin ProjectManager::Data
//---------------------------

class ProjectManager::Data {
    public:
        /** Constructor. */
        Data(ProjectManager* parent);

        /** Destructor. */
        ~Data();

        /** Back pointer to ProjectManger */
        ProjectManager* m_Parent;

        /** The currently opened project */
        VidProject* m_Project;

        /** The last used project directory */
        syString m_LastProjectDir;

        /** The Codec Plugins directory */
        syString m_CodecPluginsDir;

        /** The last error we stumbled upon. */
        syString m_LastError;

        /** The application's main event handler (usually the main window). */
        syEvtHandler* m_EventHandler;

        /** The list of the recently-opened projects. */
        RecentFilesList m_RecentFiles;

        /** The list of the recently-imported files. */
        RecentFilesList m_RecentImports;

        /** Our Video/Audio presets manager. */
        PresetManager m_Presets;

        bool m_ClearUndoHistoryOnSave;

        void LoadCodecPlugins();
};

ProjectManager::Data::Data(ProjectManager* parent) :
m_Parent(parent),
m_Project(0),
m_LastProjectDir(""),
m_LastError(""),
m_EventHandler(0),
m_RecentFiles(9),
m_RecentImports(9),
m_ClearUndoHistoryOnSave(true)
{
    LoadCodecPlugins();
}

void ProjectManager::Data::LoadCodecPlugins() {
    CodecPlugin::RegisterPlugins(CodecPlugin::GetCodecPluginsPath());
    CodecPlugin::LoadPlugin("default");
}

ProjectManager::Data::~Data() {
    m_EventHandler = 0;
    delete m_Project;
    m_Project = 0;
    CodecPlugin::UnloadAllPlugins();
    CodecPlugin::UnregisterAllPlugins();
}

// ----------------------
// end ProjectManagerData
//-----------------------

// --------------------
// begin ProjectManager
//---------------------

ProjectManager::ProjectManager() :
m_Data(new Data(this))
{
}

ProjectManager::~ProjectManager() {
    //dtor
    TheProjectManager = 0;
    delete m_Data;
}

ProjectManager* ProjectManager::Get() {
    if(TheProjectManager == 0 && !IsAppShuttingDown()) {
        TheProjectManager = new ProjectManager();
    }
    return TheProjectManager;
}

void ProjectManager::Unload() {
    delete TheProjectManager;
}

VidProject* ProjectManager::GetProject() const {
    if(this && this->m_Data) {
        return m_Data->m_Project;
    }
    return 0;
}

bool ProjectManager::HasProject() const {
    return (m_Data->m_Project != 0);
}

const syString ProjectManager::GetLastProjectDir() const {
    return m_Data->m_LastProjectDir;;
}

bool ProjectManager::LoadConfig() {
    // Load configuration for the project manager
    syConfig* cfg = syApp::Get()->CreateConfig();
    {
        std::auto_ptr<syConfig> tmpptr(cfg);
        syString key;
        syString tmpname;

        // Read last used directory
        key = "paths/LastProjectDir";
        if (cfg->Exists(key.c_str()))
            m_Data->m_LastProjectDir = cfg->Read(key.c_str(),"");

        unsigned int i;
        m_Data->m_RecentFiles.clear();
        m_Data->m_RecentImports.clear();
        for(i = 1; i <= 9; i++) {
            key.Printf("RecentProjects/File%u",i);
            DebugLog("Reading key: " + key);
            if(cfg->Exists(key.c_str())) {
                tmpname = cfg->Read(key.c_str(),"");
                m_Data->m_RecentFiles.Add(tmpname.c_str(),false);
            }
        }
        for(i = 1; i <= 9; i++) {
            key.Printf("RecentImports/File%u",i);
            DebugLog("Reading key: " + key);
            if(cfg->Exists(key.c_str())) {
                tmpname = cfg->Read(key.c_str(),"");
                m_Data->m_RecentImports.Add(tmpname.c_str(),false);
            }
        }
        //TODO: Read in the configuration which codec plugins we must load, and load them.
        CodecPlugin::SelectPlugin("default");

        // TODO: Read in the configuration which non-codec plugins we must load, and load them.
        DebugLog(_("Loading registered plugins..."));
        CodecPlugin::LoadAllRegisteredPlugins(); // This is a workaround, loads all registered plugins.
    }
    return true;
}

bool ProjectManager::SaveConfig() {
    // Save configuration for the project manager
    syConfig* cfg = syApp::Get()->CreateConfig();
    {
        std::auto_ptr<syConfig> tmpptr(cfg);

        syString key;

        // Save Codec Plugins directory
        cfg->Write("paths/CodecPluginsDir",m_Data->m_CodecPluginsDir.c_str());

        // Save last used directory
        cfg->Write("paths/LastProjectDir",m_Data->m_LastProjectDir.c_str());


        // Save Recent Projects list

        key = "";
        unsigned int i;
        for(i = 1; i <= 9; ++i) {
            key.Printf("RecentProjects/File%u",i);
            DebugLog(key.c_str());
            if(i>m_Data->m_RecentFiles.size()) {
                cfg->Write(key.c_str(),"");
            } else {
                cfg->Write(key.c_str(),m_Data->m_RecentFiles.item(i).c_str());
            }
        }
        for(i = 1; i <= 9; ++i) {
            key.Printf("RecentImports/File%u",i);
            DebugLog(key.c_str());
            if(i>m_Data->m_RecentImports.size()) {
                cfg->Write(key.c_str(),"");
            } else {
                cfg->Write(key.c_str(),m_Data->m_RecentImports.item(i).c_str());
            }
        }
    }
    return true;
}

bool ProjectManager::LoadProject(const syString& filename) {
    syString data("");
    bool result = false;
    CloseProject(true); // Close any project we have in memory
    VidProject* prj = VidProject::Load(filename.c_str(),m_Data->m_LastError);
    if(prj != 0) {
        m_Data->m_RecentFiles.Add(filename.c_str());
        m_Data->m_LastProjectDir = ioCommon::GetPathname(filename.c_str()); // Extract last project directory from opened file path
        m_Data->m_Project = prj;
        result = true;
    } else {
        syMessageBox(m_Data->m_LastError.c_str(),_("Error loading project"), syOK | syICON_EXCLAMATION);
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
    if(fileno <= 0 || fileno > static_cast<int>(m_Data->m_RecentFiles.size())) { return false; }
    syString filename = m_Data->m_RecentFiles.item(fileno);
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

    if(m_Data->m_Project == 0) return true; // No error, since there's no file to save
    bool result = false;
    if(m_Data->m_Project->IsNew()) {
        result = InteractiveSaveProjectAs();
    } else {
        result = SaveProject();
        if(!result) {
            bool answer = false;
            syString msg = _("Couldn't save the file! Try with a different name?");
            syString caption = _("Error Saving");
            answer = syYES == syMessageBox(msg.c_str(),caption.c_str(),syYES_NO | syICON_QUESTION);
            if(answer) {
                result = InteractiveSaveProjectAs();
            }
        }
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectAs() {
    if(m_Data->m_Project == 0)
        return true;
    bool result = false;
    syString filename = syFileSelector(_("Save File as..."),"","","saya","*.saya",syFD_SAVE | syFD_OVERWRITE_PROMPT,syApp::Get()->GetTopWindow());
    if(!filename.empty()) {
        result = SaveProjectAs(filename);
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectCopy() {
    if(m_Data->m_Project == 0)
        return true;
    bool result = false;
    syString filename = syFileSelector(_("Save Copy as..."),"","","saya","*.saya",syFD_SAVE | syFD_OVERWRITE_PROMPT,syApp::Get()->GetTopWindow());
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
        int answer = syNO;
        answer = syMessageBox(_("Project has been modified. Save?\nChoose 'Cancel' "
            "to continue working on the project."),
            _("Save project?"),syYES_NO_CANCEL);
        if(answer == syYES) {
            result = InteractiveSaveProject();
            if(!result) {
                syMessageBox(_("Could not save file! Project will not be closed."), _("Info"),syOK | syICON_EXCLAMATION);
            }
        } else {
            result = (answer == syNO); // if user chooses No (don't save), return success.
            // else (user chooses Cancel) return failure.
        }
    }
    if(result) {
        delete m_Data->m_Project;
        m_Data->m_Project = 0;
    }
    OnProjectStatusModified();
    return result;
}

const char* ProjectManager::GetLastError() const {
    return m_Data->m_LastError.c_str();
}

void ProjectManager::OnProjectStatusModified() {
    if(m_Data->m_EventHandler) {
        syProjectStatusEvent event;
        syApp::Get()->PostEvent(m_Data->m_EventHandler, event);
    }
}

void ProjectManager::SetEventHandler(syEvtHandler* handler) {
    m_Data->m_EventHandler = handler;
}

/** A list of the most recently opened project files. */
RecentFilesList* ProjectManager::GetRecentFiles() const {
    if(!this || !m_Data) return 0;
    return &(m_Data->m_RecentFiles);
}

/** A list of the most recently imported clips. */
RecentFilesList* ProjectManager::GetRecentImports() const {
    if(!this || !m_Data) return 0;
    return &(m_Data->m_RecentImports);
}

/** Retutns the Audio/Video Presets handler */
PresetManager* ProjectManager::GetPresets() const {
    return &(m_Data->m_Presets);
}

/** Tells whether to clear the Undo History after the project's successfully saved. */
void ProjectManager::SetClearUndoHistoryOnSave(bool flag) {
    m_Data->m_ClearUndoHistoryOnSave = flag;
}

/** Gets the "clear undo history on save" status. */
bool ProjectManager::GetClearUndoHistoryOnSave() const {
    return m_Data->m_ClearUndoHistoryOnSave;
}

unsigned int ProjectManager::ImportFile(const syString& filename, syString& errortext) {
    unsigned int result = 0;
    if(m_Data->m_Project) {
        result = m_Data->m_Project->ImportFile(filename, errortext);
        if(result) {
            m_Data->m_RecentImports.Add(filename.c_str());
        }
    }
    return result;
}

const AVResources* ProjectManager::GetResources() const {
    const AVResources* result = 0;
    if(m_Data->m_Project) {
        result = m_Data->m_Project->GetResources();
    }
    return result;
}

unsigned int ProjectManager::GetNewResourceId() {
    unsigned int result = 0;
    if(m_Data->m_Project) {
        result = m_Data->m_Project->GetNewResourceId();
    }
    return result;
}

// ------------------
// end ProjectManager
//-------------------
