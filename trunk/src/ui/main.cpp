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
#include <qdesktopwidget.h>
#include <QCloseEvent>

#include "ui/main.ui.h"

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
    // To test with an empty QWidget, comment the above lines and uncomment the following line.
    // QMainWindow* frame = new QMainWindow;
    syApp::Get()->SetTopWindow(frame);

    // Uncomment the following line if you want to show the main frame anyway (for debugging purposes).
    // frame->show();
    return frame;
}

// ------------------------------
// begin constants and event id's
// ------------------------------

const syString CFG_LAYOUTS = "Layouts";
const syString CFG_LAYOUT_DEFAULT = CFG_LAYOUTS + "/Default";
const syString CFG_LOCATION = CFG_LAYOUT_DEFAULT + "/Location";
const syString CFG_PERSPECTIVE_DEFAULT = CFG_LAYOUT_DEFAULT + "/Layout";
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
unsigned int idQuit = syActionEvent::RegisterId("action_Quit", "OnQuit()");

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
        void CreateConnections(QWidget* parentwidget);
        bool CreatePanels();
        void ShowLayout(bool show);

        /** Updates the Layout (wxAUI replacement) */
        void UpdateLayout();

        /** Saves the current layout into a string. */
        const syString SaveLayout();

        /** Loads the current layout from a string. */
        bool LoadLayout(const syString& layoutdata, bool update = true);

        void CenterOnScreen();

        void ShowWelcomeDialog();
        void ShowMainWindow();

        void OpenRecentFile(unsigned int fileno);

        void OnProjectStatusChanged(syProjectStatusEvent& event);

    public:
        bool m_hadproject;
        bool m_panes_status_checked;
        bool m_layouthidden;
        syString m_CurrentLayout;
        syString m_FactoryDefaultLayout;
        unsigned int m_recentfilesmodcounter;
        unsigned int m_recentimportsmodcounter;
        Ui::MainWindow* m_Ui;

        void RegisterAction(unsigned int id, QAction* action);
        void UnregisterAction(unsigned int id);
        void UnregisterAllActions();
        void EnableAction(unsigned int id, bool enable = true);
        void DisableAction(unsigned int id);
        QAction* GetAction(unsigned int id);
        void OnActionEvent(syActionEvent& event);

    private:
        typedef std::map<unsigned int, QAction*> ActionsMap;
        ActionsMap m_ActionsMap;
        syMutex m_ActionsMutex;

    public slots:

//        void OnResourceTreeContextMenu(wxTreeEvent& event);

        void OnFileOpen();
        void OnFileClose();

        void OnClearRecentProjectList();
        void OnClearRecentImportList();
        void OnOpenRecentFile1() { OpenRecentFile(0); }
        void OnOpenRecentFile2() { OpenRecentFile(1); }
        void OnOpenRecentFile3() { OpenRecentFile(2); }
        void OnOpenRecentFile4() { OpenRecentFile(3); }
        void OnOpenRecentFile5() { OpenRecentFile(4); }
        void OnOpenRecentFile6() { OpenRecentFile(5); }
        void OnOpenRecentFile7() { OpenRecentFile(6); }
        void OnOpenRecentFile8() { OpenRecentFile(7); }
        void OnOpenRecentFile9() { OpenRecentFile(8); }
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

        void LoadAndSetFrameSize();
        void SaveDefaultLayout(bool showmsg);
        bool LoadDefaultLayout(bool firsttime = false);
//
        void DoUpdateAppTitle();
//
//        void OnUpdateProjectPaneUI();

};

AppFrame::Data::Data(AppFrame* parent) :
m_Parent(parent),
m_WelcomeDialog(0),
m_hadproject(false),
m_panes_status_checked(false),
m_layouthidden(false),
m_CurrentLayout(""),
m_FactoryDefaultLayout(""),
m_recentfilesmodcounter(0),
m_recentimportsmodcounter(0),
m_Ui(new Ui::MainWindow)
{
    UnregisterAllActions();
    m_Ui->setupUi(m_Parent);
    CreateConnections(m_Ui->menubar);
}

