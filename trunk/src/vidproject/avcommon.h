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

class serializable {
    public:
        serializable() {}

        /** Loads serialized data into the object.
         *  @param data The data to unserialize into the object
         *  @return the result. true if successful.
         */
        virtual bool unserialize(const wxString& data);

        /** Compresses the object data into a serialized string format.
         *  @param data The data to unserialize into the object
         *  @return The serialized data
         */
        virtual wxString serialize();

        virtual ~serializable() {}
};


class VideoSettings: public serializable {
    public:
        /** Standard constructor. */
        VideoSettings();
        /** Standard destructor. */
        virtual ~VideoSettings();

        /** Resets the settings to their default values. */
        void ResetToDefaults();

        /** Specifies the width of the video in pixels. */
        unsigned int width;

        /** Specifies the height of the video in pixels. */
        unsigned int height;

        /** Specifies the aspect ratio of the video. */
        float aspectratio;

        /** Specifies the fps rate of the video. */
        float fps;

        /** Specifies the container format of the video. */
        wxString vidformat;

        /** Specifies the codec in which the video is encoded. */
        wxString videocodec;

        /** Specifies the codec in which the audio is encoded. */
        wxString audiocodec;

        /** Specifies the settings for the video codec. */
        propertymap videocodecsettings;

        /** Specifies the settings for the audio codec. */
        propertymap audiocodecsettings;

        /** Specifies the settings for the container format. */
        propertymap formatsettings;

        /** @see serializable::unserialize */
        virtual bool unserialize(const wxString& data);

        /** @see serializable::serialize */
        virtual wxString serialize();
};

#endif
