/***************************************************************
 * Name:      newprojectdlg.cpp
 * Purpose:   Implements New Project Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-29
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__


#ifndef WX_PRECOMP
    #include <wx/xrc/xmlres.h>
    #include <wx/config.h>
    #include <wx/filedlg.h>
    #include <wx/aui/aui.h>
    #include <wx/splitter.h>
    #include <wx/treectrl.h>
    #include <wx/sizer.h>
    #include <wx/html/htmlwin.h>
#endif
#include "vidproject/projectmanager.h"
#include "App.h"
#include "newprojectdlg.h"

int idNewProjectDialog = XRCID("new_project_dialog");
extern int idNewProject;

BEGIN_EVENT_TABLE(NewProjectDlg, wxDialog)
END_EVENT_TABLE()

NewProjectDlg::NewProjectDlg(wxWindow* parent) : m_parent(0)
{
    if(!wxXmlResource::Get()->LoadDialog(this,parent, _T("new_project_dialog"))) {
        Destroy();
    } else {
        m_parent = parent;
        CenterOnScreen();
    }
}

NewProjectDlg::~NewProjectDlg() {
}
