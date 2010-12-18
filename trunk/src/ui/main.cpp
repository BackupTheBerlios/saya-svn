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

#include "main.ui.h"
#include "widgets/playbackcontrol/playbackcontrol.h"
#include "widgets/videoplaybackcontrol/videoplaybackcontrol.h"

#include <saya/core/app.h>
#include <saya/core/systring.h>
#include <saya/core/intl.h>
#include <saya/core/iocommon.h>
#include <saya/core/config.h>
#include <saya/core/debuglog.h>
#include <saya/core/evtregistry.h>
#include <saya/core/sythread.h>
#include <saya/core/dialogs.h>
#include <saya/core/sigslot.h>

#include <saya/saya_events.h>
#include <saya/vidproject.h>
#include <saya/recentfileslist.h>
#include <saya/projectmanager.h>
#include <saya/playbackmanager.h>

#include "main.h"
#include "dialogs/welcomedlg.h"
#include "dialogs/newprojectdlg.h"
// #include "classes/qsyapp.h"
#include "widgets/projectpane/projectpane.h"
using namespace sigslot;

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
const syString CFG_DEFAULT_PRJ_SASHDATA = CFG_LAYOUT_DEFAULT + "/PrjSashData";

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
unsigned int idFileClearRecentProjectList = syActionEvent::RegisterId("action_ClearRecentProjectsHistory", "OnClearRecentProjectList()");
unsigned int idFileClearRecentImportList = syActionEvent::RegisterId("action_ClearRecentImportsHistory", "OnClearRecentImportList()");
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
unsigned int idWorkspaceDefault = syActionEvent::RegisterId("action_WorkspaceDefault","OnLoadDefaultLayout()");
unsigned int idWorkspaceFactoryDefault = syActionEvent::RegisterId("action_WorkspaceFactoryDefault");
unsigned int idWorkspaceSaveAs = syActionEvent::RegisterId("action_WorkspaceSaveAs");
unsigned int idWorkspaceDelete = syActionEvent::RegisterId("action_WorkspaceDelete");
unsigned int idWorkspaceCustom = syActionEvent::RegisterId("action_WorkspaceCustom");
unsigned int idMenuSaveFrameLayout = syActionEvent::RegisterId("action_MenuSaveFrameLayout","OnSaveFrameLayout()");
unsigned int idWindowEffects = syActionEvent::RegisterId("action_WindowEffects");
unsigned int idWindowEffectControls = syActionEvent::RegisterId("action_WindowEffectControls");
unsigned int idWindowHistory = syActionEvent::RegisterId("action_WindowHistory");
unsigned int idWindowInfo = syActionEvent::RegisterId("action_WindowInfo");
unsigned int idWindowTools = syActionEvent::RegisterId("action_WindowTools");
unsigned int idWindowAudioMixer = syActionEvent::RegisterId("action_WindowAudioMixer");
unsigned int idWindowMonitor = syActionEvent::RegisterId("action_WindowMonitor");
unsigned int idWindowProject = syActionEvent::RegisterId("action_WindowProject", "OnShowProjectWindow()");
unsigned int idWindowTimelinesMenu = syActionEvent::RegisterId("action_WindowTimelinesMenu");
unsigned int idHelpAbout = syActionEvent::RegisterId("action_About");

// ----------------------------
// end constants and event id's
// ----------------------------

// --------------------
// begin AppFrame::Data
// --------------------

class AppFrame::Data : public syEvtHandler, public has_slots {
    public:
        Data(AppFrame* parent);
        virtual ~Data();
        AppFrame* m_Parent;

        QDockWidget* CreateProjectPane(); /// Creates the project pane
        QDockWidget* m_ProjectPanel; /// Project Panel
        VideoPlaybackControl* m_MonitorPanel; /// Monitor Panel
        VideoPlaybackControl* m_EffectsPanel; /// Effects Panel
        QDockWidget* m_TimelinePanel; /// Timeline Panel
        WelcomeDialog* m_WelcomeDialog;
        NewProjectDlg *m_NewProjectDlg;

        bool LoadResources();
        /** Adds the Dock Widgets into the corresponding dock areas. */
        void FillDockAreas();
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
        void OpenRecentImport(unsigned int fileno);

        void OnProjectStatusChanged(syProjectStatusEvent& event);

