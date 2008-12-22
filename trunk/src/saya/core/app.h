/****************************************************************
 * Name:      app.h
 * Purpose:   Declaration of a UI-neutral syApplication class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#ifndef saya_app_h
#define saya_app_h

class syDebugLog;
class syConfig;
class syString;
class syFileDialogResult;
class syEvent;
class syEvtHandler;

/** The application's name. This must be implemented in your personalized app.cpp*/
extern const char* APP_NAME;
/** The application vendor's name (that would be moi). This must be implemented in your personalized app.cpp */
extern const char* APP_VENDOR;
/** The application's official name. This must be implemented in your personalized app.cpp */
extern const char* APP_SHOWNAME;
/** The application's name and tagline for showing off. This must be implemented in your personalized app.cpp */
extern const char* APP_SHOWOFFNAME;

/** An alias for syApp::IsAppShuttingDown().
 *  @see syApp::IsAppShuttingDown()
 */
bool IsAppShuttingDown();

class syApp
{
    public:
        /** Gets the main application object. Set by syApp::Start() and cleared by syApp::~syApp(). */
        static syApp* Get();

        /** Returns true if the application has begun shutdown. You should abort all the operations if this is true. */
        static bool IsAppShuttingDown();

        /** Begins application shutdown. */
        static void ShutDown();

        /** Constructor. */
        syApp();

        /** @brief Initializer. Call this method to start up the application.
         *  Start() calls OnInit(), and if the result was false, the object is destroyed immediately.
         *  Therefore, all your cleanup functions should be handled in the constructor.
         *  After calling OnInit(), it calls Run(), your application's main loop.
         *  @param argc The number of arguments, just as in ::main().
         *  @param argv An array of c-strings, just as in ::main().
         *  @note There must be only one running syApp class instance. If another class has been created
         *  and is running, Start() will exit, returning -2.
         *  @return -1 if OnInit() returned false; otherwise it returns Result.
         */
        int Start(int argc, const char** argv);

        static syConfig* GetConfig();
        /** @brief Creates a config handler.
         *  Called by Start() just before calling OnInit().
         */
        virtual syConfig* CreateConfig() const = 0;

        /** @brief Creates a toolkit-specific debug log.
         *  Called by Start() just before calling OnInit().
         */
        virtual syDebugLog* CreateDebugLog() const;

        /** Gets the application name to initialize the configuration object. */
        virtual const char* GetApplicationName() const = 0;

        /** Gets the application display name for the main window. */
        virtual const char* GetApplicationDisplayName() const = 0;

        /** Gets the application Vendor. */
        virtual const char* GetApplicationVendor() const = 0;

        /** Gets the Application Full name and Motto. */
        virtual const char* GetApplicationShowOffName() const = 0;

        /** @brief UI Initializer (called by Init()). To be overriden by your UI application.
         *  @return Set to true if the application was initialized correctly; false if there was an error.
         */
        virtual bool OnInit(int argc, const char** argv);

        /** Exits the main loop on the next iteration (if now == false), or right now (if now == true). */
        virtual void Exit(bool now = false) = 0;

        /** @brief Exit routine. Called just before the object is destroyed. */
        virtual void OnExit();

        /** This enters the application's main loop. It should set Result on exit.*/
        virtual void Run() = 0;

        /** Returns true if there are unprocessed events in the event queue. */
        bool Pending() const;

        /** @brief Process the first available event in the event queue.
         */
        void ProcessNextEvent() const;

        /** Posts an event to the event queue. */
        virtual void PostEvent(syEvtHandler* handler, syEvent& event) = 0;

        /** Returns true if the application's main loop is running. */
        virtual bool IsMainLoopRunning() const = 0;

        /** Destructor. */
        virtual ~syApp();

        /** @brief Shows a standard message box.
         *  @see syDIALOG_ICON_TYPES
         *  @see syDIALOG_BUTTON_TYPES
         */
        virtual int MessageBox(const syString& message, const syString& caption,unsigned int flags,void* parent) const = 0;

        /** Shows an plain, OS-friendly error message box. */
        virtual void ErrorMessageBox(const syString& message) const = 0;

        /** Shows a temporary message in your main window's status bar. */
        virtual void LogStatus(const syString& message) const = 0;

        /** Sets the application's main window. */
        virtual void SetTopWindow(void* window) = 0;

        /** Gets the application's main window. */
        virtual void* GetTopWindow() const = 0;

        /** Shows a File selection dialog. */
        virtual syFileDialogResult FileSelector(
            const syString& message,
            const syString& default_path,
            const syString& default_filename,
            const syString& default_extension,
            const syString& wildcard,
            int flags,
            void* parent,
            int x,
            int y) const = 0;


    protected:
        static int Result;
};

#endif
