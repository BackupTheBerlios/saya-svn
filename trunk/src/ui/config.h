/***************************************************************
 * Name:      ui/config.h
 * Purpose:   Declares the Application Configuration Handler
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef saya_wx_config_h
#define saya_wx_config_h

#include "../saya/core/config.h"
class wxConfig;

/** Our Implementation of syConfig with wxWidgets. @see syConfig */
class wxsyConfig : public syConfig {
    public:

        /** Standard constructor. */
        wxsyConfig(const char* application_name);

        /** Reads configuration. @see syConfig::Read */
        virtual syString Read(const char* key, const char* defaultvalue);

        /** Writes configuration. @see syConfig::Read */
        virtual bool Write(const char* key, const char* value);

        /** Checks whether a configuration key exists. @see syConfig::Read */
        virtual bool Exists(const char* key);

        /** Standard destructor. */
        virtual ~wxsyConfig();
    private:
        /** Our configuration object */
        wxConfig* m_config;
};


#endif