AppFrame::Data::~Data() {

    UnregisterAllActions();
    delete m_WelcomeDialog;
    delete m_Ui;

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
    if(m_WelcomeDialog) {
        m_WelcomeDialog->activateWindow();
        m_WelcomeDialog->raise();
        m_WelcomeDialog->show();
    }
}

void AppFrame::Data::ShowMainWindow() {
    if(m_WelcomeDialog) m_WelcomeDialog->hide();
    m_Parent->activateWindow();
    m_Parent->raise();
    m_Parent->show();
}

void AppFrame::Data::CreateConnections(QWidget* parentwidget) {
    QList<QAction*> all_actions = parentwidget->actions();
    QList<QAction*>::iterator it = all_actions.begin();
    for(;it != all_actions.end();++it) {
        QAction* theaction = *it;
        if(!theaction) continue;

        QMenu* themenu = theaction->menu();
        if(themenu) {
            CreateConnections(themenu);
        } else {
            syString thename(theaction->objectName());
            unsigned int id = syActionEvent::GetRegisteredId(thename.c_str());
            if(!id) continue;
            m_ActionsMap[id] = theaction;
            syString theslot(syActionEvent::GetUserStringFromId(id));
            if(!theslot.empty()) {
                theslot = syString(SLOT()) + theslot;
                theaction->connect(static_cast<QObject*>(theaction), SIGNAL(triggered()), dynamic_cast<QObject*>(this), theslot.c_str(), Qt::QueuedConnection);
            }
        }
    }
    // Now we'll Register OnActionEvent as an event handler function.
    m_Parent->m_Delegate = this;
    syConnect(this, -1, &AppFrame::Data::OnActionEvent);
    syConnect(this, -1, &AppFrame::Data::OnProjectStatusChanged);
}

bool AppFrame::Data::LoadDefaultLayout(bool firsttime) {
    bool result = false;
    syString strlayout = syApp::GetConfig()->Read(CFG_PERSPECTIVE_DEFAULT.c_str(), "");
    if(!strlayout.empty()) {
        result = LoadLayout(strlayout,false);
    }
    if(firsttime) {
        syProjectStatusEvent event;
        syApp::Get()->PostEvent(static_cast<syEvtHandler*>(this), event);
    }
    UpdateLayout();
    return result;
}

void AppFrame::Data::CenterOnScreen() {
    QRect rect = QApplication::desktop()->availableGeometry(m_Parent);
    m_Parent->move(rect.center() - m_Parent->rect().center());
}

void AppFrame::Data::OnProjectStatusChanged(syProjectStatusEvent& event) {
    if(IsAppShuttingDown())
        return;
    if(m_hadproject != ProjectManager::Get()->HasProject() || !m_panes_status_checked) {
        // Closed or opened a project
        if(!m_panes_status_checked) m_panes_status_checked = true;
        ShowLayout(ProjectManager::Get()->HasProject());
        m_hadproject = ProjectManager::Get()->HasProject();
    }
    DoUpdateAppTitle();
}

// -----------------
// begin event slots
// -----------------

void AppFrame::Data::OnFileOpen(){
    if(IsAppShuttingDown())
        return;
    syString lastdir = ProjectManager::Get()->GetLastProjectDir();

    syFileDialogResult tmpresult = syFileSelector(
        _("Choose a project"),lastdir,"","saya","*.saya", syFD_OPEN | syFD_FILE_MUST_EXIST);

    if(tmpresult.GetOKResult()) {
        if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
            syString chosenpath(tmpresult[0]);
            bool result = ProjectManager::Get()->LoadProject(chosenpath);
            if(!result) {
                syString msg;
                msg.Printf(_("Error opening file '%s'!"),chosenpath.c_str());
                syMessageBox(msg,_("Error"),syCANCEL | syICON_ERROR,m_Parent);
            }
            ShowMainWindow();
            DoUpdateAppTitle();
        }
    }
}