    public:
        bool m_hadproject;
        bool m_panes_status_checked;
        bool m_layouthidden;
        syString m_CurrentLayout;
        syString m_FactoryDefaultLayout;
        unsigned int m_recentfilesmodcounter;
        unsigned int m_recentimportsmodcounter;
        syAction* m_RecentFiles[9];
        syAction* m_RecentImports[9];

        Ui::MainWindow* m_Ui;

        void RegisterAction(unsigned int id, syAction* action);
        void UnregisterAction(unsigned int id);
        void UnregisterAllActions();
        void EnableAction(unsigned int id, bool enable = true);
        void EnableAction(syAction* action, bool enable = true);
        void DisableAction(unsigned int id);
        void DisableAction(syAction* action);
        syAction* GetAction(unsigned int id);
        void OnActionEvent(syActionEvent& event);

    private:
        void InnerRegisterAction(unsigned int id, syAction* action);
        void RegisterSlots();
        void RegisterSlot(unsigned int id, void (AppFrame::Data::*pmemfun)());
        void (AppFrame::Data::*FindSlot(unsigned int id))();

        typedef std::map<unsigned int, syAction*> ActionsMap;
        typedef std::map<unsigned int, void (AppFrame::Data::*)()> SlotsMap;
        ActionsMap m_ActionsMap;
        SlotsMap m_SlotsMap;
        syMutex m_ActionsMutex;

    public: // slots

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
        void OnOpenRecentImport1() { OpenRecentImport(0); }
        void OnOpenRecentImport2() { OpenRecentImport(1); }
        void OnOpenRecentImport3() { OpenRecentImport(2); }
        void OnOpenRecentImport4() { OpenRecentImport(3); }
        void OnOpenRecentImport5() { OpenRecentImport(4); }
        void OnOpenRecentImport6() { OpenRecentImport(5); }
        void OnOpenRecentImport7() { OpenRecentImport(6); }
        void OnOpenRecentImport8() { OpenRecentImport(7); }
        void OnOpenRecentImport9() { OpenRecentImport(8); }

        void OnFileImport();
        void OnFileSave();
        void OnFileSaveAs();
        void OnFileSaveCopy();
        void OnNewProject();
        void OnFileRevert();
        void OnFileCapture();
        void OnFileBatchCapture();
        void OnFileInterpretFootage();
        void OnFileTimecode();

        void OnQuit();
        void OnAbout();
        void OnSaveFrameLayout();
        void OnLoadDefaultLayout();
        void OnWorkspaceFactoryDefault();

        // Window Menu

        void OnShowProjectWindow();


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


    public:
        void LoadAndSetFrameSize();
        void SaveDefaultLayout(bool showmsg);
        bool LoadDefaultLayout(bool firsttime = false);
//
        void DoUpdateAppTitle();

};

AppFrame::Data::Data(AppFrame* parent) :
m_Parent(parent),
m_WelcomeDialog(0),
m_NewProjectDlg(0),
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
    m_RecentFiles[0] = m_Ui->action_RecentProject1;
    m_RecentFiles[1] = m_Ui->action_RecentProject2;
    m_RecentFiles[2] = m_Ui->action_RecentProject3;
    m_RecentFiles[3] = m_Ui->action_RecentProject4;
    m_RecentFiles[4] = m_Ui->action_RecentProject5;
    m_RecentFiles[5] = m_Ui->action_RecentProject6;
    m_RecentFiles[6] = m_Ui->action_RecentProject7;
    m_RecentFiles[7] = m_Ui->action_RecentProject8;
    m_RecentFiles[8] = m_Ui->action_RecentProject9;

    m_RecentImports[0] = m_Ui->action_RecentImport1;
    m_RecentImports[1] = m_Ui->action_RecentImport2;
    m_RecentImports[2] = m_Ui->action_RecentImport3;
    m_RecentImports[3] = m_Ui->action_RecentImport4;
    m_RecentImports[4] = m_Ui->action_RecentImport5;
    m_RecentImports[5] = m_Ui->action_RecentImport6;
    m_RecentImports[6] = m_Ui->action_RecentImport7;
    m_RecentImports[7] = m_Ui->action_RecentImport8;
    m_RecentImports[8] = m_Ui->action_RecentImport9;

    RegisterSlots();
    CreateConnections(m_Ui->menubar);

    // Connect the aboutToShow() signals to the On*UpdateUI() slots.

    m_Ui->file_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnFileMenuUpdateUI);
    m_Ui->action_FileOpenRecentProject->sigaboutToShow.connect(this, &AppFrame::Data::OnRecentFilesMenuUpdateUI);
    m_Ui->action_FileImportRecent->sigaboutToShow.connect(this, &AppFrame::Data::OnRecentImportsMenuUpdateUI);
    m_Ui->edit_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnEditMenuUpdateUI);
    m_Ui->project_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnProjectMenuUpdateUI);
    m_Ui->clip_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnClipMenuUpdateUI);
    m_Ui->sequence_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnSequenceMenuUpdateUI);
    m_Ui->marker_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnMarkerMenuUpdateUI);
    m_Ui->window_menu->sigaboutToShow.connect(this, &AppFrame::Data::OnWindowMenuUpdateUI);

    m_Ui->action_EditLabel->setEnabled(false); // TODO: Re-enable the "Edit Label" menu and set the actions as mutually exclusive.

}

