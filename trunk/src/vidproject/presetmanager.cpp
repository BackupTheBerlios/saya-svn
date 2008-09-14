/***************************************************************
 * Name:      presetmanager.h
 * Purpose:   Implementation of the Preset Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "presetmanager.h"
using namespace std;

PresetManager::PresetManager() {
}

PresetManager::~PresetManager() {
}

std::list<std::string> PresetManager::GetPresets() {
    std::list<std::string> presets;

    presets.push_back("Predef 1");
    presets.push_back("Predef 2");
    presets.push_back("Predef 3");
    presets.push_back("Predef 4");
    presets.push_back("Predef 5");
    return presets;
}

std::map<std::string, std::string> PresetManager::GetPresetData(std::string preset) {
    map<std::string, std::string> configs;

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

bool PresetManager::SaveNewPreset(std::string preset, std::map<std::string, std::string>) {
    return true;
}
