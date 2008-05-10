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

wxString g_statustext;

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_CLOSE(AppFrame::OnClose)
    EVT_MENU(idFileExit, AppFrame::OnQuit)
    EVT_MENU(idMenuAbout, AppFrame::OnAbout)
    EVT_MENU(idMenuSaveFrameLayout, AppFrame::OnSaveFrameLayout)
    EVT_MENU(idFileOpen, AppFrame::OnFileOpen)
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
        ProjectManager::Get()->LoadProject(myDialog.GetPath());
    }
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

void AppFrame::OnAbout(wxCommandEvent &event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