AppFrame::Data::~Data() {

    UnregisterAllActions();
    delete m_WelcomeDialog;
    delete m_Ui;
    if (m_NewProjectDlg) delete m_NewProjectDlg;

}

void AppFrame::Data::RegisterSlots() {

        RegisterSlot(idFileOpen, &AppFrame::Data::OnFileOpen);
        RegisterSlot(idFileClose, &AppFrame::Data::OnFileClose);

        RegisterSlot(idFileClearRecentProjectList, &AppFrame::Data::OnClearRecentProjectList);
        RegisterSlot(idFileClearRecentImportList, &AppFrame::Data::OnClearRecentImportList);
        RegisterSlot(idRecentProject1, &AppFrame::Data::OnOpenRecentFile1);
        RegisterSlot(idRecentProject2, &AppFrame::Data::OnOpenRecentFile2);
        RegisterSlot(idRecentProject3, &AppFrame::Data::OnOpenRecentFile3);
        RegisterSlot(idRecentProject4, &AppFrame::Data::OnOpenRecentFile4);
        RegisterSlot(idRecentProject5, &AppFrame::Data::OnOpenRecentFile5);
        RegisterSlot(idRecentProject6, &AppFrame::Data::OnOpenRecentFile6);
        RegisterSlot(idRecentProject7, &AppFrame::Data::OnOpenRecentFile7);
        RegisterSlot(idRecentProject8, &AppFrame::Data::OnOpenRecentFile8);
        RegisterSlot(idRecentProject9, &AppFrame::Data::OnOpenRecentFile9);
        RegisterSlot(idRecentImport1, &AppFrame::Data::OnOpenRecentImport1);
        RegisterSlot(idRecentImport2, &AppFrame::Data::OnOpenRecentImport2);
        RegisterSlot(idRecentImport3, &AppFrame::Data::OnOpenRecentImport3);
        RegisterSlot(idRecentImport4, &AppFrame::Data::OnOpenRecentImport4);
        RegisterSlot(idRecentImport5, &AppFrame::Data::OnOpenRecentImport5);
        RegisterSlot(idRecentImport6, &AppFrame::Data::OnOpenRecentImport6);
        RegisterSlot(idRecentImport7, &AppFrame::Data::OnOpenRecentImport7);
        RegisterSlot(idRecentImport8, &AppFrame::Data::OnOpenRecentImport8);
        RegisterSlot(idRecentImport9, &AppFrame::Data::OnOpenRecentImport9);

        RegisterSlot(idFileImport,&AppFrame::Data::OnFileImport);
        RegisterSlot(idFileSave,&AppFrame::Data::OnFileSave);
        RegisterSlot(idFileSaveAs,&AppFrame::Data::OnFileSaveAs);
        RegisterSlot(idFileSaveCopy,&AppFrame::Data::OnFileSaveCopy);
        RegisterSlot(idNewProject,&AppFrame::Data::OnNewProject);
        RegisterSlot(idFileRevert,&AppFrame::Data::OnFileRevert);
        RegisterSlot(idFileCapture,&AppFrame::Data::OnFileCapture);
        RegisterSlot(idFileBatchCapture,&AppFrame::Data::OnFileBatchCapture);
        RegisterSlot(idFileInterpretFootage,&AppFrame::Data::OnFileInterpretFootage);
        RegisterSlot(idFileTimecode,&AppFrame::Data::OnFileTimecode);

        RegisterSlot(idQuit, &AppFrame::Data::OnQuit);
        RegisterSlot(idHelpAbout, &AppFrame::Data::OnAbout);
        RegisterSlot(idMenuSaveFrameLayout, &AppFrame::Data::OnSaveFrameLayout);
        RegisterSlot(idWorkspaceDefault, &AppFrame::Data::OnLoadDefaultLayout);
        RegisterSlot(idWorkspaceFactoryDefault, &AppFrame::Data::OnWorkspaceFactoryDefault);

        // Window Menu
        RegisterSlot(idWindowProject, &AppFrame::Data::OnShowProjectWindow);
}


