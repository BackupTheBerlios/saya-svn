/***************************************************************
 * Name:      sayaMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef MAIN_H
#define MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/aui/aui.h>
#endif

#include "App.h"
#include "vidproject/projectmanager.h"
#include <string>

class WelcomeDialog;
class wxUpdateUIEvent;
class wxPanel;
class wxTreeCtrl;

class AppFrame: public wxFrame, public sayaEvtHandler
{
    public:
        AppFrame(wxFrame *frame, const wxString& title);
        void OnFrameLayout (wxCommandEvent& event);
        void UpdateStatustext();
        bool SaveProject();
        bool SaveProjectAs();
        bool SaveProjectCopy();

        bool IsClipSelected();          /// True if one or more timeline clips are selected
        bool IsClipboardSet();          /** True if clipboard contains a resource clip or a timeline clip
                                         *  (resource clips will paste into timeline clips) */
        bool IsSelectionMultiple();     /** True if Selection covers more than one timeline clip, or even full tracks
                                         *  - or if more than one resource clip is selected */
        bool IsResourceClipSelected();  /// True if a resource clip is selected
        bool IsTimelineActive();        /// True if the active window is the timeline window
        bool IsResourceWindowActive();  /// True if the active window is the resource window
        bool IsMonitorActive();         /// True if the active window is the monitor window
        bool IsRenderMonitorActive();   /// True if the *render* monitor is active
        bool IsClipMonitorActive();     /// True if the *clip* monitor is active
                                        /// (render and clip monitors can't be active (selected) at the same time)
        bool IsEffectsWindowActive();   /// True if the effects monitor/window is active
        bool IsTitleWindowActive();     /// True if the title window is active
        bool CanUndo();                 /// Does the project have an item in the "undo" list?
        bool CanRedo();                 /// Does the project have an item in the "redo" list?

        /// Shows an error about Loading an XML resource.
        void LoadFail(wxString resourcename);

        /// Processes events related to Saya projects
        virtual void ProcessSayaEvent(sayaEventType id, void* data = NULL);

        /// Shows an error message box.
        virtual void ErrorMessageBox(const char* msg,const char* caption);

        /// Shows a Yes/No message box.
        virtual bool YesNoMessageBox(const char* msg,const char* caption,bool exclamation);

        /// Shows a Yes/No/Cancel message box.
        virtual sayaYesNoCancel YesNoCancelMessageBox(const char* msg,const char* caption,bool exclamation);

        /// Shows "Save Project As" dialog.
        virtual std::string ShowDialogSaveProjectAs();

        /// Shows "Save Project Copy As" dialog.
        virtual std::string ShowDialogSaveProjectCopyAs();

        wxMenu* FindMenu(const wxString name);
        ~AppFrame();
        ProjectManager* m_prjMan;
    private:

        sayaEvtHandler* m_handler;
        wxPanel* CreateProjectPane(); /// Creates the project pane
        wxPanel* m_projectpanel; /// Project Panel
        wxPanel* m_monitorpanel; /// Monitor Panel
        wxPanel* m_effectspanel; /// Effects Panel
        wxPanel* m_timelinepanel; /// Timeline Panel
        WelcomeDialog* m_welcomedialog;
        wxTreeCtrl* m_ResourcesTree; /// Resources Tree in the Project Panel

        bool LoadResources();
        void CreateDockAreas();
        bool CreateDialogs();
        bool CreateMenuBar();
        bool CreatePanels();
        long GetProjectPanelSashPos();
        void ShowLayout(bool show);
        void ShowWelcomeDialog();

        void OnResourceTreeContextMenu(wxTreeEvent& event);

        void OnFileOpen(wxCommandEvent& event);
        void OnFileClose(wxCommandEvent& event);


        void OnClearRecentProjectList(wxCommandEvent& event);
        void OnOpenRecentFile(wxCommandEvent& event);
        void OnFileSave(wxCommandEvent& event);
        void OnFileSaveAs(wxCommandEvent& event);
        void OnFileSaveCopy(wxCommandEvent& event);
        void OnNewProject(wxCommandEvent& event);

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void LoadAndSetFrameSize();
        void OnSaveFrameLayout(wxCommandEvent& event);
        void SaveDefaultLayout(bool showmsg);
        void OnLoadDefaultLayout(wxCommandEvent& event);
        void OnWorkspaceFactoryDefault(wxCommandEvent& event);
        bool LoadDefaultLayout(bool firsttime = false);

        void DoUpdateAppTitle();
        // UpdateUI events
        void OnProjectStatusChanged(wxUpdateUIEvent& event);

        void OnFileMenuUpdateUI(wxUpdateUIEvent& event);
        void OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event);
        void OnRecentImportsMenuUpdateUI(wxUpdateUIEvent& event);
        void OnEditMenuUpdateUI(wxUpdateUIEvent& event);
        void OnProjectMenuUpdateUI(wxUpdateUIEvent& event);
        void OnClipMenuUpdateUI(wxUpdateUIEvent& event);
        void OnSequenceMenuUpdateUI(wxUpdateUIEvent& event);
        void OnMarkerMenuUpdateUI(wxUpdateUIEvent& event);
        void OnWindowMenuUpdateUI(wxUpdateUIEvent& event);

        void OnUpdateProjectPaneUI(wxUpdateUIEvent& event);

        wxAuiManager m_mgr;
        wxConfig* m_cfg;
        bool m_hadproject;
        bool m_panes_status_checked;
        bool m_layouthidden;
        wxString m_CurrentPerspective;
        wxString m_FactoryDefaultLayout;
        DECLARE_EVENT_TABLE()
};

const wxString std2wx(const std::string& str);
const wxString std2wx(const char* str);

#endif // SAYAMAIN_H
