/***************************************************************
 * Name:      sayaMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
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

int main_RegisterId(int id)
{
    wxRegisterId(id);
    return id;
}



const wxString LOCATION = _T("Location");
const wxString LOCATION_X = _T("xpos");
const wxString LOCATION_Y = _T("ypos");
const wxString LOCATION_W = _T("width");
const wxString LOCATION_H = _T("height");

int idFileNew = XRCID("idFileNew");
int idFileOpen = XRCID("idFileOpen");
int idFileOpenRecentProject = XRCID("idFileOpenRecentProject");
int idFileClearRecentProjectList = XRCID("idFileClearRecentProjectList");
int idFileClearRecentImportList = XRCID("idFileClearRecentImportList");
int idFileClose = XRCID("idFileClose");
int idFileSave = XRCID("idFileSave");
int idFileSaveAs = XRCID("idFileSaveAs");
int idFileSaveCopy = XRCID("idFileSaveCopy");
int idFileRevert = XRCID("idFileRevert");
int idFileCapture = XRCID("idFileCapture");
int idFileBatchCapture = XRCID("idFileBatchCapture");
int idFileImport = XRCID("idFileImport");
int idFileImportRecent = XRCID("idFileImportRecent");
int idFileExport = XRCID("idFileExport");
int idFileGetProperties = XRCID("idFileGetProperties");
int idFileGetPropertiesFile = XRCID("idFileGetPropertiesFile");
int idFileGetPropertiesSelection = XRCID("idFileGetPropertiesSelection");
int idFileInterpretFootage = XRCID("idFileInterpretFootage");
int idFileTimecode = XRCID("idFileTimecode");
int idFileExit = XRCID("idFileExit");


int wxID_IMPORT1 = wxNewId();
int wxID_IMPORT2 = main_RegisterId(wxID_IMPORT1 + 1);
int wxID_IMPORT3 = main_RegisterId(wxID_IMPORT1 + 2);
int wxID_IMPORT4 = main_RegisterId(wxID_IMPORT1 + 3);
int wxID_IMPORT5 = main_RegisterId(wxID_IMPORT1 + 4);
int wxID_IMPORT6 = main_RegisterId(wxID_IMPORT1 + 5);
int wxID_IMPORT7 = main_RegisterId(wxID_IMPORT1 + 6);
int wxID_IMPORT8 = main_RegisterId(wxID_IMPORT1 + 7);
int wxID_IMPORT9 = main_RegisterId(wxID_IMPORT1 + 8);

int idEditUndo = XRCID("idEditUndo");
int idEditRedo = XRCID("idEditRedo");
int idEditClearUndoHistory = XRCID("idEditClearUndoHistory");
int idEditCut = XRCID("idEditCut");
int idEditCopy = XRCID("idEditCopy");
int idEditPaste = XRCID("idEditPaste");
int idEditPasteInsert = XRCID("idEditPasteInsert");
int idEditPasteAttributes = XRCID("idEditPasteAttributes");
int idEditClear = XRCID("idEditClear");
int idEditRippleDelete = XRCID("idEditRippleDelete");
int idEditDuplicate = XRCID("idEditDuplicate");
int idEditSelectAll = XRCID("idEditSelectAll");
int idEditDeselectAll = XRCID("idEditDeselectAll");
int idEditFind = XRCID("idEditFind");
int idEditLabel = XRCID("idEditLabel");
int idEditLabelBlue = XRCID("idEditLabelBlue");
int idEditLabelCyan = XRCID("idEditLabelCyan");
int idEditLabelGreen = XRCID("idEditLabelGreen");
int idEditLabelViolet = XRCID("idEditLabelViolet");
int idEditLabelPink = XRCID("idEditLabelPink");
int idEditLabelGray = XRCID("idEditLabelGray");
int idEditLabelRed = XRCID("idEditLabelRed");
int idEditLabelOrange = XRCID("idEditLabelOrange");
int idEditOriginal = XRCID("idEditOriginal");
int idEditSequenceMarker = XRCID("idEditSequenceMarker");

int idMenuSaveFrameLayout = XRCID("idMenuSaveFrameLayout");








wxString g_statustext;

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_CLOSE(AppFrame::OnClose)
    EVT_MENU(idMenuAbout, AppFrame::OnAbout)
    EVT_MENU(idMenuSaveFrameLayout, AppFrame::OnSaveFrameLayout)

//  File menu
    EVT_MENU(idFileOpen, AppFrame::OnFileOpen)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, AppFrame::OnOpenRecentFile)
    EVT_MENU(idFileClearRecentProjectList, AppFrame::OnClearRecentProjectList)
    EVT_MENU(idFileClose, AppFrame::OnFileClose)
    EVT_MENU(idFileSave, AppFrame::OnFileSave)
    EVT_MENU(idFileSaveAs, AppFrame::OnFileSaveAs)
    EVT_MENU(idFileSaveCopy, AppFrame::OnFileSaveCopy)
    EVT_MENU(idFileExit, AppFrame::OnQuit)

    EVT_UPDATE_UI(idFileNew, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileOpen, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileOpenRecentProject, AppFrame::OnRecentFilesMenuUpdateUI)
    EVT_UPDATE_UI(idFileClose, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSave, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveAs, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileSaveCopy, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileRevert, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileCapture, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileBatchCapture, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileImport, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileGetProperties, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileGetPropertiesFile, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idFileGetPropertiesSelection, AppFrame::OnFileMenuUpdateUI)

    EVT_UPDATE_UI(idFileImportRecent, AppFrame::OnRecentImportsMenuUpdateUI)

    EVT_UPDATE_UI(idEditUndo, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditRedo, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCut, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditCopy, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPaste, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPasteInsert, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditPasteAttributes, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditClear, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditRippleDelete, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditDuplicate, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSelectAll, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditDeselectAll, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabel, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelBlue, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelCyan, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelGreen, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelViolet, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelPink, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelGray, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelRed, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditLabelOrange, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditOriginal, AppFrame::OnEditMenuUpdateUI)
    EVT_UPDATE_UI(idEditSequenceMarker, AppFrame::OnEditMenuUpdateUI)

    EVT_UPDATE_UI(idFrameUpdateTitleUI, AppFrame::OnUpdateTitleUI)
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

    // create a menu bar
    wxMenuBar* mbar = wxXmlResource::Get()->LoadMenuBar(wxT("main_menu_bar"));
    if(mbar) {
        SetMenuBar(mbar);
    } else {
        wxLogError(_("Could not find the XRC resource 'main_menu_bar'!\nAre you sure the program was installed correctly?"));
        Destroy();
        return;
    }

    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(wxbuildinfo(short_f), 1);
    g_statustext = _("Welcome to ") + APP_SHOWNAME + _T("! ^_^");
    UpdateStatustext();

}

wxMenu* AppFrame::FindMenu(const wxString name) {
    wxMenu* result = NULL;
    wxMenuBar* bar = GetMenuBar();
    do {
        if(!bar) break;
        int myMenu = bar->FindMenu(name);
        if(myMenu == wxNOT_FOUND) break;
        result = bar->GetMenu(myMenu);
    }while(false);
    return result;
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
    ShutDownApp();
    ProjectManager::Unload();
}

bool AppFrame::IsClipSelected() {
    return false;
}
bool AppFrame::IsSelectionMultiple() {
    return false;
}

bool AppFrame::IsResourceClipSelected() {
    return false;
}

bool AppFrame::IsTimelineActive() { return false; }
bool AppFrame::IsResourceWindowActive() { return false; }
bool AppFrame::IsMonitorActive() { return false; }
bool AppFrame::IsRenderMonitorActive() { return false; }
bool AppFrame::IsClipMonitorActive() { return false; }
bool AppFrame::IsEffectsWindowActive() { return false; }
bool AppFrame::IsTitleWindowActive() { return false; }
bool AppFrame::IsClipboardSet() {
    return false;
}


bool AppFrame::CanUndo() {
    if(IsAppShuttingDown())
        return false;
    if(!ProjectManager::Get()->HasProject())
        return false;
    return ProjectManager::Get()->GetProject()->CanUndo();
}
bool AppFrame::CanRedo() {
    if(IsAppShuttingDown())
        return false;
    if(!ProjectManager::Get()->HasProject())
        return false;
    return ProjectManager::Get()->GetProject()->CanUndo();
}

void AppFrame::OnFileOpen(wxCommandEvent& event) {
    if(IsAppShuttingDown())
        return;
    wxString lastdir = ProjectManager::Get()->GetLastProjectDir();
    wxFileDialog myDialog(this, _("Choose a project"), lastdir, _T(""), _T("*.saya"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("opendlg"));
    int dialogresult = myDialog.ShowModal();
    if(dialogresult == wxID_OK) {
        if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
            bool result = ProjectManager::Get()->LoadProject(myDialog.GetPath());
            if(!result) {
                wxString msg;
                msg.Printf(_("Error opening file '%s'!"),myDialog.GetPath().c_str());
                wxMessageBox(msg,_("Error"),wxCANCEL | wxICON_ERROR,this);
            }
            DoUpdateAppTitle();
        }
    }
}

void AppFrame::OnOpenRecentFile(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
        int fileno = event.GetId() - wxID_FILE1;
        bool result = ProjectManager::Get()->LoadRecentProject(fileno);
        if(!result) {
            wxString msg;
            msg.Printf(_("Error opening file '%s'!"),ProjectManager::Get()->GetRecentProjectName(fileno).c_str());
            wxMessageBox(msg,_("Error"),wxCANCEL | wxICON_ERROR,this);
        }
        DoUpdateAppTitle();
    }
}

void AppFrame::OnClearRecentProjectList(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->ClearRecentFiles();
}

void AppFrame::OnFileClose(wxCommandEvent& event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->CloseProject(false);
    DoUpdateAppTitle();
}


void AppFrame::OnClose(wxCloseEvent &event) {
    bool willveto = false;
    if(event.CanVeto()) {
        if(!IsAppShuttingDown()) {
            if(!ProjectManager::Get()->CloseProject(false))
                willveto = true;
        }
    } else {
        if(!IsAppShuttingDown()) {
            ProjectManager::Get()->CloseProject(true);
        }
    }

    if(willveto) {
        event.Veto();
    } else {
        Destroy();
    }
}

void AppFrame::OnFileSave(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProject();
}

void AppFrame::OnFileSaveAs(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProjectAs();
}

void AppFrame::OnFileSaveCopy(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProjectCopy();
}


void AppFrame::OnQuit(wxCommandEvent &event) {
    Close();
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
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    VidProject* prj = pmgr->GetProject();

    bool hasproject = (prj);
    bool isModified = hasproject && prj->IsModified();
    bool isNew = hasproject && prj->IsNew();

    wxMenuBar* mbar = GetMenuBar();
    mbar->Enable(idFileClose,hasproject);
    mbar->Enable(idFileSave,hasproject && (isModified || isNew));
    mbar->Enable(idFileSaveAs,hasproject);
    mbar->Enable(idFileSaveCopy,hasproject);
    mbar->Enable(idFileRevert,hasproject && isModified && !isNew);
    mbar->Enable(idFileInterpretFootage,hasproject && (IsClipSelected() || IsResourceClipSelected()));
    mbar->Enable(idFileTimecode,hasproject && IsClipSelected());
    mbar->Enable(idFileImport,hasproject);
    mbar->Enable(idFileExport,hasproject);
    mbar->Enable(idFileGetPropertiesSelection, hasproject && (IsClipSelected() || IsResourceClipSelected()));
}

void AppFrame::OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event) {
    // Update the Recent Projects list
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->m_recentfilesmodified) {
        pmgr->m_recentfilesmodified = false;
        wxMenuItem* myItem = GetMenuBar()-> FindItem(idFileOpenRecentProject);
        if(myItem) {
            wxMenu* mySubMenu = myItem->GetSubMenu();

            if(mySubMenu) { // Clear all items in the submenu
                while(mySubMenu->GetMenuItemCount()) {
                    wxMenuItem* subitem = mySubMenu->FindItemByPosition(0);
                    if(!subitem)
                        break;
                    mySubMenu->Delete(subitem);
                }
            } else { // Add a new submenu
                mySubMenu = new wxMenu();
                myItem->SetSubMenu(mySubMenu);
            }
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

void AppFrame::OnRecentImportsMenuUpdateUI(wxUpdateUIEvent& event) {
    // Update the Recent Imported files list
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->m_recentimportsmodified) {
        pmgr->m_recentimportsmodified = false;
        wxMenuItem* myItem = GetMenuBar()-> FindItem(idFileImportRecent);
        if(myItem) {
            wxMenu* mySubMenu = myItem->GetSubMenu();

            if(mySubMenu) { // Clear all items in the submenu
                while(mySubMenu->GetMenuItemCount()) {
                    wxMenuItem* subitem = mySubMenu->FindItemByPosition(0);
                    if(!subitem)
                        break;
                    mySubMenu->Delete(subitem);
                }
            } else { // Add a new submenu
                mySubMenu = new wxMenu();
                myItem->SetSubMenu(mySubMenu);
            }
            size_t i = 0;
            mySubMenu->Append(idFileClearRecentImportList,_T("&Clear"),_("Clears Recent Imported Files List"));
            mySubMenu->AppendSeparator();
            for(i = 0; i < pmgr->m_recentimports.size(); i++) {
                wxString tmptext;
                tmptext.Printf(_T("&%d ") + pmgr->m_recentimports[i],i+1);
                mySubMenu->Append(wxID_IMPORT1+i,tmptext,wxEmptyString);
            }
        }
    }
    wxMenuBar* mbar = GetMenuBar();
    mbar->Enable(idFileImportRecent,pmgr->m_recentimports.size() > 0 && (pmgr->HasProject()));
}

void AppFrame::OnEditMenuUpdateUI(wxUpdateUIEvent& event) {

    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    VidProject* prj = pmgr->GetProject();

    bool hasproject = (prj);
    bool canUndo = CanUndo();
    bool canRedo = CanRedo();
    bool canCutOrCopy = hasproject && (IsClipSelected() || IsResourceClipSelected());
    bool canPaste = hasproject && (IsClipboardSet() && IsTimelineActive());

    wxMenuBar* mbar = GetMenuBar();
    mbar->Enable(idEditUndo,canUndo);
    mbar->Enable(idEditRedo,canRedo);
    mbar->Enable(idEditClearUndoHistory,canUndo || canRedo);
    mbar->Enable(idEditCut, canCutOrCopy);
    mbar->Enable(idEditCopy, canCutOrCopy);
    mbar->Enable(idEditFind, hasproject);
    mbar->Enable(idEditPaste, canPaste);
    mbar->Enable(idEditPasteInsert, canPaste);
    mbar->Enable(idEditPasteAttributes, canPaste);
    mbar->Enable(idEditClear, hasproject && IsClipSelected());
    mbar->Enable(idEditRippleDelete, hasproject && IsClipSelected());
    mbar->Enable(idEditDuplicate, hasproject && IsClipSelected());
    mbar->Enable(idEditSelectAll, hasproject);
    mbar->Enable(idEditDeselectAll, hasproject);
    mbar->Enable(idEditLabel, hasproject && IsClipSelected());
    mbar->Enable(idEditOriginal, hasproject && IsClipSelected() && !IsSelectionMultiple());
//    idEditLabel
//    idEditLabelBlue
//    idEditLabelCyan
//    idEditLabelGreen
//    idEditLabelViolet
//    idEditLabelPink
//    idEditLabelGray
//    idEditLabelRed
//    idEditLabelOrange
//
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

void AppFrame::OnUpdateTitleUI(wxUpdateUIEvent& event) {
    if(IsAppShuttingDown())
        return;
    DoUpdateAppTitle();
}

void AppFrame::DoUpdateAppTitle() {
    if(IsAppShuttingDown())
        return;
    wxString title;
    wxString modified_str = wxEmptyString;
    VidProject* prj = ProjectManager::Get()->GetProject();

    if(prj) {
        if(prj->IsModified()) {
            modified_str = _T("* ");
        }
        title = modified_str + prj->m_Title;

        if(!prj->m_Filename.IsEmpty()) {
            wxFileName fname(prj->m_Filename);
            fname.GetFullName();
            title += _T(" [") + fname.GetFullName() + _T("]");
        } else {
            title += wxString(_T(" [")) + _("untitled") + _T("]");
        }
        title += _T(" - ");
        title += APP_SHOWNAME;
    } else {
        title = APP_SHOWOFFNAME;
    }
    SetTitle(title);
}

void AppFrame::OnAbout(wxCommandEvent &event) {
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
