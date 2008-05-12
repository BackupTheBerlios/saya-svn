#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

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
        bool CloseProject(bool force = false);
        bool LoadConfig();
        bool SaveConfig();
        void SetMainFrame(wxFrame* frame);
        const wxString GetLastProjectDir();
        void AddToRecentFiles(const wxString& s,bool fromthebeginning = true);
        void ClearRecentFiles();
        static ProjectManager* Get();
        static void Unload();
        VidProject* m_project;
        deque<wxString> m_recentfiles;
        wxString m_lasterror;

        bool m_recentfilesmodified; // Flag, resettable for refresh purposes

        void OnProjectStatusModified(); // Called whenever project's status or name has changed

    protected:
    private:
        wxString m_LastProjectDir;
        wxFrame* m_MainFrame;
};

#endif // PROJECTMANAGER_H
