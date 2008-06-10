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
int idNewPrjAVSettingsPanel = XRCID("idNewPrjAVSettingsPanel");
int idNewPrjAVSettings_width = XRCID("idNewPrjAVSettings_width");
int idNewPrjAVSettings_height = XRCID("idNewPrjAVSettings_height");
int idNewPrjAVSettings_fps = XRCID("idNewPrjAVSettings_fps");
int idNewPrjAVSettings_interlacing = XRCID("idNewPrjAVSettings_interlacing");
int idNewPrjAVSettings_aspectratio = XRCID("idNewPrjAVSettings_aspectratio");
int idNewPrjAVSettings_pixelaspect = XRCID("idNewPrjAVSettings_pixelaspect");
int idNewPrjAVSettings_samplerate = XRCID("idNewPrjAVSettings_samplerate");
int idNewPrjAVSettings_samplesize = XRCID("idNewPrjAVSettings_samplesize");
int idNewPrjAVSettings_surround = XRCID("idNewPrjAVSettings_surround");
int idNewPrjAVSettings_channels = XRCID("idNewPrjAVSettings_channels");
int idNewPrjAVSettings_description = XRCID("idNewPrjAVSettings_description");

extern int idNewProject;

BEGIN_EVENT_TABLE(NewProjectDlg, wxDialog)
    EVT_UPDATE_UI(idNewPrjAVSettingsPanel, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_width, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_height, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_fps, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_interlacing, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_aspectratio, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_pixelaspect, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_samplerate, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_samplesize, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_surround, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_channels, NewProjectDlg::OnAVSettingsUpdateUI)
    EVT_UPDATE_UI(idNewPrjAVSettings_description, NewProjectDlg::OnAVSettingsUpdateUI)
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

void NewProjectDlg::OnAVSettingsUpdateUI(wxUpdateUIEvent& event) {
    bool canEdit = false;
}