void AppFrame::Data::RegisterSlot(unsigned int id, void (AppFrame::Data::*pmemfun)()) {
    m_SlotsMap[id] = pmemfun;
}

void (AppFrame::Data::*AppFrame::Data::FindSlot(unsigned int id))() {
    SlotsMap::iterator it = m_SlotsMap.find(id);
    if(it == m_SlotsMap.end()) {
        return 0;
    }
    return it->second;
}

void AppFrame::Data::RegisterAction(unsigned int id, syAction* action) {
    if(!this) return;
    syMutexLocker locker(m_ActionsMutex);
    InnerRegisterAction(id, action);
}

void AppFrame::Data::InnerRegisterAction(unsigned int id, syAction* action) {
    if(!this) return;
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

inline syAction* AppFrame::Data::GetAction(unsigned int id) {
    if(!this) return 0;
    ActionsMap::iterator it = m_ActionsMap.find(id);
    if(it == m_ActionsMap.end()) {
        return 0;
    }
    return it->second;
}

inline void AppFrame::Data::EnableAction(unsigned int id, bool enable) {
    syAction* action = GetAction(id);
    if(action) {
        action->setEnabled(enable);
    }
}

inline void AppFrame::Data::EnableAction(syAction* action, bool enable) {
    if(action) {
        action->setEnabled(enable);
    }
}

inline void AppFrame::Data::DisableAction(unsigned int id) {
    EnableAction(id, false);
}

inline void AppFrame::Data::DisableAction(syAction* action) {
    if(action) {
        action->setEnabled(false);
    }
}

void AppFrame::Data::OnActionEvent(syActionEvent& event) {
    if(!this) return;
    syAction* action = GetAction(event.EventId);
    if(action) {
        action->trigger();
    }
}

bool AppFrame::Data::LoadResources() {
    return true;
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
    void (AppFrame::Data::*pmemfun)();

    QList<QAction*> all_actions = parentwidget->actions();
    QList<QAction*>::iterator it = all_actions.begin();
    for(;it != all_actions.end();++it) {
        QAction* theaction = *it;
        if(!theaction) {
            continue;
        }

        QMenu* themenu = theaction->menu();
        if(themenu) {
            CreateConnections(themenu);
        } else {
            syString thename(theaction->objectName());
            unsigned int id = syActionEvent::GetRegisteredId(thename.c_str());
            if(!id) { continue; }

            syAction* tmpaction = dynamic_cast<syAction*>(theaction);
            InnerRegisterAction(id, tmpaction);

            pmemfun = FindSlot(id);
            if(pmemfun) {
                tmpaction->sigtriggered.connect(this,pmemfun);
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

// -------------------------
// begin slots for File Menu
// -------------------------

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
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->GetRecentImports()->clear();
}

void AppFrame::Data::OnFileImport() {
    if(IsAppShuttingDown())
        return;
    #warning TODO: Implement AppFrame::Data::OnFileImport
}

void AppFrame::Data::OpenRecentImport(unsigned int fileno) {
    if(IsAppShuttingDown())
        return;
    if(fileno < 0) fileno = 0;
    if(fileno > 8) fileno = 8;
    ++fileno; // The list is one-based so we add 1.
    #warning TODO: Implement AppFrame::Data::OpenRecentImport(unsigned int fileno)
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
    if (m_NewProjectDlg) {
        delete m_NewProjectDlg;
        m_NewProjectDlg = 0;
    }

    m_NewProjectDlg = new NewProjectDlg();
    if (m_NewProjectDlg) m_NewProjectDlg->show();
}

void AppFrame::Data::OnFileRevert(){
    #warning TODO: Implement AppFrame::Data::OnFileRevert
}

void AppFrame::Data::OnFileCapture(){
    #warning TODO: Implement AppFrame::Data::OnFileCapture
}

void AppFrame::Data::OnFileBatchCapture(){
    #warning TODO: Implement AppFrame::Data::OnFileBatchCapture
}

void AppFrame::Data::OnFileInterpretFootage(){
    #warning TODO: Implement AppFrame::Data::OnFileInterpretFootage
}

void AppFrame::Data::OnFileTimecode(){
    #warning TODO: Implement AppFrame::Data::OnFileTimecode
}

void AppFrame::Data::OnQuit(){
    m_Parent->close();
}

// -------------
// end File Menu
// -------------

// -----------------
// begin Window Menu
// -----------------

void AppFrame::Data::OnShowProjectWindow() {
    m_ProjectPanel->show();
}

// -----------------------
// begin Workspace submenu
// -----------------------

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

// ---------------------
// end Workspace submenu
// ---------------------

void AppFrame::Data::OnFileMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr) return;

    VidProject* prj = pmgr->GetProject();

    bool hasproject = (prj);
    bool isModified = hasproject && prj->IsModified();
    bool isNew = hasproject && prj->IsNew();

    EnableAction(idNewSequence, hasproject);
    EnableAction(idNewBin, hasproject);
    EnableAction(idNewOfflineFile, hasproject);
    EnableAction(idNewTitle, hasproject);
    EnableAction(idNewBarsandTone, hasproject);
    EnableAction(idNewBlackVideo, hasproject);
    EnableAction(idNewColorMatte, hasproject);
    EnableAction(idNewUniversalCountingLeader, hasproject);
    EnableAction(idFileClose,hasproject);
    EnableAction(idFileSave,hasproject && (isModified || isNew));
    EnableAction(idFileSaveAs,hasproject);
    EnableAction(idFileSaveCopy,hasproject);
    EnableAction(idFileRevert,hasproject && isModified && !isNew);
    EnableAction(idFileInterpretFootage,hasproject && (m_Parent->IsClipSelected() || m_Parent->IsResourceClipSelected()));
    EnableAction(idFileTimecode,hasproject && m_Parent->IsClipSelected());
    EnableAction(idFileImport,hasproject);
    EnableAction(idFileExport,hasproject);
    EnableAction(idFileGetPropertiesSelection, hasproject && (m_Parent->IsClipSelected() || m_Parent->IsResourceClipSelected()));
    EnableAction(idFileCapture, hasproject);
    EnableAction(idFileBatchCapture, hasproject);
    EnableAction(idFileGetProperties, hasproject);
    EnableAction(idFileGetPropertiesFile, hasproject);
    EnableAction(idFileGetPropertiesSelection, hasproject);

    m_Ui->action_FileOpenRecentProject->setEnabled(pmgr->GetRecentFiles()->size() > 0);
    m_Ui->action_FileImportRecent->setEnabled(pmgr->GetRecentImports()->size() > 0);
}

void AppFrame::Data::OnRecentFilesMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->GetRecentFiles()->UpdateCounter(m_recentfilesmodcounter)) {
        // Replace the items corresponding to the recently opened projects

        size_t i = 0;
        size_t numrecentprojects = pmgr->GetRecentFiles()->size();
        for(i = 0; i < 9; ++i) {
            if(i >= numrecentprojects) {
                m_RecentFiles[i]->setVisible(false);
                m_RecentFiles[i]->setEnabled(false);
            } else {
                syString tmptext;
                tmptext.Printf("&%d %s",i + 1,pmgr->GetRecentFiles()->item(i + 1).c_str());
                m_RecentFiles[i]->setText(tmptext.c_str());
                m_RecentFiles[i]->setVisible(true);
                m_RecentFiles[i]->setEnabled(true);
            }
        }
    }
}

