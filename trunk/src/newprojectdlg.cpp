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
#include "PickNameDlg.h"

#include <map>
#include <list>
#include <iterator>

using namespace std;

int idNewProjectDialog = XRCID("new_project_dialog");
int idNewPrjAVSettingsPanel = XRCID("idNewPrjAVSettingsPanel");
int idNewPrjPresets = XRCID("idNewPrjPresets");
int idNewPrjSaveSettingsAs = XRCID("idNewPrjSaveSettingsAs");
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
    EVT_CHOICE(idNewPrjPresets, NewProjectDlg::OnPjrPresetsChanged)
    EVT_BUTTON(idNewPrjSaveSettingsAs, NewProjectDlg::OnPrjSaveSettingsAsClicked)

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
        idNewPrjPresetsChoice = XRCCTRL(*this, "idNewPrjPresets", wxChoice);
        idNewPrjAVSettings_widthTextCtrl = XRCCTRL(*this, "idNewPrjAVSettings_width", wxTextCtrl);
        idNewPrjAVSettings_heightTextCtrl = XRCCTRL(*this, "idNewPrjAVSettings_height", wxTextCtrl);
        idNewPrjAVSettings_fpsComboBox = XRCCTRL(*this, "idNewPrjAVSettings_fps", wxComboBox);
        idNewPrjAVSettings_interlacingChoice = XRCCTRL(*this, "idNewPrjAVSettings_interlacing", wxChoice);
        idNewPrjAVSettings_pixelaspectTextCtrl = XRCCTRL(*this, "idNewPrjAVSettings_pixelaspect", wxTextCtrl);
        idNewPrjAVSettings_samplerateComboBox = XRCCTRL(*this, "idNewPrjAVSettings_samplerate", wxComboBox);
        idNewPrjAVSettings_samplesizeChoice = XRCCTRL(*this, "idNewPrjAVSettings_samplesize", wxChoice);
        idNewPrjAVSettings_surroundChoice = XRCCTRL(*this, "idNewPrjAVSettings_surround", wxChoice);
        //idNewPrjAVSettings_channelsSpinCtrl = XRCCTRL(*this, "idNewPrjAVSettings_channels", wxSpinCtrl);
        idNewPrjAVSettings_descriptionTextCtrl = XRCCTRL(*this, "idNewPrjAVSettings_description", wxTextCtrl);

        //put this call here because we need presest widget
        if(!LoadPresets()){
            Destroy();
        } else{
            CenterOnScreen();
        }
    }
}

NewProjectDlg::~NewProjectDlg() {
}

void NewProjectDlg::OnAVSettingsUpdateUI(wxUpdateUIEvent& event) {
    //bool canEdit = false;
    // NOTE This is where we need to disable the widgets on the "new project" dialog.
}

