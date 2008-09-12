/*****************************************************************
 * Name:      avsettings.h
 * Purpose:   Definition for the AVSettings class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ****************************************************************/

#include "../iomgr/iocommon.h"

#include "avsettings.h"
#include "propertymap.h"

using namespace std;

AVSettings::AVSettings() {
    videocodecsettings = new PropertyMap;
    audiocodecsettings = new PropertyMap;
    formatsettings = new PropertyMap;
    ResetToDefaults();
}

AVSettings::~AVSettings() {
    delete formatsettings;
    delete audiocodecsettings;
    delete videocodecsettings;
}


string AVSettings::serialize() {
    string result;

    result = ioCommon::Printf("<AVSettings width=\"%u\" height=\"%u\" pixelaspect=\"%f\" fps=\"%f\">",
        width, height, pixelaspect, fps);
    result += ioCommon::PrintfBig(65536,"<vidformat type=\"%s\">%s</vidformat>",
              vidformat.c_str(),formatsettings->serialize().c_str());
    result += ioCommon::PrintfBig(65536,"<videocodec type=\"%s\">%s</videocodec>",
              videocodec.c_str(),videocodecsettings->serialize().c_str());
    result += ioCommon::PrintfBig(65536,"<audiocodec type=\"%s\">%s</audiocodec>",
              audiocodec.c_str(),audiocodecsettings->serialize().c_str());
    result += "</AVSettings>";
    return result;
}

bool AVSettings::unserialize(const string& data) {
// TODO: Implement AVSettings::unserialize
    return false;
}

void AVSettings::ResetToDefaults() {
    width = 720;
    height = 480;
    fps = 29.997;
    pixelaspect = 1.0;
    vidformat = "avi";
    videocodec = "";
    audiocodec = "";
    videocodecsettings->data.clear();
    audiocodecsettings->data.clear();
    formatsettings->data.clear();
}

