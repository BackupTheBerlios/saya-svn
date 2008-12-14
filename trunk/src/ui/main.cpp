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


#ifndef WX_PRECOMP
    #include <wx/app.h>
    #include <wx/xrc/xmlres.h>
    #include <wx/filedlg.h>
    #include <wx/aui/aui.h>
    #include <wx/splitter.h>
    #include <wx/treectrl.h>
    #include <wx/sizer.h>
    #include <wx/menu.h>
    #include <wx/msgdlg.h>
    #include <wx/aui/aui.h>
#endif

#include "../saya/core/systring.h"
#include "../saya/core/intl.h"
#include "../saya/core/iocommon.h"
#include "../saya/core/config.h"
#include "../saya/core/debuglog.h"
#include "../saya/vidproject.h"
#include "../saya/recentfileslist.h"
#include "../saya/projectmanager.h"

#include "s2wx.h"
#include "main.h"
#include "welcomedlg.h"
#include "newprojectdlg.h"
#include "app.h"

#include "wxplaybackcontrolpanel.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

int idProjectStatusChanged = XRCID("idProjectStatusChanged");

syString wxbuildinfo(wxbuildinfoformat format)
{
    syString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << "-Windows";
#elif defined(__WXMAC__)
        wxbuild << "-Mac";
#elif defined(__UNIX__)
        wxbuild << "-Linux";
#endif

#if wxUSE_UNICODE
        wxbuild << "-Unicode build";
#else
        wxbuild << "-ANSI build";
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

int main_RegisterId(int id)
{
    wxRegisterId(id);
    return id;
}

wxFrame* CreateMainFrame() {
    AppFrame* frame = new AppFrame(NULL, _w("Saya - Unsheathe your Creativity"));
    wxTheApp->SetTopWindow(frame);
    ProjectManager::Get()->SetEventHandler(frame);
    return frame;
}



const syString CFG_LAYOUTS = "Layouts";
const syString CFG_LAYOUT_DEFAULT = CFG_LAYOUTS + "/Default";
const syString CFG_LOCATION = CFG_LAYOUT_DEFAULT + "/Location";
const syString CFG_PERSPECTIVE_DEFAULT = CFG_LAYOUT_DEFAULT + "/Perspective";
const syString CFG_DEFAULT_PRJ_SASHPOS = CFG_LAYOUT_DEFAULT + "/PrjSashPos";

int idFileNew = XRCID("idFileNew");
int idNewProject = XRCID("idNewProject");
int idNewSequence = XRCID("idNewSequence");
int idNewBin = XRCID("idNewBin");
int idNewOfflineFile = XRCID("idNewOfflineFile");
int idNewTitle = XRCID("idNewTitle");
int idNewBarsandTone = XRCID("idNewBarsandTone");
int idNewBlackVideo = XRCID("idNewBlackVideo");
int idNewColorMatte = XRCID("idNewColorMatte");
int idNewUniversalCountingLeader = XRCID("idNewUniversalCountingLeader");

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

int idProjectSettings = XRCID("idProjectSettings");
int idProjectSettingsGeneral = XRCID("idProjectSettingsGeneral");
int idProjectSettingsCapture = XRCID("idProjectSettingsCapture");
int idProjectSettingsVideoRendering = XRCID("idProjectSettingsVideoRendering");
int idProjectSettingsDefaultSequence = XRCID("idProjectSettingsDefaultSequence");
int idProjectLinkMedia = XRCID("idProjectLinkMedia");
int idProjectUnlinkMedia = XRCID("idProjectUnlinkMedia");
int idProjectAutomateToSequence = XRCID("idProjectAutomateToSequence");
int idProjectImportBatchList = XRCID("idProjectImportBatchList");
int idProjectExportBatchList = XRCID("idProjectExportBatchList");
int idProjectExportAsAAF = XRCID("idProjectExportAsAAF");

int idClipRename = XRCID("idClipRename");
int idClipCaptureSettings = XRCID("idClipCaptureSettings");
int idClipSetCaptureSettings = XRCID("idClipSetCaptureSettings");
int idClipClearCaptureSettings = XRCID("idClipClearCaptureSettings");
int idClipInsert = XRCID("idClipInsert");
int idClipOverlay = XRCID("idClipOverlay");
int idClipToggleEnable = XRCID("idClipToggleEnable");
int idClipUnlinkAV = XRCID("idClipUnlinkAV");
int idClipLinkAV = XRCID("idClipLinkAV");
int idClipGroup = XRCID("idClipGroup");
int idClipUngroup = XRCID("idClipUngroup");
int idClipVideoOptions = XRCID("idClipVideoOptions");
int idClipVOptFrameHold = XRCID("idClipVOptFrameHold");
int idClipVOptFieldOptions = XRCID("idClipVOptFieldOptions");
int idClipAudioOptions = XRCID("idClipAudioOptions");
int idClipAOptGain = XRCID("idClipAOptGain");
int idClipAOptBreakoutToMono = XRCID("idClipAOptBreakoutToMono");
int idClipOptTreatAsStereo = XRCID("idClipOptTreatAsStereo");
int idClipOptRenderAndReplace = XRCID("idClipOptRenderAndReplace");
int idClipSpeedDuration = XRCID("idClipSpeedDuration");

int idSequenceRender = XRCID("idSequenceRender");
int idSequenceDeleteRenderFiles = XRCID("idSequenceDeleteRenderFiles");
int idSequenceRazor = XRCID("idSequenceRazor");
int idSequenceRazorSelectedTracks = XRCID("idSequenceRazorSelectedTracks");
int idSequenceLift = XRCID("idSequenceLift");
int idSequenceExtract = XRCID("idSequenceExtract");
int idSequenceApplyVideoTransition = XRCID("idSequenceApplyVideoTransition");
int idSequenceApplyAudioTransition = XRCID("idSequenceApplyAudioTransition");
int idSequenceZoomIn = XRCID("idSequenceZoomIn");
int idSequenceZoomOut = XRCID("idSequenceZoomOut");
int idSequenceSnap = XRCID("idSequenceSnap");
int idSequenceAddTracks = XRCID("idSequenceAddTracks");
int idSequenceDelTracks = XRCID("idSequenceDelTracks");

int idSetClipMarkerMenu = XRCID("idSetClipMarkerMenu");
int idGotoClipMarkerMenu = XRCID("idGotoClipMarkerMenu");
int idGotoNextClipMarker = XRCID("idGotoNextClipMarker");
int idGotoPrevClipMarker = XRCID("idGotoPrevClipMarker");
int idGotoInClipMarker = XRCID("idGotoInClipMarker");
int idGotoOutClipMarker = XRCID("idGotoOutClipMarker");
int idGotoVideoInClipMarker = XRCID("idGotoVideoInClipMarker");
int idGotoVideoOutClipMarker = XRCID("idGotoVideoOutClipMarker");
int idGotoAudioInClipMarker = XRCID("idGotoAudioInClipMarker");
int idGotoAudioOutClipMarker = XRCID("idGotoAudioOutClipMarker");
int idGotoNumberedClipMarker = XRCID("idGotoNumberedClipMarker");
int idClearClipMarkerMenu = XRCID("idClearClipMarkerMenu");
int idClearCurrentClipMarker = XRCID("idClearCurrentClipMarker");
int idClearAllClipMarkers = XRCID("idClearAllClipMarkers");
int idClearInOutClipMarkers = XRCID("idClearInOutClipMarkers");
int idClearInClipMarker = XRCID("idClearInClipMarker");
int idClearOutClipMarker = XRCID("idClearOutClipMarker");
int idClearNumberedClipMarker = XRCID("idClearNumberedClipMarker");
int idSetSequenceMarkerMenu = XRCID("idSetSequenceMarkerMenu");
int idSetInSequenceMarker = XRCID("idSetInSequenceMarker");
int idSetOutSequenceMarker = XRCID("idSetOutSequenceMarker");
int idSetInOutAroundSelSeqMarker = XRCID("idSetInOutAroundSelSeqMarker");
int idSetUnnumberedSequenceMarker = XRCID("idSetUnnumberedSequenceMarker");
int idSetNextAvailNumberedSeqMarker = XRCID("idSetNextAvailNumberedSeqMarker");
int idSetOtherNumberedSequenceMarker = XRCID("idSetOtherNumberedSequenceMarker");
int idGotoSequenceMarkerMenu = XRCID("idGotoSequenceMarkerMenu");
int idGotoNextSequenceMarker = XRCID("idGotoNextSequenceMarker");
int idGotoPrevSequenceMarker = XRCID("idGotoPrevSequenceMarker");
int idGotoInSequenceMarker = XRCID("idGotoInSequenceMarker");
int idGotoOutSequenceMarker = XRCID("idGotoOutSequenceMarker");
int idGotoNumberedSeqMarker = XRCID("idGotoNumberedSeqMarker");
int idClearSequenceMarkerMenu = XRCID("idClearSequenceMarkerMenu");
int idClearCurSequenceMarker = XRCID("idClearCurSequenceMarker");
int idClearAllSequenceMarkers = XRCID("idClearAllSequenceMarkers");
int idClearInOutSeqMarkers = XRCID("idClearInOutSeqMarkers");
int idClearInSeqMarker = XRCID("idClearInSeqMarker");
int idClearOutSeqMarker = XRCID("idClearOutSeqMarker");
int idClearNumberedSeqMarker = XRCID("idClearNumberedSeqMarker");
int idEditSequenceMarker = XRCID("idEditSequenceMarker");

int idWindowWorkspaceMenu = XRCID("idWindowWorkspaceMenu");
int idWorkspaceEditing = XRCID("idWorkspaceEditing");
int idWorkspaceEffects = XRCID("idWorkspaceEffects");
int idWorkspaceAudio = XRCID("idWorkspaceAudio");
int idWorkspaceColorCorrection = XRCID("idWorkspaceColorCorrection");
int idWorkspaceDefault = XRCID("idWorkspaceDefault");
int idWorkspaceFactoryDefault = XRCID("idWorkspaceFactoryDefault");
int idWorkspaceSaveAs = XRCID("idWorkspaceSaveAs");
int idWorkspaceDelete = XRCID("idWorkspaceDelete");
int idWorkspaceCustom = XRCID("idWorkspaceCustom");
int idMenuSaveFrameLayout = XRCID("idMenuSaveFrameLayout");
int idWindowEffects = XRCID("idWindowEffects");
int idWindowEffectControls = XRCID("idWindowEffectControls");
int idWindowHistory = XRCID("idWindowHistory");
int idWindowInfo = XRCID("idWindowInfo");
int idWindowTools = XRCID("idWindowTools");
int idWindowAudioMixer = XRCID("idWindowAudioMixer");
int idWindowMonitor = XRCID("idWindowMonitor");
int idWindowProject = XRCID("idWindowProject");
int idWindowTimelinesMenu = XRCID("idWindowTimelinesMenu");

int idProjectPane = XRCID("idProjectPane");
int idPrjSplitter = XRCID("idPrjSplitter");
int idPrjResourcesTree = XRCID("idPrjResourcesTree");
syString g_statustext;

BEGIN_EVENT_TABLE(AppFrame, wxFrame)
    EVT_CLOSE(AppFrame::OnClose)
    EVT_MENU(wxID_ABOUT, AppFrame::OnAbout)
    EVT_MENU(idMenuSaveFrameLayout, AppFrame::OnSaveFrameLayout)
    EVT_MENU(idWorkspaceDefault, AppFrame::OnLoadDefaultLayout)

    EVT_TREE_ITEM_MENU(idPrjResourcesTree, AppFrame::OnResourceTreeContextMenu)

//  File menu
    EVT_MENU(idNewProject, AppFrame::OnNewProject)
    EVT_MENU(idFileOpen, AppFrame::OnFileOpen)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, AppFrame::OnOpenRecentFile)
    EVT_MENU(idFileClearRecentProjectList, AppFrame::OnClearRecentProjectList)
    EVT_MENU(idFileClose, AppFrame::OnFileClose)
    EVT_MENU(idFileSave, AppFrame::OnFileSave)
    EVT_MENU(idFileSaveAs, AppFrame::OnFileSaveAs)
    EVT_MENU(idFileSaveCopy, AppFrame::OnFileSaveCopy)
    EVT_MENU(wxID_EXIT, AppFrame::OnQuit)

    EVT_MENU(idWorkspaceFactoryDefault, AppFrame::OnWorkspaceFactoryDefault)

    EVT_UPDATE_UI(idFileNew, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewSequence, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewBin, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewOfflineFile, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewTitle, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewBarsandTone, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewBlackVideo, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewColorMatte, AppFrame::OnFileMenuUpdateUI)
    EVT_UPDATE_UI(idNewUniversalCountingLeader, AppFrame::OnFileMenuUpdateUI)
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

    EVT_UPDATE_UI(idProjectSettings, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSettingsGeneral, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSettingsCapture, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSettingsVideoRendering, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectSettingsDefaultSequence, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectLinkMedia, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectUnlinkMedia, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectAutomateToSequence, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectImportBatchList, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectExportBatchList, AppFrame::OnProjectMenuUpdateUI)
    EVT_UPDATE_UI(idProjectExportAsAAF, AppFrame::OnProjectMenuUpdateUI)

    EVT_UPDATE_UI(idClipRename, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipCaptureSettings, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipSetCaptureSettings, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipClearCaptureSettings, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipInsert, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipOverlay, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipToggleEnable, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipUnlinkAV, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipLinkAV, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipGroup, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipUngroup, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipVideoOptions, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipVOptFrameHold, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipVOptFieldOptions, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipAudioOptions, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipAOptGain, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipAOptBreakoutToMono, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipOptTreatAsStereo, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipOptRenderAndReplace, AppFrame::OnClipMenuUpdateUI)
    EVT_UPDATE_UI(idClipSpeedDuration, AppFrame::OnClipMenuUpdateUI)

    EVT_UPDATE_UI(idSequenceRender, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceDeleteRenderFiles, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceRazor, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceRazorSelectedTracks, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceLift, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceExtract, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceApplyVideoTransition, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceApplyAudioTransition, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceZoomIn, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceZoomOut, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceSnap, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceAddTracks, AppFrame::OnSequenceMenuUpdateUI)
    EVT_UPDATE_UI(idSequenceDelTracks, AppFrame::OnSequenceMenuUpdateUI)

    EVT_UPDATE_UI(idSetClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoNextClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoPrevClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoVideoInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoVideoOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoAudioInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoAudioOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoNumberedClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearClipMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearCurrentClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearAllClipMarkers, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearInOutClipMarkers, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearInClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearOutClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearNumberedClipMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetInSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetOutSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetInOutAroundSelSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetUnnumberedSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetNextAvailNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idSetOtherNumberedSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoNextSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoPrevSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoInSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoOutSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idGotoNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearSequenceMarkerMenu, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearCurSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearAllSequenceMarkers, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearInOutSeqMarkers, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearInSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearOutSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idClearNumberedSeqMarker, AppFrame::OnMarkerMenuUpdateUI)
    EVT_UPDATE_UI(idEditSequenceMarker, AppFrame::OnMarkerMenuUpdateUI)

    EVT_UPDATE_UI(idWindowWorkspaceMenu, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceEditing, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceEffects, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceAudio, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceColorCorrection, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceDefault, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceFactoryDefault, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceSaveAs, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceDelete, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWorkspaceCustom, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idMenuSaveFrameLayout, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowEffects, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowEffectControls, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowHistory, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowInfo, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowTools, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowAudioMixer, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowMonitor, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowProject, AppFrame::OnWindowMenuUpdateUI)
    EVT_UPDATE_UI(idWindowTimelinesMenu, AppFrame::OnWindowMenuUpdateUI)

    EVT_UPDATE_UI(idProjectStatusChanged, AppFrame::OnProjectStatusChanged)