void AppFrame::Data::OnRecentImportsMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->GetRecentImports()->UpdateCounter(m_recentimportsmodcounter)) {
        // Replace the items corresponding to the recently imported files

        size_t i = 0;
        size_t numrecentimports = pmgr->GetRecentImports()->size();
        for(i = 0; i < 9; ++i) {
            if(i >= numrecentimports) {
                m_RecentFiles[i]->setVisible(false);
                m_RecentFiles[i]->setEnabled(false);
            } else {
                syString tmptext;
                tmptext.Printf("&%d %s",i + 1,pmgr->GetRecentImports()->item(i + 1).c_str());
                m_RecentFiles[i]->setText(tmptext.c_str());
                m_RecentFiles[i]->setVisible(true);
                m_RecentFiles[i]->setEnabled(true);
            }
        }
    }
}

void AppFrame::Data::OnEditMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    VidProject* prj = pmgr->GetProject();

    bool hasproject = (prj);
    bool canUndo = m_Parent->CanUndo();
    bool canRedo = m_Parent->CanRedo();
    bool canCutOrCopy = hasproject && (m_Parent->IsClipSelected() || m_Parent->IsResourceClipSelected());
    bool canPaste = hasproject && (m_Parent->IsClipboardSet() && m_Parent->IsTimelineActive());

    EnableAction(idEditUndo,canUndo);
    EnableAction(idEditRedo,canRedo);
    EnableAction(idEditClearUndoHistory,canUndo || canRedo);
    EnableAction(idEditCut, canCutOrCopy);
    EnableAction(idEditCopy, canCutOrCopy);
    EnableAction(idEditFind, hasproject);
    EnableAction(idEditPaste, canPaste);
    EnableAction(idEditPasteInsert, canPaste);
    EnableAction(idEditPasteAttributes, canPaste);
    EnableAction(idEditClear, hasproject && m_Parent->IsClipSelected());
    EnableAction(idEditRippleDelete, hasproject && m_Parent->IsClipSelected());
    EnableAction(idEditDuplicate, hasproject && m_Parent->IsClipSelected());
    EnableAction(idEditSelectAll, hasproject);
    EnableAction(idEditDeselectAll, hasproject);
    EnableAction(idEditLabel, hasproject && m_Parent->IsClipSelected());
    EnableAction(idEditOriginal, hasproject && m_Parent->IsClipSelected() && !m_Parent->IsSelectionMultiple());

    // TODO: Update the color labels menu and update the corresponding checkboxes
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
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    EnableAction(idProjectSettings,hasproject);
    EnableAction(idProjectSettingsGeneral,hasproject);
    EnableAction(idProjectSettingsCapture,hasproject);
    EnableAction(idProjectSettingsVideoRendering,hasproject);
    EnableAction(idProjectSettingsDefaultSequence,hasproject);
    EnableAction(idProjectLinkMedia,hasproject);
    EnableAction(idProjectUnlinkMedia,hasproject);
    EnableAction(idProjectAutomateToSequence,hasproject);
    EnableAction(idProjectImportBatchList,hasproject);
    EnableAction(idProjectExportBatchList,hasproject);
    EnableAction(idProjectExportAsAAF,hasproject);
}

