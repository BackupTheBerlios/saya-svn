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
#include "vidproject/projectmanager.h"

/** Our Implementation of sayaConfig with wxWidgets. @see sayaConfig */
class AppConfig : public sayaConfig {
    public:

        /** Standard constructor. */
        AppConfig(std::string application_name);

        /** Reads configuration. @see sayaConfig::Read */
        virtual std::string Read(const std::string& key, const std::string& defaultvalue);

        /** Writes configuration. @see sayaConfig::Read */
        virtual bool Write(const std::string& key, const std::string& value);

        /** Checks whether a configuration key exists. @see sayaConfig::Read */
        virtual bool Exists(const std::string& key);

        /** Standard destructor. */
        virtual ~AppConfig();
    private:
        /** Our configuration object */
        wxConfig* m_config;
};

/** Our Configuration object provider. @see sayaConfigProvider */
class AppConfigProvider : public sayaConfigProvider {
    public:
        /** Creates a sayaConfig object */
        virtual sayaConfig* Create(const std::string application_name);

        /** Virtual destructor. */
        virtual ~AppConfigProvider() {}
};

// *** Our App ***
class App : public wxApp
{
    public:
        virtual bool OnInit();
        bool LoadConfig();
        bool LoadXRCResources();
        void InitManagers();
        DECLARE_EVENT_TABLE()
    private:
        AppConfigProvider m_configprovider;
};

#endif // SAYAAPP_H
