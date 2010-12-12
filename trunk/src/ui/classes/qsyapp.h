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

#include <saya/core/app.h>
#include <saya/core/dialogs.h>

class syString;

class qsyApp: public syApp
{
    public:
        qsyApp(int argc, char** argv);

        /** Gets the Application Path */
        const char* GetApplicationPath() const;

        /** Gets the Application Filename */
        const char* GetApplicationFilename() const;

        syDebugLog* CreateDebugLog() const;

        /** Creates a Config handler. */
        syConfig* CreateConfig() const;

        /** Initializer. */
        bool OnInit();

        /** Main loop. */
        void Run();

        /** Exits the main loop on the next iteration (if now == false), or right now (if now == true). */
        void Exit(bool now = false);

        /** Exit handler. */
        void OnExit();

        /** Destructor. */
        virtual ~qsyApp();

        /** Posts an event to the event queue. */
        void PostEvent(syEvtHandler* handler, syEvent& event);

        /** Sets the application's main window. */
        void SetTopWindow(void* window);

        /** Gets the application's main window. */
        void* GetTopWindow() const;

        /** Returns true if the application's main loop is running. */
        bool IsMainLoopRunning() const;

        /** Wakes up the main thread to begin event processing. */
        void WakeUpIdle();

        /** @brief Shows a standard message box.
         *  @see syDIALOG_ICON_TYPES
         *  @see syDIALOG_BUTTON_TYPES
         */
        int MessageBox(const syString& message, const syString& caption,unsigned int flags = syOK,void* parent = 0) const;

        /** Shows an plain, OS-friendly error message box. Use this in case you messed up with your UI-toolkit.*/
        void ErrorMessageBox(const syString& message) const;

        /** Shows a temporary message in your main window's status bar. */
        void LogStatus(const syString& message) const;

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

    protected:

        /** Loads a resource given its filename (relative to the application's resources/ directory.
         *  Used by LoadResources().
         */
        bool LoadResource(const syString& filename);

    public:

        /** Does the clean up before the Application Main Loop is exited. */
        virtual void OnBeforeExit() = 0;

        /** Initializes the application's objects. Called by OnInit() */
        virtual bool InitializeApplicationObjects() = 0;

        /** Loads all the application resources. Called by OnInit(), after InitializeObjects().
         *  To load a Resource, use the LoadResource() function.
         */
        virtual bool LoadResources() = 0;

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // ui_app_h