void AppFrame::Data::OnClearRecentProjectList() {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->GetRecentFiles()->clear();
}

void AppFrame::Data::OnFileClose() {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->CloseProject(false);
    syProjectStatusEvent evt;
    syApp::Get()->PostEvent(m_Parent, evt);
}

void AppFrame::Data::OnFileSave() {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProject();
}

void AppFrame::Data::OnFileSaveAs() {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProjectAs();
}

void AppFrame::Data::OnFileSaveCopy() {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->InteractiveSaveProjectCopy();
}

void AppFrame::Data::OnClearRecentImportList(){
}

void AppFrame::Data::OpenRecentFile(unsigned int fileno) {
    if(IsAppShuttingDown())
        return;
    if(fileno < 0) fileno = 0;
    if(fileno > 8) fileno = 8;
    ++fileno; // The list is one-based so we add 1.

    if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.

        bool result = ProjectManager::Get()->LoadRecentProject(fileno);
        if(!result) {
            syString msg;
            msg.Printf(_("Error opening file '%s'!"),ProjectManager::Get()->GetRecentFiles()->item(fileno).c_str());
            syMessageBox(msg,_("Error"),syCANCEL | syICON_ERROR,this);
        }
        ShowMainWindow();
        DoUpdateAppTitle();
    }

}

void AppFrame::Data::OnNewProject(){
    syMessageBox("OnNewProject() called.");
//    NewProjectDlg* mydialog = new NewProjectDlg(this);
//    if(!mydialog) {
//        return;
//    }
//    mydialog->ShowModal();
}

void AppFrame::Data::OnFileRevert(){
}

void AppFrame::Data::OnFileCapture(){
}

void AppFrame::Data::OnFileBatchCapture(){
}

void AppFrame::Data::OnQuit(){
    m_Parent->close();
}

void AppFrame::Data::OnSaveFrameLayout(){
    SaveDefaultLayout(true);
}

void AppFrame::Data::OnLoadDefaultLayout() {
    LoadDefaultLayout();
}

void AppFrame::Data::OnWorkspaceFactoryDefault(){

    bool result = false;
    if(!m_FactoryDefaultLayout.empty()) {
        result = LoadLayout(m_FactoryDefaultLayout,false);
    }
    UpdateLayout();
}

void AppFrame::Data::OnFileMenuUpdateUI() {
//    ProjectManager* pmgr = ProjectManager::Get();
//    if(!pmgr) return;
//
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
}

void AppFrame::Data::OnRecentFilesMenuUpdateUI() {
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
}

void AppFrame::Data::OnRecentImportsMenuUpdateUI() {
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
}

void AppFrame::Data::OnEditMenuUpdateUI() {
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
}

void AppFrame::Data::OnProjectMenuUpdateUI() {
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
}

void AppFrame::Data::OnClipMenuUpdateUI() {
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
}

void AppFrame::Data::OnSequenceMenuUpdateUI() {
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
}


void AppFrame::Data::OnMarkerMenuUpdateUI() {
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
}

void AppFrame::Data::OnWindowMenuUpdateUI() {
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
}

void AppFrame::Data::OnAbout() {
    syString msg;
    msg << syApp::Get()->GetApplicationShowOffName() << "\n" << " by " << syApp::Get()->GetApplicationVendor() << "\n";
    msg << qbuildinfo(long_f);
    syMessageBox(msg, _("Welcome to..."));
}

// ---------------
// end event slots
// ---------------

void AppFrame::Data::UpdateLayout() {
}

const syString AppFrame::Data::SaveLayout() {
    return "";
}

bool AppFrame::Data::LoadLayout(const syString& layoutdata, bool update) {
    bool result = false;
    if(result && update) {
        UpdateLayout();
    }
    return result;
}


// ------------------
// end AppFrame::Data
// ------------------

