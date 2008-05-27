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
#endif
#include "vidproject/projectmanager.h"
#include "App.h"
#include "welcomedlg.h"

int idWelcomeDialog = XRCID("welcome_dialog");

BEGIN_EVENT_TABLE(WelcomeDialog, wxFrame)
    EVT_CLOSE(WelcomeDialog::OnClose)
END_EVENT_TABLE()

WelcomeDialog::WelcomeDialog(wxFrame *frame) :
m_panel(NULL)
{
    if(!wxXmlResource::Get()->LoadFrame(this,frame, _T("welcome_dialog"))) {
        Destroy();
    } else {
        m_parent = frame;
        CenterOnScreen();
    }
}

void WelcomeDialog::OnClose(wxCloseEvent& event) {
    m_parent->Close();
}

WelcomeDialog::~WelcomeDialog() {
}
