#ifndef AVCOMMON_H
#define AVCOMMON_H

#include <map>
#include <wx/string.h>
/***************************************************************
 * Name:      avcommon.h
 * Purpose:   Common classes and definitions for timeline handling
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

enum AVType {
    AVTVideo = 0,
    AVTAudio
};

typedef std::map<wxString,wxString> propertymap;

class VideoSettings {
    public:
        VideoSettings();
        virtual ~VideoSettings();
        void ResetToDefaults();
        unsigned int width;
        unsigned int height;
        float aspectratio;
        float fps;
        wxString vidformat;
        wxString videocodec;
        wxString audiocodec;
        propertymap videocodecsettings;
        propertymap audiocodecsettings;
        propertymap formatsettings;
};

#endif
