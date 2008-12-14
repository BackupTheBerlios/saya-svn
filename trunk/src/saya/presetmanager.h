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

class SStringVector;
class SMapStrStr;
class syString;

class PresetManager {

    public:

        /** Standard Constructor */
        PresetManager();

        /** Standard Desstructor */
        ~PresetManager();

        /** Gets (only names) all predefined settings saved by user (not include custom)  */
        SStringVector GetPresets();

        /** Returns the info for the specified preset. The map contains the id of the component and the value.
        The value is a string, but it is well formed for every value type of every widget */
        SMapStrStr GetPresetData(const char* preset);

        SMapStrStr GetPresetData(const syString& preset);

        /** Save new predefined setting */
        bool SaveNewPreset(const char* preset, const SMapStrStr& data);

        bool SaveNewPreset(const syString& preset, const SMapStrStr& data);
};

#endif