void AppFrame::Data::OnClipMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    EnableAction(idClipRename, hasproject);
    EnableAction(idClipCaptureSettings, hasproject);
    EnableAction(idClipSetCaptureSettings, hasproject);
    EnableAction(idClipClearCaptureSettings, hasproject);
    EnableAction(idClipInsert, hasproject);
    EnableAction(idClipOverlay, hasproject);
    EnableAction(idClipToggleEnable, hasproject);
    EnableAction(idClipUnlinkAV, hasproject);
    EnableAction(idClipLinkAV, hasproject);
    EnableAction(idClipGroup, hasproject);
    EnableAction(idClipUngroup, hasproject);
    EnableAction(idClipVideoOptions, hasproject);
    EnableAction(idClipVOptFrameHold, hasproject);
    EnableAction(idClipVOptFieldOptions, hasproject);
    EnableAction(idClipAudioOptions, hasproject);
    EnableAction(idClipAOptGain, hasproject);
    EnableAction(idClipAOptBreakoutToMono, hasproject);
    EnableAction(idClipOptTreatAsStereo, hasproject);
    EnableAction(idClipOptRenderAndReplace, hasproject);
    EnableAction(idClipSpeedDuration, hasproject);
}

void AppFrame::Data::OnSequenceMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    EnableAction(idSequenceRender, hasproject);
    EnableAction(idSequenceDeleteRenderFiles, hasproject);
    EnableAction(idSequenceRazor, hasproject);
    EnableAction(idSequenceRazorSelectedTracks, hasproject);
    EnableAction(idSequenceLift, hasproject);
    EnableAction(idSequenceExtract, hasproject);
    EnableAction(idSequenceApplyVideoTransition, hasproject);
    EnableAction(idSequenceApplyAudioTransition, hasproject);
    EnableAction(idSequenceZoomIn, hasproject);
    EnableAction(idSequenceZoomOut, hasproject);
    EnableAction(idSequenceSnap, hasproject);
    EnableAction(idSequenceAddTracks, hasproject);
    EnableAction(idSequenceDelTracks, hasproject);
}