//  Project pane events

    EVT_UPDATE_UI(idProjectPane, AppFrame:: OnUpdateProjectPaneUI)


END_EVENT_TABLE()



AppFrame::AppFrame(wxFrame *frame, const wxString& title) :
wxFrame(frame, -1, title),
m_welcomedialog(NULL),
m_hadproject(false),
m_panes_status_checked(false),
m_layouthidden(false),
m_recentfilesmodcounter(0),
m_recentimportsmodcounter(0)
{
    m_mgr = new wxAuiManager;
    bool result = false;
    do {
        if(!CreateMenuBar()) break;
        if(!CreateDialogs()) break;
        CreateStatusBar(2);

        m_mgr->SetManagedWindow(this);
        m_prjMan = ProjectManager::Get();
        LoadAndSetFrameSize();

        if(!CreatePanels()) break;
        CreateDockAreas();

        // Update Status bar
        SetStatusText(s2wx(wxbuildinfo(short_f)), 1);
        g_statustext << _("Welcome to ") << APP_SHOWNAME << "! ^_^";
        UpdateStatustext();



        m_FactoryDefaultLayout = m_mgr->SavePerspective();
        LoadDefaultLayout(true);

        result = true;
    }while(false);
    if(!result) {
        Destroy();
    }
}

bool AppFrame::CreateMenuBar() {
    bool result = false;
    wxMenuBar* mbar = wxXmlResource::Get()->LoadMenuBar(wxT("main_menu_bar"));
    if(mbar) {
        SetMenuBar(mbar);
        result = true;
    } else {
        LoadFail("main_menu_bar");
    }
    return result;
}

