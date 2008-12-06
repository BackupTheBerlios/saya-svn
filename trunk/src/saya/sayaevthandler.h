/***************************************************************
 * Name:      sayaevthandler.h
 * Purpose:   Definition for the sayaEvtHandler class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef sayaevthandler_h
#define sayaevthandler_h

/** Used for UI-toolkit-independent event handling */
enum sayaEventType {
    sayaevt_DoNothing = 0,
    sayaevt_ProjectStatusChanged
};

/** Used for Yes/No/Cancel dialogs */
enum sayaYesNoCancel {
    sayaYes = 0,
    sayaNo,
    sayaCancel
};

/** @brief Abstract UI-toolkit-independent Event Handler.
  *
  * In an effort to make Saya as independent as possible regarding UI Toolkits, we're using
  * a wrapper class for your main event handler to deal with user interaction.
  * All strings passed and received are ANSI strings in multibyte format.
  * @note To receive project-related events, you must call ProjectMananger::SetEventHandler.
  */
class sayaEvtHandler {
    public:
        /** Standard constructor */
        sayaEvtHandler() {}

        /** Standard destructor */
        virtual ~sayaEvtHandler() {}

        /** Event processing function */
        virtual void ProcessSayaEvent(sayaEventType id, void* data = NULL) = 0;

        /** Shows an Error message box */
        virtual void ErrorMessageBox(const char* msg,const char* caption) = 0;

        /** Shows a yes/no message box and returns the answer (true = yes, false = no) */
        virtual bool YesNoMessageBox(const char* msg,const char* caption,bool exclamation) = 0;

        /** Shows a yes/no/cancel message box and returns the answer */
        virtual sayaYesNoCancel YesNoCancelMessageBox(const char* msg,const char* caption,bool exclamation) = 0;

        /** Shows the "Save Project As" dialog */
        virtual syString ShowDialogSaveProjectAs() = 0;

        /** Shows the "Save Project Copy As" dialog */
        virtual syString ShowDialogSaveProjectCopyAs() = 0;

};

#endif
