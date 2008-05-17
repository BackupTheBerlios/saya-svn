#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

/***************************************************************
 * Name:      projectmanager.h
 * Purpose:   Definition for the Project Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "vidproject.h"
#include <deque>
#include <wx/wx.h>
using namespace std;

bool IsAppShuttingDown();

void ShutDownApp();

extern const wxString APP_NAME;
extern const wxString APP_VENDOR;
extern const wxString APP_SHOWNAME;
extern const wxString APP_SHOWOFFNAME;
extern int idFrameUpdateTitleUI;

class ProjectManager
{
    public:
        ProjectManager();
        virtual ~ProjectManager();
        bool LoadProject(const wxString filename);
        bool LoadRecentProject(int fileno);
        bool SaveProject();
        bool SaveProjectAs(const wxString filename);
        bool SaveProjectCopy(const wxString filename);
        bool InteractiveSaveProject();
        bool InteractiveSaveProjectAs();
        bool InteractiveSaveProjectCopy();

        bool CloseProject(bool force = false);
        bool LoadConfig();
        bool SaveConfig();
        bool GetClearUndoHistoryOnSave();
        void SetMainFrame(wxFrame* frame);
        const wxString GetLastProjectDir();
        void AddToRecentFiles(const wxString& s,bool fromthebeginning = true);
        void ClearRecentFiles();
        static ProjectManager* Get();
        static void Unload();
        VidProject* GetProject();
        deque<wxString> m_recentfiles;
        deque<wxString> m_recentimports;
        wxString m_lasterror;

        bool m_recentfilesmodified; // Flag, resettable for refresh purposes
        bool m_clearundohistoryonsave;

        void OnProjectStatusModified(); // Called whenever project's status or name has changed

    protected:
    private:
        VidProject* m_project;
        wxString m_LastProjectDir;
        wxFrame* m_MainFrame;
};

#endif // PROJECTMANAGER_H
