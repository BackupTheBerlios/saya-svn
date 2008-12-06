/***************************************************************
 * Name:      configprovider.h
 * Purpose:   Definition for the SayaConfigProvider class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef configprovider_h
#define configprovider_h

class SayaConfig;

/** Abstract Cross-platform Configuration Provider class.
  *
  * This class creates a Configuration object for you to use.
  * @note All strings passed and received are ANSI strings in multibyte format.
  */
class SayaConfigProvider {
    public:
        /** Standard Constructor */
        SayaConfigProvider() {};

        /** Standard Destructor */
        virtual ~SayaConfigProvider() {};

        virtual SayaConfig* Create(const char* application_name) = 0;
};

#endif