void NewProjectDlg::OnPjrPresetsChanged(wxCommandEvent& event) {

    //enable all
    if(idNewPrjPresetsChoice->GetSelection() == 0){
        idNewPrjAVSettings_widthTextCtrl->Enable();
        idNewPrjAVSettings_heightTextCtrl->Enable();
        idNewPrjAVSettings_fpsComboBox->Enable();
        idNewPrjAVSettings_interlacingChoice->Enable();
        idNewPrjAVSettings_pixelaspectTextCtrl->Enable();
        idNewPrjAVSettings_samplerateComboBox->Enable();
        idNewPrjAVSettings_samplesizeChoice->Enable();
        idNewPrjAVSettings_surroundChoice->Enable();
        //idNewPrjAVSettings_channelsSpinCtrl->Enable();
        idNewPrjAVSettings_descriptionTextCtrl->Enable();
    }
    //disable all and put values
    else{
        std::string selPreset = std::string(idNewPrjPresetsChoice->GetStringSelection().mb_str(wxConvUTF8));
        map<std::string, std::string> values = ProjectManager::Get()->GetPresetData(selPreset);

        map<std::string, std::string>::iterator iter;

        iter = values.find("idNewPrjAVSettings_width");
        if(iter != values.end()){idNewPrjAVSettings_widthTextCtrl->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_widthTextCtrl->Disable();

        iter = values.find("idNewPrjAVSettings_height");
        if(iter != values.end()){idNewPrjAVSettings_heightTextCtrl->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_heightTextCtrl->Disable();

        iter = values.find("idNewPrjAVSettings_fps");
        if(iter != values.end()){idNewPrjAVSettings_fpsComboBox->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_fpsComboBox->Disable();

        iter = values.find("idNewPrjAVSettings_interlacing");
        if(iter != values.end()){idNewPrjAVSettings_interlacingChoice->SetStringSelection(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_interlacingChoice->Disable();

        iter = values.find("idNewPrjAVSettings_pixelaspect");
        if(iter != values.end()){idNewPrjAVSettings_pixelaspectTextCtrl->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_pixelaspectTextCtrl->Disable();

        iter = values.find("idNewPrjAVSettings_samplerate");
        if(iter != values.end()){idNewPrjAVSettings_samplerateComboBox->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_samplerateComboBox->Disable();

        iter = values.find("idNewPrjAVSettings_samplesize");
        if(iter != values.end()){idNewPrjAVSettings_samplesizeChoice->SetStringSelection(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_samplesizeChoice->Disable();

        iter = values.find("idNewPrjAVSettings_surround");
        if(iter != values.end()){idNewPrjAVSettings_surroundChoice->SetStringSelection(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_surroundChoice->Disable();

        //iter = values.find("idNewPrjAVSettings_channels");
        //if(iter != values.end()){idNewPrjAVSettings_channels->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        //idNewPrjAVSettings_channelsSpinCtrl->Disable();

        iter = values.find("idNewPrjAVSettings_description");
        if(iter != values.end()){idNewPrjAVSettings_descriptionTextCtrl->SetValue(wxString(iter->second.c_str(), wxConvUTF8));}
        idNewPrjAVSettings_descriptionTextCtrl->Disable();
    }
}

void NewProjectDlg::OnPrjSaveSettingsAsClicked(wxCommandEvent& event) {
    PickNameDlg* dlg = new PickNameDlg(this);
    if(!dlg) {
        return;
    }

    if(dlg->ShowModal() == wxID_OK){
        //save preset (veirification not neede)
        map<std::string, std::string> configs;
        configs["idNewPrjAVSettings_width"] = std::string(idNewPrjAVSettings_widthTextCtrl->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_height"] = std::string(idNewPrjAVSettings_heightTextCtrl->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_fps"] = std::string(idNewPrjAVSettings_fpsComboBox->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_interlacing"] = std::string(idNewPrjAVSettings_interlacingChoice->GetStringSelection().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_pixelaspect"] = std::string(idNewPrjAVSettings_pixelaspectTextCtrl->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_samplerate"] = std::string(idNewPrjAVSettings_samplerateComboBox->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_samplesize"] = std::string(idNewPrjAVSettings_samplesizeChoice->GetStringSelection().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_surround"] = std::string(idNewPrjAVSettings_surroundChoice->GetStringSelection().mb_str(wxConvUTF8));
        //configs["idNewPrjAVSettings_channels"] = std::string(idNewPrjAVSettings_channelsSpinCtrl->GetValue().mb_str(wxConvUTF8));
        configs["idNewPrjAVSettings_description"] = std::string(idNewPrjAVSettings_descriptionTextCtrl->GetValue().mb_str(wxConvUTF8));


        ProjectManager::Get()->SaveNewPreset(std::string(dlg->GetName().mb_str(wxConvUTF8)), configs);
        int columns = idNewPrjPresetsChoice->GetCount();
        idNewPrjPresetsChoice->Append(dlg->GetName());
        idNewPrjPresetsChoice->Select(columns);
    }
}

bool NewProjectDlg::LoadPresets(){
    std::list<std::string> presets = ProjectManager::Get()->GetPresets();

    idNewPrjPresetsChoice->Append(wxString(_T("<Custom>")));
    list<std::string>::iterator i;

    for(i=presets.begin(); i != presets.end(); ++i){
        idNewPrjPresetsChoice->Append(wxString(i->c_str(), wxConvUTF8));
    }

    //select custom preset by default
    idNewPrjPresetsChoice->Select(0);
    return true;
}
