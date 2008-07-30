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
#include "PickNameDlg.h"

int idPickNameDialog = XRCID("pick_name_dialog");
int idPickNameDlgNameText = XRCID("idPickNameDlgNameText");
int idPickNameDlgButtonPanelOK = XRCID("idPickNameDlgButtonPanelOK");
int idPickNameDlgButtonPanelCancel = XRCID("idPickNameDlgButtonPanelCancel");

PickNameDlg::PickNameDlg(wxWindow* parent) : m_parent(0)
{
    if(!wxXmlResource::Get()->LoadDialog(this,parent, _T("pick_name_dialog"))) {
        Destroy();
    } else {
        m_parent = parent;
        CenterOnScreen();
    }

    nameTextCtrl = XRCCTRL(*this, "idPickNameDlgNameText", wxTextCtrl);
}

PickNameDlg::~PickNameDlg() {
}

wxString PickNameDlg::GetName(){
    return nameTextCtrl->GetValue();
}
