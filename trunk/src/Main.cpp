/***************************************************************
 * Name:      sayaMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ricardo Garcia ()
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/xrc/xmlres.h>
#include <wx/config.h>
#include <wx/filedlg.h>
#include "App.h"
#include "Main.h"

#include <deque>
using namespace std;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


const wxString LOCATION = _T("Location");
const wxString LOCATION_X = _T("xpos");
const wxString LOCATION_Y = _T("ypos");
const wxString LOCATION_W = _T("width");
const wxString LOCATION_H = _T("height");


int idFileExit = XRCID("idFileExit");
int idMenuSaveFrameLayout = XRCID("idMenuSaveFrameLayout");
int idFileOpen = XRCID("idFileOpen");

int idFileOpenRecentProject = XRCID("idFileOpenRecentProject");
int idFileClearRecentProjectList = XRCID("idFileClearRecentProjectList");

wxString g_statustext;

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_CLOSE(AppFrame::OnClose)
    EVT_MENU(idFileExit, AppFrame::OnQuit)
    EVT_MENU(idMenuAbout, AppFrame::OnAbout)
    EVT_MENU(idMenuSaveFrameLayout, AppFrame::OnSaveFrameLayout)
    EVT_MENU(idFileOpen, AppFrame::OnFileOpen)
    EVT_MENU(idFileClearRecentProjectList, AppFrame::OnClearRecentProjectList)

    EVT_UPDATE_UI(idFileOpenRecentProject, AppFrame::OnRecentFilesMenuUpdateUI)
END_EVENT_TABLE()

AppFrame::AppFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title)
{
    bool bRet = wxXmlResource::Get()->Load(wxT("resources/mainmenu.xrc"));

    if(!bRet) {
        Destroy();
        return;
    }
    m_prjMan = ProjectManager::Get();
    SetSize (DetermineFrameSize ());
    CenterOnScreen();

#if wxUSE_MENUS
    // create a menu bar
    wxMenuBar* mbar = wxXmlResource::Get()->LoadMenuBar(wxT("main_menu_bar"));
    if(mbar) {
        SetMenuBar(mbar);
    }
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxbuildinfo(short_f), 1);
    g_statustext = _("Welcome to ") + APP_SHOWNAME + _T("! ^_^");
    UpdateStatustext();
#endif // wxUSE_STATUSBAR

}

wxRect AppFrame::DetermineFrameSize () {
    const int minFrameWidth = 980;
    const int minFrameHight = 680;

    // load stored size or defaults
    wxRect rect;
    wxConfig* cfg = new wxConfig (APP_NAME);
    wxString key = LOCATION;
    if (cfg->Exists (key)) {
        rect.x = cfg->Read (key + _T("/") + LOCATION_X, rect.x);
        rect.y = cfg->Read (key + _T("/") + LOCATION_Y, rect.y);
        rect.width = cfg->Read (key + _T("/") + LOCATION_W, rect.width);
        rect.height = cfg->Read (key + _T("/") + LOCATION_H, rect.height);
    }
    delete cfg;

    // check for reasonable values (within screen)
    wxSize scr = wxGetDisplaySize();
    rect.x = wxMin (abs (rect.x), (scr.x - minFrameWidth));
    rect.y = wxMin (abs (rect.y), (scr.y - minFrameHight));
    rect.width = wxMax (abs (rect.width), (minFrameWidth));
    rect.width = wxMin (abs (rect.width), (scr.x - rect.x));
    rect.height = wxMax (abs (rect.height), (minFrameHight));
    rect.height = wxMin (abs (rect.height), (scr.y - rect.y));

    return rect;
}

AppFrame::~AppFrame() {
    ShutdDownApp();
    ProjectManager::Unload();
}

void AppFrame::OnFileOpen(wxCommandEvent& event) {
    if(IsAppShuttingDown())
        return;
    wxString lastdir = ProjectManager::Get()->GetLastProjectDir();
    wxFileDialog myDialog(this, _("Choose a project"), lastdir, _T(""), _T("*.saya"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("opendlg"));
    int dialogresult = myDialog.ShowModal();
    if(dialogresult == wxID_OK) {
        bool result = ProjectManager::Get()->LoadProject(myDialog.GetPath());
        if(result) {
            // TODO (rick#1#): Remove setting of modified flag on project loading (was done for debug purposes)
            ProjectManager::Get()->m_project->SetModified();
        }
    }
}

void AppFrame::OnClearRecentProjectList(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->ClearRecentFiles();
}

void AppFrame::OnClose(wxCloseEvent &event) {
    Destroy();
}

void AppFrame::OnQuit(wxCommandEvent &event) {
    Destroy();
}

void AppFrame::OnSaveFrameLayout(wxCommandEvent& event) {
    StoreCurrentLayout();
}

void AppFrame::UpdateStatustext() {
    SetStatusText (g_statustext, 0);
}

void AppFrame::StoreCurrentLayout() {
    StoreFrameSize (GetRect ());

    wxMessageBox (_("Current Layout has been saved."),
                  _("Save Layout"), wxOK);

}
void AppFrame::StoreFrameSize (wxRect rect) {

    // store size
    wxConfig* cfg = new wxConfig (APP_NAME);
    wxString key = LOCATION;
    cfg->Write (key + _T("/") + LOCATION_X, rect.x);
    cfg->Write (key + _T("/") + LOCATION_Y, rect.y);
    cfg->Write (key + _T("/") + LOCATION_W, rect.width);
    cfg->Write (key + _T("/") + LOCATION_H, rect.height);
    // TODO: Save other windows' layout
    delete cfg;
}

void AppFrame::OnFileMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event) {
    // Update the Recent Projects list
    ProjectManager* pmgr = ProjectManager::Get();
    if(pmgr == NULL)
        return;
    if(pmgr->m_recentfilesmodified) {
        pmgr->m_recentfilesmodified = false;
        wxMenuItem* myItem = GetMenuBar()-> FindItem(idFileOpenRecentProject);
        if(myItem) {
            wxMenu* mySubMenu = myItem->GetSubMenu();

            if(mySubMenu) { // Clear all items in the submenu
                while(mySubMenu->GetMenuItemCount()) {
                    wxMenuItem* subitem = mySubMenu->FindItemByPosition(0);
                    if(subitem == NULL)
                        break;
                    mySubMenu->Delete(subitem);
                }
            } else { // Add a new submenu
                mySubMenu = new wxMenu();
                myItem->SetSubMenu(mySubMenu);
            }
            // TODO (rick#1#): Add the recently opened projects to the menu in OnUpdateUI
            size_t i = 0;
            mySubMenu->Append(idFileClearRecentProjectList,_T("&Clear"),_("Clears Recent Projects List"));
            mySubMenu->AppendSeparator();
            for(i = 0; i < pmgr->m_recentfiles.size(); i++) {
                wxString tmptext;
                tmptext.Printf(_T("&%d ") + pmgr->m_recentfiles[i],i+1);
                mySubMenu->Append(wxID_FILE1+i,tmptext,wxEmptyString);
            }
            // wxID_FILE1
            myItem->Enable(pmgr->m_recentfiles.size() > 0);
        }
    }
}

void AppFrame::OnEditMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnProjectMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnClipMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnSequenceMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnMarkerMenuUpdateUI(wxUpdateUIEvent& event) {
}

void AppFrame::OnWindowMenuUpdateUI(wxUpdateUIEvent& event) {
}



void AppFrame::OnAbout(wxCommandEvent &event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
