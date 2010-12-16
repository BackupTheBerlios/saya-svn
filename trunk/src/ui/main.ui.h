/********************************************************************************
** Form generated from reading ui file 'mainwindowTi6980.ui'
**
** Created: Wed Dec 15 23:30:13 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef MAINWINDOWTI6980_H
#define MAINWINDOWTI6980_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <ui/widgets/generic/menu.h>
#include <ui/widgets/generic/action.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    syAction *action_NewProject;
    syAction *action_NewSequence;
    syAction *action_NewBin;
    syAction *action_NewOfflineFile;
    syAction *action_NewTitle;
    syAction *action_NewBarsandTone;
    syAction *action_NewBlackVideo;
    syAction *action_NewColorMatte;
    syAction *action_NewUniversalCountingLeader;
    syAction *action_FileOpen;
    syAction *action_ClearRecentProjectsHistory;
    syAction *action_RecentProject1;
    syAction *action_RecentProject2;
    syAction *action_RecentProject3;
    syAction *action_RecentProject4;
    syAction *action_RecentProject5;
    syAction *action_RecentProject6;
    syAction *action_RecentProject7;
    syAction *action_RecentProject8;
    syAction *action_RecentProject9;
    syAction *action_ClearRecentImportsHistory;
    syAction *action_RecentImport1;
    syAction *action_RecentImport2;
    syAction *action_RecentImport3;
    syAction *action_RecentImport4;
    syAction *action_RecentImport5;
    syAction *action_RecentImport6;
    syAction *action_RecentImport7;
    syAction *action_RecentImport8;
    syAction *action_RecentImport9;
    syAction *action_FileClose;
    syAction *action_FileSave;
    syAction *action_FileSaveAs;
    syAction *action_FileSaveCopy;
    syAction *action_FileRevert;
    syAction *action_FileCapture;
    syAction *action_FileBatchCapture;
    syAction *action_FileImport;
    syAction *action_ExportMovie;
    syAction *action_ExportFrame;
    syAction *action_ExportAudio;
    syAction *action_ExportToTape;
    syAction *action_ExportMediaEncoder;
    syAction *action_ExportToDVD;
    syAction *action_FileGetPropertiesFile;
    syAction *action_FileGetPropertiesSelection;
    syAction *action_FileInterpretFootage;
    syAction *action_FileTimecode;
    syAction *action_Quit;
    syAction *action_EditUndo;
    syAction *action_EditRedo;
    syAction *action_EditClearUndoHistory;
    syAction *action_EditCut;
    syAction *action_EditCopy;
    syAction *action_EditPaste;
    syAction *action_EditPasteInsert;
    syAction *action_EditPasteAttributes;
    syAction *action_EditClear;
    syAction *action_EditRippleDelete;
    syAction *action_EditDuplicate;
    syAction *action_EditSelectAll;
    syAction *action_EditDeselectAll;
    syAction *action_EditFind;
    syAction *action_EditLabelSelectLabelGroup;
    syAction *action_EditLabelBlue;
    syAction *action_EditLabelCyan;
    syAction *action_EditLabelGreen;
    syAction *action_EditLabelViolet;
    syAction *action_EditLabelPink;
    syAction *action_EditLabelGray;
    syAction *action_EditLabelRed;
    syAction *action_EditLabelOrange;
    syAction *action_EditOriginal;
    syAction *action_ProjectSettingsGeneral;
    syAction *action_ProjectSettingsCapture;
    syAction *action_ProjectSettingsVideoRendering;
    syAction *action_ProjectSettingsDefaultSequence;
    syAction *action_ProjectLinkMedia;
    syAction *action_ProjectUnlinkMedia;
    syAction *action_ProjectAutomateToSequence;
    syAction *action_ProjectImportBatchList;
    syAction *action_ProjectExportBatchList;
    syAction *action_ProjectExportAsAAF;
    syAction *action_ClipRename;
    syAction *action_ClipSetCaptureSettings;
    syAction *action_ClipClearCaptureSettings;
    syAction *action_ClipInsert;
    syAction *action_ClipOverlay;
    syAction *action_ClipToggleEnable;
    syAction *action_ClipUnlinkAV;
    syAction *action_ClipLinkAV;
    syAction *action_ClipGroup;
    syAction *action_ClipUngroup;
    syAction *action_ClipVOptFrameHold;
    syAction *action_ClipVOptFieldOptions;
    syAction *action_ClipAOptGain;
    syAction *action_ClipAOptBreakoutToMono;
    syAction *action_ClipOptTreatAsStereo;
    syAction *action_ClipOptRenderAndReplace;
    syAction *action_ClipSpeedDuration;
    syAction *action_SequenceRender;
    syAction *action_SequenceDeleteRenderFiles;
    syAction *action_SequenceRazor;
    syAction *action_SequenceRazorSelectedTracks;
    syAction *action_SequenceLift;
    syAction *action_SequenceExtract;
    syAction *action_SequenceApplyVideoTransition;
    syAction *action_SequenceApplyAudioTransition;
    syAction *action_SequenceZoomIn;
    syAction *action_SequenceZoomOut;
    syAction *action_SequenceSnap;
    syAction *action_SequenceAddTracks;
    syAction *action_SequenceDelTracks;
    syAction *action_SetClipMarkerIn;
    syAction *action_SetClipMarkerOut;
    syAction *action_SetClipMarkerVideoIn;
    syAction *action_SetClipMarkerVideoOut;
    syAction *action_SetClipMarkerAudioIn;
    syAction *action_SetClipMarkerAudioOut;
    syAction *action_SetClipMarkerUnnumbered;
    syAction *action_SetClipMarkerNextAvailableNumbered;
    syAction *action_SetClipMarkerOtherNumbered;
    syAction *action_GotoNextClipMarker;
    syAction *action_GotoPrevClipMarker;
    syAction *action_GotoInClipMarker;
    syAction *action_GotoOutClipMarker;
    syAction *action_GotoVideoInClipMarker;
    syAction *action_GotoVideoOutClipMarker;
    syAction *action_GotoAudioInClipMarker;
    syAction *action_GotoAudioOutClipMarker;
    syAction *action_GotoNumberedClipMarker;
    syAction *action_ClearCurrentClipMarker;
    syAction *action_ClearAllClipMarkers;
    syAction *action_ClearInOutClipMarkers;
    syAction *action_ClearInClipMarker;
    syAction *action_ClearOutClipMarker;
    syAction *action_ClearNumberedClipMarker;
    syAction *action_SetInSequenceMarker;
    syAction *action_SetOutSequenceMarker;
    syAction *action_SetInOutAroundSelSeqMarker;
    syAction *action_SetUnnumberedSequenceMarker;
    syAction *action_SetNextAvailNumberedSeqMarker;
    syAction *action_SetOtherNumberedSequenceMarker;
    syAction *action_GotoNextSequenceMarker;
    syAction *action_GotoPrevSequenceMarker;
    syAction *action_GotoInSequenceMarker;
    syAction *action_GotoOutSequenceMarker;
    syAction *action_GotoNumberedSeqMarker;
    syAction *action_ClearCurSequenceMarker;
    syAction *action_ClearAllSequenceMarkers;
    syAction *action_ClearInOutSeqMarkers;
    syAction *action_ClearInSeqMarker;
    syAction *action_ClearOutSeqMarker;
    syAction *action_ClearNumberedSeqMarker;
    syAction *action_EditSequenceMarker;
    syAction *action_WorkspaceEditing;
    syAction *action_WorkspaceEffects;
    syAction *action_WorkspaceAudio;
    syAction *action_WorkspaceColorCorrection;
    syAction *action_WorkspaceCustom;
    syAction *action_WorkspaceDefault;
    syAction *action_WorkspaceFactoryDefault;
    syAction *action_WorkspaceSaveAs;
    syAction *action_WorkspaceDelete;
    syAction *action_MenuSaveFrameLayout;
    syAction *action_WindowEffects;
    syAction *action_WindowEffectControls;
    syAction *action_WindowHistory;
    syAction *action_WindowInfo;
    syAction *action_WindowTools;
    syAction *action_WindowAudioMixer;
    syAction *action_WindowMonitor;
    syAction *action_WindowProject;
    syAction *action_PrefsGeneral;
    syAction *action_PrefsAudio;
    syAction *action_PrefsAudioHardware;
    syAction *action_PrefsAutoSave;
    syAction *action_PrefsCapture;
    syAction *action_PrefsDeviceControl;
    syAction *action_PrefsLabelColors;
    syAction *action_PrefsLabelDefaults;
    syAction *action_PrefsScratchDisks;
    syAction *action_PrefsStillImage;
    syAction *action_PrefsTitler;
    syAction *action_PrefsTrim;
    syAction *action_KeyboardCustomization;
    syAction *action_HelpContents;
    syAction *action_HelpSearch;
    syAction *action_HelpKeyboard;
    syAction *action_HelpWebsite;
    syAction *action_About;
    syAction *action_AboutQt;
    syAction *action_ResourcesScan;
    QWidget *centralwidget;
    QMenuBar *menubar;
    syMenu *file_menu;
    syMenu *action_FileNew;
    syMenu *action_FileOpenRecentProject;
    syMenu *action_FileImportRecent;
    syMenu *action_FileExport;
    syMenu *action_FileGetProperties;
    syMenu *edit_menu;
    syMenu *action_EditLabel;
    syMenu *project_menu;
    syMenu *action_ProjectSettings;
    syMenu *clip_menu;
    syMenu *action_ClipCaptureSettings;
    syMenu *action_ClipVideoOptions;
    syMenu *action_ClipAudioOptions;
    syMenu *sequence_menu;
    syMenu *marker_menu;
    syMenu *action_SetClipMarkerMenu;
    syMenu *action_GotoClipMarkerMenu;
    syMenu *action_ClearClipMarkerMenu;
    syMenu *action_SetSequenceMarkerMenu;
    syMenu *action_GotoSequenceMarkerMenu;
    syMenu *action_ClearSequenceMarkerMenu;
    syMenu *window_menu;
    syMenu *action_WindowWorkspaceMenu;
    syMenu *action_WindowTimelinesMenu;
    syMenu *options_menu;
    syMenu *help_menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        action_NewProject = new syAction(MainWindow);
        action_NewProject->setObjectName(QString::fromUtf8("action_NewProject"));
        action_NewSequence = new syAction(MainWindow);
        action_NewSequence->setObjectName(QString::fromUtf8("action_NewSequence"));
        action_NewBin = new syAction(MainWindow);
        action_NewBin->setObjectName(QString::fromUtf8("action_NewBin"));
        action_NewOfflineFile = new syAction(MainWindow);
        action_NewOfflineFile->setObjectName(QString::fromUtf8("action_NewOfflineFile"));
        action_NewTitle = new syAction(MainWindow);
        action_NewTitle->setObjectName(QString::fromUtf8("action_NewTitle"));
        action_NewBarsandTone = new syAction(MainWindow);
        action_NewBarsandTone->setObjectName(QString::fromUtf8("action_NewBarsandTone"));
        action_NewBlackVideo = new syAction(MainWindow);
        action_NewBlackVideo->setObjectName(QString::fromUtf8("action_NewBlackVideo"));
        action_NewColorMatte = new syAction(MainWindow);
        action_NewColorMatte->setObjectName(QString::fromUtf8("action_NewColorMatte"));
        action_NewUniversalCountingLeader = new syAction(MainWindow);
        action_NewUniversalCountingLeader->setObjectName(QString::fromUtf8("action_NewUniversalCountingLeader"));
        action_FileOpen = new syAction(MainWindow);
        action_FileOpen->setObjectName(QString::fromUtf8("action_FileOpen"));
        action_ClearRecentProjectsHistory = new syAction(MainWindow);
        action_ClearRecentProjectsHistory->setObjectName(QString::fromUtf8("action_ClearRecentProjectsHistory"));
        action_RecentProject1 = new syAction(MainWindow);
        action_RecentProject1->setObjectName(QString::fromUtf8("action_RecentProject1"));
        action_RecentProject2 = new syAction(MainWindow);
        action_RecentProject2->setObjectName(QString::fromUtf8("action_RecentProject2"));
        action_RecentProject3 = new syAction(MainWindow);
        action_RecentProject3->setObjectName(QString::fromUtf8("action_RecentProject3"));
        action_RecentProject4 = new syAction(MainWindow);
        action_RecentProject4->setObjectName(QString::fromUtf8("action_RecentProject4"));
        action_RecentProject5 = new syAction(MainWindow);
        action_RecentProject5->setObjectName(QString::fromUtf8("action_RecentProject5"));
        action_RecentProject6 = new syAction(MainWindow);
        action_RecentProject6->setObjectName(QString::fromUtf8("action_RecentProject6"));
        action_RecentProject7 = new syAction(MainWindow);
        action_RecentProject7->setObjectName(QString::fromUtf8("action_RecentProject7"));
        action_RecentProject8 = new syAction(MainWindow);
        action_RecentProject8->setObjectName(QString::fromUtf8("action_RecentProject8"));
        action_RecentProject9 = new syAction(MainWindow);
        action_RecentProject9->setObjectName(QString::fromUtf8("action_RecentProject9"));
        action_ClearRecentImportsHistory = new syAction(MainWindow);
        action_ClearRecentImportsHistory->setObjectName(QString::fromUtf8("action_ClearRecentImportsHistory"));
        action_RecentImport1 = new syAction(MainWindow);
        action_RecentImport1->setObjectName(QString::fromUtf8("action_RecentImport1"));
        action_RecentImport2 = new syAction(MainWindow);
        action_RecentImport2->setObjectName(QString::fromUtf8("action_RecentImport2"));
        action_RecentImport3 = new syAction(MainWindow);
        action_RecentImport3->setObjectName(QString::fromUtf8("action_RecentImport3"));
        action_RecentImport4 = new syAction(MainWindow);
        action_RecentImport4->setObjectName(QString::fromUtf8("action_RecentImport4"));
        action_RecentImport5 = new syAction(MainWindow);
        action_RecentImport5->setObjectName(QString::fromUtf8("action_RecentImport5"));
        action_RecentImport6 = new syAction(MainWindow);
        action_RecentImport6->setObjectName(QString::fromUtf8("action_RecentImport6"));
        action_RecentImport7 = new syAction(MainWindow);
        action_RecentImport7->setObjectName(QString::fromUtf8("action_RecentImport7"));
        action_RecentImport8 = new syAction(MainWindow);
        action_RecentImport8->setObjectName(QString::fromUtf8("action_RecentImport8"));
        action_RecentImport9 = new syAction(MainWindow);
        action_RecentImport9->setObjectName(QString::fromUtf8("action_RecentImport9"));
        action_FileClose = new syAction(MainWindow);
        action_FileClose->setObjectName(QString::fromUtf8("action_FileClose"));
        action_FileSave = new syAction(MainWindow);
        action_FileSave->setObjectName(QString::fromUtf8("action_FileSave"));
        action_FileSaveAs = new syAction(MainWindow);
        action_FileSaveAs->setObjectName(QString::fromUtf8("action_FileSaveAs"));
        action_FileSaveCopy = new syAction(MainWindow);
        action_FileSaveCopy->setObjectName(QString::fromUtf8("action_FileSaveCopy"));
        action_FileRevert = new syAction(MainWindow);
        action_FileRevert->setObjectName(QString::fromUtf8("action_FileRevert"));
        action_FileRevert->setEnabled(false);
        action_FileCapture = new syAction(MainWindow);
        action_FileCapture->setObjectName(QString::fromUtf8("action_FileCapture"));
        action_FileBatchCapture = new syAction(MainWindow);
        action_FileBatchCapture->setObjectName(QString::fromUtf8("action_FileBatchCapture"));
        action_FileImport = new syAction(MainWindow);
        action_FileImport->setObjectName(QString::fromUtf8("action_FileImport"));
        action_ExportMovie = new syAction(MainWindow);
        action_ExportMovie->setObjectName(QString::fromUtf8("action_ExportMovie"));
        action_ExportFrame = new syAction(MainWindow);
        action_ExportFrame->setObjectName(QString::fromUtf8("action_ExportFrame"));
        action_ExportAudio = new syAction(MainWindow);
        action_ExportAudio->setObjectName(QString::fromUtf8("action_ExportAudio"));
        action_ExportToTape = new syAction(MainWindow);
        action_ExportToTape->setObjectName(QString::fromUtf8("action_ExportToTape"));
        action_ExportToTape->setEnabled(false);
        action_ExportMediaEncoder = new syAction(MainWindow);
        action_ExportMediaEncoder->setObjectName(QString::fromUtf8("action_ExportMediaEncoder"));
        action_ExportToDVD = new syAction(MainWindow);
        action_ExportToDVD->setObjectName(QString::fromUtf8("action_ExportToDVD"));
        action_FileGetPropertiesFile = new syAction(MainWindow);
        action_FileGetPropertiesFile->setObjectName(QString::fromUtf8("action_FileGetPropertiesFile"));
        action_FileGetPropertiesSelection = new syAction(MainWindow);
        action_FileGetPropertiesSelection->setObjectName(QString::fromUtf8("action_FileGetPropertiesSelection"));
        action_FileInterpretFootage = new syAction(MainWindow);
        action_FileInterpretFootage->setObjectName(QString::fromUtf8("action_FileInterpretFootage"));
        action_FileInterpretFootage->setEnabled(false);
        action_FileTimecode = new syAction(MainWindow);
        action_FileTimecode->setObjectName(QString::fromUtf8("action_FileTimecode"));
        action_FileTimecode->setEnabled(false);
        action_Quit = new syAction(MainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        action_Quit->setMenuRole(syAction::QuitRole);
        action_EditUndo = new syAction(MainWindow);
        action_EditUndo->setObjectName(QString::fromUtf8("action_EditUndo"));
        action_EditRedo = new syAction(MainWindow);
        action_EditRedo->setObjectName(QString::fromUtf8("action_EditRedo"));
        action_EditClearUndoHistory = new syAction(MainWindow);
        action_EditClearUndoHistory->setObjectName(QString::fromUtf8("action_EditClearUndoHistory"));
        action_EditCut = new syAction(MainWindow);
        action_EditCut->setObjectName(QString::fromUtf8("action_EditCut"));
        action_EditCopy = new syAction(MainWindow);
        action_EditCopy->setObjectName(QString::fromUtf8("action_EditCopy"));
        action_EditPaste = new syAction(MainWindow);
        action_EditPaste->setObjectName(QString::fromUtf8("action_EditPaste"));
        action_EditPasteInsert = new syAction(MainWindow);
        action_EditPasteInsert->setObjectName(QString::fromUtf8("action_EditPasteInsert"));
        action_EditPasteAttributes = new syAction(MainWindow);
        action_EditPasteAttributes->setObjectName(QString::fromUtf8("action_EditPasteAttributes"));
        action_EditClear = new syAction(MainWindow);
        action_EditClear->setObjectName(QString::fromUtf8("action_EditClear"));
        action_EditRippleDelete = new syAction(MainWindow);
        action_EditRippleDelete->setObjectName(QString::fromUtf8("action_EditRippleDelete"));
        action_EditDuplicate = new syAction(MainWindow);
        action_EditDuplicate->setObjectName(QString::fromUtf8("action_EditDuplicate"));
        action_EditSelectAll = new syAction(MainWindow);
        action_EditSelectAll->setObjectName(QString::fromUtf8("action_EditSelectAll"));
        action_EditDeselectAll = new syAction(MainWindow);
        action_EditDeselectAll->setObjectName(QString::fromUtf8("action_EditDeselectAll"));
        action_EditFind = new syAction(MainWindow);
        action_EditFind->setObjectName(QString::fromUtf8("action_EditFind"));
        action_EditLabelSelectLabelGroup = new syAction(MainWindow);
        action_EditLabelSelectLabelGroup->setObjectName(QString::fromUtf8("action_EditLabelSelectLabelGroup"));
        action_EditLabelSelectLabelGroup->setEnabled(false);
        action_EditLabelBlue = new syAction(MainWindow);
        action_EditLabelBlue->setObjectName(QString::fromUtf8("action_EditLabelBlue"));
        action_EditLabelBlue->setCheckable(true);
        action_EditLabelCyan = new syAction(MainWindow);
        action_EditLabelCyan->setObjectName(QString::fromUtf8("action_EditLabelCyan"));
        action_EditLabelCyan->setCheckable(true);
        action_EditLabelGreen = new syAction(MainWindow);
        action_EditLabelGreen->setObjectName(QString::fromUtf8("action_EditLabelGreen"));
        action_EditLabelGreen->setCheckable(true);
        action_EditLabelViolet = new syAction(MainWindow);
        action_EditLabelViolet->setObjectName(QString::fromUtf8("action_EditLabelViolet"));
        action_EditLabelViolet->setCheckable(true);
        action_EditLabelPink = new syAction(MainWindow);
        action_EditLabelPink->setObjectName(QString::fromUtf8("action_EditLabelPink"));
        action_EditLabelPink->setCheckable(true);
        action_EditLabelGray = new syAction(MainWindow);
        action_EditLabelGray->setObjectName(QString::fromUtf8("action_EditLabelGray"));
        action_EditLabelGray->setCheckable(true);
        action_EditLabelRed = new syAction(MainWindow);
        action_EditLabelRed->setObjectName(QString::fromUtf8("action_EditLabelRed"));
        action_EditLabelRed->setCheckable(true);
        action_EditLabelOrange = new syAction(MainWindow);
        action_EditLabelOrange->setObjectName(QString::fromUtf8("action_EditLabelOrange"));
        action_EditLabelOrange->setCheckable(true);
        action_EditOriginal = new syAction(MainWindow);
        action_EditOriginal->setObjectName(QString::fromUtf8("action_EditOriginal"));
        action_ProjectSettingsGeneral = new syAction(MainWindow);
        action_ProjectSettingsGeneral->setObjectName(QString::fromUtf8("action_ProjectSettingsGeneral"));
        action_ProjectSettingsCapture = new syAction(MainWindow);
        action_ProjectSettingsCapture->setObjectName(QString::fromUtf8("action_ProjectSettingsCapture"));
        action_ProjectSettingsVideoRendering = new syAction(MainWindow);
        action_ProjectSettingsVideoRendering->setObjectName(QString::fromUtf8("action_ProjectSettingsVideoRendering"));
        action_ProjectSettingsDefaultSequence = new syAction(MainWindow);
        action_ProjectSettingsDefaultSequence->setObjectName(QString::fromUtf8("action_ProjectSettingsDefaultSequence"));
        action_ProjectLinkMedia = new syAction(MainWindow);
        action_ProjectLinkMedia->setObjectName(QString::fromUtf8("action_ProjectLinkMedia"));
        action_ProjectUnlinkMedia = new syAction(MainWindow);
        action_ProjectUnlinkMedia->setObjectName(QString::fromUtf8("action_ProjectUnlinkMedia"));
        action_ProjectAutomateToSequence = new syAction(MainWindow);
        action_ProjectAutomateToSequence->setObjectName(QString::fromUtf8("action_ProjectAutomateToSequence"));
        action_ProjectImportBatchList = new syAction(MainWindow);
        action_ProjectImportBatchList->setObjectName(QString::fromUtf8("action_ProjectImportBatchList"));
        action_ProjectExportBatchList = new syAction(MainWindow);
        action_ProjectExportBatchList->setObjectName(QString::fromUtf8("action_ProjectExportBatchList"));
        action_ProjectExportAsAAF = new syAction(MainWindow);
        action_ProjectExportAsAAF->setObjectName(QString::fromUtf8("action_ProjectExportAsAAF"));
        action_ClipRename = new syAction(MainWindow);
        action_ClipRename->setObjectName(QString::fromUtf8("action_ClipRename"));
        action_ClipSetCaptureSettings = new syAction(MainWindow);
        action_ClipSetCaptureSettings->setObjectName(QString::fromUtf8("action_ClipSetCaptureSettings"));
        action_ClipClearCaptureSettings = new syAction(MainWindow);
        action_ClipClearCaptureSettings->setObjectName(QString::fromUtf8("action_ClipClearCaptureSettings"));
        action_ClipInsert = new syAction(MainWindow);
        action_ClipInsert->setObjectName(QString::fromUtf8("action_ClipInsert"));
        action_ClipOverlay = new syAction(MainWindow);
        action_ClipOverlay->setObjectName(QString::fromUtf8("action_ClipOverlay"));
        action_ClipToggleEnable = new syAction(MainWindow);
        action_ClipToggleEnable->setObjectName(QString::fromUtf8("action_ClipToggleEnable"));
        action_ClipToggleEnable->setCheckable(true);
        action_ClipUnlinkAV = new syAction(MainWindow);
        action_ClipUnlinkAV->setObjectName(QString::fromUtf8("action_ClipUnlinkAV"));
        action_ClipUnlinkAV->setEnabled(false);
        action_ClipLinkAV = new syAction(MainWindow);
        action_ClipLinkAV->setObjectName(QString::fromUtf8("action_ClipLinkAV"));
        action_ClipGroup = new syAction(MainWindow);
        action_ClipGroup->setObjectName(QString::fromUtf8("action_ClipGroup"));
        action_ClipUngroup = new syAction(MainWindow);
        action_ClipUngroup->setObjectName(QString::fromUtf8("action_ClipUngroup"));
        action_ClipVOptFrameHold = new syAction(MainWindow);
        action_ClipVOptFrameHold->setObjectName(QString::fromUtf8("action_ClipVOptFrameHold"));
        action_ClipVOptFieldOptions = new syAction(MainWindow);
        action_ClipVOptFieldOptions->setObjectName(QString::fromUtf8("action_ClipVOptFieldOptions"));
        action_ClipAOptGain = new syAction(MainWindow);
        action_ClipAOptGain->setObjectName(QString::fromUtf8("action_ClipAOptGain"));
        action_ClipAOptBreakoutToMono = new syAction(MainWindow);
        action_ClipAOptBreakoutToMono->setObjectName(QString::fromUtf8("action_ClipAOptBreakoutToMono"));
        action_ClipOptTreatAsStereo = new syAction(MainWindow);
        action_ClipOptTreatAsStereo->setObjectName(QString::fromUtf8("action_ClipOptTreatAsStereo"));
        action_ClipOptRenderAndReplace = new syAction(MainWindow);
        action_ClipOptRenderAndReplace->setObjectName(QString::fromUtf8("action_ClipOptRenderAndReplace"));
        action_ClipSpeedDuration = new syAction(MainWindow);
        action_ClipSpeedDuration->setObjectName(QString::fromUtf8("action_ClipSpeedDuration"));
        action_SequenceRender = new syAction(MainWindow);
        action_SequenceRender->setObjectName(QString::fromUtf8("action_SequenceRender"));
        action_SequenceDeleteRenderFiles = new syAction(MainWindow);
        action_SequenceDeleteRenderFiles->setObjectName(QString::fromUtf8("action_SequenceDeleteRenderFiles"));
        action_SequenceRazor = new syAction(MainWindow);
        action_SequenceRazor->setObjectName(QString::fromUtf8("action_SequenceRazor"));
        action_SequenceRazorSelectedTracks = new syAction(MainWindow);
        action_SequenceRazorSelectedTracks->setObjectName(QString::fromUtf8("action_SequenceRazorSelectedTracks"));
        action_SequenceLift = new syAction(MainWindow);
        action_SequenceLift->setObjectName(QString::fromUtf8("action_SequenceLift"));
        action_SequenceExtract = new syAction(MainWindow);
        action_SequenceExtract->setObjectName(QString::fromUtf8("action_SequenceExtract"));
        action_SequenceApplyVideoTransition = new syAction(MainWindow);
        action_SequenceApplyVideoTransition->setObjectName(QString::fromUtf8("action_SequenceApplyVideoTransition"));
        action_SequenceApplyAudioTransition = new syAction(MainWindow);
        action_SequenceApplyAudioTransition->setObjectName(QString::fromUtf8("action_SequenceApplyAudioTransition"));
        action_SequenceZoomIn = new syAction(MainWindow);
        action_SequenceZoomIn->setObjectName(QString::fromUtf8("action_SequenceZoomIn"));
        action_SequenceZoomOut = new syAction(MainWindow);
        action_SequenceZoomOut->setObjectName(QString::fromUtf8("action_SequenceZoomOut"));
        action_SequenceSnap = new syAction(MainWindow);
        action_SequenceSnap->setObjectName(QString::fromUtf8("action_SequenceSnap"));
        action_SequenceSnap->setCheckable(true);
        action_SequenceSnap->setChecked(true);
        action_SequenceAddTracks = new syAction(MainWindow);
        action_SequenceAddTracks->setObjectName(QString::fromUtf8("action_SequenceAddTracks"));
        action_SequenceDelTracks = new syAction(MainWindow);
        action_SequenceDelTracks->setObjectName(QString::fromUtf8("action_SequenceDelTracks"));
        action_SetClipMarkerIn = new syAction(MainWindow);
        action_SetClipMarkerIn->setObjectName(QString::fromUtf8("action_SetClipMarkerIn"));
        action_SetClipMarkerOut = new syAction(MainWindow);
        action_SetClipMarkerOut->setObjectName(QString::fromUtf8("action_SetClipMarkerOut"));
        action_SetClipMarkerVideoIn = new syAction(MainWindow);
        action_SetClipMarkerVideoIn->setObjectName(QString::fromUtf8("action_SetClipMarkerVideoIn"));
        action_SetClipMarkerVideoOut = new syAction(MainWindow);
        action_SetClipMarkerVideoOut->setObjectName(QString::fromUtf8("action_SetClipMarkerVideoOut"));
        action_SetClipMarkerAudioIn = new syAction(MainWindow);
        action_SetClipMarkerAudioIn->setObjectName(QString::fromUtf8("action_SetClipMarkerAudioIn"));
        action_SetClipMarkerAudioOut = new syAction(MainWindow);
        action_SetClipMarkerAudioOut->setObjectName(QString::fromUtf8("action_SetClipMarkerAudioOut"));
        action_SetClipMarkerUnnumbered = new syAction(MainWindow);
        action_SetClipMarkerUnnumbered->setObjectName(QString::fromUtf8("action_SetClipMarkerUnnumbered"));
        action_SetClipMarkerNextAvailableNumbered = new syAction(MainWindow);
        action_SetClipMarkerNextAvailableNumbered->setObjectName(QString::fromUtf8("action_SetClipMarkerNextAvailableNumbered"));
        action_SetClipMarkerOtherNumbered = new syAction(MainWindow);
        action_SetClipMarkerOtherNumbered->setObjectName(QString::fromUtf8("action_SetClipMarkerOtherNumbered"));
        action_GotoNextClipMarker = new syAction(MainWindow);
        action_GotoNextClipMarker->setObjectName(QString::fromUtf8("action_GotoNextClipMarker"));
        action_GotoPrevClipMarker = new syAction(MainWindow);
        action_GotoPrevClipMarker->setObjectName(QString::fromUtf8("action_GotoPrevClipMarker"));
        action_GotoInClipMarker = new syAction(MainWindow);
        action_GotoInClipMarker->setObjectName(QString::fromUtf8("action_GotoInClipMarker"));
        action_GotoOutClipMarker = new syAction(MainWindow);
        action_GotoOutClipMarker->setObjectName(QString::fromUtf8("action_GotoOutClipMarker"));
        action_GotoVideoInClipMarker = new syAction(MainWindow);
        action_GotoVideoInClipMarker->setObjectName(QString::fromUtf8("action_GotoVideoInClipMarker"));
        action_GotoVideoOutClipMarker = new syAction(MainWindow);
        action_GotoVideoOutClipMarker->setObjectName(QString::fromUtf8("action_GotoVideoOutClipMarker"));
        action_GotoAudioInClipMarker = new syAction(MainWindow);
        action_GotoAudioInClipMarker->setObjectName(QString::fromUtf8("action_GotoAudioInClipMarker"));
        action_GotoAudioOutClipMarker = new syAction(MainWindow);
        action_GotoAudioOutClipMarker->setObjectName(QString::fromUtf8("action_GotoAudioOutClipMarker"));
        action_GotoNumberedClipMarker = new syAction(MainWindow);
        action_GotoNumberedClipMarker->setObjectName(QString::fromUtf8("action_GotoNumberedClipMarker"));
        action_ClearCurrentClipMarker = new syAction(MainWindow);
        action_ClearCurrentClipMarker->setObjectName(QString::fromUtf8("action_ClearCurrentClipMarker"));
        action_ClearAllClipMarkers = new syAction(MainWindow);
        action_ClearAllClipMarkers->setObjectName(QString::fromUtf8("action_ClearAllClipMarkers"));
        action_ClearInOutClipMarkers = new syAction(MainWindow);
        action_ClearInOutClipMarkers->setObjectName(QString::fromUtf8("action_ClearInOutClipMarkers"));
        action_ClearInClipMarker = new syAction(MainWindow);
        action_ClearInClipMarker->setObjectName(QString::fromUtf8("action_ClearInClipMarker"));
        action_ClearOutClipMarker = new syAction(MainWindow);
        action_ClearOutClipMarker->setObjectName(QString::fromUtf8("action_ClearOutClipMarker"));
        action_ClearNumberedClipMarker = new syAction(MainWindow);
        action_ClearNumberedClipMarker->setObjectName(QString::fromUtf8("action_ClearNumberedClipMarker"));
        action_SetInSequenceMarker = new syAction(MainWindow);
        action_SetInSequenceMarker->setObjectName(QString::fromUtf8("action_SetInSequenceMarker"));
        action_SetOutSequenceMarker = new syAction(MainWindow);
        action_SetOutSequenceMarker->setObjectName(QString::fromUtf8("action_SetOutSequenceMarker"));
        action_SetInOutAroundSelSeqMarker = new syAction(MainWindow);
        action_SetInOutAroundSelSeqMarker->setObjectName(QString::fromUtf8("action_SetInOutAroundSelSeqMarker"));
        action_SetUnnumberedSequenceMarker = new syAction(MainWindow);
        action_SetUnnumberedSequenceMarker->setObjectName(QString::fromUtf8("action_SetUnnumberedSequenceMarker"));
        action_SetNextAvailNumberedSeqMarker = new syAction(MainWindow);
        action_SetNextAvailNumberedSeqMarker->setObjectName(QString::fromUtf8("action_SetNextAvailNumberedSeqMarker"));
        action_SetOtherNumberedSequenceMarker = new syAction(MainWindow);
        action_SetOtherNumberedSequenceMarker->setObjectName(QString::fromUtf8("action_SetOtherNumberedSequenceMarker"));
        action_GotoNextSequenceMarker = new syAction(MainWindow);
        action_GotoNextSequenceMarker->setObjectName(QString::fromUtf8("action_GotoNextSequenceMarker"));
        action_GotoPrevSequenceMarker = new syAction(MainWindow);
        action_GotoPrevSequenceMarker->setObjectName(QString::fromUtf8("action_GotoPrevSequenceMarker"));
        action_GotoInSequenceMarker = new syAction(MainWindow);
        action_GotoInSequenceMarker->setObjectName(QString::fromUtf8("action_GotoInSequenceMarker"));
        action_GotoOutSequenceMarker = new syAction(MainWindow);
        action_GotoOutSequenceMarker->setObjectName(QString::fromUtf8("action_GotoOutSequenceMarker"));
        action_GotoNumberedSeqMarker = new syAction(MainWindow);
        action_GotoNumberedSeqMarker->setObjectName(QString::fromUtf8("action_GotoNumberedSeqMarker"));
        action_ClearCurSequenceMarker = new syAction(MainWindow);
        action_ClearCurSequenceMarker->setObjectName(QString::fromUtf8("action_ClearCurSequenceMarker"));
        action_ClearAllSequenceMarkers = new syAction(MainWindow);
        action_ClearAllSequenceMarkers->setObjectName(QString::fromUtf8("action_ClearAllSequenceMarkers"));
        action_ClearInOutSeqMarkers = new syAction(MainWindow);
        action_ClearInOutSeqMarkers->setObjectName(QString::fromUtf8("action_ClearInOutSeqMarkers"));
        action_ClearInSeqMarker = new syAction(MainWindow);
        action_ClearInSeqMarker->setObjectName(QString::fromUtf8("action_ClearInSeqMarker"));
        action_ClearOutSeqMarker = new syAction(MainWindow);
        action_ClearOutSeqMarker->setObjectName(QString::fromUtf8("action_ClearOutSeqMarker"));
        action_ClearNumberedSeqMarker = new syAction(MainWindow);
        action_ClearNumberedSeqMarker->setObjectName(QString::fromUtf8("action_ClearNumberedSeqMarker"));
        action_EditSequenceMarker = new syAction(MainWindow);
        action_EditSequenceMarker->setObjectName(QString::fromUtf8("action_EditSequenceMarker"));
        action_WorkspaceEditing = new syAction(MainWindow);
        action_WorkspaceEditing->setObjectName(QString::fromUtf8("action_WorkspaceEditing"));
        action_WorkspaceEffects = new syAction(MainWindow);
        action_WorkspaceEffects->setObjectName(QString::fromUtf8("action_WorkspaceEffects"));
        action_WorkspaceAudio = new syAction(MainWindow);
        action_WorkspaceAudio->setObjectName(QString::fromUtf8("action_WorkspaceAudio"));
        action_WorkspaceColorCorrection = new syAction(MainWindow);
        action_WorkspaceColorCorrection->setObjectName(QString::fromUtf8("action_WorkspaceColorCorrection"));
        action_WorkspaceCustom = new syAction(MainWindow);
        action_WorkspaceCustom->setObjectName(QString::fromUtf8("action_WorkspaceCustom"));
        action_WorkspaceDefault = new syAction(MainWindow);
        action_WorkspaceDefault->setObjectName(QString::fromUtf8("action_WorkspaceDefault"));
        action_WorkspaceFactoryDefault = new syAction(MainWindow);
        action_WorkspaceFactoryDefault->setObjectName(QString::fromUtf8("action_WorkspaceFactoryDefault"));
        action_WorkspaceSaveAs = new syAction(MainWindow);
        action_WorkspaceSaveAs->setObjectName(QString::fromUtf8("action_WorkspaceSaveAs"));
        action_WorkspaceDelete = new syAction(MainWindow);
        action_WorkspaceDelete->setObjectName(QString::fromUtf8("action_WorkspaceDelete"));
        action_MenuSaveFrameLayout = new syAction(MainWindow);
        action_MenuSaveFrameLayout->setObjectName(QString::fromUtf8("action_MenuSaveFrameLayout"));
        action_WindowEffects = new syAction(MainWindow);
        action_WindowEffects->setObjectName(QString::fromUtf8("action_WindowEffects"));
        action_WindowEffectControls = new syAction(MainWindow);
        action_WindowEffectControls->setObjectName(QString::fromUtf8("action_WindowEffectControls"));
        action_WindowHistory = new syAction(MainWindow);
        action_WindowHistory->setObjectName(QString::fromUtf8("action_WindowHistory"));
        action_WindowInfo = new syAction(MainWindow);
        action_WindowInfo->setObjectName(QString::fromUtf8("action_WindowInfo"));
        action_WindowTools = new syAction(MainWindow);
        action_WindowTools->setObjectName(QString::fromUtf8("action_WindowTools"));
        action_WindowAudioMixer = new syAction(MainWindow);
        action_WindowAudioMixer->setObjectName(QString::fromUtf8("action_WindowAudioMixer"));
        action_WindowMonitor = new syAction(MainWindow);
        action_WindowMonitor->setObjectName(QString::fromUtf8("action_WindowMonitor"));
        action_WindowProject = new syAction(MainWindow);
        action_WindowProject->setObjectName(QString::fromUtf8("action_WindowProject"));
        action_PrefsGeneral = new syAction(MainWindow);
        action_PrefsGeneral->setObjectName(QString::fromUtf8("action_PrefsGeneral"));
        action_PrefsGeneral->setMenuRole(syAction::PreferencesRole);
        action_PrefsAudio = new syAction(MainWindow);
        action_PrefsAudio->setObjectName(QString::fromUtf8("action_PrefsAudio"));
        action_PrefsAudioHardware = new syAction(MainWindow);
        action_PrefsAudioHardware->setObjectName(QString::fromUtf8("action_PrefsAudioHardware"));
        action_PrefsAutoSave = new syAction(MainWindow);
        action_PrefsAutoSave->setObjectName(QString::fromUtf8("action_PrefsAutoSave"));
        action_PrefsCapture = new syAction(MainWindow);
        action_PrefsCapture->setObjectName(QString::fromUtf8("action_PrefsCapture"));
        action_PrefsDeviceControl = new syAction(MainWindow);
        action_PrefsDeviceControl->setObjectName(QString::fromUtf8("action_PrefsDeviceControl"));
        action_PrefsLabelColors = new syAction(MainWindow);
        action_PrefsLabelColors->setObjectName(QString::fromUtf8("action_PrefsLabelColors"));
        action_PrefsLabelDefaults = new syAction(MainWindow);
        action_PrefsLabelDefaults->setObjectName(QString::fromUtf8("action_PrefsLabelDefaults"));
        action_PrefsScratchDisks = new syAction(MainWindow);
        action_PrefsScratchDisks->setObjectName(QString::fromUtf8("action_PrefsScratchDisks"));
        action_PrefsStillImage = new syAction(MainWindow);
        action_PrefsStillImage->setObjectName(QString::fromUtf8("action_PrefsStillImage"));
        action_PrefsTitler = new syAction(MainWindow);
        action_PrefsTitler->setObjectName(QString::fromUtf8("action_PrefsTitler"));
        action_PrefsTrim = new syAction(MainWindow);
        action_PrefsTrim->setObjectName(QString::fromUtf8("action_PrefsTrim"));
        action_KeyboardCustomization = new syAction(MainWindow);
        action_KeyboardCustomization->setObjectName(QString::fromUtf8("action_KeyboardCustomization"));
        action_HelpContents = new syAction(MainWindow);
        action_HelpContents->setObjectName(QString::fromUtf8("action_HelpContents"));
        action_HelpSearch = new syAction(MainWindow);
        action_HelpSearch->setObjectName(QString::fromUtf8("action_HelpSearch"));
        action_HelpKeyboard = new syAction(MainWindow);
        action_HelpKeyboard->setObjectName(QString::fromUtf8("action_HelpKeyboard"));
        action_HelpWebsite = new syAction(MainWindow);
        action_HelpWebsite->setObjectName(QString::fromUtf8("action_HelpWebsite"));
        action_About = new syAction(MainWindow);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        action_About->setMenuRole(syAction::AboutRole);
        action_AboutQt = new syAction(MainWindow);
        action_AboutQt->setObjectName(QString::fromUtf8("action_AboutQt"));
        action_AboutQt->setMenuRole(syAction::AboutQtRole);
        action_ResourcesScan = new syAction(MainWindow);
        action_ResourcesScan->setObjectName(QString::fromUtf8("action_ResourcesScan"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 28));
        file_menu = new syMenu(menubar);
        file_menu->setObjectName(QString::fromUtf8("file_menu"));
        action_FileNew = new syMenu(file_menu);
        action_FileNew->setObjectName(QString::fromUtf8("action_FileNew"));
        action_FileOpenRecentProject = new syMenu(file_menu);
        action_FileOpenRecentProject->setObjectName(QString::fromUtf8("action_FileOpenRecentProject"));
        action_FileImportRecent = new syMenu(file_menu);
        action_FileImportRecent->setObjectName(QString::fromUtf8("action_FileImportRecent"));
        action_FileImportRecent->setEnabled(false);
        action_FileExport = new syMenu(file_menu);
        action_FileExport->setObjectName(QString::fromUtf8("action_FileExport"));
        action_FileGetProperties = new syMenu(file_menu);
        action_FileGetProperties->setObjectName(QString::fromUtf8("action_FileGetProperties"));
        edit_menu = new syMenu(menubar);
        edit_menu->setObjectName(QString::fromUtf8("edit_menu"));
        action_EditLabel = new syMenu(edit_menu);
        action_EditLabel->setObjectName(QString::fromUtf8("action_EditLabel"));
        project_menu = new syMenu(menubar);
        project_menu->setObjectName(QString::fromUtf8("project_menu"));
        action_ProjectSettings = new syMenu(project_menu);
        action_ProjectSettings->setObjectName(QString::fromUtf8("action_ProjectSettings"));
        clip_menu = new syMenu(menubar);
        clip_menu->setObjectName(QString::fromUtf8("clip_menu"));
        action_ClipCaptureSettings = new syMenu(clip_menu);
        action_ClipCaptureSettings->setObjectName(QString::fromUtf8("action_ClipCaptureSettings"));
        action_ClipVideoOptions = new syMenu(clip_menu);
        action_ClipVideoOptions->setObjectName(QString::fromUtf8("action_ClipVideoOptions"));
        action_ClipAudioOptions = new syMenu(clip_menu);
        action_ClipAudioOptions->setObjectName(QString::fromUtf8("action_ClipAudioOptions"));
        sequence_menu = new syMenu(menubar);
        sequence_menu->setObjectName(QString::fromUtf8("sequence_menu"));
        marker_menu = new syMenu(menubar);
        marker_menu->setObjectName(QString::fromUtf8("marker_menu"));
        action_SetClipMarkerMenu = new syMenu(marker_menu);
        action_SetClipMarkerMenu->setObjectName(QString::fromUtf8("action_SetClipMarkerMenu"));
        action_GotoClipMarkerMenu = new syMenu(marker_menu);
        action_GotoClipMarkerMenu->setObjectName(QString::fromUtf8("action_GotoClipMarkerMenu"));
        action_ClearClipMarkerMenu = new syMenu(marker_menu);
        action_ClearClipMarkerMenu->setObjectName(QString::fromUtf8("action_ClearClipMarkerMenu"));
        action_SetSequenceMarkerMenu = new syMenu(marker_menu);
        action_SetSequenceMarkerMenu->setObjectName(QString::fromUtf8("action_SetSequenceMarkerMenu"));
        action_GotoSequenceMarkerMenu = new syMenu(marker_menu);
        action_GotoSequenceMarkerMenu->setObjectName(QString::fromUtf8("action_GotoSequenceMarkerMenu"));
        action_ClearSequenceMarkerMenu = new syMenu(marker_menu);
        action_ClearSequenceMarkerMenu->setObjectName(QString::fromUtf8("action_ClearSequenceMarkerMenu"));
        window_menu = new syMenu(menubar);
        window_menu->setObjectName(QString::fromUtf8("window_menu"));
        action_WindowWorkspaceMenu = new syMenu(window_menu);
        action_WindowWorkspaceMenu->setObjectName(QString::fromUtf8("action_WindowWorkspaceMenu"));
        action_WindowTimelinesMenu = new syMenu(window_menu);
        action_WindowTimelinesMenu->setObjectName(QString::fromUtf8("action_WindowTimelinesMenu"));
        options_menu = new syMenu(menubar);
        options_menu->setObjectName(QString::fromUtf8("options_menu"));
        help_menu = new syMenu(menubar);
        help_menu->setObjectName(QString::fromUtf8("help_menu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(file_menu->menuAction());
        menubar->addAction(edit_menu->menuAction());
        menubar->addAction(project_menu->menuAction());
        menubar->addAction(clip_menu->menuAction());
        menubar->addAction(sequence_menu->menuAction());
        menubar->addAction(marker_menu->menuAction());
        menubar->addAction(window_menu->menuAction());
        menubar->addAction(options_menu->menuAction());
        menubar->addAction(help_menu->menuAction());
        file_menu->addAction(action_FileNew->menuAction());
        file_menu->addAction(action_FileOpen);
        file_menu->addAction(action_FileOpenRecentProject->menuAction());
        file_menu->addSeparator();
        file_menu->addAction(action_FileClose);
        file_menu->addAction(action_FileSave);
        file_menu->addAction(action_FileSaveAs);
        file_menu->addAction(action_FileSaveCopy);
        file_menu->addAction(action_FileRevert);
        file_menu->addSeparator();
        file_menu->addAction(action_FileCapture);
        file_menu->addAction(action_FileBatchCapture);
        file_menu->addSeparator();
        file_menu->addAction(action_FileImport);
        file_menu->addAction(action_FileImportRecent->menuAction());
        file_menu->addSeparator();
        file_menu->addAction(action_FileExport->menuAction());
        file_menu->addSeparator();
        file_menu->addAction(action_FileGetProperties->menuAction());
        file_menu->addSeparator();
        file_menu->addAction(action_FileInterpretFootage);
        file_menu->addAction(action_FileTimecode);
        file_menu->addSeparator();
        file_menu->addAction(action_Quit);
        action_FileNew->addAction(action_NewProject);
        action_FileNew->addAction(action_NewSequence);
        action_FileNew->addAction(action_NewBin);
        action_FileNew->addAction(action_NewOfflineFile);
        action_FileNew->addAction(action_NewTitle);
        action_FileNew->addSeparator();
        action_FileNew->addAction(action_NewBarsandTone);
        action_FileNew->addAction(action_NewBlackVideo);
        action_FileNew->addAction(action_NewColorMatte);
        action_FileNew->addAction(action_NewUniversalCountingLeader);
        action_FileOpenRecentProject->addAction(action_ClearRecentProjectsHistory);
        action_FileOpenRecentProject->addSeparator();
        action_FileOpenRecentProject->addAction(action_RecentProject1);
        action_FileOpenRecentProject->addAction(action_RecentProject2);
        action_FileOpenRecentProject->addAction(action_RecentProject3);
        action_FileOpenRecentProject->addAction(action_RecentProject4);
        action_FileOpenRecentProject->addAction(action_RecentProject5);
        action_FileOpenRecentProject->addAction(action_RecentProject6);
        action_FileOpenRecentProject->addAction(action_RecentProject7);
        action_FileOpenRecentProject->addAction(action_RecentProject8);
        action_FileOpenRecentProject->addAction(action_RecentProject9);
        action_FileImportRecent->addAction(action_ClearRecentImportsHistory);
        action_FileImportRecent->addSeparator();
        action_FileImportRecent->addAction(action_RecentImport1);
        action_FileImportRecent->addAction(action_RecentImport2);
        action_FileImportRecent->addAction(action_RecentImport3);
        action_FileImportRecent->addAction(action_RecentImport4);
        action_FileImportRecent->addAction(action_RecentImport5);
        action_FileImportRecent->addAction(action_RecentImport6);
        action_FileImportRecent->addAction(action_RecentImport7);
        action_FileImportRecent->addAction(action_RecentImport8);
        action_FileImportRecent->addAction(action_RecentImport9);
        action_FileExport->addAction(action_ExportMovie);
        action_FileExport->addAction(action_ExportFrame);
        action_FileExport->addAction(action_ExportAudio);
        action_FileExport->addSeparator();
        action_FileExport->addAction(action_ExportToTape);
        action_FileExport->addSeparator();
        action_FileExport->addAction(action_ExportMediaEncoder);
        action_FileExport->addAction(action_ExportToDVD);
        action_FileGetProperties->addAction(action_FileGetPropertiesFile);
        action_FileGetProperties->addAction(action_FileGetPropertiesSelection);
        edit_menu->addAction(action_EditUndo);
        edit_menu->addAction(action_EditRedo);
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditClearUndoHistory);
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditCut);
        edit_menu->addAction(action_EditCopy);
        edit_menu->addAction(action_EditPaste);
        edit_menu->addAction(action_EditPasteInsert);
        edit_menu->addAction(action_EditPasteAttributes);
        edit_menu->addAction(action_EditClear);
        edit_menu->addAction(action_EditRippleDelete);
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditDuplicate);
        edit_menu->addAction(action_EditSelectAll);
        edit_menu->addAction(action_EditDeselectAll);
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditFind);
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditLabel->menuAction());
        edit_menu->addSeparator();
        edit_menu->addAction(action_EditOriginal);
        action_EditLabel->addAction(action_EditLabelSelectLabelGroup);
        action_EditLabel->addSeparator();
        action_EditLabel->addAction(action_EditLabelBlue);
        action_EditLabel->addAction(action_EditLabelCyan);
        action_EditLabel->addAction(action_EditLabelGreen);
        action_EditLabel->addAction(action_EditLabelViolet);
        action_EditLabel->addAction(action_EditLabelPink);
        action_EditLabel->addAction(action_EditLabelGray);
        action_EditLabel->addAction(action_EditLabelRed);
        action_EditLabel->addAction(action_EditLabelOrange);
        project_menu->addAction(action_ProjectSettings->menuAction());
        project_menu->addAction(action_ProjectLinkMedia);
        project_menu->addAction(action_ProjectUnlinkMedia);
        project_menu->addSeparator();
        project_menu->addAction(action_ProjectAutomateToSequence);
        project_menu->addSeparator();
        project_menu->addAction(action_ProjectImportBatchList);
        project_menu->addAction(action_ProjectExportBatchList);
        project_menu->addSeparator();
        project_menu->addAction(action_ProjectExportAsAAF);
        action_ProjectSettings->addAction(action_ProjectSettingsGeneral);
        action_ProjectSettings->addAction(action_ProjectSettingsCapture);
        action_ProjectSettings->addAction(action_ProjectSettingsVideoRendering);
        action_ProjectSettings->addAction(action_ProjectSettingsDefaultSequence);
        clip_menu->addAction(action_ClipRename);
        clip_menu->addSeparator();
        clip_menu->addAction(action_ClipCaptureSettings->menuAction());
        clip_menu->addSeparator();
        clip_menu->addAction(action_ClipInsert);
        clip_menu->addAction(action_ClipOverlay);
        clip_menu->addSeparator();
        clip_menu->addAction(action_ClipToggleEnable);
        clip_menu->addAction(action_ClipUnlinkAV);
        clip_menu->addAction(action_ClipLinkAV);
        clip_menu->addAction(action_ClipGroup);
        clip_menu->addAction(action_ClipUngroup);
        clip_menu->addSeparator();
        clip_menu->addAction(action_ClipVideoOptions->menuAction());
        clip_menu->addAction(action_ClipAudioOptions->menuAction());
        clip_menu->addAction(action_ClipSpeedDuration);
        action_ClipCaptureSettings->addAction(action_ClipSetCaptureSettings);
        action_ClipCaptureSettings->addAction(action_ClipClearCaptureSettings);
        action_ClipVideoOptions->addAction(action_ClipVOptFrameHold);
        action_ClipVideoOptions->addAction(action_ClipVOptFieldOptions);
        action_ClipAudioOptions->addAction(action_ClipAOptGain);
        action_ClipAudioOptions->addAction(action_ClipAOptBreakoutToMono);
        action_ClipAudioOptions->addAction(action_ClipOptTreatAsStereo);
        action_ClipAudioOptions->addAction(action_ClipOptRenderAndReplace);
        sequence_menu->addAction(action_SequenceRender);
        sequence_menu->addAction(action_SequenceDeleteRenderFiles);
        sequence_menu->addSeparator();
        sequence_menu->addAction(action_SequenceRazor);
        sequence_menu->addAction(action_SequenceRazorSelectedTracks);
        sequence_menu->addAction(action_SequenceLift);
        sequence_menu->addAction(action_SequenceExtract);
        sequence_menu->addSeparator();
        sequence_menu->addAction(action_SequenceApplyVideoTransition);
        sequence_menu->addAction(action_SequenceApplyAudioTransition);
        sequence_menu->addSeparator();
        sequence_menu->addAction(action_SequenceZoomIn);
        sequence_menu->addAction(action_SequenceZoomOut);
        sequence_menu->addSeparator();
        sequence_menu->addAction(action_SequenceSnap);
        sequence_menu->addSeparator();
        sequence_menu->addAction(action_SequenceAddTracks);
        sequence_menu->addAction(action_SequenceDelTracks);
        marker_menu->addAction(action_SetClipMarkerMenu->menuAction());
        marker_menu->addAction(action_GotoClipMarkerMenu->menuAction());
        marker_menu->addAction(action_ClearClipMarkerMenu->menuAction());
        marker_menu->addSeparator();
        marker_menu->addAction(action_SetSequenceMarkerMenu->menuAction());
        marker_menu->addAction(action_GotoSequenceMarkerMenu->menuAction());
        marker_menu->addAction(action_ClearSequenceMarkerMenu->menuAction());
        marker_menu->addSeparator();
        marker_menu->addAction(action_EditSequenceMarker);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerIn);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerOut);
        action_SetClipMarkerMenu->addSeparator();
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerVideoIn);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerVideoOut);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerAudioIn);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerAudioOut);
        action_SetClipMarkerMenu->addSeparator();
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerUnnumbered);
        action_SetClipMarkerMenu->addSeparator();
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerNextAvailableNumbered);
        action_SetClipMarkerMenu->addAction(action_SetClipMarkerOtherNumbered);
        action_GotoClipMarkerMenu->addAction(action_GotoNextClipMarker);
        action_GotoClipMarkerMenu->addAction(action_GotoPrevClipMarker);
        action_GotoClipMarkerMenu->addSeparator();
        action_GotoClipMarkerMenu->addAction(action_GotoInClipMarker);
        action_GotoClipMarkerMenu->addAction(action_GotoOutClipMarker);
        action_GotoClipMarkerMenu->addSeparator();
        action_GotoClipMarkerMenu->addAction(action_GotoVideoInClipMarker);
        action_GotoClipMarkerMenu->addAction(action_GotoVideoOutClipMarker);
        action_GotoClipMarkerMenu->addAction(action_GotoAudioInClipMarker);
        action_GotoClipMarkerMenu->addAction(action_GotoAudioOutClipMarker);
        action_GotoClipMarkerMenu->addSeparator();
        action_GotoClipMarkerMenu->addAction(action_GotoNumberedClipMarker);
        action_ClearClipMarkerMenu->addAction(action_ClearCurrentClipMarker);
        action_ClearClipMarkerMenu->addAction(action_ClearAllClipMarkers);
        action_ClearClipMarkerMenu->addSeparator();
        action_ClearClipMarkerMenu->addAction(action_ClearInOutClipMarkers);
        action_ClearClipMarkerMenu->addAction(action_ClearInClipMarker);
        action_ClearClipMarkerMenu->addAction(action_ClearOutClipMarker);
        action_ClearClipMarkerMenu->addSeparator();
        action_ClearClipMarkerMenu->addAction(action_ClearNumberedClipMarker);
        action_SetSequenceMarkerMenu->addAction(action_SetInSequenceMarker);
        action_SetSequenceMarkerMenu->addAction(action_SetOutSequenceMarker);
        action_SetSequenceMarkerMenu->addAction(action_SetInOutAroundSelSeqMarker);
        action_SetSequenceMarkerMenu->addSeparator();
        action_SetSequenceMarkerMenu->addAction(action_SetUnnumberedSequenceMarker);
        action_SetSequenceMarkerMenu->addSeparator();
        action_SetSequenceMarkerMenu->addAction(action_SetNextAvailNumberedSeqMarker);
        action_SetSequenceMarkerMenu->addAction(action_SetOtherNumberedSequenceMarker);
        action_GotoSequenceMarkerMenu->addAction(action_GotoNextSequenceMarker);
        action_GotoSequenceMarkerMenu->addAction(action_GotoPrevSequenceMarker);
        action_GotoSequenceMarkerMenu->addSeparator();
        action_GotoSequenceMarkerMenu->addAction(action_GotoInSequenceMarker);
        action_GotoSequenceMarkerMenu->addAction(action_GotoOutSequenceMarker);
        action_GotoSequenceMarkerMenu->addSeparator();
        action_GotoSequenceMarkerMenu->addAction(action_GotoNumberedSeqMarker);
        action_ClearSequenceMarkerMenu->addAction(action_ClearCurSequenceMarker);
        action_ClearSequenceMarkerMenu->addAction(action_ClearAllSequenceMarkers);
        action_ClearSequenceMarkerMenu->addSeparator();
        action_ClearSequenceMarkerMenu->addAction(action_ClearInOutSeqMarkers);
        action_ClearSequenceMarkerMenu->addAction(action_ClearInSeqMarker);
        action_ClearSequenceMarkerMenu->addAction(action_ClearOutSeqMarker);
        action_ClearSequenceMarkerMenu->addSeparator();
        action_ClearSequenceMarkerMenu->addAction(action_ClearNumberedSeqMarker);
        window_menu->addAction(action_WindowWorkspaceMenu->menuAction());
        window_menu->addSeparator();
        window_menu->addAction(action_WindowEffects);
        window_menu->addAction(action_WindowEffectControls);
        window_menu->addSeparator();
        window_menu->addAction(action_WindowHistory);
        window_menu->addAction(action_WindowInfo);
        window_menu->addAction(action_WindowTools);
        window_menu->addSeparator();
        window_menu->addAction(action_WindowAudioMixer);
        window_menu->addAction(action_WindowMonitor);
        window_menu->addAction(action_WindowProject);
        window_menu->addAction(action_WindowTimelinesMenu->menuAction());
        action_WindowWorkspaceMenu->addAction(action_WorkspaceEditing);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceEffects);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceAudio);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceColorCorrection);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceDefault);
        action_WindowWorkspaceMenu->addSeparator();
        action_WindowWorkspaceMenu->addAction(action_WorkspaceFactoryDefault);
        action_WindowWorkspaceMenu->addSeparator();
        action_WindowWorkspaceMenu->addAction(action_WorkspaceSaveAs);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceDelete);
        action_WindowWorkspaceMenu->addAction(action_WorkspaceCustom);
        action_WindowWorkspaceMenu->addSeparator();
        action_WindowWorkspaceMenu->addAction(action_MenuSaveFrameLayout);
        options_menu->addAction(action_PrefsGeneral);
        options_menu->addAction(action_PrefsAudio);
        options_menu->addAction(action_PrefsAudioHardware);
        options_menu->addAction(action_PrefsAutoSave);
        options_menu->addAction(action_PrefsCapture);
        options_menu->addAction(action_PrefsDeviceControl);
        options_menu->addAction(action_PrefsLabelColors);
        options_menu->addAction(action_PrefsLabelDefaults);
        options_menu->addAction(action_PrefsScratchDisks);
        options_menu->addAction(action_PrefsStillImage);
        options_menu->addAction(action_PrefsTitler);
        options_menu->addAction(action_PrefsTrim);
        options_menu->addSeparator();
        options_menu->addAction(action_KeyboardCustomization);
        help_menu->addAction(action_HelpContents);
        help_menu->addSeparator();
        help_menu->addAction(action_HelpSearch);
        help_menu->addAction(action_HelpKeyboard);
        help_menu->addAction(action_HelpWebsite);
        help_menu->addSeparator();
        help_menu->addAction(action_About);
        help_menu->addAction(action_AboutQt);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Saya-VE ain't yet Another Video Editor", 0, QApplication::UnicodeUTF8));
        action_NewProject->setText(QApplication::translate("MainWindow", "&Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewProject->setStatusTip(QApplication::translate("MainWindow", "Creates a new project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewProject->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        action_NewSequence->setText(QApplication::translate("MainWindow", "&Sequence...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewSequence->setStatusTip(QApplication::translate("MainWindow", "Inserts a new sequence into the current project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewBin->setText(QApplication::translate("MainWindow", "&Bin", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewBin->setStatusTip(QApplication::translate("MainWindow", "Creates a new Bin", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewOfflineFile->setText(QApplication::translate("MainWindow", "&Offline File...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewOfflineFile->setStatusTip(QApplication::translate("MainWindow", "Creates a placeholder for a file currently unavailable", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewTitle->setText(QApplication::translate("MainWindow", "&Title...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewTitle->setStatusTip(QApplication::translate("MainWindow", "Creates a new title screen", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewTitle->setShortcut(QApplication::translate("MainWindow", "F9", 0, QApplication::UnicodeUTF8));
        action_NewBarsandTone->setText(QApplication::translate("MainWindow", "Bars &and Tone", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewBarsandTone->setStatusTip(QApplication::translate("MainWindow", "Creates a new Bars / Tone clip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewBlackVideo->setText(QApplication::translate("MainWindow", "Blac&k Video", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewBlackVideo->setStatusTip(QApplication::translate("MainWindow", "Creates a new clip with black background", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewColorMatte->setText(QApplication::translate("MainWindow", "&Color Matte...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewColorMatte->setStatusTip(QApplication::translate("MainWindow", "Creates a new clip of a specified color", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_NewUniversalCountingLeader->setText(QApplication::translate("MainWindow", "&Universal Counting Leader...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_NewUniversalCountingLeader->setStatusTip(QApplication::translate("MainWindow", "Creates a 5...4...3...2... clip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileOpen->setText(QApplication::translate("MainWindow", "&Open Project...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileOpen->setStatusTip(QApplication::translate("MainWindow", "Opens a project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        action_ClearRecentProjectsHistory->setText(QApplication::translate("MainWindow", "&Clear History", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearRecentProjectsHistory->setStatusTip(QApplication::translate("MainWindow", "Clears Recent Projects History", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_RecentProject1->setText(QApplication::translate("MainWindow", "&1. Recent Project 1.", 0, QApplication::UnicodeUTF8));
        action_RecentProject2->setText(QApplication::translate("MainWindow", "&2. Recent Project 2.", 0, QApplication::UnicodeUTF8));
        action_RecentProject3->setText(QApplication::translate("MainWindow", "&3. Recent Project 3.", 0, QApplication::UnicodeUTF8));
        action_RecentProject4->setText(QApplication::translate("MainWindow", "&4. Recent Project 4.", 0, QApplication::UnicodeUTF8));
        action_RecentProject5->setText(QApplication::translate("MainWindow", "&5. Recent Project 5.", 0, QApplication::UnicodeUTF8));
        action_RecentProject6->setText(QApplication::translate("MainWindow", "&6. Recent Project 6.", 0, QApplication::UnicodeUTF8));
        action_RecentProject7->setText(QApplication::translate("MainWindow", "&7. Recent Project 7.", 0, QApplication::UnicodeUTF8));
        action_RecentProject8->setText(QApplication::translate("MainWindow", "&8. Recent Project 8.", 0, QApplication::UnicodeUTF8));
        action_RecentProject9->setText(QApplication::translate("MainWindow", "&9. Recent Project 9.", 0, QApplication::UnicodeUTF8));
        action_ClearRecentImportsHistory->setText(QApplication::translate("MainWindow", "&Clear History", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearRecentImportsHistory->setStatusTip(QApplication::translate("MainWindow", "Clears Recent Imports History", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_RecentImport1->setText(QApplication::translate("MainWindow", "&1. Recent Import 1.", 0, QApplication::UnicodeUTF8));
        action_RecentImport2->setText(QApplication::translate("MainWindow", "&2. Recent Import 2.", 0, QApplication::UnicodeUTF8));
        action_RecentImport3->setText(QApplication::translate("MainWindow", "&3. Recent Import 3.", 0, QApplication::UnicodeUTF8));
        action_RecentImport4->setText(QApplication::translate("MainWindow", "&4. Recent Import 4.", 0, QApplication::UnicodeUTF8));
        action_RecentImport5->setText(QApplication::translate("MainWindow", "&5. Recent Import 5.", 0, QApplication::UnicodeUTF8));
        action_RecentImport6->setText(QApplication::translate("MainWindow", "&6. Recent Import 6.", 0, QApplication::UnicodeUTF8));
        action_RecentImport7->setText(QApplication::translate("MainWindow", "&7. Recent Import 7.", 0, QApplication::UnicodeUTF8));
        action_RecentImport8->setText(QApplication::translate("MainWindow", "&8. Recent Import 8.", 0, QApplication::UnicodeUTF8));
        action_RecentImport9->setText(QApplication::translate("MainWindow", "&9. Recent Import 9.", 0, QApplication::UnicodeUTF8));
        action_FileClose->setText(QApplication::translate("MainWindow", "&Close", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileClose->setStatusTip(QApplication::translate("MainWindow", "Closes project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        action_FileSave->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileSave->setStatusTip(QApplication::translate("MainWindow", "Saves current project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        action_FileSaveAs->setText(QApplication::translate("MainWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileSaveAs->setStatusTip(QApplication::translate("MainWindow", "Saves project under a new name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileSaveAs->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        action_FileSaveCopy->setText(QApplication::translate("MainWindow", "Sa&ve a Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileSaveCopy->setStatusTip(QApplication::translate("MainWindow", "Saves a copy of the current project (original file will be unmodified)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileSaveCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+S", 0, QApplication::UnicodeUTF8));
        action_FileRevert->setText(QApplication::translate("MainWindow", "&Revert", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileRevert->setStatusTip(QApplication::translate("MainWindow", "Reloads current project from disk", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileCapture->setText(QApplication::translate("MainWindow", "Capt&ure...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileCapture->setStatusTip(QApplication::translate("MainWindow", "Captures video from an external source", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileCapture->setShortcut(QApplication::translate("MainWindow", "F5", 0, QApplication::UnicodeUTF8));
        action_FileBatchCapture->setText(QApplication::translate("MainWindow", "&Batch Capture...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileBatchCapture->setStatusTip(QApplication::translate("MainWindow", "Captures several videos without user intervention", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileBatchCapture->setShortcut(QApplication::translate("MainWindow", "F6", 0, QApplication::UnicodeUTF8));
        action_FileImport->setText(QApplication::translate("MainWindow", "&Import...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileImport->setStatusTip(QApplication::translate("MainWindow", "Imports a clip from a file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileImport->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        action_ExportMovie->setText(QApplication::translate("MainWindow", "&Movie", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportMovie->setStatusTip(QApplication::translate("MainWindow", "Exports project as a Movie", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ExportMovie->setShortcut(QApplication::translate("MainWindow", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        action_ExportFrame->setText(QApplication::translate("MainWindow", "&Frame", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportFrame->setStatusTip(QApplication::translate("MainWindow", "Renders and Export the current frame", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ExportFrame->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+M", 0, QApplication::UnicodeUTF8));
        action_ExportAudio->setText(QApplication::translate("MainWindow", "&Audio...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportAudio->setStatusTip(QApplication::translate("MainWindow", "Exports the project's rendered audio", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ExportAudio->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+Shift+M", 0, QApplication::UnicodeUTF8));
        action_ExportToTape->setText(QApplication::translate("MainWindow", "Export to &Tape...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportToTape->setStatusTip(QApplication::translate("MainWindow", "Exports the project to Tape", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ExportMediaEncoder->setText(QApplication::translate("MainWindow", "Media &Encoder...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportMediaEncoder->setStatusTip(QApplication::translate("MainWindow", "Launches the Media Encoder Tool", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ExportToDVD->setText(QApplication::translate("MainWindow", "Export To &DVD...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ExportToDVD->setStatusTip(QApplication::translate("MainWindow", "Creates a DVD ISO image of the current project", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileGetPropertiesFile->setText(QApplication::translate("MainWindow", "&File...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileGetPropertiesFile->setStatusTip(QApplication::translate("MainWindow", "Gets a file's properties", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileGetPropertiesSelection->setText(QApplication::translate("MainWindow", "&Selection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileGetPropertiesSelection->setStatusTip(QApplication::translate("MainWindow", "Gets properties for current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileGetPropertiesSelection->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+H", 0, QApplication::UnicodeUTF8));
        action_FileInterpretFootage->setText(QApplication::translate("MainWindow", "Interpret &Footage...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileInterpretFootage->setStatusTip(QApplication::translate("MainWindow", "Change clip's framerate, aspect ratio or transparency", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_FileTimecode->setText(QApplication::translate("MainWindow", "Timeco&de...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_FileTimecode->setStatusTip(QApplication::translate("MainWindow", "Adds a timecode to an analog video clip.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_Quit->setStatusTip(QApplication::translate("MainWindow", "Exits the program", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_Quit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        action_EditUndo->setText(QApplication::translate("MainWindow", "&Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditUndo->setStatusTip(QApplication::translate("MainWindow", "Undoes last operation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        action_EditRedo->setText(QApplication::translate("MainWindow", "&Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditRedo->setStatusTip(QApplication::translate("MainWindow", "Redoes last undone operation", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        action_EditClearUndoHistory->setText(QApplication::translate("MainWindow", "Clear Undo History", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditClearUndoHistory->setStatusTip(QApplication::translate("MainWindow", "Clears the Undo History", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditCut->setText(QApplication::translate("MainWindow", "Cu&t", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditCut->setStatusTip(QApplication::translate("MainWindow", "Cuts current selection and adds it to the clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        action_EditCopy->setText(QApplication::translate("MainWindow", "&Copy", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditCopy->setStatusTip(QApplication::translate("MainWindow", "Copies current selection into the clipboard", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        action_EditPaste->setText(QApplication::translate("MainWindow", "&Paste", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditPaste->setStatusTip(QApplication::translate("MainWindow", "Pastes the clipboard's contents and replaces the current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        action_EditPasteInsert->setText(QApplication::translate("MainWindow", "Paste &Insert", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditPasteInsert->setStatusTip(QApplication::translate("MainWindow", "Pastes the clipboard's contents and inserts them at the current position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditPasteInsert->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+V", 0, QApplication::UnicodeUTF8));
        action_EditPasteAttributes->setText(QApplication::translate("MainWindow", "Paste &Attributes", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditPasteAttributes->setStatusTip(QApplication::translate("MainWindow", "Pastes the clipboard's contents' attributes into the current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditPasteAttributes->setShortcut(QApplication::translate("MainWindow", "Ctrl+Alt+V", 0, QApplication::UnicodeUTF8));
        action_EditClear->setText(QApplication::translate("MainWindow", "Cl&ear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditClear->setStatusTip(QApplication::translate("MainWindow", "Clears the current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditClear->setShortcut(QApplication::translate("MainWindow", "Del", 0, QApplication::UnicodeUTF8));
        action_EditRippleDelete->setText(QApplication::translate("MainWindow", "Ripple De&lete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditRippleDelete->setStatusTip(QApplication::translate("MainWindow", "Deletes the current selection and drags the clips after it", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditRippleDelete->setShortcut(QApplication::translate("MainWindow", "Shift+Del", 0, QApplication::UnicodeUTF8));
        action_EditDuplicate->setText(QApplication::translate("MainWindow", "D&uplicate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditDuplicate->setStatusTip(QApplication::translate("MainWindow", "Duplicates the current selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditDuplicate->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+C", 0, QApplication::UnicodeUTF8));
        action_EditSelectAll->setText(QApplication::translate("MainWindow", "&Select All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditSelectAll->setStatusTip(QApplication::translate("MainWindow", "Selects everything", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditSelectAll->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", 0, QApplication::UnicodeUTF8));
        action_EditDeselectAll->setText(QApplication::translate("MainWindow", "&Deselect All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditDeselectAll->setStatusTip(QApplication::translate("MainWindow", "Deselects everything", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditDeselectAll->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+A", 0, QApplication::UnicodeUTF8));
        action_EditFind->setText(QApplication::translate("MainWindow", "&Find...", 0, QApplication::UnicodeUTF8));
        action_EditFind->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        action_EditLabelSelectLabelGroup->setText(QApplication::translate("MainWindow", "Select Label Group", 0, QApplication::UnicodeUTF8));
        action_EditLabelBlue->setText(QApplication::translate("MainWindow", "Blue", 0, QApplication::UnicodeUTF8));
        action_EditLabelCyan->setText(QApplication::translate("MainWindow", "Cyan", 0, QApplication::UnicodeUTF8));
        action_EditLabelGreen->setText(QApplication::translate("MainWindow", "Green", 0, QApplication::UnicodeUTF8));
        action_EditLabelViolet->setText(QApplication::translate("MainWindow", "Violet", 0, QApplication::UnicodeUTF8));
        action_EditLabelPink->setText(QApplication::translate("MainWindow", "Pink", 0, QApplication::UnicodeUTF8));
        action_EditLabelGray->setText(QApplication::translate("MainWindow", "Gray", 0, QApplication::UnicodeUTF8));
        action_EditLabelRed->setText(QApplication::translate("MainWindow", "Red", 0, QApplication::UnicodeUTF8));
        action_EditLabelOrange->setText(QApplication::translate("MainWindow", "Orange", 0, QApplication::UnicodeUTF8));
        action_EditOriginal->setText(QApplication::translate("MainWindow", "Edit &Original", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditOriginal->setStatusTip(QApplication::translate("MainWindow", "Edits a clip in its original application", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditOriginal->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        action_ProjectSettingsGeneral->setText(QApplication::translate("MainWindow", "&General...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectSettingsGeneral->setStatusTip(QApplication::translate("MainWindow", "Changes Project's general settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectSettingsCapture->setText(QApplication::translate("MainWindow", "&Capture...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectSettingsCapture->setStatusTip(QApplication::translate("MainWindow", "Changes Project's video capture settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectSettingsVideoRendering->setText(QApplication::translate("MainWindow", "&Video Rendering...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectSettingsVideoRendering->setStatusTip(QApplication::translate("MainWindow", "Changes Project's rendering settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectSettingsDefaultSequence->setText(QApplication::translate("MainWindow", "&Default Sequence...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectSettingsDefaultSequence->setStatusTip(QApplication::translate("MainWindow", "Changes the default parameters for new sequences", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectLinkMedia->setText(QApplication::translate("MainWindow", "&Link Media", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectLinkMedia->setStatusTip(QApplication::translate("MainWindow", "Links an Offline File to a media file on disk", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectUnlinkMedia->setText(QApplication::translate("MainWindow", "&Unlink Media", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectUnlinkMedia->setStatusTip(QApplication::translate("MainWindow", "Unlinks the media file from the currently selected Offline File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectAutomateToSequence->setText(QApplication::translate("MainWindow", "&Automate to Sequence...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectAutomateToSequence->setStatusTip(QApplication::translate("MainWindow", "Joins the selected clips into a sequence with transitions", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectImportBatchList->setText(QApplication::translate("MainWindow", "&Import Batch List...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectImportBatchList->setStatusTip(QApplication::translate("MainWindow", "Imports Offline files from a batch-list timecode log (see Help)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectExportBatchList->setText(QApplication::translate("MainWindow", "&Export Batch List...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectExportBatchList->setStatusTip(QApplication::translate("MainWindow", "Exports currently selected files into a batch-list timecode log (see Help)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ProjectExportAsAAF->setText(QApplication::translate("MainWindow", "E&xport Project as AAF...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ProjectExportAsAAF->setStatusTip(QApplication::translate("MainWindow", "Exports the current project as an Advanced Authoring Format (AAF) file", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipRename->setText(QApplication::translate("MainWindow", "&Rename", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipRename->setStatusTip(QApplication::translate("MainWindow", "Gives the currently selected clip a new internal (not OS-based) name", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipRename->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        action_ClipSetCaptureSettings->setText(QApplication::translate("MainWindow", "&Set Capture Settings...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipSetCaptureSettings->setStatusTip(QApplication::translate("MainWindow", "Sets the capture settings for the current Offline File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipClearCaptureSettings->setText(QApplication::translate("MainWindow", "&Clear Capture Settings...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipClearCaptureSettings->setStatusTip(QApplication::translate("MainWindow", "Clears the capture settings for the current Offline File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipInsert->setText(QApplication::translate("MainWindow", "&Insert", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipInsert->setStatusTip(QApplication::translate("MainWindow", "Inserts the currently selected clip at the cursor position.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipInsert->setShortcut(QApplication::translate("MainWindow", ",", 0, QApplication::UnicodeUTF8));
        action_ClipOverlay->setText(QApplication::translate("MainWindow", "&Overlay", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipOverlay->setStatusTip(QApplication::translate("MainWindow", "Overwrites whatever is at the cursor position with the currently selected clip.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipOverlay->setShortcut(QApplication::translate("MainWindow", ".", 0, QApplication::UnicodeUTF8));
        action_ClipToggleEnable->setText(QApplication::translate("MainWindow", "&Enable", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipToggleEnable->setStatusTip(QApplication::translate("MainWindow", "Enables or disables the clip at the cursor position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipUnlinkAV->setText(QApplication::translate("MainWindow", "&Unlink Audio and Video", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipUnlinkAV->setStatusTip(QApplication::translate("MainWindow", "Separates the clip's video and audio in the timeline so you can edit one without affecting the other.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipLinkAV->setText(QApplication::translate("MainWindow", "&Link Audio and Video", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipLinkAV->setStatusTip(QApplication::translate("MainWindow", "Links a video and audio clips in the timeline so any action affects both at the same time.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipGroup->setText(QApplication::translate("MainWindow", "&Group", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipGroup->setStatusTip(QApplication::translate("MainWindow", "Groups various clips so you can move them as if they were a single clip.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipGroup->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", 0, QApplication::UnicodeUTF8));
        action_ClipUngroup->setText(QApplication::translate("MainWindow", "&Ungroup", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipUngroup->setStatusTip(QApplication::translate("MainWindow", "Ungroups clips so you can move and edit them separately.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipUngroup->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+G", 0, QApplication::UnicodeUTF8));
        action_ClipVOptFrameHold->setText(QApplication::translate("MainWindow", "&Frame Hold...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipVOptFrameHold->setStatusTip(QApplication::translate("MainWindow", "Sets the options for freezing a frame of the clip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipVOptFieldOptions->setText(QApplication::translate("MainWindow", "Field &Options...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipVOptFieldOptions->setStatusTip(QApplication::translate("MainWindow", "Sets the clip's field processing options", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipAOptGain->setText(QApplication::translate("MainWindow", "&Audio Gain...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipAOptGain->setStatusTip(QApplication::translate("MainWindow", "Sets the clip's audio gain", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipAOptBreakoutToMono->setText(QApplication::translate("MainWindow", "&Breakout to Mono Clips", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipAOptBreakoutToMono->setStatusTip(QApplication::translate("MainWindow", "Separates a clip's stereo or 5.1 surround audio tracks into multiple mono clips.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipOptTreatAsStereo->setText(QApplication::translate("MainWindow", "&Treat as Stereo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipOptTreatAsStereo->setStatusTip(QApplication::translate("MainWindow", "Treats a mono audio clip as if it was stereo", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipOptRenderAndReplace->setText(QApplication::translate("MainWindow", "&Render and Replace", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipOptRenderAndReplace->setStatusTip(QApplication::translate("MainWindow", "Applies effects / trimming to an audio clip and replaces it in the timeline. The original is kept unaltered.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipSpeedDuration->setText(QApplication::translate("MainWindow", "&Speed/Duration...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClipSpeedDuration->setStatusTip(QApplication::translate("MainWindow", "Changes the Speed / Duration of a clip", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClipSpeedDuration->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        action_SequenceRender->setText(QApplication::translate("MainWindow", "&Render Work Area", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceRender->setStatusTip(QApplication::translate("MainWindow", "Renders the work area into a temporary file and plays it", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceRender->setShortcut(QApplication::translate("MainWindow", "Enter", 0, QApplication::UnicodeUTF8));
        action_SequenceDeleteRenderFiles->setText(QApplication::translate("MainWindow", "&Delete Render Files", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceDeleteRenderFiles->setStatusTip(QApplication::translate("MainWindow", "Deletes the temporary render files", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceRazor->setText(QApplication::translate("MainWindow", "Ra&zor at Current Time Indicator ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceRazor->setStatusTip(QApplication::translate("MainWindow", "Splits all clips at the cursor position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceRazor->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", 0, QApplication::UnicodeUTF8));
        action_SequenceRazorSelectedTracks->setText(QApplication::translate("MainWindow", "Razor o&nly selected track(s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceRazorSelectedTracks->setStatusTip(QApplication::translate("MainWindow", "Splits the selected tracks at the cursor position", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceRazorSelectedTracks->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+K", 0, QApplication::UnicodeUTF8));
        action_SequenceLift->setText(QApplication::translate("MainWindow", "&Lift", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceLift->setStatusTip(QApplication::translate("MainWindow", "Removes a range of frames without affecting the other clips", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceLift->setShortcut(QApplication::translate("MainWindow", ";", 0, QApplication::UnicodeUTF8));
        action_SequenceExtract->setText(QApplication::translate("MainWindow", "&Extract ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceExtract->setStatusTip(QApplication::translate("MainWindow", "Removes a range of frames, shifting other clips in the affected tracks", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceExtract->setShortcut(QApplication::translate("MainWindow", "'", 0, QApplication::UnicodeUTF8));
        action_SequenceApplyVideoTransition->setText(QApplication::translate("MainWindow", "Apply &Video Transition", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceApplyVideoTransition->setStatusTip(QApplication::translate("MainWindow", "Applies a video transition between two adjacent clips", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceApplyVideoTransition->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        action_SequenceApplyAudioTransition->setText(QApplication::translate("MainWindow", "Apply &Audio Transition ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceApplyAudioTransition->setStatusTip(QApplication::translate("MainWindow", "Applies an audio transition between two adjacent clips", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceApplyAudioTransition->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+D", 0, QApplication::UnicodeUTF8));
        action_SequenceZoomIn->setText(QApplication::translate("MainWindow", "Zoom &In ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceZoomIn->setStatusTip(QApplication::translate("MainWindow", "Zooms in the timeline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceZoomIn->setShortcut(QApplication::translate("MainWindow", "+", 0, QApplication::UnicodeUTF8));
        action_SequenceZoomOut->setText(QApplication::translate("MainWindow", "Zoom &Out ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceZoomOut->setStatusTip(QApplication::translate("MainWindow", "Zooms out the timeline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceZoomOut->setShortcut(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
        action_SequenceSnap->setText(QApplication::translate("MainWindow", "&Snap ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceSnap->setStatusTip(QApplication::translate("MainWindow", "Turns on/off snap when moving clips", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceSnap->setShortcut(QApplication::translate("MainWindow", "S", 0, QApplication::UnicodeUTF8));
        action_SequenceAddTracks->setText(QApplication::translate("MainWindow", "Add &Tracks...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceAddTracks->setStatusTip(QApplication::translate("MainWindow", "Adds tracks to the currently edited sequence", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SequenceDelTracks->setText(QApplication::translate("MainWindow", "Delete Tra&cks...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SequenceDelTracks->setStatusTip(QApplication::translate("MainWindow", "Deletes tracks from the currently edited sequence", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerIn->setText(QApplication::translate("MainWindow", "&Video In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerIn->setStatusTip(QApplication::translate("MainWindow", "Sets the \"Video In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerOut->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerOut->setStatusTip(QApplication::translate("MainWindow", "Sets the \"out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerVideoIn->setText(QApplication::translate("MainWindow", "&Video In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerVideoIn->setStatusTip(QApplication::translate("MainWindow", "Sets the \"Video In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerVideoOut->setText(QApplication::translate("MainWindow", "Vid&eo Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerVideoOut->setStatusTip(QApplication::translate("MainWindow", "Sets the \"Video Out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerAudioIn->setText(QApplication::translate("MainWindow", "&Audio In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerAudioIn->setStatusTip(QApplication::translate("MainWindow", "Sets the \"Audio In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerAudioOut->setText(QApplication::translate("MainWindow", "Au&dio Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerAudioOut->setStatusTip(QApplication::translate("MainWindow", "Sets the \"Audio Out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerUnnumbered->setText(QApplication::translate("MainWindow", "&Unnumbered", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerUnnumbered->setStatusTip(QApplication::translate("MainWindow", "Sets an unnumbered Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerNextAvailableNumbered->setText(QApplication::translate("MainWindow", "&Next Available Numbered", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerNextAvailableNumbered->setStatusTip(QApplication::translate("MainWindow", "Sets a Clip Marker with the next available number", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetClipMarkerOtherNumbered->setText(QApplication::translate("MainWindow", "Other Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetClipMarkerOtherNumbered->setStatusTip(QApplication::translate("MainWindow", "Sets a Numbered Clip Marker with a number of your choice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNextClipMarker->setText(QApplication::translate("MainWindow", "&Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoNextClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the next Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNextClipMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Left", 0, QApplication::UnicodeUTF8));
        action_GotoPrevClipMarker->setText(QApplication::translate("MainWindow", "&Previous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoPrevClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the previous Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoPrevClipMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Right", 0, QApplication::UnicodeUTF8));
        action_GotoInClipMarker->setText(QApplication::translate("MainWindow", "&In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoInClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"in\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoOutClipMarker->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoOutClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoVideoInClipMarker->setText(QApplication::translate("MainWindow", "&Video In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoVideoInClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"Video In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoVideoOutClipMarker->setText(QApplication::translate("MainWindow", "Vid&eo Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoVideoOutClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"Video Out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoAudioInClipMarker->setText(QApplication::translate("MainWindow", "&Audio In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoAudioInClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"Audio In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoAudioOutClipMarker->setText(QApplication::translate("MainWindow", "Au&dio Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoAudioOutClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"Audio Out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNumberedClipMarker->setText(QApplication::translate("MainWindow", "Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoNumberedClipMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to a numbered clip marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNumberedClipMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+3", 0, QApplication::UnicodeUTF8));
        action_ClearCurrentClipMarker->setText(QApplication::translate("MainWindow", "&Current Marker", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearCurrentClipMarker->setStatusTip(QApplication::translate("MainWindow", "Clears current Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearCurrentClipMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+0", 0, QApplication::UnicodeUTF8));
        action_ClearAllClipMarkers->setText(QApplication::translate("MainWindow", "&All Markers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearAllClipMarkers->setStatusTip(QApplication::translate("MainWindow", "Clears all Clip Markers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearAllClipMarkers->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+0", 0, QApplication::UnicodeUTF8));
        action_ClearInOutClipMarkers->setText(QApplication::translate("MainWindow", "In a&nd Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearInOutClipMarkers->setStatusTip(QApplication::translate("MainWindow", "Clears \"In\" and \"Out\" Clip Markers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearInClipMarker->setText(QApplication::translate("MainWindow", "&In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearInClipMarker->setStatusTip(QApplication::translate("MainWindow", "Clears \"In\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearOutClipMarker->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearOutClipMarker->setStatusTip(QApplication::translate("MainWindow", "Clears \"Out\" Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearNumberedClipMarker->setText(QApplication::translate("MainWindow", "Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearNumberedClipMarker->setStatusTip(QApplication::translate("MainWindow", "Clears a numbered Clip Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetInSequenceMarker->setText(QApplication::translate("MainWindow", "&In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetInSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Sets the \"in\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetOutSequenceMarker->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetOutSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Sets the \"out\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetInOutAroundSelSeqMarker->setText(QApplication::translate("MainWindow", "In and Out Around &Selection", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetInOutAroundSelSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Sets the \"in\" and \"out\" Sequence Markers to the selection's begin and end, respectively", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetUnnumberedSequenceMarker->setText(QApplication::translate("MainWindow", "Unnum&bered", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetUnnumberedSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Sets an unnumbered Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetNextAvailNumberedSeqMarker->setText(QApplication::translate("MainWindow", "&Next Available Numbered", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetNextAvailNumberedSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Sets a Sequence Marker with the next available number", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_SetOtherNumberedSequenceMarker->setText(QApplication::translate("MainWindow", "Other Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_SetOtherNumberedSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Sets a Numbered Sequence Marker with a number of your choice", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNextSequenceMarker->setText(QApplication::translate("MainWindow", "&Next", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoNextSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the next Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNextSequenceMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Right", 0, QApplication::UnicodeUTF8));
        action_GotoPrevSequenceMarker->setText(QApplication::translate("MainWindow", "&Previous", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoPrevSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the previous Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoPrevSequenceMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+Left", 0, QApplication::UnicodeUTF8));
        action_GotoInSequenceMarker->setText(QApplication::translate("MainWindow", "&In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoInSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"in\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoOutSequenceMarker->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoOutSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to the \"out\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNumberedSeqMarker->setText(QApplication::translate("MainWindow", "Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_GotoNumberedSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Goes to a Sequence Marker with a specific number", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_GotoNumberedSeqMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+3", 0, QApplication::UnicodeUTF8));
        action_ClearCurSequenceMarker->setText(QApplication::translate("MainWindow", "&Current Marker", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearCurSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Clears current Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearCurSequenceMarker->setShortcut(QApplication::translate("MainWindow", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        action_ClearAllSequenceMarkers->setText(QApplication::translate("MainWindow", "&All Markers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearAllSequenceMarkers->setStatusTip(QApplication::translate("MainWindow", "Clears All Sequence Markers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearAllSequenceMarkers->setShortcut(QApplication::translate("MainWindow", "Alt+0", 0, QApplication::UnicodeUTF8));
        action_ClearInOutSeqMarkers->setText(QApplication::translate("MainWindow", "In a&nd Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearInOutSeqMarkers->setStatusTip(QApplication::translate("MainWindow", "Clears the \"in\" and \"out\" Sequence Markers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearInSeqMarker->setText(QApplication::translate("MainWindow", "&In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearInSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Clears the \"in\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearOutSeqMarker->setText(QApplication::translate("MainWindow", "&Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearOutSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Clears the \"out\" Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ClearNumberedSeqMarker->setText(QApplication::translate("MainWindow", "Nu&mbered...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ClearNumberedSeqMarker->setStatusTip(QApplication::translate("MainWindow", "Clears a numbered Sequence Marker", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_EditSequenceMarker->setText(QApplication::translate("MainWindow", "&Edit Sequence Marker...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_EditSequenceMarker->setStatusTip(QApplication::translate("MainWindow", "Edits the selected Sequence Marker's properties and info", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceEditing->setText(QApplication::translate("MainWindow", "&Editing", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceEditing->setStatusTip(QApplication::translate("MainWindow", "Switches to the Editing Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceEditing->setShortcut(QApplication::translate("MainWindow", "Shift+F9", 0, QApplication::UnicodeUTF8));
        action_WorkspaceEffects->setText(QApplication::translate("MainWindow", "E&ffects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceEffects->setStatusTip(QApplication::translate("MainWindow", "Switches to the Effects Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceEffects->setShortcut(QApplication::translate("MainWindow", "Shift+F10", 0, QApplication::UnicodeUTF8));
        action_WorkspaceAudio->setText(QApplication::translate("MainWindow", "&Audio", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceAudio->setStatusTip(QApplication::translate("MainWindow", "Switches to the Audio Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceAudio->setShortcut(QApplication::translate("MainWindow", "Shift+F11", 0, QApplication::UnicodeUTF8));
        action_WorkspaceColorCorrection->setText(QApplication::translate("MainWindow", "&Color Correction", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceColorCorrection->setStatusTip(QApplication::translate("MainWindow", "Switches to the Color Correction Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceColorCorrection->setShortcut(QApplication::translate("MainWindow", "Shift+F12", 0, QApplication::UnicodeUTF8));
        action_WorkspaceCustom->setText(QApplication::translate("MainWindow", "C&ustom", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceCustom->setStatusTip(QApplication::translate("MainWindow", "Switches to a previously saved Custom Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceDefault->setText(QApplication::translate("MainWindow", "Defaul&t", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceDefault->setStatusTip(QApplication::translate("MainWindow", "Switches to the default Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceFactoryDefault->setText(QApplication::translate("MainWindow", "&Factory Default", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceFactoryDefault->setStatusTip(QApplication::translate("MainWindow", "Switches to the Factory Default", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceSaveAs->setText(QApplication::translate("MainWindow", "&Save Workspace as...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceSaveAs->setStatusTip(QApplication::translate("MainWindow", "Saves a Custom workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WorkspaceDelete->setText(QApplication::translate("MainWindow", "&Delete Workspace...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WorkspaceDelete->setStatusTip(QApplication::translate("MainWindow", "Deletes a saved Custom Workspace", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_MenuSaveFrameLayout->setText(QApplication::translate("MainWindow", "Save as Defau&lt", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_MenuSaveFrameLayout->setStatusTip(QApplication::translate("MainWindow", "Saves the current Workspace as the default", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowEffects->setText(QApplication::translate("MainWindow", "&Effects", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowEffects->setStatusTip(QApplication::translate("MainWindow", "Switches to the Effects Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowEffectControls->setText(QApplication::translate("MainWindow", "Effect &Controls", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowEffectControls->setStatusTip(QApplication::translate("MainWindow", "Switches to the Effect Controls Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowHistory->setText(QApplication::translate("MainWindow", "&History", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowHistory->setStatusTip(QApplication::translate("MainWindow", "Switches to the History Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowInfo->setText(QApplication::translate("MainWindow", "&Info", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowInfo->setStatusTip(QApplication::translate("MainWindow", "Switches to the Info Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowTools->setText(QApplication::translate("MainWindow", "&Tools", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowTools->setStatusTip(QApplication::translate("MainWindow", "Switches to the Tools Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowAudioMixer->setText(QApplication::translate("MainWindow", "&Audio Mixer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowAudioMixer->setStatusTip(QApplication::translate("MainWindow", "Switches to the Audio Mixer Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowMonitor->setText(QApplication::translate("MainWindow", "&Monitor", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowMonitor->setStatusTip(QApplication::translate("MainWindow", "Switches to the Monitor Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowProject->setText(QApplication::translate("MainWindow", "&Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowProject->setStatusTip(QApplication::translate("MainWindow", "Switches to the Project Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_PrefsGeneral->setText(QApplication::translate("MainWindow", "&General...", 0, QApplication::UnicodeUTF8));
        action_PrefsAudio->setText(QApplication::translate("MainWindow", "&Audio...", 0, QApplication::UnicodeUTF8));
        action_PrefsAudioHardware->setText(QApplication::translate("MainWindow", "Audio &Hardware...", 0, QApplication::UnicodeUTF8));
        action_PrefsAutoSave->setText(QApplication::translate("MainWindow", "A&uto Save...", 0, QApplication::UnicodeUTF8));
        action_PrefsCapture->setText(QApplication::translate("MainWindow", "&Capture...", 0, QApplication::UnicodeUTF8));
        action_PrefsDeviceControl->setText(QApplication::translate("MainWindow", "&Device Control...", 0, QApplication::UnicodeUTF8));
        action_PrefsLabelColors->setText(QApplication::translate("MainWindow", "&Label Colors...", 0, QApplication::UnicodeUTF8));
        action_PrefsLabelDefaults->setText(QApplication::translate("MainWindow", "Label De&faults...", 0, QApplication::UnicodeUTF8));
        action_PrefsScratchDisks->setText(QApplication::translate("MainWindow", "&Scratch Disks...", 0, QApplication::UnicodeUTF8));
        action_PrefsStillImage->setText(QApplication::translate("MainWindow", "Still &Image...", 0, QApplication::UnicodeUTF8));
        action_PrefsTitler->setText(QApplication::translate("MainWindow", "&Titler...", 0, QApplication::UnicodeUTF8));
        action_PrefsTrim->setText(QApplication::translate("MainWindow", "T&rim...", 0, QApplication::UnicodeUTF8));
        action_KeyboardCustomization->setText(QApplication::translate("MainWindow", "&Keyboard Customization...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_KeyboardCustomization->setStatusTip(QApplication::translate("MainWindow", "Customizes the keyboard shortcuts", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_HelpContents->setText(QApplication::translate("MainWindow", "&Contents", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_HelpContents->setStatusTip(QApplication::translate("MainWindow", "Help Contents", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_HelpContents->setShortcut(QApplication::translate("MainWindow", "F1", 0, QApplication::UnicodeUTF8));
        action_HelpSearch->setText(QApplication::translate("MainWindow", "&Search...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_HelpSearch->setStatusTip(QApplication::translate("MainWindow", "Search for a help topic", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_HelpKeyboard->setText(QApplication::translate("MainWindow", "&Keyboard...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_HelpKeyboard->setStatusTip(QApplication::translate("MainWindow", "Shows Keyboard Shortcuts", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_HelpWebsite->setText(QApplication::translate("MainWindow", "Visit &Website", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_HelpWebsite->setStatusTip(QApplication::translate("MainWindow", "Visit our website and forums", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_About->setText(QApplication::translate("MainWindow", "&About Saya...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_About->setStatusTip(QApplication::translate("MainWindow", "Launches the famous \"about\" dialog", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_AboutQt->setText(QApplication::translate("MainWindow", "&About Qt...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_AboutQt->setStatusTip(QApplication::translate("MainWindow", "Shows information of the Qt library being used", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_ResourcesScan->setText(QApplication::translate("MainWindow", "&Re-scan project directory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_ResourcesScan->setStatusTip(QApplication::translate("MainWindow", "Scans the project directory for resources", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        file_menu->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
        action_FileNew->setTitle(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
        action_FileOpenRecentProject->setTitle(QApplication::translate("MainWindow", "Open recent &project", 0, QApplication::UnicodeUTF8));
        action_FileImportRecent->setTitle(QApplication::translate("MainWindow", "I&mport Recent File", 0, QApplication::UnicodeUTF8));
        action_FileExport->setTitle(QApplication::translate("MainWindow", "&Export", 0, QApplication::UnicodeUTF8));
        action_FileGetProperties->setTitle(QApplication::translate("MainWindow", "&Get Properties for", 0, QApplication::UnicodeUTF8));
        edit_menu->setTitle(QApplication::translate("MainWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        action_EditLabel->setTitle(QApplication::translate("MainWindow", "Label", 0, QApplication::UnicodeUTF8));
        project_menu->setTitle(QApplication::translate("MainWindow", "&Project", 0, QApplication::UnicodeUTF8));
        action_ProjectSettings->setTitle(QApplication::translate("MainWindow", "Project &Settings", 0, QApplication::UnicodeUTF8));
        clip_menu->setTitle(QApplication::translate("MainWindow", "&Clip", 0, QApplication::UnicodeUTF8));
        action_ClipCaptureSettings->setTitle(QApplication::translate("MainWindow", "&Capture Settings", 0, QApplication::UnicodeUTF8));
        action_ClipVideoOptions->setTitle(QApplication::translate("MainWindow", "&Video Options", 0, QApplication::UnicodeUTF8));
        action_ClipAudioOptions->setTitle(QApplication::translate("MainWindow", "&Audio Options", 0, QApplication::UnicodeUTF8));
        sequence_menu->setTitle(QApplication::translate("MainWindow", "&Sequence", 0, QApplication::UnicodeUTF8));
        marker_menu->setTitle(QApplication::translate("MainWindow", "&Marker", 0, QApplication::UnicodeUTF8));
        action_SetClipMarkerMenu->setTitle(QApplication::translate("MainWindow", "&Set Clip Marker", 0, QApplication::UnicodeUTF8));
        action_GotoClipMarkerMenu->setTitle(QApplication::translate("MainWindow", "&Go to Clip Marker", 0, QApplication::UnicodeUTF8));
        action_ClearClipMarkerMenu->setTitle(QApplication::translate("MainWindow", "&Clear Clip Marker", 0, QApplication::UnicodeUTF8));
        action_SetSequenceMarkerMenu->setTitle(QApplication::translate("MainWindow", "Se&t Sequence Marker", 0, QApplication::UnicodeUTF8));
        action_GotoSequenceMarkerMenu->setTitle(QApplication::translate("MainWindow", "G&o to Sequence Marker", 0, QApplication::UnicodeUTF8));
        action_ClearSequenceMarkerMenu->setTitle(QApplication::translate("MainWindow", "Cle&ar Sequence Marker", 0, QApplication::UnicodeUTF8));
        window_menu->setTitle(QApplication::translate("MainWindow", "&Window", 0, QApplication::UnicodeUTF8));
        action_WindowWorkspaceMenu->setTitle(QApplication::translate("MainWindow", "&Workspace", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        action_WindowTimelinesMenu->setStatusTip(QApplication::translate("MainWindow", "Timelines Menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        action_WindowTimelinesMenu->setTitle(QApplication::translate("MainWindow", "Time&lines", 0, QApplication::UnicodeUTF8));
        options_menu->setTitle(QApplication::translate("MainWindow", "&Options", 0, QApplication::UnicodeUTF8));
        help_menu->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWTI6980_H