void AppFrame::Data::OnMarkerMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    EnableAction(idSetClipMarkerMenu, hasproject);
    EnableAction(idGotoClipMarkerMenu, hasproject);
    EnableAction(idGotoNextClipMarker, hasproject);
    EnableAction(idGotoPrevClipMarker, hasproject);
    EnableAction(idGotoInClipMarker, hasproject);
    EnableAction(idGotoOutClipMarker, hasproject);
    EnableAction(idGotoVideoInClipMarker, hasproject);
    EnableAction(idGotoVideoOutClipMarker, hasproject);
    EnableAction(idGotoAudioInClipMarker, hasproject);
    EnableAction(idGotoAudioOutClipMarker, hasproject);
    EnableAction(idGotoNumberedClipMarker, hasproject);
    EnableAction(idClearClipMarkerMenu, hasproject);
    EnableAction(idClearCurrentClipMarker, hasproject);
    EnableAction(idClearAllClipMarkers, hasproject);
    EnableAction(idClearInOutClipMarkers, hasproject);
    EnableAction(idClearInClipMarker, hasproject);
    EnableAction(idClearOutClipMarker, hasproject);
    EnableAction(idClearNumberedClipMarker, hasproject);
    EnableAction(idSetSequenceMarkerMenu, hasproject);
    EnableAction(idSetInSequenceMarker, hasproject);
    EnableAction(idSetOutSequenceMarker, hasproject);
    EnableAction(idSetInOutAroundSelSeqMarker, hasproject);
    EnableAction(idSetUnnumberedSequenceMarker, hasproject);
    EnableAction(idSetNextAvailNumberedSeqMarker, hasproject);
    EnableAction(idSetOtherNumberedSequenceMarker, hasproject);
    EnableAction(idGotoSequenceMarkerMenu, hasproject);
    EnableAction(idGotoNextSequenceMarker, hasproject);
    EnableAction(idGotoPrevSequenceMarker, hasproject);
    EnableAction(idGotoInSequenceMarker, hasproject);
    EnableAction(idGotoOutSequenceMarker, hasproject);
    EnableAction(idGotoNumberedSeqMarker, hasproject);
    EnableAction(idClearSequenceMarkerMenu, hasproject);
    EnableAction(idClearCurSequenceMarker, hasproject);
    EnableAction(idClearAllSequenceMarkers, hasproject);
    EnableAction(idClearInOutSeqMarkers, hasproject);
    EnableAction(idClearInSeqMarker, hasproject);
    EnableAction(idClearOutSeqMarker, hasproject);
    EnableAction(idClearNumberedSeqMarker, hasproject);
    EnableAction(idEditSequenceMarker, hasproject);
}

void AppFrame::Data::OnWindowMenuUpdateUI() {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    EnableAction(idWindowWorkspaceMenu, hasproject);
    EnableAction(idWorkspaceEditing, hasproject);
    EnableAction(idWorkspaceEffects, hasproject);
    EnableAction(idWorkspaceAudio, hasproject);
    EnableAction(idWorkspaceColorCorrection, hasproject);
    EnableAction(idWorkspaceDefault, hasproject);
    EnableAction(idWorkspaceSaveAs, hasproject);
    EnableAction(idWorkspaceDelete, hasproject);
    EnableAction(idWorkspaceCustom, hasproject);
    EnableAction(idMenuSaveFrameLayout, hasproject);
    EnableAction(idWindowEffects, hasproject);
    EnableAction(idWindowEffectControls, hasproject);
    EnableAction(idWindowHistory, hasproject);
    EnableAction(idWindowInfo, hasproject);
    EnableAction(idWindowTools, hasproject);
    EnableAction(idWindowAudioMixer, hasproject);
    EnableAction(idWindowMonitor, hasproject);
    EnableAction(idWindowProject, hasproject);
    EnableAction(idWindowTimelinesMenu, hasproject);
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
    QByteArray tmpdata = m_Parent->saveState().toBase64();
    return syString(tmpdata.data());
}

bool AppFrame::Data::LoadLayout(const syString& layoutdata, bool update) {
    bool result = false;
    result = m_Parent->restoreState(QByteArray::fromBase64(QByteArray(layoutdata.c_str())));
    if(result && update) {
        UpdateLayout();
    }
    return result;
}


