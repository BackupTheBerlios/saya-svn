#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "vidproject.h"
#include <deque>
#include <wx/wx.h>
using namespace std;

bool IsAppShuttingDown();

void ShutdDownApp();

extern const wxString APP_NAME;
extern const wxString APP_VENDOR;
extern const wxString APP_SHOWNAME;

class ProjectManager
{
    public:
        ProjectManager();
        virtual ~ProjectManager();
        bool LoadProject(const wxString filename);
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

        bool m_recentfilesmodified;

    protected:
    private:
        wxString m_LastProjectDir;
        wxFrame* m_MainFrame;
};

#endif // PROJECTMANAGER_H
