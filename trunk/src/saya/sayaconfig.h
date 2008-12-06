/***************************************************************
 * Name:      sayaconfig.h
 * Purpose:   Definition for the SayaConfig class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef sayaconfig_h
#define sayaconfig_h

#include <string>

/** Abstract Cross-platform Configuration class.
  *
  * @note All strings passed and received are ANSI strings in multibyte format.
  */
class SayaConfig {
    public:

        /** Standard constructor */
        SayaConfig(const char* application_name) {}

        /** Standard destructor */
        virtual ~SayaConfig() {};

        /** @brief Reads a string configuration value.
          *
          * @param key The key to read the configuration value from.
          * @param defaultvalue The default value to return, if the value is not found.
          * @return The value of the read configuration.
          */
        virtual syString Read(const char* key, const char* defaultvalue) = 0;

        /** @brief Writes a string configuration value.
          *
          * @param key The key to write the configuration value into.
          * @param value The value to write.
          * @return true on success; false otherwise.
          */
        virtual bool Write(const char* key, const char* value) = 0;

        /** @brief Checks if a configuration key exists.
          *
          * @param key The key to search in the config.
          * @return true if the key exists; false otherwise.
          */
        virtual bool Exists(const char* key) = 0;
};

#endif
