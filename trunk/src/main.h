/***************************************************************
 * Name:      /main.h
 * Purpose:   Include file for the main application.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   This file is Public Domain
 **************************************************************/

#ifndef saya_main_program_h
#define saya_main_program_h

#include <ui/classes/qsyapp.h>

class SayaApplication : public qsyApp {
    public:

        SayaApplication(int argc, char** argv);

        /** Initializes the application's objects. Called by OnInit() */
        bool InitializeApplicationObjects();

        /** Loads all the application resources. Called by OnInit(), after InitializeObjects() */
        bool LoadResources();

        /** Does the clean up before the Application Main Loop is exited. */
        void OnBeforeExit();

        virtual ~SayaApplication();

        /** Gets the application name to initialize the configuration object. */
        const char* GetApplicationName() const;

        /** Gets the application display name for the main window. */
        const char* GetApplicationDisplayName() const;

        /** Gets the application Vendor. */
        const char* GetApplicationVendor() const;

        /** Gets the Application Full name and Motto. */
        const char* GetApplicationShowOffName() const;

};

#endif