bool AppFrame::CreateDialogs() {
    bool result = false;
    do {
        m_welcomedialog = new WelcomeDialog(this);
        if(!m_welcomedialog) {
            LoadFail("welcome_dialog");
            break;
        }
        result = true;
    } while(false);
    return result;
}

bool AppFrame::CreatePanels() {
    bool result = false;
    do {
        m_projectpanel = CreateProjectPane(); // wxXmlResource::Get()->LoadPanel(this,wxT("project_panel"));
        if(!m_projectpanel) { LoadFail("project_panel"); break; }
           m_monitorpanel = new wxVideoPlaybackPanel(this);
           m_effectspanel = new wxVideoPlaybackPanel(this);

        m_timelinepanel = new wxPanel(this, -1,wxDefaultPosition, wxSize(800,400));
        result = true;
    }while(false);
    return result;
}

void AppFrame::CreateDockAreas() {
    m_mgr->SetFlags(m_mgr->GetFlags() | wxAUI_MGR_ALLOW_ACTIVE_PANE);
     // add the panes to the manager
//   m_mgr->SetDockSizeConstraint(0.3,0.45);
    m_mgr->AddPane(m_projectpanel, wxAuiPaneInfo().
                            Name(wxT("Project")).Caption(_w("Project")).
                              BestSize(wxSize(200, 300)).MaximizeButton().MinimizeButton().PinButton().
                              Left().Layer(1));
    m_mgr->AddPane(m_monitorpanel, wxAuiPaneInfo().
                            Name(wxT("Monitor")).Caption(_w("Monitor / Preview")).
                              BestSize(wxSize(250, 300)).MaximizeButton().MinimizeButton().PinButton().
                              Bottom().Layer(1));
    m_mgr->AddPane(m_effectspanel, wxAuiPaneInfo().
                            Name(wxT("Effects")).Caption(_w("Effects Monitor")).
                              BestSize(wxSize(250, 300)).MaximizeButton().MinimizeButton().PinButton().
                              Bottom().Layer(1));
    m_mgr->AddPane(m_timelinepanel, wxAuiPaneInfo().Name(wxT("MainPane")).CentrePane().MinSize(wxSize(500,200)).MaximizeButton().Caption(_w("Timeline")).CaptionVisible(true));
}

