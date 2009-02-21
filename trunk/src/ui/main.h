/***************************************************************
 * Name:      sayaMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <qmainwindow.h>

#include "../saya/core/events.h"

class syString;
class ProjectManager;
class WelcomeDialog;
class QCloseEvent;

class syProjectStatusEvent;

class AppFrame: public QMainWindow, public syEvtHandler {
    public:
        AppFrame(const syString& title);
        ~AppFrame();
        void UpdateStatustext();

        bool IsClipSelected();          ///< True if one or more timeline clips are selected
        bool IsClipboardSet();          /**< True if clipboard contains a resource clip or a timeline clip
                                         *  (resource clips will paste into timeline clips) */
        bool IsSelectionMultiple();     /**< True if Selection covers more than one timeline clip, or even full tracks
                                         *  - or if more than one resource clip is selected */
        bool IsResourceClipSelected();  ///< True if a resource clip is selected
        bool IsTimelineActive();        ///< True if the active window is the timeline window
        bool IsResourceWindowActive();  ///< True if the active window is the resource window
        bool IsMonitorActive();         ///< True if the active window is the monitor window
        bool IsRenderMonitorActive();   ///< True if the *render* monitor is active
        bool IsClipMonitorActive();     ///< True if the *clip* monitor is active
                                        ///< (render and clip monitors can't be active (selected) at the same time)
        bool IsEffectsWindowActive();   ///< True if the effects monitor/window is active
        bool IsTitleWindowActive();     ///< True if the title window is active
        bool CanUndo();                 ///< Does the project have an item in the "undo" list?
        bool CanRedo();                 ///< Does the project have an item in the "redo" list?

    protected:
        virtual void closeEvent(QCloseEvent *event);

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif // SAYAMAIN_H
