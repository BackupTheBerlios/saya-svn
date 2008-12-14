/***************************************************************
 * Name:      newprojectdlg.h
 * Purpose:   Defines New Project Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-29
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#ifndef WX_PRECOMP
    #include <wx/dialog.h>
#endif

class wxUpdateUIEvent;
class wxPanel;
class wxTreeCtrl;
class wxSpinCtrl;
class wxChoice;
class wxTextCtrl;
class wxComboBox;

class NewProjectDlg : public wxDialog
{
    public:
        NewProjectDlg(wxWindow *parent);
        virtual ~NewProjectDlg();
    private:
        wxWindow* m_parent;
        wxChoice* idNewPrjPresetsChoice;
        wxTextCtrl* idNewPrjAVSettings_widthTextCtrl;
        wxTextCtrl* idNewPrjAVSettings_heightTextCtrl;
        wxComboBox* idNewPrjAVSettings_fpsComboBox;
        wxChoice* idNewPrjAVSettings_interlacingChoice;
        wxTextCtrl* idNewPrjAVSettings_pixelaspectTextCtrl;
        wxComboBox* idNewPrjAVSettings_samplerateComboBox;
        wxChoice* idNewPrjAVSettings_samplesizeChoice;
        wxChoice* idNewPrjAVSettings_surroundChoice;
        wxSpinCtrl* idNewPrjAVSettings_channelsSpinCtrl;
        wxTextCtrl* idNewPrjAVSettings_descriptionTextCtrl;

        void OnAVSettingsUpdateUI(wxUpdateUIEvent& event);
        void OnPjrPresetsChanged(wxCommandEvent& event);
        void OnPrjSaveSettingsAsClicked(wxCommandEvent& event);
        bool LoadPresets();
	DECLARE_EVENT_TABLE()

};

#endif