// ------------------
// end AppFrame::Data
// ------------------

syString g_statustext;

AppFrame::AppFrame(const syString& title) :
QMainWindow(),
m_Data(new Data(this))
{
    bool result = false;
    do {
        if(!m_Data->CreateDialogs()) break;

        m_Data->LoadAndSetFrameSize();

        if(!m_Data->CreatePanels()) break;
        m_Data->FillDockAreas();

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
        // QWidget* tmpwidget = new VideoPlaybackControl(0);
        // tmpwidget->show();

        m_ProjectPanel = CreateProjectPane();
        if(!m_ProjectPanel) { LoadFail("project_panel"); break; }
        // TODO: Create the timeline and video playback widgets here.
        m_EffectsPanel = new VideoPlaybackControl(0);
        m_EffectsPanel->setWindowTitle(_("Input / Effects"));
        m_MonitorPanel = new VideoPlaybackControl(0);
        m_MonitorPanel->setWindowTitle(_("Output Monitor"));
        m_TimelinePanel = new QDockWidget;
        m_TimelinePanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
        m_TimelinePanel->setWindowTitle(_("Timeline"));

        m_EffectsPanel->SetAVPlayer(PlaybackManager::Get()->GetInputMonitor());
        m_MonitorPanel->SetAVPlayer(PlaybackManager::Get()->GetPreviewMonitor());

//           m_effectspanel = new wxVideoPlaybackPanel(this);
//           m_monitorpanel = new wxVideoPlaybackPanel(this);
//           static_cast<wxVideoPlaybackPanel*>(m_effectspanel)->SetAVPlayer(PlaybackManager::Get()->GetInputMonitor());
//           static_cast<wxVideoPlaybackPanel*>(m_monitorpanel)->SetAVPlayer(PlaybackManager::Get()->GetPreviewMonitor());
//        m_timelinepanel = new wxPanel(this, -1,wxDefaultPosition, wxSize(800,400));
        result = true;
    }while(false);
    return result;
}

void AppFrame::Data::FillDockAreas() {

    m_Parent->addDockWidget(Qt::LeftDockWidgetArea, m_ProjectPanel);
    m_Parent->setCentralWidget(m_TimelinePanel);
    m_Parent->addDockWidget(Qt::BottomDockWidgetArea, m_EffectsPanel);
    m_Parent->addDockWidget(Qt::BottomDockWidgetArea, m_MonitorPanel);
    m_EffectsPanel->show();
    m_MonitorPanel->show();
    m_TimelinePanel->show();
}

QDockWidget* AppFrame::Data::CreateProjectPane() {
    ProjectPane* prjpane = new ProjectPane(m_Parent);

    syString splitterstate = syApp::GetConfig()->Read(CFG_DEFAULT_PRJ_SASHDATA, "");
    prjpane->RestoreSplitterState(splitterstate);
    return dynamic_cast<QDockWidget*>(prjpane);
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
        DebugLog("SaveDefaultLayout: Layout hidden. Not saving.");
        return;
    }
    DebugLog("SaveDefaultLayout: Saving default layout...");
    QRect rect = m_Parent->geometry();
    syString key = CFG_LOCATION;
    syApp::GetConfig()->WriteInt(key + "/xpos", rect.x());
    syApp::GetConfig()->WriteInt(key + "/ypos", rect.y());
    syApp::GetConfig()->WriteInt(key + "/width", rect.width());
    syApp::GetConfig()->WriteInt(key + "/height", rect.height());

    syString strlayout(SaveLayout());
    syApp::GetConfig()->Write(CFG_PERSPECTIVE_DEFAULT, strlayout);

    syApp::GetConfig()->Write(CFG_DEFAULT_PRJ_SASHDATA,dynamic_cast<ProjectPane*>(m_ProjectPanel)->SaveSplitterState());

    if(showmsg) {
        syMessageBox(_("Current Layout has been saved."),
                      _("Save Layout"), syOK);
    }
}

void AppFrame::Data::ShowLayout(bool show) {
    if(IsAppShuttingDown())
        return;
    if(show) {
        if(m_layouthidden) {
            LoadLayout(m_CurrentLayout,true);
            m_layouthidden = false;
        }
    } else {
        if(!m_layouthidden) {
            m_CurrentLayout = SaveLayout();
            m_Parent->hide();
            // TODO: Hide all dock widgets.
            m_ProjectPanel->hide();
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
