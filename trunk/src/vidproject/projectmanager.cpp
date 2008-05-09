#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "projectmanager.h"
#include "vidproject.h"
#include <wx/wx.h>
#include <wx/ffile.h>

ProjectManager::ProjectManager() {
    //ctor
    m_project = 0;
    m_recentfiles.clear();
    m_project_xml = wxEmptyString;
    m_original_xml = wxEmptyString;
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
        }
    } while(false);

    if(myfile.IsOpened()) {
        myfile.Close();
    }

    return result;
}

ProjectManager::~ProjectManager() {
    //dtor
}
