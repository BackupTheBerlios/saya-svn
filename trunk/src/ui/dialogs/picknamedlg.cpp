//#ifdef WX_PRECOMP
//#include "wx_pch.h"
//#endif
//
//#ifdef __BORLANDC__
//#pragma hdrstop
//#endif //__BORLANDC__
//
//
//#ifndef WX_PRECOMP
//    #include <wx/xrc/xmlres.h>
//    #include <wx/textctrl.h>
//    #include <wx/string.h>
//    #include <wx/sizer.h>
//#endif
//
//#include "../saya/projectmanager.h"
//#include "app.h"
//#include "picknamedlg.h"
//
//int idPickNameDialog = XRCID("pick_name_dialog");
//int idPickNameDlgNameText = XRCID("idPickNameDlgNameText");
//int idPickNameDlgButtonPanelOK = XRCID("idPickNameDlgButtonPanelOK");
//int idPickNameDlgButtonPanelCancel = XRCID("idPickNameDlgButtonPanelCancel");
//
//PickNameDlg::PickNameDlg(wxWindow* parent) : m_parent(0)
//{
//    if(!wxXmlResource::Get()->LoadDialog(this,parent, _T("pick_name_dialog"))) {
//        Destroy();
//    } else {
//        m_parent = parent;
//        CenterOnScreen();
//    }
//
//    nameTextCtrl = XRCCTRL(*this, "idPickNameDlgNameText", wxTextCtrl);
//}
//
//PickNameDlg::~PickNameDlg() {
//}
//
//wxString PickNameDlg::GetName(){
//    return nameTextCtrl->GetValue();
//}
