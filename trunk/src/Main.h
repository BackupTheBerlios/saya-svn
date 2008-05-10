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

class AppFrame: public wxFrame
{
    public:
        AppFrame(wxFrame *frame, const wxString& title);
        void OnFrameLayout (wxCommandEvent &event);
        void UpdateStatustext();
        ~AppFrame();
        ProjectManager* m_prjMan;
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout,
        };
        void OnFileOpen(wxCommandEvent& event);

        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        wxRect DetermineFrameSize();
        void OnSaveFrameLayout(wxCommandEvent& event);
        void StoreFrameSize (wxRect rect);
        void StoreCurrentLayout();

        DECLARE_EVENT_TABLE()
};


#endif // SAYAMAIN_H
