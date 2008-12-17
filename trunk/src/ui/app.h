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

class wxSayaApp: public syApp
{
    public:
        wxSayaApp();

        /** Gets the application name to initialize the configuration object. */
        virtual const char* GetApplicationName() const;

        /** Gets the application display name for the main window. */
        virtual const char* GetApplicationDisplayName() const;

        /** Gets the application Vendor. */
        virtual const char* GetApplicationVendor() const;

        /** Gets the Application Full name and Motto. */
        virtual const char* GetApplicationShowOffName() const;

        virtual syDebugLog* CreateDebugLog() const;

        /** Creates a Config handler. */
        virtual syConfig* CreateConfig() const;

        /** Initializer. */
        virtual bool OnInit(int argc, const char** argv);

        /** Main loop. */
        virtual void Run();

        /** Exits the main loop on the next iteration (if now == false), or right now (if now == true). */
        virtual void Exit(bool now = false);

        /** Exit handler. */
        virtual void OnExit();

        /** Destructor. */
        virtual ~wxSayaApp();

        /** Shows an error message box. */
        void ErrorMessageBox(const char* str) const;

        /** Shows a standard message box. */
        void MessageBox(const syString& message, const syString& caption) const;

        syFileDialogResult FileSelector(
            const syString& message,
            const syString& default_path,
            const syString& default_filename,
            const syString& default_extension,
            const syString& wildcard,
            int flags,
            void* parent,
            int x,
            int y);

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // ui_app_h
