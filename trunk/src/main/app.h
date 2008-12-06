/***************************************************************
 * Name:      sayaApp.h
 * Purpose:   Defines Application Class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef APP_H
#define APP_H

#include <wx/app.h>
#include <string>
#include "../saya/projectmanager.h"
#include "../saya/configprovider.h"
#include "../saya/sayaconfig.h"

class AppDebugLog;
class syString;

/** Our Implementation of SayaConfig with wxWidgets. @see SayaConfig */
class AppConfig : public SayaConfig {
    public:

        /** Standard constructor. */
        AppConfig(const char* application_name);

        /** Reads configuration. @see SayaConfig::Read */
        virtual syString Read(const char* key, const char* defaultvalue);

        /** Writes configuration. @see SayaConfig::Read */
        virtual bool Write(const char* key, const char* value);

        /** Checks whether a configuration key exists. @see SayaConfig::Read */
        virtual bool Exists(const char* key);

        /** Standard destructor. */
        virtual ~AppConfig();
    private:
        /** Our configuration object */
        wxConfig* m_config;
};

/** Our Configuration object provider. @see SayaConfigProvider */
class AppConfigProvider : public SayaConfigProvider {
    public:
        /** Creates a SayaConfig object */
        virtual SayaConfig* Create(const char* application_name);

        /** Virtual destructor. */
        virtual ~AppConfigProvider() {}
};

// *** Our App ***
class App : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
        bool LoadConfig();
        bool LoadXRCResources();
        void InitManagers();
        DECLARE_EVENT_TABLE()
        virtual ~App();
    private:
        void OnExitApp(wxCommandEvent& event);
        AppConfigProvider m_configprovider;
        AppDebugLog* m_debuglog;
};

#endif // SAYAAPP_H
