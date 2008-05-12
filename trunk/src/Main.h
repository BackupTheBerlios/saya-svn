/***************************************************************
 * Name:      sayaMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ricardo Garcia ()
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia ()
 * License:
 **************************************************************/

#ifndef MAIN_H
#define MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "App.h"
#include "vidproject/projectmanager.h"
class wxUpdateUIEvent;

class AppFrame: public wxFrame
{
    public:
        AppFrame(wxFrame *frame, const wxString& title);
        void OnFrameLayout (wxCommandEvent &event);
        void UpdateStatustext();
        bool SaveProject();
        bool SaveProjectAs();
        bool SaveProjectCopy();
        ~AppFrame();
        ProjectManager* m_prjMan;
    private:
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
        wxRect DetermineFrameSize();
        void OnSaveFrameLayout(wxCommandEvent& event);
        void StoreFrameSize (wxRect rect);
        void StoreCurrentLayout();

        void DoUpdateAppTitle();
        // UpdateUI events
        void OnUpdateTitleUI(wxUpdateUIEvent& event);

        void OnFileMenuUpdateUI(wxUpdateUIEvent& event);
        void OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event);
        void OnEditMenuUpdateUI(wxUpdateUIEvent& event);
        void OnProjectMenuUpdateUI(wxUpdateUIEvent& event);
        void OnClipMenuUpdateUI(wxUpdateUIEvent& event);
        void OnSequenceMenuUpdateUI(wxUpdateUIEvent& event);
        void OnMarkerMenuUpdateUI(wxUpdateUIEvent& event);
        void OnWindowMenuUpdateUI(wxUpdateUIEvent& event);

        DECLARE_EVENT_TABLE()
};


#endif // SAYAMAIN_H
