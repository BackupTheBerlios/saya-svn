/***************************************************************
 * Name:      sayaconfig.h
 * Purpose:   Definition for the syConfig class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef sayaconfig_h
#define sayaconfig_h

#include "systring.h"

/** Abstract Cross-platform Configuration class.
  *
  * @note All strings passed and received are ANSI strings in multibyte format.
  */
class syConfig {
    public:

        /** Standard constructor */
        syConfig(const char* application_name) {}

        /** Standard destructor */
        virtual ~syConfig() {};

        /** @brief Reads a string configuration value.
          *
          * @param key The key to read the configuration value from.
          * @param defaultvalue The default value to return, if the value is not found.
          * @return The value of the read configuration.
          */
        virtual syString Read(const char* key, const char* defaultvalue) const = 0;
        syString Read(const syString& key, const char* defaultvalue) const;

        virtual unsigned int ReadUint(const char* key, unsigned int defaultvalue = 0)  const = 0;
        unsigned int ReadUint(const syString& key, unsigned int defaultvalue = 0) const;

        virtual int ReadInt(const char* key, int defaultvalue = 0)  const = 0;
        int ReadInt(const syString& key, int defaultvalue = 0) const;

        virtual bool ReadBool(const char* key, bool defaultvalue = false) const = 0;
        bool ReadBool(const syString& key, bool defaultvalue = false) const;

        virtual double ReadFloat(const char* key, double defaultvalue = 0.0) const = 0;
        double ReadFloat(const syString& key, double defaultvalue = 0.0) const;

        /** @brief Writes a string configuration value.
          *
          * @param key The key to write the configuration value into.
          * @param value The value to write.
          * @return true on success; false otherwise.
          */
        virtual bool Write(const char* key, const char* value) = 0;
        virtual bool Write(const syString& key, const char* value);
        virtual bool Write(const syString& key, const syString& value);

        virtual bool WriteUint(const char* key, unsigned int value)  = 0;
        virtual bool WriteUint(const syString& key, unsigned int value);

        virtual bool WriteInt(const char* key, int value) = 0;
        virtual bool WriteInt(const syString& key, int value);

        virtual bool WriteBool(const char* key, bool value) = 0;
        virtual bool WriteBool(const syString& key, bool value);

        virtual bool WriteFloat(const char* key, double value) = 0;
        virtual bool WriteFloat(const syString& key, double value);

        /** @brief Checks if a configuration key exists.
          *
          * @param key The key to search in the config.
          * @return true if the key exists; false otherwise.
          */
        virtual bool Exists(const char* key) const = 0;
        bool Exists(const syString& key) const;
};

#endif
