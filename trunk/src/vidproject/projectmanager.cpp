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

#include "projectmanager.h"
#include "vidproject.h"
#include <wx/wx.h>
#include <wx/ffile.h>
#include <wx/filedlg.h>

static bool s_IsAppShuttingDown = false;
const wxString APP_NAME = _T("SayaVideoEditor");
const wxString APP_VENDOR = _T("Rick Garcia");
const wxString APP_SHOWNAME = _T("Saya");
const wxString APP_SHOWOFFNAME = _T("Saya - Swift audiovisual Authoring for You and Anyone");

int idFrameUpdateTitleUI = XRCID("idUpdateTitleUI");

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
    m_recentfilesmodified = true;
    m_MainFrame = NULL;
    m_clearundohistoryonsave = true;
}

bool ProjectManager::GetClearUndoHistoryOnSave() {
    return m_clearundohistoryonsave;
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

VidProject* ProjectManager::GetProject() {
    return m_project;
}
const wxString ProjectManager::GetLastProjectDir() {
    return m_LastProjectDir;
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
    CloseProject(true); // Close any project we have in memory
    VidProject* prj = VidProject::Load(filename,m_lasterror);
    if(prj != NULL) {
        wxFileName fullname;
        fullname.Assign(filename);
        m_LastProjectDir = fullname.GetPath(); // Extract last project directory from opened file path
        AddToRecentFiles(filename);
        m_project = prj;
        result = true;
    } else {
        wxMessageBox(m_lasterror,_("Error loading project"),wxOK | wxICON_ERROR, m_MainFrame);
    }
    OnProjectStatusModified();
    return result;
}

bool ProjectManager::LoadRecentProject(int fileno) {
    int maxfileno = m_recentfiles.size();
    if(fileno < 1)
        fileno = 1;
    if(fileno >= maxfileno)
        fileno = maxfileno;
    if(!fileno)
        return false; //
    fileno--; // Zero based now
    bool result = LoadProject(m_recentfiles[fileno]);
    OnProjectStatusModified();
    return result;
}

bool ProjectManager::SaveProject() {
    if(!m_project)
        return false;
    return m_project->Save(); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectAs(const wxString filename) {
    if(!m_project)
        return false;
    return m_project->SaveAs(filename); // Title update is invoked by the project saving method
}

bool ProjectManager::SaveProjectCopy(const wxString filename) {
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
            int answer = wxMessageBox(_("Couldn't save the file! Try with a different name?"),_("Error saving"),wxYES_NO | wxICON_EXCLAMATION,m_MainFrame);
            if(answer == wxYES) {
                result = InteractiveSaveProjectAs();
            }
        }
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectAs() {
    if(m_project == NULL)
        return true;
    // TODO (rick#1#): Use default project directory for saving
    // Show file save-as dialog and get filename, with overwrite prompt.

    wxFileDialog mydialog(m_MainFrame,_("Save file as..."),wxEmptyString,wxEmptyString,_T("*.saya"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    bool result = false;
    if(mydialog.ShowModal() == wxID_OK) {
        wxString filename = mydialog.GetPath(); // Gets full path including filename
        result = SaveProjectAs(filename);
    }
    return result;
}

bool ProjectManager::InteractiveSaveProjectCopy() {
    VidProject* prj = ProjectManager::Get()->GetProject();
    if(prj == NULL)
        return true;
    // TODO (rick#1#): Use default project directory for saving
    // Show file save-as dialog and get filename, with overwrite prompt.
    wxFileDialog mydialog(m_MainFrame,_("Save Copy As"),wxEmptyString,wxEmptyString,_T("*.saya"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    bool result = false;
    if(mydialog.ShowModal() == wxID_OK) {
        wxString filename = mydialog.GetPath(); // Gets full path including filename
        result = ProjectManager::Get()->SaveProjectCopy(filename);
    }
    return result;
}

bool ProjectManager::CloseProject(bool force) {
    if(!m_project)
        return true;
    bool result = (force || !(m_project->IsModified()));
    // If not modified (or forced), return success.
    if(!result) {
        int answer = wxMessageBox(_("Project has been modified. Save?\nChoose 'Cancel' "
        "to continue working on the project."),
        _("Save project?"),wxYES_NO | wxCANCEL | wxICON_QUESTION,m_MainFrame);
        if(answer == wxYES) {
            result = InteractiveSaveProject();
            if(!result) {
                wxMessageBox(_("Could not save file! Project will not be closed."),
                _("Info"),wxOK | wxICON_INFORMATION,m_MainFrame);
            }
        } else {
            result = (answer == wxNO); // if user chooses No (don't save), return success.
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
    wxUpdateUIEvent event(idFrameUpdateTitleUI);
    if(m_MainFrame) {
        wxPostEvent(m_MainFrame,event);
    }
}

ProjectManager::~ProjectManager() {
    //dtor
    SaveConfig();
    CloseProject(true);
}
