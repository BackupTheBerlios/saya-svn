/***************************************************************
 * Name:      sayaMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <memory>

#include <qmainwindow.h>
#include <qstatusbar.h>
#include <qtreewidget.h>
#include <qmenubar.h>
#include <qmenu.h>
#include <qaction.h>

#include "main.ui.h"

#include "../saya/core/app.h"
#include "../saya/core/systring.h"
#include "../saya/core/intl.h"
#include "../saya/core/iocommon.h"
#include "../saya/core/config.h"
#include "../saya/core/debuglog.h"
#include "../saya/core/evtregistry.h"
#include "../saya/core/sythread.h"

#include "../saya/saya_events.h"
#include "../saya/vidproject.h"
#include "../saya/recentfileslist.h"
#include "../saya/projectmanager.h"
#include "../saya/playbackmanager.h"

#include "main.h"
#include "welcomedlg.h"
#include "newprojectdlg.h"
#include "app.h"

//#include "wxplaybackcontrolpanel.h"

//helper functions
enum qbuildinfoformat {
    short_f, long_f
};

syString qbuildinfo(qbuildinfoformat format)
{
    syString s("Qt version: ");
    s << QT_VERSION_STR;

    if (format == long_f )
    {
        s << "(" <<
#if defined(__WIN32__)
        "Windows"
#elif defined(__APPLE__)
        "Mac"
#elif defined(__linux__)
        "Linux"
#elif defined(__unix__)
        "Unix"
#endif
        << ")";
    }

    return s;
}

void LoadFail(const char* resourcename) {
    syString s;
    s.Printf(_("Could not find the resource '%s'!\nAre you sure the program was installed correctly?"),resourcename);
    syApp::Get()->ErrorMessageBox(s.c_str());
}

QMainWindow* CreateMainFrame() {
    AppFrame* frame = 0;
    frame = new AppFrame(_("Saya - Unsheathe your Creativity"));
//    QMainWindow* frame = new QMainWindow;
    syApp::Get()->SetTopWindow(frame);
    frame->show();
    return frame;
}

// ------------------------------
// begin constants and event id's
// ------------------------------

const syString CFG_LAYOUTS = "Layouts";
const syString CFG_LAYOUT_DEFAULT = CFG_LAYOUTS + "/Default";
const syString CFG_LOCATION = CFG_LAYOUT_DEFAULT + "/Location";
const syString CFG_PERSPECTIVE_DEFAULT = CFG_LAYOUT_DEFAULT + "/Perspective";
const syString CFG_DEFAULT_PRJ_SASHPOS = CFG_LAYOUT_DEFAULT + "/PrjSashPos";

unsigned int idFileNew = syActionEvent::RegisterId("action_FileNew");
unsigned int idNewProject = syActionEvent::RegisterId("action_NewProject", "OnNewProject()");
unsigned int idNewSequence = syActionEvent::RegisterId("action_NewSequence");
unsigned int idNewBin = syActionEvent::RegisterId("action_NewBin");
unsigned int idNewOfflineFile = syActionEvent::RegisterId("action_NewOfflineFile");
unsigned int idNewTitle = syActionEvent::RegisterId("action_NewTitle");
unsigned int idNewBarsandTone = syActionEvent::RegisterId("action_NewBarsandTone");
unsigned int idNewBlackVideo = syActionEvent::RegisterId("action_NewBlackVideo");
unsigned int idNewColorMatte = syActionEvent::RegisterId("action_NewColorMatte");
unsigned int idNewUniversalCountingLeader = syActionEvent::RegisterId("action_NewUniversalCountingLeader");

unsigned int idFileOpen = syActionEvent::RegisterId("action_FileOpen","OnFileOpen()");
unsigned int idFileOpenRecentProject = syActionEvent::RegisterId("action_FileOpenRecentProject");
unsigned int idFileClearRecentProjectList = syActionEvent::RegisterId("OnClearRecentProjectList()");
unsigned int idFileClearRecentImportList = syActionEvent::RegisterId("OnClearRecentImportList()");
unsigned int idFileClose = syActionEvent::RegisterId("action_FileClose","OnFileClose()");
unsigned int idFileSave = syActionEvent::RegisterId("action_FileSave","OnFileSave()");
unsigned int idFileSaveAs = syActionEvent::RegisterId("action_FileSaveAs","OnFileSaveAs()");
unsigned int idFileSaveCopy = syActionEvent::RegisterId("action_FileSaveCopy","OnFileSaveCopy()");
unsigned int idFileRevert = syActionEvent::RegisterId("action_FileRevert","OnFileRevert()");
unsigned int idFileCapture = syActionEvent::RegisterId("action_FileCapture","OnFileCapture()");
unsigned int idFileBatchCapture = syActionEvent::RegisterId("action_FileBatchCapture","OnFileBatchCapture()");
unsigned int idFileImport = syActionEvent::RegisterId("action_FileImport","OnFileImport()");
unsigned int idFileImportRecent = syActionEvent::RegisterId("action_FileImportRecent");
unsigned int idFileExport = syActionEvent::RegisterId("action_FileExport","OnFileExport()");
unsigned int idFileGetProperties = syActionEvent::RegisterId("action_FileGetProperties");
unsigned int idFileGetPropertiesFile = syActionEvent::RegisterId("action_FileGetPropertiesFile");
unsigned int idFileGetPropertiesSelection = syActionEvent::RegisterId("action_FileGetPropertiesSelection");
unsigned int idFileInterpretFootage = syActionEvent::RegisterId("action_FileInterpretFootage","OnFileInterpretFootage()");
unsigned int idFileTimecode = syActionEvent::RegisterId("action_FileTimecode","OnFileTimecode()");

unsigned int idRecentProject1 = syActionEvent::RegisterId("action_RecentProject1","OnOpenRecentFile1()");
unsigned int idRecentProject2 = syActionEvent::RegisterId(idRecentProject1 + 1,"action_RecentProject2","OnOpenRecentFile2()");
unsigned int idRecentProject3 = syActionEvent::RegisterId(idRecentProject1 + 2,"action_RecentProject3","OnOpenRecentFile3()");
unsigned int idRecentProject4 = syActionEvent::RegisterId(idRecentProject1 + 3,"action_RecentProject4","OnOpenRecentFile4()");
unsigned int idRecentProject5 = syActionEvent::RegisterId(idRecentProject1 + 4,"action_RecentProject5","OnOpenRecentFile5()");
unsigned int idRecentProject6 = syActionEvent::RegisterId(idRecentProject1 + 5,"action_RecentProject6","OnOpenRecentFile6()");
unsigned int idRecentProject7 = syActionEvent::RegisterId(idRecentProject1 + 6,"action_RecentProject7","OnOpenRecentFile7()");
unsigned int idRecentProject8 = syActionEvent::RegisterId(idRecentProject1 + 7,"action_RecentProject8","OnOpenRecentFile8()");
unsigned int idRecentProject9 = syActionEvent::RegisterId(idRecentProject1 + 8,"action_RecentProject9","OnOpenRecentFile9()");

unsigned int idRecentImport1 = syActionEvent::RegisterId("action_RecentImport1","OnOpenRecentImport1()");
unsigned int idRecentImport2 = syActionEvent::RegisterId(idRecentImport1 + 1,"action_RecentImport2","OnOpenRecentImport2()");
unsigned int idRecentImport3 = syActionEvent::RegisterId(idRecentImport1 + 2,"action_RecentImport3","OnOpenRecentImport3()");
unsigned int idRecentImport4 = syActionEvent::RegisterId(idRecentImport1 + 3,"action_RecentImport4","OnOpenRecentImport4()");
unsigned int idRecentImport5 = syActionEvent::RegisterId(idRecentImport1 + 4,"action_RecentImport5","OnOpenRecentImport5()");
unsigned int idRecentImport6 = syActionEvent::RegisterId(idRecentImport1 + 5,"action_RecentImport6","OnOpenRecentImport6()");
unsigned int idRecentImport7 = syActionEvent::RegisterId(idRecentImport1 + 6,"action_RecentImport7","OnOpenRecentImport7()");
unsigned int idRecentImport8 = syActionEvent::RegisterId(idRecentImport1 + 7,"action_RecentImport8","OnOpenRecentImport8()");
unsigned int idRecentImport9 = syActionEvent::RegisterId(idRecentImport1 + 8,"action_RecentImport9","OnOpenRecentImport9()");

unsigned int idEditUndo = syActionEvent::RegisterId("action_EditUndo");
unsigned int idEditRedo = syActionEvent::RegisterId("action_EditRedo");
unsigned int idEditClearUndoHistory = syActionEvent::RegisterId("action_EditClearUndoHistory");
unsigned int idEditCut = syActionEvent::RegisterId("action_EditCut");
unsigned int idEditCopy = syActionEvent::RegisterId("action_EditCopy");
unsigned int idEditPaste = syActionEvent::RegisterId("action_EditPaste");
unsigned int idEditPasteInsert = syActionEvent::RegisterId("action_EditPasteInsert");
unsigned int idEditPasteAttributes = syActionEvent::RegisterId("action_EditPasteAttributes");
unsigned int idEditClear = syActionEvent::RegisterId("action_EditClear");
unsigned int idEditRippleDelete = syActionEvent::RegisterId("action_EditRippleDelete");
unsigned int idEditDuplicate = syActionEvent::RegisterId("action_EditDuplicate");
unsigned int idEditSelectAll = syActionEvent::RegisterId("action_EditSelectAll");
unsigned int idEditDeselectAll = syActionEvent::RegisterId("action_EditDeselectAll");
unsigned int idEditFind = syActionEvent::RegisterId("action_EditFind");
unsigned int idEditLabel = syActionEvent::RegisterId("action_EditLabel");
unsigned int idEditLabelBlue = syActionEvent::RegisterId("action_EditLabelBlue");
unsigned int idEditLabelCyan = syActionEvent::RegisterId("action_EditLabelCyan");
unsigned int idEditLabelGreen = syActionEvent::RegisterId("action_EditLabelGreen");
unsigned int idEditLabelViolet = syActionEvent::RegisterId("action_EditLabelViolet");
unsigned int idEditLabelPink = syActionEvent::RegisterId("action_EditLabelPink");
unsigned int idEditLabelGray = syActionEvent::RegisterId("action_EditLabelGray");
unsigned int idEditLabelRed = syActionEvent::RegisterId("action_EditLabelRed");
unsigned int idEditLabelOrange = syActionEvent::RegisterId("action_EditLabelOrange");
unsigned int idEditOriginal = syActionEvent::RegisterId("action_EditOriginal");

unsigned int idProjectSettings = syActionEvent::RegisterId("action_ProjectSettings");
unsigned int idProjectSettingsGeneral = syActionEvent::RegisterId("action_ProjectSettingsGeneral");
unsigned int idProjectSettingsCapture = syActionEvent::RegisterId("action_ProjectSettingsCapture");
unsigned int idProjectSettingsVideoRendering = syActionEvent::RegisterId("action_ProjectSettingsVideoRendering");
unsigned int idProjectSettingsDefaultSequence = syActionEvent::RegisterId("action_ProjectSettingsDefaultSequence");
unsigned int idProjectLinkMedia = syActionEvent::RegisterId("action_ProjectLinkMedia");
unsigned int idProjectUnlinkMedia = syActionEvent::RegisterId("action_ProjectUnlinkMedia");
unsigned int idProjectAutomateToSequence = syActionEvent::RegisterId("action_ProjectAutomateToSequence");
unsigned int idProjectImportBatchList = syActionEvent::RegisterId("action_ProjectImportBatchList");
unsigned int idProjectExportBatchList = syActionEvent::RegisterId("action_ProjectExportBatchList");
unsigned int idProjectExportAsAAF = syActionEvent::RegisterId("action_ProjectExportAsAAF");

unsigned int idClipRename = syActionEvent::RegisterId("action_ClipRename");
unsigned int idClipCaptureSettings = syActionEvent::RegisterId("action_ClipCaptureSettings");
unsigned int idClipSetCaptureSettings = syActionEvent::RegisterId("action_ClipSetCaptureSettings");
unsigned int idClipClearCaptureSettings = syActionEvent::RegisterId("action_ClipClearCaptureSettings");
unsigned int idClipInsert = syActionEvent::RegisterId("action_ClipInsert");
unsigned int idClipOverlay = syActionEvent::RegisterId("action_ClipOverlay");
unsigned int idClipToggleEnable = syActionEvent::RegisterId("action_ClipToggleEnable");
unsigned int idClipUnlinkAV = syActionEvent::RegisterId("action_ClipUnlinkAV");
unsigned int idClipLinkAV = syActionEvent::RegisterId("action_ClipLinkAV");
unsigned int idClipGroup = syActionEvent::RegisterId("action_ClipGroup");
unsigned int idClipUngroup = syActionEvent::RegisterId("action_ClipUngroup");
unsigned int idClipVideoOptions = syActionEvent::RegisterId("action_ClipVideoOptions");
unsigned int idClipVOptFrameHold = syActionEvent::RegisterId("action_ClipVOptFrameHold");
unsigned int idClipVOptFieldOptions = syActionEvent::RegisterId("action_ClipVOptFieldOptions");
unsigned int idClipAudioOptions = syActionEvent::RegisterId("action_ClipAudioOptions");
unsigned int idClipAOptGain = syActionEvent::RegisterId("action_ClipAOptGain");
unsigned int idClipAOptBreakoutToMono = syActionEvent::RegisterId("action_ClipAOptBreakoutToMono");
unsigned int idClipOptTreatAsStereo = syActionEvent::RegisterId("action_ClipOptTreatAsStereo");
unsigned int idClipOptRenderAndReplace = syActionEvent::RegisterId("action_ClipOptRenderAndReplace");
unsigned int idClipSpeedDuration = syActionEvent::RegisterId("action_ClipSpeedDuration");

unsigned int idSequenceRender = syActionEvent::RegisterId("action_SequenceRender");
unsigned int idSequenceDeleteRenderFiles = syActionEvent::RegisterId("action_SequenceDeleteRenderFiles");
unsigned int idSequenceRazor = syActionEvent::RegisterId("action_SequenceRazor");
unsigned int idSequenceRazorSelectedTracks = syActionEvent::RegisterId("action_SequenceRazorSelectedTracks");
unsigned int idSequenceLift = syActionEvent::RegisterId("action_SequenceLift");
unsigned int idSequenceExtract = syActionEvent::RegisterId("action_SequenceExtract");
unsigned int idSequenceApplyVideoTransition = syActionEvent::RegisterId("action_SequenceApplyVideoTransition");
unsigned int idSequenceApplyAudioTransition = syActionEvent::RegisterId("action_SequenceApplyAudioTransition");
unsigned int idSequenceZoomIn = syActionEvent::RegisterId("action_SequenceZoomIn");
unsigned int idSequenceZoomOut = syActionEvent::RegisterId("action_SequenceZoomOut");
unsigned int idSequenceSnap = syActionEvent::RegisterId("action_SequenceSnap");
unsigned int idSequenceAddTracks = syActionEvent::RegisterId("action_SequenceAddTracks");
unsigned int idSequenceDelTracks = syActionEvent::RegisterId("action_SequenceDelTracks");

unsigned int idSetClipMarkerMenu = syActionEvent::RegisterId("action_SetClipMarkerMenu");
unsigned int idGotoClipMarkerMenu = syActionEvent::RegisterId("action_GotoClipMarkerMenu");
unsigned int idGotoNextClipMarker = syActionEvent::RegisterId("action_GotoNextClipMarker");
unsigned int idGotoPrevClipMarker = syActionEvent::RegisterId("action_GotoPrevClipMarker");
unsigned int idGotoInClipMarker = syActionEvent::RegisterId("action_GotoInClipMarker");
unsigned int idGotoOutClipMarker = syActionEvent::RegisterId("action_GotoOutClipMarker");
unsigned int idGotoVideoInClipMarker = syActionEvent::RegisterId("action_GotoVideoInClipMarker");
unsigned int idGotoVideoOutClipMarker = syActionEvent::RegisterId("action_GotoVideoOutClipMarker");
unsigned int idGotoAudioInClipMarker = syActionEvent::RegisterId("action_GotoAudioInClipMarker");
unsigned int idGotoAudioOutClipMarker = syActionEvent::RegisterId("action_GotoAudioOutClipMarker");
unsigned int idGotoNumberedClipMarker = syActionEvent::RegisterId("action_GotoNumberedClipMarker");
unsigned int idClearClipMarkerMenu = syActionEvent::RegisterId("action_ClearClipMarkerMenu");
unsigned int idClearCurrentClipMarker = syActionEvent::RegisterId("action_ClearCurrentClipMarker");
unsigned int idClearAllClipMarkers = syActionEvent::RegisterId("action_ClearAllClipMarkers");
unsigned int idClearInOutClipMarkers = syActionEvent::RegisterId("action_ClearInOutClipMarkers");
unsigned int idClearInClipMarker = syActionEvent::RegisterId("action_ClearInClipMarker");
unsigned int idClearOutClipMarker = syActionEvent::RegisterId("action_ClearOutClipMarker");
unsigned int idClearNumberedClipMarker = syActionEvent::RegisterId("action_ClearNumberedClipMarker");
unsigned int idSetSequenceMarkerMenu = syActionEvent::RegisterId("action_SetSequenceMarkerMenu");
unsigned int idSetInSequenceMarker = syActionEvent::RegisterId("action_SetInSequenceMarker");
unsigned int idSetOutSequenceMarker = syActionEvent::RegisterId("action_SetOutSequenceMarker");
unsigned int idSetInOutAroundSelSeqMarker = syActionEvent::RegisterId("action_SetInOutAroundSelSeqMarker");
unsigned int idSetUnnumberedSequenceMarker = syActionEvent::RegisterId("action_SetUnnumberedSequenceMarker");
unsigned int idSetNextAvailNumberedSeqMarker = syActionEvent::RegisterId("action_SetNextAvailNumberedSeqMarker");
unsigned int idSetOtherNumberedSequenceMarker = syActionEvent::RegisterId("action_SetOtherNumberedSequenceMarker");
unsigned int idGotoSequenceMarkerMenu = syActionEvent::RegisterId("action_GotoSequenceMarkerMenu");
unsigned int idGotoNextSequenceMarker = syActionEvent::RegisterId("action_GotoNextSequenceMarker");
unsigned int idGotoPrevSequenceMarker = syActionEvent::RegisterId("action_GotoPrevSequenceMarker");
unsigned int idGotoInSequenceMarker = syActionEvent::RegisterId("action_GotoInSequenceMarker");
unsigned int idGotoOutSequenceMarker = syActionEvent::RegisterId("action_GotoOutSequenceMarker");
unsigned int idGotoNumberedSeqMarker = syActionEvent::RegisterId("action_GotoNumberedSeqMarker");
unsigned int idClearSequenceMarkerMenu = syActionEvent::RegisterId("action_ClearSequenceMarkerMenu");
unsigned int idClearCurSequenceMarker = syActionEvent::RegisterId("action_ClearCurSequenceMarker");
unsigned int idClearAllSequenceMarkers = syActionEvent::RegisterId("action_ClearAllSequenceMarkers");
unsigned int idClearInOutSeqMarkers = syActionEvent::RegisterId("action_ClearInOutSeqMarkers");
unsigned int idClearInSeqMarker = syActionEvent::RegisterId("action_ClearInSeqMarker");
unsigned int idClearOutSeqMarker = syActionEvent::RegisterId("action_ClearOutSeqMarker");
unsigned int idClearNumberedSeqMarker = syActionEvent::RegisterId("action_ClearNumberedSeqMarker");
unsigned int idEditSequenceMarker = syActionEvent::RegisterId("action_EditSequenceMarker");

unsigned int idWindowWorkspaceMenu = syActionEvent::RegisterId("action_WindowWorkspaceMenu");
unsigned int idWorkspaceEditing = syActionEvent::RegisterId("action_WorkspaceEditing");
unsigned int idWorkspaceEffects = syActionEvent::RegisterId("action_WorkspaceEffects");
unsigned int idWorkspaceAudio = syActionEvent::RegisterId("action_WorkspaceAudio");
unsigned int idWorkspaceColorCorrection = syActionEvent::RegisterId("action_WorkspaceColorCorrection");
unsigned int idWorkspaceDefault = syActionEvent::RegisterId("action_WorkspaceDefault");
unsigned int idWorkspaceFactoryDefault = syActionEvent::RegisterId("action_WorkspaceFactoryDefault");
unsigned int idWorkspaceSaveAs = syActionEvent::RegisterId("action_WorkspaceSaveAs");
unsigned int idWorkspaceDelete = syActionEvent::RegisterId("action_WorkspaceDelete");
unsigned int idWorkspaceCustom = syActionEvent::RegisterId("action_WorkspaceCustom");
unsigned int idMenuSaveFrameLayout = syActionEvent::RegisterId("action_MenuSaveFrameLayout");
unsigned int idWindowEffects = syActionEvent::RegisterId("action_WindowEffects");
unsigned int idWindowEffectControls = syActionEvent::RegisterId("action_WindowEffectControls");
unsigned int idWindowHistory = syActionEvent::RegisterId("action_WindowHistory");
unsigned int idWindowInfo = syActionEvent::RegisterId("action_WindowInfo");
unsigned int idWindowTools = syActionEvent::RegisterId("action_WindowTools");
unsigned int idWindowAudioMixer = syActionEvent::RegisterId("action_WindowAudioMixer");
unsigned int idWindowMonitor = syActionEvent::RegisterId("action_WindowMonitor");
unsigned int idWindowProject = syActionEvent::RegisterId("action_WindowProject");
unsigned int idWindowTimelinesMenu = syActionEvent::RegisterId("action_WindowTimelinesMenu");

// ----------------------------
// end constants and event id's
// ----------------------------

// --------------------
// begin AppFrame::Data
// --------------------

class AppFrame::Data : public QObject, public syEvtHandler {
    Q_OBJECT
    public:
        Data(AppFrame* parent);
        virtual ~Data();
        AppFrame* m_Parent;

        QWidget* CreateProjectPane(); /// Creates the project pane
        QWidget* m_ProjectPanel; /// Project Panel
        QWidget* m_MonitorPanel; /// Monitor Panel
        QWidget* m_EffectsPanel; /// Effects Panel
        QWidget* m_TimelinePanel; /// Timeline Panel
        WelcomeDialog* m_WelcomeDialog;
        QTreeWidget* m_ResourcesTree; /// Resources Tree in the Project Panel

        bool LoadResources();
        void CreateDockAreas();
        bool CreateDialogs();
        void CreateConnections();
        bool CreateMenuBar();
        bool CreatePanels();
        long GetProjectPanelSashPos();
        void ShowLayout(bool show);

        void ShowWelcomeDialog();

    public:
//        wxAuiManager *m_mgr;
        bool m_hadproject;
        bool m_panes_status_checked;
        bool m_layouthidden;
        unsigned int m_recentfilesmodcounter;
        unsigned int m_recentimportsmodcounter;
//        wxString m_CurrentPerspective;
//        wxString m_FactoryDefaultLayout;
        void RegisterAction(unsigned int id, QAction* action);
        void UnregisterAction(unsigned int id);
        void UnregisterAllActions();
        void EnableAction(unsigned int id, bool enable = true);
        void DisableAction(unsigned int id);
        QAction* GetAction(unsigned int id);
        void OnActionEvent(syActionEvent& event);
        Ui::MainWindow* m_Ui;
    private:
        typedef std::map<unsigned int, QAction*> ActionsMap;
        ActionsMap m_ActionsMap;
        syMutex m_ActionsMutex;
    public slots:

//        void OnResourceTreeContextMenu(wxTreeEvent& event);

//
        void OnFileOpen();
        void OnFileClose();

        void OnClearRecentProjectList();
        void OnClearRecentImportList();
        void OnOpenRecentFile();
        void OnFileSave();
        void OnFileSaveAs();
        void OnFileSaveCopy();
        void OnNewProject();
        void OnFileRevert();
        void OnFileCapture();
        void OnFileBatchCapture();

        void OnQuit();
        void OnAbout();
        void OnSaveFrameLayout();
        void OnLoadDefaultLayout();
        void OnWorkspaceFactoryDefault();

        // UpdateUI events
        void OnFileMenuUpdateUI();
        void OnRecentFilesMenuUpdateUI();
        void OnRecentImportsMenuUpdateUI();
        void OnEditMenuUpdateUI();
        void OnProjectMenuUpdateUI();
        void OnClipMenuUpdateUI();
        void OnSequenceMenuUpdateUI();
        void OnMarkerMenuUpdateUI();
        void OnWindowMenuUpdateUI();

//        void LoadAndSetFrameSize();
//        void SaveDefaultLayout(bool showmsg);
//        bool LoadDefaultLayout(bool firsttime = false);
//
//        void DoUpdateAppTitle();
//
//        void OnUpdateProjectPaneUI(wxUpdateUIEvent& event);

};

AppFrame::Data::Data(AppFrame* parent) :
m_Parent(parent),
m_WelcomeDialog(0),
m_hadproject(false),
m_panes_status_checked(false),
m_layouthidden(false),
m_recentfilesmodcounter(0),
m_recentimportsmodcounter(0),
m_Ui(new Ui::MainWindow)
{
    UnregisterAllActions();
    m_Ui->setupUi(m_Parent);
}

AppFrame::Data::~Data() {

//    m_mgr->UnInit();
    UnregisterAllActions();
    delete m_WelcomeDialog;
    delete m_Ui;
//    delete m_mgr;

}

bool AppFrame::Data::LoadResources() {
    return true;
}

void AppFrame::Data::RegisterAction(unsigned int id, QAction* action) {
    if(!this) return;
    syMutexLocker locker(m_ActionsMutex);
    m_ActionsMap[id] = action;
    action->setData(id);
}

void AppFrame::Data::UnregisterAction(unsigned int id) {
    if(!this) return;
    syMutexLocker locker(m_ActionsMutex);
    m_ActionsMap[id] = 0;
}

void AppFrame::Data::UnregisterAllActions() {
    if(!this) return;
    syMutexLocker locker(m_ActionsMutex);
    m_ActionsMap.clear();
}

inline QAction* AppFrame::Data::GetAction(unsigned int id) {
    if(!this) return 0;
    ActionsMap::iterator it = m_ActionsMap.find(id);
    if(it == m_ActionsMap.end()) {
        return 0;
    }
    return it->second;
}

inline void AppFrame::Data::EnableAction(unsigned int id, bool enable) {
    if(!this) return;
    ActionsMap::iterator it = m_ActionsMap.find(id);
    if(it != m_ActionsMap.end()) {
        it->second->setEnabled(enable);
    }
}

inline void AppFrame::Data::DisableAction(unsigned int id) {
    EnableAction(id, false);
}


void AppFrame::Data::OnActionEvent(syActionEvent& event) {
    if(!this) return;
    QAction* action = GetAction(event.EventId);
    if(action) {
        action->trigger();
    }
}

void AppFrame::Data::ShowWelcomeDialog() {
    m_Parent->hide();
    m_WelcomeDialog->raise();
    m_WelcomeDialog->show();
}

bool AppFrame::Data::CreateMenuBar() {
    return true;
}

void AppFrame::Data::CreateConnections() {
    // TODO: Connect the actions to the corresponding procedures using their id to obtain the corresponding slot.
}

void AppFrame::Data::OnFileOpen(){
}

void AppFrame::Data::OnFileClose(){
}

void AppFrame::Data::OnClearRecentProjectList(){
}

void AppFrame::Data::OnClearRecentImportList(){
}

void AppFrame::Data::OnOpenRecentFile(){
}

void AppFrame::Data::OnFileSave(){
}

void AppFrame::Data::OnFileSaveAs(){
}

void AppFrame::Data::OnFileSaveCopy(){
}

void AppFrame::Data::OnNewProject(){
}

void AppFrame::Data::OnFileRevert(){
}

void AppFrame::Data::OnFileCapture(){
}

void AppFrame::Data::OnFileBatchCapture(){
}

void AppFrame::Data::OnQuit(){
}

void AppFrame::Data::OnAbout(){
}

void AppFrame::Data::OnSaveFrameLayout(){
}

void AppFrame::Data::OnLoadDefaultLayout(){
}

void AppFrame::Data::OnWorkspaceFactoryDefault(){
}

void AppFrame::Data::OnFileMenuUpdateUI(){
}

void AppFrame::Data::OnRecentFilesMenuUpdateUI(){
}

void AppFrame::Data::OnRecentImportsMenuUpdateUI(){
}

void AppFrame::Data::OnEditMenuUpdateUI(){
}

void AppFrame::Data::OnProjectMenuUpdateUI(){
}

void AppFrame::Data::OnClipMenuUpdateUI(){
}

void AppFrame::Data::OnSequenceMenuUpdateUI(){
}

void AppFrame::Data::OnMarkerMenuUpdateUI(){
}

void AppFrame::Data::OnWindowMenuUpdateUI(){
}

// ------------------
// end AppFrame::Data
// ------------------

//int idProjectPane = XRCID("action_ProjectPane");
//int idPrjSplitter = XRCID("action_PrjSplitter");
//int idPrjResourcesTree = XRCID("action_PrjResourcesTree");

syString g_statustext;

//BEGIN_EVENT_TABLE(AppFrame, wxFrame)
//    EVT_CLOSE(AppFrame::OnClose)
//    EVT_MENU(wxID_ABOUT, AppFrame::OnAbout)
//    EVT_MENU(idMenuSaveFrameLayout, AppFrame::OnSaveFrameLayout)
//    EVT_MENU(idWorkspaceDefault, AppFrame::OnLoadDefaultLayout)
//
//    EVT_TREE_ITEM_MENU(idPrjResourcesTree, AppFrame::OnResourceTreeContextMenu)
//
////  File menu
//    EVT_MENU(idNewProject, AppFrame::OnNewProject)
//    EVT_MENU(idFileOpen, AppFrame::OnFileOpen)
//    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, AppFrame::OnOpenRecentFile)
//    EVT_MENU(idFileClearRecentProjectList, AppFrame::OnClearRecentProjectList)
//    EVT_MENU(idFileClose, AppFrame::OnFileClose)
//    EVT_MENU(idFileSave, AppFrame::OnFileSave)
//    EVT_MENU(idFileSaveAs, AppFrame::OnFileSaveAs)
//    EVT_MENU(idFileSaveCopy, AppFrame::OnFileSaveCopy)
//    EVT_MENU(wxID_EXIT, AppFrame::OnQuit)
//
//    EVT_MENU(idWorkspaceFactoryDefault, AppFrame::OnWorkspaceFactoryDefault)
//
//    EVT_UPDATE_UI(idFileNew, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewSequence, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewBin, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewOfflineFile, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewTitle, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewBarsandTone, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewBlackVideo, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewColorMatte, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idNewUniversalCountingLeader, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileOpen, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileOpenRecentProject, AppFrame::OnRecentFilesMenuUpdateUI)
//    EVT_UPDATE_UI(idFileClose, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileSave, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileSaveAs, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileSaveCopy, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileRevert, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileCapture, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileBatchCapture, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileImport, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileGetProperties, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileGetPropertiesFile, AppFrame::OnFileMenuUpdateUI)
//    EVT_UPDATE_UI(idFileGetPropertiesSelection, AppFrame::OnFileMenuUpdateUI)
//
//    EVT_UPDATE_UI(idFileImportRecent, AppFrame::OnRecentImportsMenuUpdateUI)
//
//    EVT_UPDATE_UI(idEditUndo, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditRedo, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditCut, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditCopy, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditPaste, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditPasteInsert, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditPasteAttributes, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditClear, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditRippleDelete, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditDuplicate, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditSelectAll, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditDeselectAll, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabel, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelBlue, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelCyan, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelGreen, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelViolet, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelPink, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelGray, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelRed, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditLabelOrange, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditOriginal, AppFrame::OnEditMenuUpdateUI)
//    EVT_UPDATE_UI(idEditSequenceMarker, AppFrame::OnEditMenuUpdateUI)
//
//    EVT_UPDATE_UI(idProjectSettings, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectSettingsGeneral, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectSettingsCapture, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectSettingsVideoRendering, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectSettingsDefaultSequence, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectLinkMedia, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectUnlinkMedia, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectAutomateToSequence, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectImportBatchList, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectExportBatchList, AppFrame::OnProjectMenuUpdateUI)
//    EVT_UPDATE_UI(idProjectExportAsAAF, AppFrame::OnProjectMenuUpdateUI)
//
//    EVT_UPDATE_UI(idClipRename, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipCaptureSettings, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipSetCaptureSettings, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipClearCaptureSettings, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipInsert, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipOverlay, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipToggleEnable, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipUnlinkAV, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipLinkAV, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipGroup, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipUngroup, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipVideoOptions, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipVOptFrameHold, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipVOptFieldOptions, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipAudioOptions, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipAOptGain, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipAOptBreakoutToMono, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipOptTreatAsStereo, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipOptRenderAndReplace, AppFrame::OnClipMenuUpdateUI)
//    EVT_UPDATE_UI(idClipSpeedDuration, AppFrame::OnClipMenuUpdateUI)
//
//    EVT_UPDATE_UI(idSequenceRender, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceDeleteRenderFiles, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceRazor, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceRazorSelectedTracks, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceLift, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceExtract, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceApplyVideoTransition, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceApplyAudioTransition, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceZoomIn, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceZoomOut, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceSnap, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceAddTracks, AppFrame::OnSequenceMenuUpdateUI)
//    EVT_UPDATE_UI(idSequenceDelTracks, AppFrame::OnSequenceMenuUpdateUI)
//
//    EVT_UPDATE_UI(idSetClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoNextClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoPrevClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoVideoInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoVideoOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoAudioInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoAudioOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoNumberedClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearCurrentClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearAllClipMarkers, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearInOutClipMarkers, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearNumberedClipMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetInSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetOutSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetInOutAroundSelSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetUnnumberedSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetNextAvailNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idSetOtherNumberedSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoNextSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoPrevSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoInSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoOutSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idGotoNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearCurSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearAllSequenceMarkers, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearInOutSeqMarkers, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearInSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearOutSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idClearNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
//    EVT_UPDATE_UI(idEditSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
//
//    EVT_UPDATE_UI(idWindowWorkspaceMenu, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceEditing, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceEffects, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceAudio, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceColorCorrection, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceDefault, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceFactoryDefault, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceSaveAs, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceDelete, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWorkspaceCustom, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idMenuSaveFrameLayout, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowEffects, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowEffectControls, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowHistory, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowInfo, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowTools, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowAudioMixer, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowMonitor, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowProject, AppFrame::OnWindowMenuUpdateUI)
//    EVT_UPDATE_UI(idWindowTimelinesMenu, AppFrame::OnWindowMenuUpdateUI)
//
////  Project pane events
//
//    EVT_UPDATE_UI(idProjectPane, AppFrame:: OnUpdateProjectPaneUI)
//
//
//END_EVENT_TABLE()

AppFrame::AppFrame(const syString& title) :
QMainWindow(),
m_Data(new Data(this))
{

//    m_mgr = new wxAuiManager;
//    bool result = false;
//    do {
//        if(!CreateMenuBar()) break;
//        if(!CreateDialogs()) break;
//        CreateStatusBar(2);
//
//        m_mgr->SetManagedWindow(this);
//        LoadAndSetFrameSize();
//
//        if(!CreatePanels()) break;
//        CreateDockAreas();
//
//        // Update Status bar
//        SetStatusText(wxbuildinfo(short_f), 1);
//        g_statustext << _("Welcome to ") << APP_SHOWNAME << "! ^_^";
//        UpdateStatustext();
//
//        m_FactoryDefaultLayout = m_mgr->SavePerspective();
//        LoadDefaultLayout(true);
//
//        result = true;
//    }while(false);
//    if(!result) {
//        Destroy();
//    } else {
//        syConnect(this, -1, &AppFrame::OnProjectStatusChanged);
//        ProjectManager::Get()->SetEventHandler(this);
//        ShowLayout(false);
//    }
}

//
//bool AppFrame::CreateDialogs() {
//    bool result = false;
//    do {
//        m_welcomedialog = new WelcomeDialog(this);
//        if(!m_welcomedialog) {
//            LoadFail("welcome_dialog");
//            break;
//        }
//        result = true;
//    } while(false);
//    return result;
//}
//
//bool AppFrame::CreatePanels() {
//    bool result = false;
//    do {
//        m_projectpanel = CreateProjectPane();
//        if(!m_projectpanel) { LoadFail("project_panel"); break; }
//           m_effectspanel = new wxVideoPlaybackPanel(this);
//           m_monitorpanel = new wxVideoPlaybackPanel(this);
//           static_cast<wxVideoPlaybackPanel*>(m_effectspanel)->SetAVPlayer(PlaybackManager::Get()->GetInputMonitor());
//           static_cast<wxVideoPlaybackPanel*>(m_monitorpanel)->SetAVPlayer(PlaybackManager::Get()->GetPreviewMonitor());
//        m_timelinepanel = new wxPanel(this, -1,wxDefaultPosition, wxSize(800,400));
//        result = true;
//    }while(false);
//    return result;
//}
//
//void AppFrame::CreateDockAreas() {
//    m_mgr->SetFlags(m_mgr->GetFlags() | wxAUI_MGR_ALLOW_ACTIVE_PANE);
//     // add the panes to the manager
////   m_mgr->SetDockSizeConstraint(0.3,0.45);
//    m_mgr->AddPane(m_projectpanel, wxAuiPaneInfo().
//                            Name(wxT("Project")).Caption(_w("Project")).
//                              BestSize(wxSize(200, 300)).MaximizeButton().MinimizeButton().PinButton().
//                              Left().Layer(1));
//    m_mgr->AddPane(m_monitorpanel, wxAuiPaneInfo().
//                            Name(wxT("Monitor")).Caption(_w("Monitor / Preview")).
//                              BestSize(wxSize(250, 300)).MaximizeButton().MinimizeButton().PinButton().
//                              Bottom().Layer(1));
//    m_mgr->AddPane(m_effectspanel, wxAuiPaneInfo().
//                            Name(wxT("Effects")).Caption(_w("Effects Monitor")).
//                              BestSize(wxSize(250, 300)).MaximizeButton().MinimizeButton().PinButton().
//                              Bottom().Layer(1));
//    m_mgr->AddPane(m_timelinepanel, wxAuiPaneInfo().Name(wxT("MainPane")).CentrePane().MinSize(wxSize(500,200)).MaximizeButton().Caption(_w("Timeline")).CaptionVisible(true));
//}
//
//bool AppFrame::LoadDefaultLayout(bool firsttime) {
//    bool result = false;
//    syString strlayout = syApp::GetConfig()->Read(CFG_PERSPECTIVE_DEFAULT.c_str(), "");
//    if(!strlayout.empty()) {
//        result = m_mgr->LoadPerspective(strlayout,false);
//    }
//    if(firsttime) {
//        syProjectStatusEvent event;
//        syApp::Get()->PostEvent(static_cast<syEvtHandler*>(this), event);
//    }
//    m_mgr->Update();
//    return result;
//}
//
//void AppFrame::OnLoadDefaultLayout(wxCommandEvent& event) {
//    LoadDefaultLayout();
//}
//
//void AppFrame::OnWorkspaceFactoryDefault(wxCommandEvent& event) {
//    bool result = false;
//    if(!m_FactoryDefaultLayout.IsEmpty()) {
//        result = m_mgr->LoadPerspective(m_FactoryDefaultLayout,false);
//    }
//    m_mgr->Update();
//}

//wxPanel* AppFrame::CreateProjectPane() {
//
//    long tabs_style = wxAUI_NB_DEFAULT_STYLE && ~ (wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_CLOSE_ON_ALL_TABS);
//    wxPanel* panel1;
//    wxAuiNotebook* auinotebook1;
//    wxPanel* resourcespage;
//    wxSplitterWindow* splitter1;
//    wxPanel* dir_panel;
//    wxPanel* files_panel;
//    wxScrolledWindow* scrolledWindow2;
//    wxPanel* effectspage;
//    m_ResourcesTree = NULL;
//
//    panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize(200,800), wxTAB_TRAVERSAL );
//	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );
//
//	auinotebook1 = new wxAuiNotebook( panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, tabs_style );
//	resourcespage = new wxPanel( auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
//	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );
//
//	splitter1 = new wxSplitterWindow( resourcespage, idPrjSplitter, wxDefaultPosition, wxDefaultSize, wxSP_3D );
//	splitter1->SetMinimumPaneSize(10); // No unsplitting!
//	dir_panel = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL );
//
//	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );
//
//	m_ResourcesTree = new wxTreeCtrl( dir_panel, idPrjResourcesTree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
//
//	wxTreeItemId daroot = m_ResourcesTree->AddRoot(_w("Resources"), -1, -1, NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Sequences"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Videos"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Images"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Sound"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Other"),-1,-1,NULL);
//
//	bSizer4->Add( m_ResourcesTree, 1, wxEXPAND, 5 );
//
//	dir_panel->SetSizer( bSizer4 );
//	dir_panel->Layout();
//	bSizer4->Fit( dir_panel );
//	files_panel = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL );
//	wxBoxSizer* bSizer5;
//	bSizer5 = new wxBoxSizer( wxVERTICAL );
//
//	scrolledWindow2 = new wxScrolledWindow( files_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
//	scrolledWindow2->SetScrollRate( 5, 5 );
//	bSizer5->Add( scrolledWindow2, 1, wxEXPAND | wxALL, 5 );
//
//	files_panel->SetSizer( bSizer5 );
//	files_panel->Layout();
//	bSizer5->Fit( files_panel );
//	splitter1->SplitHorizontally( dir_panel, files_panel, 170 );
//	bSizer3->Add( splitter1, 1, wxEXPAND, 5 );
//
//	resourcespage->SetSizer( bSizer3 );
//	resourcespage->Layout();
//	bSizer3->Fit( resourcespage );
//	auinotebook1->AddPage( resourcespage, wxT("Resources"), false, wxNullBitmap );
//	effectspage = new wxPanel( auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
//	auinotebook1->AddPage( effectspage, wxT("Effects"), true, wxNullBitmap );
//    auinotebook1->SetSelection(0); // Go to the first page
//
//	bSizer2->Add( auinotebook1, 1, wxEXPAND | wxALL, 5 );
//
//	panel1->SetSizer( bSizer2 );
//	panel1->Layout();
//	bSizer2->Fit( panel1 );
//	{
//        long curheight = GetRect().GetHeight();
//        long defaultsashpos =  curheight / 2;
//        long sashpos = defaultsashpos;
//        sashpos = syApp::GetConfig()->ReadUint(CFG_DEFAULT_PRJ_SASHPOS, defaultsashpos);
//        if(sashpos==0) sashpos = defaultsashpos;
//        sashpos = std::min(curheight,std::max((long)20,sashpos));
//        splitter1->SetSashPosition(sashpos);
//	}
//
//	return panel1;
//}
//
//long AppFrame::GetProjectPanelSashPos() {
//    wxSplitterWindow* splitter = (wxSplitterWindow*)FindWindow(idPrjSplitter);
//    if(splitter) {
//        return splitter->GetSashPosition();
//    } else {
//        return 0;
//    }
//}


//wxMenu* AppFrame::FindMenu(const wxString name) {
//    wxMenu* result = NULL;
//    wxMenuBar* bar = GetMenuBar();
//    do {
//        if(!bar) break;
//        int myMenu = bar->FindMenu(name);
//        if(myMenu == wxNOT_FOUND) break;
//        result = bar->GetMenu(myMenu);
//    }while(false);
//    return result;
//}
//
//void AppFrame::LoadAndSetFrameSize() {
//    const int minFrameWidth = 980;
//    const int minFrameHight = 680;
//    bool nocfg = false;
//
//    // load stored size or defaults
//    wxRect rect;
//    syString key = CFG_LOCATION;
//    if (syApp::GetConfig()->Exists (key)) {
//        rect.x = syApp::GetConfig()->ReadInt(key + "/xpos", rect.x);
//        rect.y = syApp::GetConfig()->ReadInt(key + "/ypos", rect.y);
//        rect.width = syApp::GetConfig()->ReadInt(key + "/width", rect.width);
//        rect.height = syApp::GetConfig()->ReadInt(key + "/height", rect.height);
//    } else {
//        nocfg = true;
//    }
//
//    // check for reasonable values (within screen)
//    wxSize scr = wxGetDisplaySize();
//    rect.x = wxMin (abs (rect.x), (scr.x - minFrameWidth));
//    rect.y = wxMin (abs (rect.y), (scr.y - minFrameHight));
//    rect.width = wxMax (abs (rect.width), (minFrameWidth));
//    rect.width = wxMin (abs (rect.width), (scr.x - rect.x));
//    rect.height = wxMax (abs (rect.height), (minFrameHight));
//    rect.height = wxMin (abs (rect.height), (scr.y - rect.y));
//    SetSize(rect);
//    if(nocfg) {
//        CenterOnScreen();
//    }
//}

AppFrame::~AppFrame() {
    syApp::Get()->Exit(false);
    delete m_Data;
    m_Data = 0;
}

bool AppFrame::IsClipSelected() {
    #warning TODO: Implement AppFrame::IsClipSelected
    return false;
}
bool AppFrame::IsSelectionMultiple() {
    #warning TODO: Implement AppFrame::IsSelectionMultiple
    return false;
}

bool AppFrame::IsResourceClipSelected() {
    #warning TODO: Implement AppFrame::IsResourceClipSelected
    return false;
}
//
bool AppFrame::IsTimelineActive() {
    #warning TODO: Implement AppFrame::IsTimelineActive
    return false;
}

bool AppFrame::IsResourceWindowActive() {
    #warning TODO: Implement AppFrame::IsResourceWindowActive
    return false;
}
bool AppFrame::IsMonitorActive() {
    #warning TODO: Implement AppFrame::IsMonitorActive
    return false;
}
bool AppFrame::IsRenderMonitorActive() {
    #warning TODO: Implement AppFrame::IsRenderMonitorActive
    return false;
}
bool AppFrame::IsClipMonitorActive() {
    #warning TODO: Implement AppFrame::IsClipMonitorActive
    return false;
}
bool AppFrame::IsEffectsWindowActive() {
    #warning TODO: Implement AppFrame::IsEffectsWindowActive
    return false;
}
bool AppFrame::IsTitleWindowActive() {
    #warning TODO: Implement AppFrame::IsTitleWindowActive
    return false;
}

bool AppFrame::IsClipboardSet() {
    #warning TODO: Implement IsClipboardSet
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

//void AppFrame::OnNewProject(wxCommandEvent& event) {
//    NewProjectDlg* mydialog = new NewProjectDlg(this);
//    if(!mydialog) {
//        return;
//    }
//    mydialog->ShowModal();
//}
//
//void AppFrame::OnFileOpen(wxCommandEvent& event) {
//    if(IsAppShuttingDown())
//        return;
//    syString lastdir = ProjectManager::Get()->GetLastProjectDir();
//    wxFileDialog myDialog(this, _w("Choose a project"), lastdir, _T(""), _T("*.saya"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("opendlg"));
//    int dialogresult = myDialog.ShowModal();
//    if(dialogresult == wxID_OK) {
//        if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
//            syString chosenpath(myDialog.GetPath());
//            bool result = ProjectManager::Get()->LoadProject(chosenpath);
//            if(!result) {
//                syString msg;
//                msg.Printf(_("Error opening file '%s'!"),chosenpath.c_str());
//                wxMessageBox(msg,_w("Error"),wxCANCEL | wxICON_ERROR,this);
//            }
//            DoUpdateAppTitle();
//        }
//    }
//}
//
//void AppFrame::OnOpenRecentFile(wxCommandEvent &event) {
//    if(IsAppShuttingDown())
//        return;
//    if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
//
//        // event.GetId() - wxID_FILE1 would give zero for wxID_FILE1;
//        // but the list is one-based so we add 1.
//        int fileno = (event.GetId() - wxID_FILE1) + 1;
//
//        bool result = ProjectManager::Get()->LoadRecentProject(fileno);
//        if(!result) {
//            syString msg;
//            msg.Printf(_("Error opening file '%s'!"),ProjectManager::Get()->GetRecentFiles()->item(fileno).c_str());
//            wxMessageBox(msg,_w("Error"),wxCANCEL | wxICON_ERROR,this);
//        }
//        DoUpdateAppTitle();
//    }
//}
//
//void AppFrame::OnClearRecentProjectList(wxCommandEvent &event) {
//    if(IsAppShuttingDown())
//        return;
//    ProjectManager::Get()->GetRecentFiles()->clear();
//}
//
//void AppFrame::OnFileClose(wxCommandEvent& event) {
//    if(IsAppShuttingDown())
//        return;
//    ProjectManager::Get()->CloseProject(false);
//    syProjectStatusEvent evt;
//    syApp::Get()->PostEvent(this, evt);
//}
//
//
//void AppFrame::OnClose(wxCloseEvent &event) {
//    bool willveto = false;
//    if(event.CanVeto()) {
//        if(!IsAppShuttingDown()) {
//            if(!ProjectManager::Get()->CloseProject(false))
//                willveto = true;
//        }
//    } else {
//        if(!IsAppShuttingDown()) {
//            ProjectManager::Get()->CloseProject(true);
//        }
//    }
//
//    if(willveto) {
//        event.Veto();
//    } else {
//        Destroy();
//    }
//}
//
//void AppFrame::OnFileSave(wxCommandEvent &event) {
//    if(IsAppShuttingDown())
//        return;
//    ProjectManager::Get()->InteractiveSaveProject();
//}
//
//void AppFrame::OnFileSaveAs(wxCommandEvent &event) {
//    if(IsAppShuttingDown())
//        return;
//    ProjectManager::Get()->InteractiveSaveProjectAs();
//}
//
//void AppFrame::OnFileSaveCopy(wxCommandEvent &event) {
//    if(IsAppShuttingDown())
//        return;
//    ProjectManager::Get()->InteractiveSaveProjectCopy();
//}
//
//
//void AppFrame::OnQuit(wxCommandEvent &event) {
//    Close();
//}
//
//void AppFrame::OnSaveFrameLayout(wxCommandEvent& event) {
//    SaveDefaultLayout(true);
//}
//
void AppFrame::UpdateStatustext() {
    QStatusBar* sb = statusBar();
    if(sb) {
        sb->showMessage(g_statustext);
    }
}
//
//void AppFrame::SaveDefaultLayout(bool showmsg) {
//    if(m_layouthidden) {
//        return;
//    }
//    wxRect rect = GetRect();
//    syString key = CFG_LOCATION;
//    syApp::GetConfig()->WriteInt(key + "/xpos", rect.x);
//    syApp::GetConfig()->WriteInt(key + "/ypos", rect.y);
//    syApp::GetConfig()->WriteInt(key + "/width", rect.width);
//    syApp::GetConfig()->WriteInt(key + "/height", rect.height);
//
//    syString strlayout(m_mgr->SavePerspective());
//    syApp::GetConfig()->Write(CFG_PERSPECTIVE_DEFAULT, strlayout);
//    syApp::GetConfig()->WriteInt(CFG_DEFAULT_PRJ_SASHPOS,GetProjectPanelSashPos());
//
//    if(showmsg) {
//        wxMessageBox (_w("Current Layout has been saved."),
//                      _w("Save Layout"), wxOK);
//    }
//}
//
//
//void AppFrame::OnFileMenuUpdateUI(wxUpdateUIEvent& event) {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    VidProject* prj = pmgr->GetProject();
//
//    bool hasproject = (prj);
//    bool isModified = hasproject && prj->IsModified();
//    bool isNew = hasproject && prj->IsNew();
//
//    wxMenuBar* mbar = GetMenuBar();
//    mbar->Enable(idNewSequence, hasproject);
//    mbar->Enable(idNewBin, hasproject);
//    mbar->Enable(idNewOfflineFile, hasproject);
//    mbar->Enable(idNewTitle, hasproject);
//    mbar->Enable(idNewBarsandTone, hasproject);
//    mbar->Enable(idNewBlackVideo, hasproject);
//    mbar->Enable(idNewColorMatte, hasproject);
//    mbar->Enable(idNewUniversalCountingLeader, hasproject);
//    mbar->Enable(idFileClose,hasproject);
//    mbar->Enable(idFileSave,hasproject && (isModified || isNew));
//    mbar->Enable(idFileSaveAs,hasproject);
//    mbar->Enable(idFileSaveCopy,hasproject);
//    mbar->Enable(idFileRevert,hasproject && isModified && !isNew);
//    mbar->Enable(idFileInterpretFootage,hasproject && (IsClipSelected() || IsResourceClipSelected()));
//    mbar->Enable(idFileTimecode,hasproject && IsClipSelected());
//    mbar->Enable(idFileImport,hasproject);
//    mbar->Enable(idFileExport,hasproject);
//    mbar->Enable(idFileGetPropertiesSelection, hasproject && (IsClipSelected() || IsResourceClipSelected()));
//    mbar->Enable(idFileCapture, hasproject);
//    mbar->Enable(idFileBatchCapture, hasproject);
//    mbar->Enable(idFileGetProperties, hasproject);
//    mbar->Enable(idFileGetPropertiesFile, hasproject);
//    mbar->Enable(idFileGetPropertiesSelection, hasproject);
//}
//
//void AppFrame::OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event) {
//    // Update the Recent Projects list
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    if(pmgr->GetRecentFiles()->UpdateCounter(m_recentfilesmodcounter)) {
//        wxMenuItem* myItem = GetMenuBar()-> FindItem(idFileOpenRecentProject);
//        if(myItem) {
//            wxMenu* mySubMenu = myItem->GetSubMenu();
//
//            if(mySubMenu) { // Clear all items in the submenu
//                while(mySubMenu->GetMenuItemCount()) {
//                    wxMenuItem* subitem = mySubMenu->FindItemByPosition(0);
//                    if(!subitem)
//                        break;
//                    mySubMenu->Delete(subitem);
//                }
//            } else { // Add a new submenu
//                mySubMenu = new wxMenu();
//                myItem->SetSubMenu(mySubMenu);
//            }
//            size_t i = 0;
//            mySubMenu->Append(idFileClearRecentProjectList,_T("&Clear"),_w("Clears Recent Projects List"));
//            mySubMenu->AppendSeparator();
//            for(i = 1; i <= pmgr->GetRecentFiles()->size(); ++i) {
//                syString tmptext;
//                tmptext.Printf("&%d %s",i,pmgr->GetRecentFiles()->item(i).c_str());
//                mySubMenu->Append(wxID_FILE1 + i - 1,tmptext,wxEmptyString);
//            }
//            myItem->Enable(pmgr->GetRecentFiles()->size() > 0);
//        }
//    }
//}
//
//void AppFrame::OnRecentImportsMenuUpdateUI(wxUpdateUIEvent& event) {
//    // Update the Recent Imported files list
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    if(pmgr->GetRecentImports()->UpdateCounter(m_recentimportsmodcounter)) {
//        wxMenuItem* myItem = GetMenuBar()-> FindItem(idFileImportRecent);
//        if(myItem) {
//            wxMenu* mySubMenu = myItem->GetSubMenu();
//
//            if(mySubMenu) { // Clear all items in the submenu
//                while(mySubMenu->GetMenuItemCount()) {
//                    wxMenuItem* subitem = mySubMenu->FindItemByPosition(0);
//                    if(!subitem)
//                        break;
//                    mySubMenu->Delete(subitem);
//                }
//            } else { // Add a new submenu
//                mySubMenu = new wxMenu();
//                myItem->SetSubMenu(mySubMenu);
//            }
//            size_t i = 0;
//            mySubMenu->Append(idFileClearRecentImportList,_T("&Clear"),_w("Clears Recent Imported Files List"));
//            mySubMenu->AppendSeparator();
//            for(i = 1; i <= pmgr->GetRecentImports()->size(); ++i) {
//                syString tmptext;
//                tmptext.Printf("&%d %s",i, pmgr->GetRecentImports()->item(i).c_str());
//                mySubMenu->Append(wxID_IMPORT1 + i -1,tmptext,wxEmptyString);
//            }
//        }
//    }
//    wxMenuBar* mbar = GetMenuBar();
//    mbar->Enable(idFileImportRecent,pmgr->GetRecentImports()->size() > 0 && (pmgr->HasProject()));
//}
//
//void AppFrame::OnEditMenuUpdateUI(wxUpdateUIEvent& event) {
//
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    VidProject* prj = pmgr->GetProject();
//
//    bool hasproject = (prj);
//    bool canUndo = CanUndo();
//    bool canRedo = CanRedo();
//    bool canCutOrCopy = hasproject && (IsClipSelected() || IsResourceClipSelected());
//    bool canPaste = hasproject && (IsClipboardSet() && IsTimelineActive());
//
//    wxMenuBar* mbar = GetMenuBar();
//    mbar->Enable(idEditUndo,canUndo);
//    mbar->Enable(idEditRedo,canRedo);
//    mbar->Enable(idEditClearUndoHistory,canUndo || canRedo);
//    mbar->Enable(idEditCut, canCutOrCopy);
//    mbar->Enable(idEditCopy, canCutOrCopy);
//    mbar->Enable(idEditFind, hasproject);
//    mbar->Enable(idEditPaste, canPaste);
//    mbar->Enable(idEditPasteInsert, canPaste);
//    mbar->Enable(idEditPasteAttributes, canPaste);
//    mbar->Enable(idEditClear, hasproject && IsClipSelected());
//    mbar->Enable(idEditRippleDelete, hasproject && IsClipSelected());
//    mbar->Enable(idEditDuplicate, hasproject && IsClipSelected());
//    mbar->Enable(idEditSelectAll, hasproject);
//    mbar->Enable(idEditDeselectAll, hasproject);
//    mbar->Enable(idEditLabel, hasproject && IsClipSelected());
//    mbar->Enable(idEditOriginal, hasproject && IsClipSelected() && !IsSelectionMultiple());
////    idEditLabel
////    idEditLabelBlue
////    idEditLabelCyan
////    idEditLabelGreen
////    idEditLabelViolet
////    idEditLabelPink
////    idEditLabelGray
////    idEditLabelRed
////    idEditLabelOrange
////
//}
//
//void AppFrame::OnProjectMenuUpdateUI(wxUpdateUIEvent& event) {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    bool hasproject = pmgr->HasProject();
//
//    wxMenuBar* mbar = GetMenuBar();
//
//    mbar->Enable(idProjectSettings,hasproject);
//    mbar->Enable(idProjectSettingsGeneral,hasproject);
//    mbar->Enable(idProjectSettingsCapture,hasproject);
//    mbar->Enable(idProjectSettingsVideoRendering,hasproject);
//    mbar->Enable(idProjectSettingsDefaultSequence,hasproject);
//    mbar->Enable(idProjectLinkMedia,hasproject);
//    mbar->Enable(idProjectUnlinkMedia,hasproject);
//    mbar->Enable(idProjectAutomateToSequence,hasproject);
//    mbar->Enable(idProjectImportBatchList,hasproject);
//    mbar->Enable(idProjectExportBatchList,hasproject);
//    mbar->Enable(idProjectExportAsAAF,hasproject);
//}
//
//void AppFrame::OnClipMenuUpdateUI(wxUpdateUIEvent& event) {
//
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    bool hasproject = pmgr->HasProject();
//
//    wxMenuBar* mbar = GetMenuBar();
//
//    mbar->Enable(idClipRename, hasproject);
//    mbar->Enable(idClipCaptureSettings, hasproject);
//    mbar->Enable(idClipSetCaptureSettings, hasproject);
//    mbar->Enable(idClipClearCaptureSettings, hasproject);
//    mbar->Enable(idClipInsert, hasproject);
//    mbar->Enable(idClipOverlay, hasproject);
//    mbar->Enable(idClipToggleEnable, hasproject);
//    mbar->Enable(idClipUnlinkAV, hasproject);
//    mbar->Enable(idClipLinkAV, hasproject);
//    mbar->Enable(idClipGroup, hasproject);
//    mbar->Enable(idClipUngroup, hasproject);
//    mbar->Enable(idClipVideoOptions, hasproject);
//    mbar->Enable(idClipVOptFrameHold, hasproject);
//    mbar->Enable(idClipVOptFieldOptions, hasproject);
//    mbar->Enable(idClipAudioOptions, hasproject);
//    mbar->Enable(idClipAOptGain, hasproject);
//    mbar->Enable(idClipAOptBreakoutToMono, hasproject);
//    mbar->Enable(idClipOptTreatAsStereo, hasproject);
//    mbar->Enable(idClipOptRenderAndReplace, hasproject);
//    mbar->Enable(idClipSpeedDuration, hasproject);
//}
//
//void AppFrame::OnSequenceMenuUpdateUI(wxUpdateUIEvent& event) {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    bool hasproject = pmgr->HasProject();
//
//    wxMenuBar* mbar = GetMenuBar();
//
//    mbar->Enable(idSequenceRender, hasproject);
//    mbar->Enable(idSequenceDeleteRenderFiles, hasproject);
//    mbar->Enable(idSequenceRazor, hasproject);
//    mbar->Enable(idSequenceRazorSelectedTracks, hasproject);
//    mbar->Enable(idSequenceLift, hasproject);
//    mbar->Enable(idSequenceExtract, hasproject);
//    mbar->Enable(idSequenceApplyVideoTransition, hasproject);
//    mbar->Enable(idSequenceApplyAudioTransition, hasproject);
//    mbar->Enable(idSequenceZoomIn, hasproject);
//    mbar->Enable(idSequenceZoomOut, hasproject);
//    mbar->Enable(idSequenceSnap, hasproject);
//    mbar->Enable(idSequenceAddTracks, hasproject);
//    mbar->Enable(idSequenceDelTracks, hasproject);
//
//}
//
//void AppFrame::OnMarkerMenuUpdateUI(wxUpdateUIEvent& event) {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    bool hasproject = pmgr->HasProject();
//
//    wxMenuBar* mbar = GetMenuBar();
//
//    mbar->Enable(idSetClipMarkerMenu, hasproject);
//    mbar->Enable(idGotoClipMarkerMenu, hasproject);
//    mbar->Enable(idGotoNextClipMarker, hasproject);
//    mbar->Enable(idGotoPrevClipMarker, hasproject);
//    mbar->Enable(idGotoInClipMarker, hasproject);
//    mbar->Enable(idGotoOutClipMarker, hasproject);
//    mbar->Enable(idGotoVideoInClipMarker, hasproject);
//    mbar->Enable(idGotoVideoOutClipMarker, hasproject);
//    mbar->Enable(idGotoAudioInClipMarker, hasproject);
//    mbar->Enable(idGotoAudioOutClipMarker, hasproject);
//    mbar->Enable(idGotoNumberedClipMarker, hasproject);
//    mbar->Enable(idClearClipMarkerMenu, hasproject);
//    mbar->Enable(idClearCurrentClipMarker, hasproject);
//    mbar->Enable(idClearAllClipMarkers, hasproject);
//    mbar->Enable(idClearInOutClipMarkers, hasproject);
//    mbar->Enable(idClearInClipMarker, hasproject);
//    mbar->Enable(idClearOutClipMarker, hasproject);
//    mbar->Enable(idClearNumberedClipMarker, hasproject);
//    mbar->Enable(idSetSequenceMarkerMenu, hasproject);
//    mbar->Enable(idSetInSequenceMarker, hasproject);
//    mbar->Enable(idSetOutSequenceMarker, hasproject);
//    mbar->Enable(idSetInOutAroundSelSeqMarker, hasproject);
//    mbar->Enable(idSetUnnumberedSequenceMarker, hasproject);
//    mbar->Enable(idSetNextAvailNumberedSeqMarker, hasproject);
//    mbar->Enable(idSetOtherNumberedSequenceMarker, hasproject);
//    mbar->Enable(idGotoSequenceMarkerMenu, hasproject);
//    mbar->Enable(idGotoNextSequenceMarker, hasproject);
//    mbar->Enable(idGotoPrevSequenceMarker, hasproject);
//    mbar->Enable(idGotoInSequenceMarker, hasproject);
//    mbar->Enable(idGotoOutSequenceMarker, hasproject);
//    mbar->Enable(idGotoNumberedSeqMarker, hasproject);
//    mbar->Enable(idClearSequenceMarkerMenu, hasproject);
//    mbar->Enable(idClearCurSequenceMarker, hasproject);
//    mbar->Enable(idClearAllSequenceMarkers, hasproject);
//    mbar->Enable(idClearInOutSeqMarkers, hasproject);
//    mbar->Enable(idClearInSeqMarker, hasproject);
//    mbar->Enable(idClearOutSeqMarker, hasproject);
//    mbar->Enable(idClearNumberedSeqMarker, hasproject);
//    mbar->Enable(idEditSequenceMarker, hasproject);
//}
//
//void AppFrame::OnWindowMenuUpdateUI(wxUpdateUIEvent& event) {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr)
//        return;
//    bool hasproject = pmgr->HasProject();
//
//    wxMenuBar* mbar = GetMenuBar();
//
//    mbar->Enable(idWindowWorkspaceMenu, hasproject);
//    mbar->Enable(idWorkspaceEditing, hasproject);
//    mbar->Enable(idWorkspaceEffects, hasproject);
//    mbar->Enable(idWorkspaceAudio, hasproject);
//    mbar->Enable(idWorkspaceColorCorrection, hasproject);
//    mbar->Enable(idWorkspaceDefault, hasproject);
//    mbar->Enable(idWorkspaceSaveAs, hasproject);
//    mbar->Enable(idWorkspaceDelete, hasproject);
//    mbar->Enable(idWorkspaceCustom, hasproject);
//    mbar->Enable(idMenuSaveFrameLayout, hasproject);
//    mbar->Enable(idWindowEffects, hasproject);
//    mbar->Enable(idWindowEffectControls, hasproject);
//    mbar->Enable(idWindowHistory, hasproject);
//    mbar->Enable(idWindowInfo, hasproject);
//    mbar->Enable(idWindowTools, hasproject);
//    mbar->Enable(idWindowAudioMixer, hasproject);
//    mbar->Enable(idWindowMonitor, hasproject);
//    mbar->Enable(idWindowProject, hasproject);
//    mbar->Enable(idWindowTimelinesMenu, hasproject);
//
//}
//
//void AppFrame::OnProjectStatusChanged(syProjectStatusEvent& event) {
//    if(IsAppShuttingDown())
//        return;
//    if(m_hadproject != ProjectManager::Get()->HasProject() || !m_panes_status_checked) {
//        // Closed or opened a project
//        if(!m_panes_status_checked) m_panes_status_checked = true;
//        ShowLayout(ProjectManager::Get()->HasProject());
//        m_hadproject = ProjectManager::Get()->HasProject();
//    }
//    DoUpdateAppTitle();
//}
//
//void AppFrame::ShowLayout(bool show) {
//    if(IsAppShuttingDown())
//        return;
//    if(show) {
//        if(m_layouthidden) {
//            m_mgr->LoadPerspective(m_CurrentPerspective,true);
//            m_layouthidden = false;
//        }
//    } else {
//        if(!m_layouthidden) {
//            // Hide All panes
//            m_CurrentPerspective = m_mgr->SavePerspective();
//            wxAuiPaneInfoArray& myarr = m_mgr->GetAllPanes();
//            size_t i;
//            for(i = 0;i < myarr.size(); i++) {
//                myarr[i].Hide();
//            }
//            m_mgr->Update();
//            m_layouthidden = true;
//        }
//    }
//    if(!show) {
////        m_mgr->Update();
//        ShowWelcomeDialog();
//    } else {
//        if(m_welcomedialog) {
//            m_welcomedialog->Hide();
//        }
//        Show();
//        m_mgr->Update();
//    }
//}
//


//
//void AppFrame::DoUpdateAppTitle() {
//    if(IsAppShuttingDown())
//        return;
//    syString title;
//    syString modified_str;
//    VidProject* prj = ProjectManager::Get()->GetProject();
//
//    if(prj) {
//        if(prj->IsModified()) {
//            modified_str = "* ";
//        }
//        title << modified_str << prj->GetTitle();
//
//        syString filename(prj->GetFilename(),true);
//        if(!filename.empty()) {
//            title << " [" << ioCommon::GetFilename(filename) << "]";
//        } else {
//            title << " [" << _("untitled") << "]";
//        }
//        title << " - ";
//        title << APP_SHOWNAME;
//    } else {
//        title << APP_SHOWOFFNAME;
//    }
//    SetTitle(title);
//}
//
//void AppFrame::OnAbout(wxCommandEvent &event) {
//    syString msg;
//    msg << syApp::Get()->GetApplicationShowOffName() << "\n" << " by " << syApp::Get()->GetApplicationVendor() << "\n";
//    msg << qbuildinfo(long_f);
//    syMessageBox(msg, _("Welcome to..."));
//}
//
//void AppFrame::OnResourceTreeContextMenu(wxTreeEvent& event) {
//
//    wxMenu *menu = wxXmlResource::Get()->LoadMenu(_T("resources_tree_menu"));
//    std::auto_ptr<wxMenu> tmpptr(menu);
//    if(menu) {
//        PopupMenu(menu);
//    }
//}
//
//void AppFrame::OnUpdateProjectPaneUI(wxUpdateUIEvent& event) {
//    if(IsAppShuttingDown())
//        return;
//    bool enablePane = ProjectManager::Get()->HasProject();
//    wxWindow* thepane = FindWindow(idProjectPane);
//    if(thepane) {
//        thepane->Enable(enablePane);
//    }
//}

#include "main.moc.h"
