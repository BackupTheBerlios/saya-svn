/***************************************************************
 * Name:      ui/app.h
 * Purpose:   Defines Application Class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef ui_app_h
#define ui_app_h

#include "../saya/core/app.h"
#include "../saya/core/dialogs.h"

class qSayaApp: public syApp
{
    public:
        qSayaApp(int argc, char** argv);

        /** Gets the application name to initialize the configuration object. */
        virtual const char* GetApplicationName() const;

        /** Gets the application display name for the main window. */
        virtual const char* GetApplicationDisplayName() const;

        /** Gets the application Vendor. */
        virtual const char* GetApplicationVendor() const;

        /** Gets the Application Full name and Motto. */
        virtual const char* GetApplicationShowOffName() const;

        /** Gets the Application Path */
        virtual const char* GetApplicationPath() const;

        /** Gets the Application Filename */
        virtual const char* GetApplicationFilename() const;

        virtual syDebugLog* CreateDebugLog() const;

        /** Creates a Config handler. */
        virtual syConfig* CreateConfig() const;

        /** Initializer. */
        virtual bool OnInit();

        /** Main loop. */
        virtual void Run();

        /** Exits the main loop on the next iteration (if now == false), or right now (if now == true). */
        virtual void Exit(bool now = false);

        /** Exit handler. */
        virtual void OnExit();

        /** Destructor. */
        virtual ~qSayaApp();

        /** Posts an event to the event queue. */
        virtual void PostEvent(syEvtHandler* handler, syEvent& event);

        /** Sets the application's main window. */
        virtual void SetTopWindow(void* window);

        /** Gets the application's main window. */
        virtual void* GetTopWindow() const;

        /** Returns true if the application's main loop is running. */
        virtual bool IsMainLoopRunning() const;

        /** Wakes up the main thread to begin event processing. */
        virtual void WakeUpIdle();

        /** @brief Shows a standard message box.
         *  @see syDIALOG_ICON_TYPES
         *  @see syDIALOG_BUTTON_TYPES
         */
        virtual int MessageBox(const syString& message, const syString& caption,unsigned int flags = syOK,void* parent = 0) const;

        /** Shows an plain, OS-friendly error message box. Use this in case you messed up with your UI-toolkit.*/
        virtual void ErrorMessageBox(const syString& message) const;

        /** Shows a temporary message in your main window's status bar. */
        virtual void LogStatus(const syString& message) const;

        syFileDialogResult FileSelector(
            const syString& message,
            const syString& default_path,
            const syString& default_filename,
            const syString& default_extension,
            const syString& wildcard,
            int flags,
            void* parent,
            int x,
            int y) const;

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // ui_app_h
