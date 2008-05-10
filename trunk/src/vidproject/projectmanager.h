#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "vidproject.h"
#include <vector>
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
        bool LoadProjectFromXml(const wxString &data);
        bool LoadConfig();
        bool SaveConfig();
        const wxString GetLastProjectDir();
        static ProjectManager* Get();
        static void Unload();
        VidProject* m_project;
        vector<wxString> m_recentfiles;
        wxString m_project_xml;
        wxString m_original_xml;
        wxString m_lasterror;
    protected:
    private:
        wxString m_LastProjectDir;
};

#endif // PROJECTMANAGER_H