//int idProjectPane = XRCID("widget_ProjectPane");
//int idPrjSplitter = XRCID("widget_PrjSplitter");
//int idPrjResourcesTree = XRCID("widget_PrjResourcesTree");

syString g_statustext;

//BEGIN_EVENT_TABLE(AppFrame, wxFrame)
//    EVT_CLOSE(AppFrame::OnClose)
//
//    EVT_TREE_ITEM_MENU(idPrjResourcesTree, AppFrame::OnResourceTreeContextMenu)
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
    bool result = false;
    do {
        if(!m_Data->CreateDialogs()) break;

        m_Data->LoadAndSetFrameSize();

        if(!m_Data->CreatePanels()) break;
        m_Data->CreateDockAreas();

        // Update Status bar
        g_statustext = "";
        g_statustext << _("Welcome to ") << APP_SHOWNAME << "! ^_^";
        UpdateStatustext();

        m_Data->m_FactoryDefaultLayout = m_Data->SaveLayout();
        m_Data->LoadDefaultLayout(true);

        result = true;
    }while(false);
    if(!result) {
        deleteLater();
    } else {
         // NOTE: Until the conversion is finished, if the main window isn't shown, comment the following lines
         ProjectManager::Get()->SetEventHandler(this);
         m_Data->ShowLayout(false);
    }
}

//
bool AppFrame::Data::CreateDialogs() {
    bool result = false;
    do {
        m_WelcomeDialog = new WelcomeDialog(m_Parent);
        if(!m_WelcomeDialog) {
            LoadFail("welcome_dialog");
            break;
        }
        result = true;
    } while(false);
    return result;
}

bool AppFrame::Data::CreatePanels() {
    bool result = false;
    do {
// These lines are remnants of the wxWidgets code. Kept for reference until we finish the conversion.

//        m_projectpanel = CreateProjectPane();
//        if(!m_projectpanel) { LoadFail("project_panel"); break; }
//           m_effectspanel = new wxVideoPlaybackPanel(this);
//           m_monitorpanel = new wxVideoPlaybackPanel(this);
//           static_cast<wxVideoPlaybackPanel*>(m_effectspanel)->SetAVPlayer(PlaybackManager::Get()->GetInputMonitor());
//           static_cast<wxVideoPlaybackPanel*>(m_monitorpanel)->SetAVPlayer(PlaybackManager::Get()->GetPreviewMonitor());
//        m_timelinepanel = new wxPanel(this, -1,wxDefaultPosition, wxSize(800,400));
        result = true;
    }while(false);
    return result;
}

void AppFrame::Data::CreateDockAreas() {
// These statements are remnants of the wxAUI dock manager.
// We're keeping them for reference.

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
}

QWidget* AppFrame::Data::CreateProjectPane() {

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
    return 0;
}

void AppFrame::Data::LoadAndSetFrameSize() {
    const int minFrameWidth = 980;
    const int minFrameHeight = 680;
    bool nocfg = false;

    // load stored size or defaults
    syString key = CFG_LOCATION;
    int xx = 0, yy = 0, ww = 0, hh = 0;


    if (syApp::GetConfig()->Exists (key)) {
        xx = syApp::GetConfig()->ReadInt(key + "/xpos", 0);
        if(xx < 0) xx = 0;
        yy = syApp::GetConfig()->ReadInt(key + "/ypos", 0);
        if(yy < 0) yy = 0;
        ww = syApp::GetConfig()->ReadInt(key + "/width", 0);
        if(ww < 0) ww = 0;
        hh = syApp::GetConfig()->ReadInt(key + "/height", 0);
        if(hh < 0) hh = 0;
    } else {
        nocfg = true;
    }

    // check for reasonable values (within screen)
    QRect rect = QApplication::desktop()->availableGeometry(m_Parent);
    int scr_w = rect.width();
    int scr_h = rect.height();
    int xxmax = scr_w - minFrameWidth;
    int yymax = scr_h - minFrameHeight;

    if(xx > xxmax) xx = xxmax;
    if(yy > yymax) yy = yymax;

    int wwmax = scr_w - xx;
    int hhmax = scr_h - yy;

    if(ww < minFrameWidth) ww = minFrameWidth;
    if(ww > wwmax) ww = wwmax;
    if(hh < minFrameHeight) hh = minFrameHeight;
    if(hh > hhmax) hh = hhmax;

    m_Parent->setGeometry(xx, yy, ww, hh);
    if(nocfg) {
        CenterOnScreen();
    }
}

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

