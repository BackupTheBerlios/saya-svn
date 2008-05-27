/***************************************************************
 * Name:      welcomedlg.h
 * Purpose:   Defines Welcome Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-26
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef WELCOMEDLG_H
#define WELCOMEDLG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "App.h"
#include "vidproject/projectmanager.h"
class wxUpdateUIEvent;
class wxPanel;
class wxTreeCtrl;

class WelcomeDialog: public wxFrame
{
    public:
        WelcomeDialog(wxFrame *frame);
        virtual ~WelcomeDialog();
    private:
        wxPanel* m_panel;
        void OnClose(wxCloseEvent& event);
        wxFrame* m_parent;

	DECLARE_EVENT_TABLE()

};

#endif
