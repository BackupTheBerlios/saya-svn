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
    #include <wx/wx.h>
#endif

#include "App.h"
#include "vidproject/projectmanager.h"
class wxUpdateUIEvent;
class wxPanel;
class wxTreeCtrl;

class NewProjectDlg : public wxDialog
{
    public:
        NewProjectDlg(wxWindow *parent);
        virtual ~NewProjectDlg();
    private:
        wxWindow* m_parent;
        void OnAVSettingsUpdateUI(wxUpdateUIEvent& event);
	DECLARE_EVENT_TABLE()

};

#endif
