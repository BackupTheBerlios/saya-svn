#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "projectmanager.h"
#include "vidproject.h"


ProjectManager::ProjectManager() {
    //ctor
    VidProject = 0;
    m_recentfiles.Clear();
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
}

bool ProjectManager::LoadProject(const wxString filename) {
    wxString data = wxEmptyString;
    bool result = false;
    wxFFile myfile;
    do {
        if(!wxFileExists(filename)) {
            m_lasterror.Printf(_("Error: Could not find file '%s'!"),filename);
            break;
        }
        if(!myfile.Open(filename)) {
            m_lasterror.Printf(_("Error: Could not open file '%s'!"),filename);
            break;
        }
        if(!myfile.ReadAll(&data)) {
            m_lasterror.Printf(_("Error: Could not read file '%s'!"),filename);
            break;
        }
        myfile.Close();
        result = LoadProjectFromXml(data);
        if(!result) {
            m_lasterror.Printf(_("Error: File '%s' contains invalid data!"),filename);
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