void AppFrame::UpdateStatustext() {
    QStatusBar* sb = statusBar();
    if(sb) {
        sb->showMessage(g_statustext);
    }
}

void AppFrame::Data::SaveDefaultLayout(bool showmsg) {
    if(m_layouthidden) {
        return;
    }
    QRect rect = m_Parent->geometry();
    syString key = CFG_LOCATION;
    syApp::GetConfig()->WriteInt(key + "/xpos", rect.x());
    syApp::GetConfig()->WriteInt(key + "/ypos", rect.y());
    syApp::GetConfig()->WriteInt(key + "/width", rect.width());
    syApp::GetConfig()->WriteInt(key + "/height", rect.height());

    syString strlayout(SaveLayout());
    syApp::GetConfig()->Write(CFG_PERSPECTIVE_DEFAULT, strlayout);

    if(showmsg) {
        syMessageBox(_("Current Layout has been saved."),
                      _("Save Layout"), syOK);
    }
}

void AppFrame::Data::ShowLayout(bool show) {
    if(IsAppShuttingDown())
        return;
//    show = true; // TODO: Remove this line after the WelcomeDialog can be shown safely.
    if(show) {
        if(m_layouthidden) {
            LoadLayout(m_CurrentLayout,true);
            m_layouthidden = false;
        }
    } else {
        if(!m_layouthidden) {
            m_CurrentLayout = SaveLayout();
            m_Parent->hide();
            // TODO: Hide all docks corresponding to the main window.
            // If Qt does this automatically when hiding the main window, we won't need to conver this code after all.
//            wxAuiPaneInfoArray& myarr = m_mgr->GetAllPanes();
//            size_t i;
//            for(i = 0;i < myarr.size(); i++) {
//                myarr[i].Hide();
//            }
            UpdateLayout();
            m_layouthidden = true;
        }
    }
    if(!show) {
        UpdateLayout();
        ShowWelcomeDialog();
    } else {
        ShowMainWindow();
        UpdateLayout();
    }
}

void AppFrame::Data::DoUpdateAppTitle() {
    if(IsAppShuttingDown())
        return;
    syString title;
    syString modified_str;
    VidProject* prj = ProjectManager::Get()->GetProject();

    if(prj) {
        if(prj->IsModified()) {
            modified_str = "* ";
        }
        title << modified_str << prj->GetTitle();

        syString filename(prj->GetFilename(),true);
        if(!filename.empty()) {
            title << " [" << ioCommon::GetFilename(filename) << "]";
        } else {
            title << " [" << _("untitled") << "]";
        }
        title << " - ";
        title << APP_SHOWNAME;
    } else {
        title << APP_SHOWOFFNAME;
    }
    m_Parent->setWindowTitle(title);
}

void AppFrame::closeEvent(QCloseEvent *event) {
    bool willveto = false;

    if(!IsAppShuttingDown()) {
        if(!ProjectManager::Get()->CloseProject(false))
            willveto = true;
    }

    if(willveto) {
        event->ignore();
    } else {
        event->accept();
        deleteLater(); // Closing the main window will quit the application.
    }
}

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
//void AppFrame::OnUpdateProjectPaneUI() {
//    if(IsAppShuttingDown())
//        return;
//    bool enablePane = ProjectManager::Get()->HasProject();
//    wxWindow* thepane = FindWindow(idProjectPane);
//    if(thepane) {
//        thepane->Enable(enablePane);
//    }
//}

#include "moc/main.moc.h"
