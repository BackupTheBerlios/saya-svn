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
    #include <wx/spinctrl.h>
    #include <wx/sizer.h>
    #include <wx/choice.h>
    #include <wx/textctrl.h>
    #include <wx/combobox.h>
#endif

#include "../saya/core/systring.h"
#include "../saya/projectmanager.h"
#include "../saya/presetmanager.h"
#include "../saya/timeline/smapxstr.h"
#include "../saya/timeline/sstrvector.h"

// #include "app.h"
#include "newprojectdlg.h"
#include "picknamedlg.h"

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
        idNewPrjAVSettings_channelsSpinCtrl = XRCCTRL(*this, "idNewPrjAVSettings_channels", wxSpinCtrl);
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
        syString selPreset(idNewPrjPresetsChoice->GetStringSelection());
        SMapStrStr values = ProjectManager::Get()->m_Presets->GetPresetData(selPreset.c_str());

        const syString* sp = values.find("idNewPrjAVSettings_width");
        if(sp) {idNewPrjAVSettings_widthTextCtrl->SetValue(*sp); }
        idNewPrjAVSettings_widthTextCtrl->Disable();

        sp = values.find("idNewPrjAVSettings_height");
        if(sp) {idNewPrjAVSettings_heightTextCtrl->SetValue(*sp);}
        idNewPrjAVSettings_heightTextCtrl->Disable();

        sp = values.find("idNewPrjAVSettings_fps");
        if(sp) {idNewPrjAVSettings_fpsComboBox->SetValue(*sp);}
        idNewPrjAVSettings_fpsComboBox->Disable();

        sp = values.find("idNewPrjAVSettings_interlacing");
        if(sp) {idNewPrjAVSettings_interlacingChoice->SetStringSelection(*sp);}
        idNewPrjAVSettings_interlacingChoice->Disable();

        sp = values.find("idNewPrjAVSettings_pixelaspect");
        if(sp) {idNewPrjAVSettings_pixelaspectTextCtrl->SetValue(*sp);}
        idNewPrjAVSettings_pixelaspectTextCtrl->Disable();

        sp = values.find("idNewPrjAVSettings_samplerate");
        if(sp) {idNewPrjAVSettings_samplerateComboBox->SetValue(*sp);}
        idNewPrjAVSettings_samplerateComboBox->Disable();

        sp = values.find("idNewPrjAVSettings_samplesize");
        if(sp) {idNewPrjAVSettings_samplesizeChoice->SetStringSelection(*sp);}
        idNewPrjAVSettings_samplesizeChoice->Disable();

        sp = values.find("idNewPrjAVSettings_surround");
        if(sp) {idNewPrjAVSettings_surroundChoice->SetStringSelection(*sp);}
        idNewPrjAVSettings_surroundChoice->Disable();

        //iter = values.find("idNewPrjAVSettings_channels");
        //if(sp){idNewPrjAVSettings_channels->SetValue(*sp);}
        //idNewPrjAVSettings_channelsSpinCtrl->Disable();

        sp = values.find("idNewPrjAVSettings_description");
        if(sp) {idNewPrjAVSettings_descriptionTextCtrl->SetValue(*sp);}
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
        SMapStrStr configs;
        configs["idNewPrjAVSettings_width"] = idNewPrjAVSettings_widthTextCtrl->GetValue();
        configs["idNewPrjAVSettings_height"] = idNewPrjAVSettings_heightTextCtrl->GetValue();
        configs["idNewPrjAVSettings_fps"] = idNewPrjAVSettings_fpsComboBox->GetValue();
        configs["idNewPrjAVSettings_interlacing"] = idNewPrjAVSettings_interlacingChoice->GetStringSelection();
        configs["idNewPrjAVSettings_pixelaspect"] = idNewPrjAVSettings_pixelaspectTextCtrl->GetValue();
        configs["idNewPrjAVSettings_samplerate"] = idNewPrjAVSettings_samplerateComboBox->GetValue();
        configs["idNewPrjAVSettings_samplesize"] = idNewPrjAVSettings_samplesizeChoice->GetStringSelection();
        configs["idNewPrjAVSettings_surround"] = idNewPrjAVSettings_surroundChoice->GetStringSelection();
        //configs["idNewPrjAVSettings_channels"] = idNewPrjAVSettings_channelsSpinCtrl->GetValue();
        configs["idNewPrjAVSettings_description"] = idNewPrjAVSettings_descriptionTextCtrl->GetValue();


        ProjectManager::Get()->m_Presets->SaveNewPreset(syString(dlg->GetName()), configs);
        int columns = idNewPrjPresetsChoice->GetCount();
        idNewPrjPresetsChoice->Append(dlg->GetName());
        idNewPrjPresetsChoice->Select(columns);
    }
}

bool NewProjectDlg::LoadPresets(){
    SStringVector presets = ProjectManager::Get()->m_Presets->GetPresets();

    idNewPrjPresetsChoice->Append(syString("<Custom>", true));
    unsigned int i, imax;

    for(i=0, imax = presets.size(); i < imax; ++i){
        idNewPrjPresetsChoice->Append(presets[i]);
    }

    //select custom preset by default
    idNewPrjPresetsChoice->Select(0);
    return true;
}
