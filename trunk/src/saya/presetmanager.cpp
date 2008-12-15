/***************************************************************
 * Name:      presetmanager.h
 * Purpose:   Implementation of the Preset Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "presetmanager.h"
#include "core/systring.h"
#include "timeline/sstrvector.h"
#include "timeline/smapxstr.h"

PresetManager::PresetManager() {
}

PresetManager::~PresetManager() {
}

SStringVector PresetManager::GetPresets() {
    SStringVector presets;

    presets.push_back("Predef 1");
    presets.push_back("Predef 2");
    presets.push_back("Predef 3");
    presets.push_back("Predef 4");
    presets.push_back("Predef 5");
    return presets;
}

SMapStrStr PresetManager::GetPresetData(const char* preset) {
    SMapStrStr configs;

    configs["idNewPrjAVSettings_width"] = "600";
    configs["idNewPrjAVSettings_height"] = "400";
    configs["idNewPrjAVSettings_fps"] = "24";
    configs["idNewPrjAVSettings_interlacing"] = "Progressive (non interlaced)";
    configs["idNewPrjAVSettings_pixelaspect"] = "1.0";
    configs["idNewPrjAVSettings_samplerate"] = "44100";
    configs["idNewPrjAVSettings_samplesize"] = "8 bit";
    configs["idNewPrjAVSettings_surround"] = "Stereo";
    configs["idNewPrjAVSettings_channels"] = "2";
    configs["idNewPrjAVSettings_description"] = "some description for the preset";

    return configs;
}

SMapStrStr GetPresetData(const syString& preset) {
    return GetPresetData(preset.c_str());
}


bool PresetManager::SaveNewPreset(const char* preset, const SMapStrStr& data) {
    return true;
}

bool PresetManager::SaveNewPreset(const syString& preset, const SMapStrStr& data) {
    return true;
}
