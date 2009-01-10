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
class QsyConfig;
class QSettings;

/** Our Implementation of syConfig with Qt. @see syConfig */
class QsyConfig : public syConfig {
    public:

        /** Standard constructor. */
        QsyConfig(const char* application_name);

        /** Reads a configuration value. @see syConfig */
        virtual syString Read(const char* key, const char* defaultvalue) const;

        /** Reads a configuration value. @see syConfig */
        virtual unsigned int ReadUint(const char* key, unsigned int defaultvalue = 0)  const;

        /** Reads a configuration value. @see syConfig */
        virtual int ReadInt(const char* key, int defaultvalue = 0)  const;

        /** Reads a configuration value. @see syConfig */
        virtual bool ReadBool(const char* key, bool defaultvalue = false) const;

        /** Reads a configuration value. @see syConfig */
        virtual double ReadFloat(const char* key, double defaultvalue = 0.0) const;

        /** Writes configuration. @see syConfig::Read */
        virtual bool Write(const char* key, const char* value);

        /** Reads a configuration value. @see syConfig */
        virtual bool WriteUint(const char* key, unsigned int value);

        /** Reads a configuration value. @see syConfig */
        virtual bool WriteInt(const char* key, int value);

        /** Reads a configuration value. @see syConfig */
        virtual bool WriteBool(const char* key, bool value);

        /** Reads a configuration value. @see syConfig */
        virtual bool WriteFloat(const char* key, double value);

        /** Checks whether a configuration key exists. @see syConfig::Read */
        virtual bool Exists(const char* key) const;

        /** Standard destructor. */
        virtual ~QsyConfig();
    private:
        /** Our configuration object */
        QSettings* m_config;
};


#endif
