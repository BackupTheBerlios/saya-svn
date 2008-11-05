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

#include "../saya/projectmanager.h"
#include "../saya/recentfileslist.h"

#include "s2wx.h"
#include "app.h"
#include "welcomedlg.h"

int idWelcomeDialog = XRCID("welcome_dialog");
int idWelcomeNewProject = XRCID("idWelcomeNewProject");
int idWelcomeOpenProject = XRCID("idWelcomeOpenProject");
int idWelcomeQuit = XRCID("idWelcomeQuit");
int idWelcomeRecentProjects = XRCID("idWelcomeRecentProjects");
extern int idFileOpen;
extern int idNewProject;


BEGIN_EVENT_TABLE(WelcomeDialog, wxFrame)
    EVT_CLOSE(WelcomeDialog::OnClose)
    EVT_BUTTON(idWelcomeNewProject, WelcomeDialog::OnNewProject)
    EVT_BUTTON(idWelcomeOpenProject, WelcomeDialog::OnOpenProject)
    EVT_BUTTON(idWelcomeQuit, WelcomeDialog::OnCloseButton)
    EVT_HTML_LINK_CLICKED(idWelcomeRecentProjects, WelcomeDialog::OnLinkClicked)
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

void WelcomeDialog::OnNewProject(wxCommandEvent& event) {
    if(IsAppShuttingDown()) return;
    wxCommandEvent tmpevent(wxEVT_COMMAND_MENU_SELECTED, idNewProject);
    m_parent->ProcessEvent(tmpevent);
}

void WelcomeDialog::OnOpenProject(wxCommandEvent& event) {
    if(IsAppShuttingDown()) return;
    wxCommandEvent tmpevent(wxEVT_COMMAND_MENU_SELECTED, idFileOpen);
    m_parent->ProcessEvent(tmpevent);
}

bool WelcomeDialog::Show(bool show) {
    RefreshRecentFilesList();
    return wxFrame::Show(show);
}

void WelcomeDialog::RefreshRecentFilesList() {
    if(IsAppShuttingDown()) return;
    wxString tmps = wxEmptyString;
    wxString curfile;

    RecentFilesList& items = *ProjectManager::Get()->m_RecentFiles;
    size_t i;
    for(i = 1; i <=9 && (i<= items.size()); ++i) {
        curfile = s2wx(items.item(i));
        if(!curfile.IsEmpty()) {
            wxString prjtitle = s2wx(ProjectManager::Get()->GetOfflineProjectTitle(wx2s(curfile)));
            if(prjtitle.IsEmpty()) {
                prjtitle = _("(Untitled)");
            }
            tmps += wxString::Format(_T("<li><a href=\"sayarecent://%d\">%s</a><br /><small>%s</small></li>"),i,prjtitle.c_str(),curfile.c_str());
        }
    }
    if(!tmps.IsEmpty()) {
        tmps = _T("<ol>") + tmps + _T("</ol>");
    }
    wxHtmlWindow* myhtml = XRCCTRL(*this,"idWelcomeRecentProjects",wxHtmlWindow);
    if(myhtml) {
        myhtml->SetPage(tmps);
    }
}

void WelcomeDialog::OnLinkClicked(wxHtmlLinkEvent& event) {
    wxHtmlLinkInfo linkinfo = event.GetLinkInfo();
    wxString href = linkinfo.GetHref();
    long fileno = 1;
    if(href.StartsWith(_T("sayarecent://"))) {
        href = href.SubString(13,1);
        if(href.ToLong(&fileno)) {
            wxCommandEvent tmpevent(wxEVT_COMMAND_MENU_SELECTED,wxID_FILE1 + (fileno - 1));
            m_parent->ProcessEvent(tmpevent);
        }
    }
}

void WelcomeDialog::OnCloseButton(wxCommandEvent& event) {
    m_parent->Close(true);
}

WelcomeDialog::~WelcomeDialog() {
}
