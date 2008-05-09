#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "vidproject.h"
#include <vector>
#include <wx/wx.h>
using namespace std;

class ProjectManager
{
    public:
        ProjectManager();
        virtual ~ProjectManager();
        bool LoadProject(const wxString filename);
        bool LoadProjectFromXml(const wxString &data);
        VidProject* m_project;
        vector<wxString> m_recentfiles;
        wxString m_project_xml;
        wxString m_original_xml;
        wxString m_lasterror;
    protected:
    private:
};

#endif // PROJECTMANAGER_H