bool AppFrame::LoadDefaultLayout(bool firsttime) {
    bool result = false;
    syString strlayout = syApp::GetConfig()->Read(CFG_PERSPECTIVE_DEFAULT.c_str(), "");
    if(!strlayout.empty()) {
        result = m_mgr->LoadPerspective(s2wx(strlayout),false);
    }
    if(firsttime)
    {
        wxUpdateUIEvent tmpevent(idProjectStatusChanged);
        wxPostEvent(this,tmpevent);
//        OnProjectStatusChanged(tmpevent);
    }
    m_mgr->Update();
    return result;
}

void AppFrame::OnLoadDefaultLayout(wxCommandEvent& event) {
    LoadDefaultLayout();
}

void AppFrame::OnWorkspaceFactoryDefault(wxCommandEvent& event) {
    bool result = false;
    if(!m_FactoryDefaultLayout.IsEmpty()) {
        result = m_mgr->LoadPerspective(m_FactoryDefaultLayout,false);
    }
    m_mgr->Update();
}

wxPanel* AppFrame::CreateProjectPane() {

    long tabs_style = wxAUI_NB_DEFAULT_STYLE && ~ (wxAUI_NB_CLOSE_BUTTON | wxAUI_NB_CLOSE_ON_ACTIVE_TAB | wxAUI_NB_CLOSE_ON_ALL_TABS);
    wxPanel* panel1;
    wxAuiNotebook* auinotebook1;
    wxPanel* resourcespage;
    wxSplitterWindow* splitter1;
    wxPanel* dir_panel;
    wxPanel* files_panel;
    wxScrolledWindow* scrolledWindow2;
    wxPanel* effectspage;
    m_ResourcesTree = NULL;

    panel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize(200,800), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	auinotebook1 = new wxAuiNotebook( panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, tabs_style );
	resourcespage = new wxPanel( auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3 = new wxBoxSizer( wxVERTICAL );

	splitter1 = new wxSplitterWindow( resourcespage, idPrjSplitter, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	splitter1->SetMinimumPaneSize(10); // No unsplitting!
	dir_panel = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL );

	wxBoxSizer* bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_ResourcesTree = new wxTreeCtrl( dir_panel, idPrjResourcesTree, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );

	wxTreeItemId daroot = m_ResourcesTree->AddRoot(_w("Resources"), -1, -1, NULL);
	m_ResourcesTree->AppendItem(daroot, _w("Sequences"),-1,-1,NULL);
	m_ResourcesTree->AppendItem(daroot, _w("Videos"),-1,-1,NULL);
	m_ResourcesTree->AppendItem(daroot, _w("Images"),-1,-1,NULL);
	m_ResourcesTree->AppendItem(daroot, _w("Sound"),-1,-1,NULL);
	m_ResourcesTree->AppendItem(daroot, _w("Other"),-1,-1,NULL);

	bSizer4->Add( m_ResourcesTree, 1, wxEXPAND, 5 );

	dir_panel->SetSizer( bSizer4 );
	dir_panel->Layout();
	bSizer4->Fit( dir_panel );
	files_panel = new wxPanel( splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	scrolledWindow2 = new wxScrolledWindow( files_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	scrolledWindow2->SetScrollRate( 5, 5 );
	bSizer5->Add( scrolledWindow2, 1, wxEXPAND | wxALL, 5 );

	files_panel->SetSizer( bSizer5 );
	files_panel->Layout();
	bSizer5->Fit( files_panel );
	splitter1->SplitHorizontally( dir_panel, files_panel, 170 );
	bSizer3->Add( splitter1, 1, wxEXPAND, 5 );

	resourcespage->SetSizer( bSizer3 );
	resourcespage->Layout();
	bSizer3->Fit( resourcespage );
	auinotebook1->AddPage( resourcespage, wxT("Resources"), false, wxNullBitmap );
	effectspage = new wxPanel( auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	auinotebook1->AddPage( effectspage, wxT("Effects"), true, wxNullBitmap );
    auinotebook1->SetSelection(0); // Go to the first page

	bSizer2->Add( auinotebook1, 1, wxEXPAND | wxALL, 5 );

	panel1->SetSizer( bSizer2 );
	panel1->Layout();
	bSizer2->Fit( panel1 );
	{
        long curheight = GetRect().GetHeight();
        long defaultsashpos =  curheight / 2;
        long sashpos = defaultsashpos;
        sashpos = syApp::GetConfig()->ReadUint(CFG_DEFAULT_PRJ_SASHPOS, defaultsashpos);
        if(sashpos==0) sashpos = defaultsashpos;
        sashpos = std::min(curheight,std::max((long)20,sashpos));
        splitter1->SetSashPosition(sashpos);
	}

	return panel1;
}

long AppFrame::GetProjectPanelSashPos() {
    wxSplitterWindow* splitter = (wxSplitterWindow*)FindWindow(idPrjSplitter);
    if(splitter) {
        return splitter->GetSashPosition();
    } else {
        return 0;
    }
}

void AppFrame::LoadFail(const char* resourcename) {
    syString s;
    s.Printf(_("Could not find the XRC resource '%s'!\nAre you sure the program was installed correctly?"),resourcename);
    syApp::Get()->ErrorMessageBox(s.c_str());
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

void AppFrame::LoadAndSetFrameSize() {
    const int minFrameWidth = 980;
    const int minFrameHight = 680;
    bool nocfg = false;

    // load stored size or defaults
    wxRect rect;
    syString key = CFG_LOCATION;
    if (syApp::GetConfig()->Exists (key)) {
        rect.x = syApp::GetConfig()->ReadInt(key + "/xpos", rect.x);
        rect.y = syApp::GetConfig()->ReadInt(key + "/ypos", rect.y);
        rect.width = syApp::GetConfig()->ReadInt(key + "/width", rect.width);
        rect.height = syApp::GetConfig()->ReadInt(key + "/height", rect.height);
    } else {
        nocfg = true;
    }

    // check for reasonable values (within screen)
    wxSize scr = wxGetDisplaySize();
    rect.x = wxMin (abs (rect.x), (scr.x - minFrameWidth));
    rect.y = wxMin (abs (rect.y), (scr.y - minFrameHight));
    rect.width = wxMax (abs (rect.width), (minFrameWidth));
    rect.width = wxMin (abs (rect.width), (scr.x - rect.x));
    rect.height = wxMax (abs (rect.height), (minFrameHight));
    rect.height = wxMin (abs (rect.height), (scr.y - rect.y));
    SetSize(rect);
    if(nocfg) {
        CenterOnScreen();
    }
}

AppFrame::~AppFrame() {
    syApp::ShutDown();
    ProjectManager::Unload();
    m_mgr->UnInit();
    delete m_welcomedialog;
    delete m_mgr;
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

void AppFrame::OnNewProject(wxCommandEvent& event) {
    NewProjectDlg* mydialog = new NewProjectDlg(this);
    if(!mydialog) {
        return;
    }
    mydialog->ShowModal();
}

void AppFrame::OnFileOpen(wxCommandEvent& event) {
    if(IsAppShuttingDown())
        return;
    wxString lastdir = s2wx(ProjectManager::Get()->GetLastProjectDir());
    wxFileDialog myDialog(this, _w("Choose a project"), lastdir, _T(""), _T("*.saya"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("opendlg"));
    int dialogresult = myDialog.ShowModal();
    if(dialogresult == wxID_OK) {
        if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.
            bool result = ProjectManager::Get()->LoadProject(syString(myDialog.GetPath()));
            if(!result) {
                wxString msg;
                msg.Printf(_w("Error opening file '%s'!"),myDialog.GetPath().c_str());
                wxMessageBox(msg,_w("Error"),wxCANCEL | wxICON_ERROR,this);
            }
            DoUpdateAppTitle();
        }
    }
}

void AppFrame::OnOpenRecentFile(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    if(ProjectManager::Get()->CloseProject(false)) { // First close current project, ask to save, etc.

        // event.GetId() - wxID_FILE1 would give zero for wxID_FILE1;
        // but the list is one-based so we add 1.
        int fileno = (event.GetId() - wxID_FILE1) + 1;

        bool result = ProjectManager::Get()->LoadRecentProject(fileno);
        if(!result) {
            wxString msg;
            msg.Printf(_w("Error opening file '%s'!"),ProjectManager::Get()->m_RecentFiles->item(fileno).c_str());
            wxMessageBox(msg,_w("Error"),wxCANCEL | wxICON_ERROR,this);
        }
        DoUpdateAppTitle();
    }
}

void AppFrame::OnClearRecentProjectList(wxCommandEvent &event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->m_RecentFiles->clear();
}

void AppFrame::OnFileClose(wxCommandEvent& event) {
    if(IsAppShuttingDown())
        return;
    ProjectManager::Get()->CloseProject(false);
    wxUpdateUIEvent tmpevent(idProjectStatusChanged);
    wxPostEvent(this,tmpevent);
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
        syApp::Get()->Exit(false);
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
    SaveDefaultLayout(true);
}

void AppFrame::UpdateStatustext() {
    SetStatusText (s2wx(g_statustext), 0);
}

void AppFrame::SaveDefaultLayout(bool showmsg) {
    if(m_layouthidden) {
        return;
    }
    wxRect rect = GetRect();
    syString key = CFG_LOCATION;
    syApp::GetConfig()->WriteInt(key + "/xpos", rect.x);
    syApp::GetConfig()->WriteInt(key + "/ypos", rect.y);
    syApp::GetConfig()->WriteInt(key + "/width", rect.width);
    syApp::GetConfig()->WriteInt(key + "/height", rect.height);

    syString strlayout(m_mgr->SavePerspective());
    syApp::GetConfig()->Write(CFG_PERSPECTIVE_DEFAULT, strlayout);
    syApp::GetConfig()->WriteInt(CFG_DEFAULT_PRJ_SASHPOS,GetProjectPanelSashPos());

    if(showmsg) {
        wxMessageBox (_w("Current Layout has been saved."),
                      _w("Save Layout"), wxOK);
    }
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
    mbar->Enable(idNewSequence, hasproject);
    mbar->Enable(idNewBin, hasproject);
    mbar->Enable(idNewOfflineFile, hasproject);
    mbar->Enable(idNewTitle, hasproject);
    mbar->Enable(idNewBarsandTone, hasproject);
    mbar->Enable(idNewBlackVideo, hasproject);
    mbar->Enable(idNewColorMatte, hasproject);
    mbar->Enable(idNewUniversalCountingLeader, hasproject);
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
    mbar->Enable(idFileCapture, hasproject);
    mbar->Enable(idFileBatchCapture, hasproject);
    mbar->Enable(idFileGetProperties, hasproject);
    mbar->Enable(idFileGetPropertiesFile, hasproject);
    mbar->Enable(idFileGetPropertiesSelection, hasproject);
}

void AppFrame::OnRecentFilesMenuUpdateUI(wxUpdateUIEvent& event) {
    // Update the Recent Projects list
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->m_RecentFiles->UpdateCounter(m_recentfilesmodcounter)) {
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
            mySubMenu->Append(idFileClearRecentProjectList,_T("&Clear"),_w("Clears Recent Projects List"));
            mySubMenu->AppendSeparator();
            for(i = 1; i <= pmgr->m_RecentFiles->size(); ++i) {
                syString tmptext;
                tmptext.Printf("&%d %s",i,pmgr->m_RecentFiles->item(i).c_str());
                mySubMenu->Append(wxID_FILE1 + i - 1,s2wx(tmptext),wxEmptyString);
            }
            myItem->Enable(pmgr->m_RecentFiles->size() > 0);
        }
    }
}

void AppFrame::OnRecentImportsMenuUpdateUI(wxUpdateUIEvent& event) {
    // Update the Recent Imported files list
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    if(pmgr->m_RecentImports->UpdateCounter(m_recentimportsmodcounter)) {
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
            mySubMenu->Append(idFileClearRecentImportList,_T("&Clear"),_w("Clears Recent Imported Files List"));
            mySubMenu->AppendSeparator();
            for(i = 1; i <= pmgr->m_RecentImports->size(); ++i) {
                syString tmptext;
                tmptext.Printf("&%d %s",i, pmgr->m_RecentImports->item(i).c_str());
                mySubMenu->Append(wxID_IMPORT1 + i -1,s2wx(tmptext),wxEmptyString);
            }
        }
    }
    wxMenuBar* mbar = GetMenuBar();
    mbar->Enable(idFileImportRecent,pmgr->m_RecentImports->size() > 0 && (pmgr->HasProject()));
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
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    wxMenuBar* mbar = GetMenuBar();

    mbar->Enable(idProjectSettings,hasproject);
    mbar->Enable(idProjectSettingsGeneral,hasproject);
    mbar->Enable(idProjectSettingsCapture,hasproject);
    mbar->Enable(idProjectSettingsVideoRendering,hasproject);
    mbar->Enable(idProjectSettingsDefaultSequence,hasproject);
    mbar->Enable(idProjectLinkMedia,hasproject);
    mbar->Enable(idProjectUnlinkMedia,hasproject);
    mbar->Enable(idProjectAutomateToSequence,hasproject);
    mbar->Enable(idProjectImportBatchList,hasproject);
    mbar->Enable(idProjectExportBatchList,hasproject);
    mbar->Enable(idProjectExportAsAAF,hasproject);
}

void AppFrame::OnClipMenuUpdateUI(wxUpdateUIEvent& event) {

    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    wxMenuBar* mbar = GetMenuBar();

    mbar->Enable(idClipRename, hasproject);
    mbar->Enable(idClipCaptureSettings, hasproject);
    mbar->Enable(idClipSetCaptureSettings, hasproject);
    mbar->Enable(idClipClearCaptureSettings, hasproject);
    mbar->Enable(idClipInsert, hasproject);
    mbar->Enable(idClipOverlay, hasproject);
    mbar->Enable(idClipToggleEnable, hasproject);
    mbar->Enable(idClipUnlinkAV, hasproject);
    mbar->Enable(idClipLinkAV, hasproject);
    mbar->Enable(idClipGroup, hasproject);
    mbar->Enable(idClipUngroup, hasproject);
    mbar->Enable(idClipVideoOptions, hasproject);
    mbar->Enable(idClipVOptFrameHold, hasproject);
    mbar->Enable(idClipVOptFieldOptions, hasproject);
    mbar->Enable(idClipAudioOptions, hasproject);
    mbar->Enable(idClipAOptGain, hasproject);
    mbar->Enable(idClipAOptBreakoutToMono, hasproject);
    mbar->Enable(idClipOptTreatAsStereo, hasproject);
    mbar->Enable(idClipOptRenderAndReplace, hasproject);
    mbar->Enable(idClipSpeedDuration, hasproject);
}

void AppFrame::OnSequenceMenuUpdateUI(wxUpdateUIEvent& event) {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    wxMenuBar* mbar = GetMenuBar();

    mbar->Enable(idSequenceRender, hasproject);
    mbar->Enable(idSequenceDeleteRenderFiles, hasproject);
    mbar->Enable(idSequenceRazor, hasproject);
    mbar->Enable(idSequenceRazorSelectedTracks, hasproject);
    mbar->Enable(idSequenceLift, hasproject);
    mbar->Enable(idSequenceExtract, hasproject);
    mbar->Enable(idSequenceApplyVideoTransition, hasproject);
    mbar->Enable(idSequenceApplyAudioTransition, hasproject);
    mbar->Enable(idSequenceZoomIn, hasproject);
    mbar->Enable(idSequenceZoomOut, hasproject);
    mbar->Enable(idSequenceSnap, hasproject);
    mbar->Enable(idSequenceAddTracks, hasproject);
    mbar->Enable(idSequenceDelTracks, hasproject);

}

void AppFrame::OnMarkerMenuUpdateUI(wxUpdateUIEvent& event) {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    wxMenuBar* mbar = GetMenuBar();

    mbar->Enable(idSetClipMarkerMenu, hasproject);
    mbar->Enable(idGotoClipMarkerMenu, hasproject);
    mbar->Enable(idGotoNextClipMarker, hasproject);
    mbar->Enable(idGotoPrevClipMarker, hasproject);
    mbar->Enable(idGotoInClipMarker, hasproject);
    mbar->Enable(idGotoOutClipMarker, hasproject);
    mbar->Enable(idGotoVideoInClipMarker, hasproject);
    mbar->Enable(idGotoVideoOutClipMarker, hasproject);
    mbar->Enable(idGotoAudioInClipMarker, hasproject);
    mbar->Enable(idGotoAudioOutClipMarker, hasproject);
    mbar->Enable(idGotoNumberedClipMarker, hasproject);
    mbar->Enable(idClearClipMarkerMenu, hasproject);
    mbar->Enable(idClearCurrentClipMarker, hasproject);
    mbar->Enable(idClearAllClipMarkers, hasproject);
    mbar->Enable(idClearInOutClipMarkers, hasproject);
    mbar->Enable(idClearInClipMarker, hasproject);
    mbar->Enable(idClearOutClipMarker, hasproject);
    mbar->Enable(idClearNumberedClipMarker, hasproject);
    mbar->Enable(idSetSequenceMarkerMenu, hasproject);
    mbar->Enable(idSetInSequenceMarker, hasproject);
    mbar->Enable(idSetOutSequenceMarker, hasproject);
    mbar->Enable(idSetInOutAroundSelSeqMarker, hasproject);
    mbar->Enable(idSetUnnumberedSequenceMarker, hasproject);
    mbar->Enable(idSetNextAvailNumberedSeqMarker, hasproject);
    mbar->Enable(idSetOtherNumberedSequenceMarker, hasproject);
    mbar->Enable(idGotoSequenceMarkerMenu, hasproject);
    mbar->Enable(idGotoNextSequenceMarker, hasproject);
    mbar->Enable(idGotoPrevSequenceMarker, hasproject);
    mbar->Enable(idGotoInSequenceMarker, hasproject);
    mbar->Enable(idGotoOutSequenceMarker, hasproject);
    mbar->Enable(idGotoNumberedSeqMarker, hasproject);
    mbar->Enable(idClearSequenceMarkerMenu, hasproject);
    mbar->Enable(idClearCurSequenceMarker, hasproject);
    mbar->Enable(idClearAllSequenceMarkers, hasproject);
    mbar->Enable(idClearInOutSeqMarkers, hasproject);
    mbar->Enable(idClearInSeqMarker, hasproject);
    mbar->Enable(idClearOutSeqMarker, hasproject);
    mbar->Enable(idClearNumberedSeqMarker, hasproject);
    mbar->Enable(idEditSequenceMarker, hasproject);
}

void AppFrame::OnWindowMenuUpdateUI(wxUpdateUIEvent& event) {
    ProjectManager* pmgr = ProjectManager::Get();
    if(!pmgr)
        return;
    bool hasproject = pmgr->HasProject();

    wxMenuBar* mbar = GetMenuBar();

    mbar->Enable(idWindowWorkspaceMenu, hasproject);
    mbar->Enable(idWorkspaceEditing, hasproject);
    mbar->Enable(idWorkspaceEffects, hasproject);
    mbar->Enable(idWorkspaceAudio, hasproject);
    mbar->Enable(idWorkspaceColorCorrection, hasproject);
    mbar->Enable(idWorkspaceDefault, hasproject);
    mbar->Enable(idWorkspaceSaveAs, hasproject);
    mbar->Enable(idWorkspaceDelete, hasproject);
    mbar->Enable(idWorkspaceCustom, hasproject);
    mbar->Enable(idMenuSaveFrameLayout, hasproject);
    mbar->Enable(idWindowEffects, hasproject);
    mbar->Enable(idWindowEffectControls, hasproject);
    mbar->Enable(idWindowHistory, hasproject);
    mbar->Enable(idWindowInfo, hasproject);
    mbar->Enable(idWindowTools, hasproject);
    mbar->Enable(idWindowAudioMixer, hasproject);
    mbar->Enable(idWindowMonitor, hasproject);
    mbar->Enable(idWindowProject, hasproject);
    mbar->Enable(idWindowTimelinesMenu, hasproject);

}

void AppFrame::OnProjectStatusChanged(wxUpdateUIEvent& event) {
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

void AppFrame::ShowLayout(bool show) {
    if(IsAppShuttingDown())
        return;
    if(show) {
        if(m_layouthidden) {
            m_mgr->LoadPerspective(m_CurrentPerspective,true);
            m_layouthidden = false;
        }
    } else {
        if(!m_layouthidden) {
            // Hide All panes
            m_CurrentPerspective = m_mgr->SavePerspective();
            wxAuiPaneInfoArray& myarr = m_mgr->GetAllPanes();
            size_t i;
            for(i = 0;i < myarr.size(); i++) {
                myarr[i].Hide();
            }
            m_mgr->Update();
            m_layouthidden = true;
        }
    }
    if(!show) {
//        m_mgr->Update();
        ShowWelcomeDialog();
    } else {
        if(m_welcomedialog) {
            m_welcomedialog->Hide();
        }
        Show();
        m_mgr->Update();
    }
}

void AppFrame::ShowWelcomeDialog() {
    Hide();

    m_welcomedialog->Show();
}

void AppFrame::DoUpdateAppTitle() {
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
    SetTitle(s2wx(title));
}

void AppFrame::OnAbout(wxCommandEvent &event) {
    syString msg;
    msg << syApp::Get()->GetApplicationShowOffName() << "\n" << " by " << syApp::Get()->GetApplicationVendor() << "\n";
    msg << wxbuildinfo(long_f);
    syMessageBox(msg, _("Welcome to..."));
}

void AppFrame::OnResourceTreeContextMenu(wxTreeEvent& event) {
    wxMenu *menu = wxXmlResource::Get()->LoadMenu(_T("resources_tree_menu"));
    if(menu) PopupMenu(menu);
    delete menu;
}

void AppFrame::OnUpdateProjectPaneUI(wxUpdateUIEvent& event) {
    if(IsAppShuttingDown())
        return;
    bool enablePane = ProjectManager::Get()->HasProject();
    wxWindow* thepane = FindWindow(idProjectPane);
    if(thepane) {
        thepane->Enable(enablePane);
    }
}

void AppFrame::ProcessSayaEvent(sayaEventType id, void* data) {
    switch(id) {
        case sayaevt_ProjectStatusChanged:
            {
                wxUpdateUIEvent event(idProjectStatusChanged);
                wxPostEvent(this,event);
            }
        break;
        default:;
    }
}

void AppFrame::ErrorMessageBox(const char* msg,const char* caption) {
    wxMessageBox(s2wx(msg), s2wx(caption), wxCANCEL | wxICON_ERROR, this);
}

bool AppFrame::YesNoMessageBox(const char* msg,const char* caption,bool exclamation) {
    int style;
    if(exclamation) {
        style = wxICON_EXCLAMATION;
    } else {
        style = wxICON_QUESTION;
    }
    int result = wxMessageBox(s2wx(msg), s2wx(caption), wxYES_NO | style, this);
    return (result == wxYES);
}

sayaYesNoCancel AppFrame::YesNoCancelMessageBox(const char* msg,const char* caption,bool exclamation) {
    int style;
    sayaYesNoCancel result;
    if(exclamation) {
        style = wxICON_EXCLAMATION;
    } else {
        style = wxICON_QUESTION;
    }
    int answer = wxMessageBox(s2wx(msg), s2wx(caption), wxYES_NO | wxCANCEL | style, this);
    if(answer == wxYES) {
        result = sayaYes;
    } else if(answer == wxNO) {
        result = sayaNo;
    } else {
        result = sayaCancel;
    }
    return result;
}

syString AppFrame::ShowDialogSaveProjectAs() {
    wxFileDialog mydialog(this,_w("Save file as..."),wxEmptyString,wxEmptyString,_T("*.saya"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    syString result = "";
    if(mydialog.ShowModal() == wxID_OK) {
        result = mydialog.GetPath();
    }
    return result;
}

syString AppFrame::ShowDialogSaveProjectCopyAs() {
    wxFileDialog mydialog(this,_w("Save Copy as..."),wxEmptyString,wxEmptyString,_T("*.saya"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);
    syString result = "";
    if(mydialog.ShowModal() == wxID_OK) {
        result = mydialog.GetPath(); // Gets full path including filename
    }
    return result;
}

