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
class wxUpdateUIEvent;
class wxPanel;
class wxTreeCtrl;

class AppFrame: public wxFrame
{
    public:
        AppFrame(wxFrame *frame, const wxString& title);
        void OnFrameLayout (wxCommandEvent &event);
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

        void LoadFail(wxString resourcename); /// Shows an error about Loading an XML resource.

        wxMenu* FindMenu(const wxString name);
        ~AppFrame();
        ProjectManager* m_prjMan;
    private:

        wxPanel* CreateProjectPane(); /// Creates the project pane
        wxPanel* m_projectpanel; /// Project Panel
        wxPanel* m_monitorpanel; /// Monitor Panel
        wxPanel* m_effectspanel; /// Effects Panel
        wxPanel* m_timelinepanel; /// Timeline Panel
        wxTreeCtrl* m_ResourcesTree; /// Resources Tree in the Project Panel

        bool LoadResources();
        void CreateDockAreas();
        bool CreateMenuBar();
        bool CreatePanels();
        long GetProjectPanelSashPos();



        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
        };
        void OnFileOpen(wxCommandEvent& event);
        void OnFileClose(wxCommandEvent& event);


        void OnClearRecentProjectList(wxCommandEvent &event);
        void OnOpenRecentFile(wxCommandEvent &event);
        void OnFileSave(wxCommandEvent &event);
        void OnFileSaveAs(wxCommandEvent &event);
        void OnFileSaveCopy(wxCommandEvent &event);

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void LoadAndSetFrameSize();
        void OnSaveFrameLayout(wxCommandEvent& event);
        void SaveDefaultLayout(bool showmsg);
        void OnLoadDefaultLayout(wxCommandEvent& event);
        bool LoadDefaultLayout();

        void DoUpdateAppTitle();
        // UpdateUI events
        void OnUpdateTitleUI(wxUpdateUIEvent& event);

        void OnFileMenuUpdateUI(wxUpdateUIEvent& event);
        void OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event);
        void OnRecentImportsMenuUpdateUI(wxUpdateUIEvent& event);
        void OnEditMenuUpdateUI(wxUpdateUIEvent& event);
        void OnProjectMenuUpdateUI(wxUpdateUIEvent& event);
        void OnClipMenuUpdateUI(wxUpdateUIEvent& event);
        void OnSequenceMenuUpdateUI(wxUpdateUIEvent& event);
        void OnMarkerMenuUpdateUI(wxUpdateUIEvent& event);
        void OnWindowMenuUpdateUI(wxUpdateUIEvent& event);

        wxAuiManager m_mgr;
        wxConfig* m_cfg;
        DECLARE_EVENT_TABLE()
};


#endif // SAYAMAIN_H
