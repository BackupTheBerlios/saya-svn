/***************************************************************
 * Name:      presetmanager.h
 * Purpose:   Declaration for the Preset Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef presetmanager_h
#define presetmanager_h

#include <list>
#include <map>
#include <string>

class PresetManager {

    public:

        /** Standard Constructor */
        PresetManager();

        /** Standard Desstructor */
        ~PresetManager();

        /** Gets (only names) all predefined settings saved by user (not include custom)  */
        std::list<std::string> GetPresets();

        /** Returns the info for the specified preset. The map contains the id of the component and the value.
        The value is a string, but it is well formed for every value type of every widget */
        std::map<std::string, std::string> GetPresetData(std::string preset);

        /** Save new predefined setting */
        bool SaveNewPreset(std::string preset, std::map<std::string, std::string>);
};

#endif
