/*****************************************************************
 * Name:      avsettings.h
 * Purpose:   Definition for the AVSettings class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ****************************************************************/

#ifndef avsettings_h
#define avsettings_h

#include "avcommon.h"
#include "serializable.h"

class PropertyMap;

class AVSettings: public serializable {

    public:

        /** Standard constructor. */
        AVSettings();
        /** Standard destructor. */
        virtual ~AVSettings();

        /** Resets the settings to their default values. */
        void ResetToDefaults();

        /** Specifies the width of the video in pixels. */
        unsigned int width;

        /** Specifies the height of the video in pixels. */
        unsigned int height;

        /** Specifies the pixels' aspect ratio. */

        float pixelaspect;

        /** Specifies the video's frame rate. */
        float fps;

        /** Specifies the video's interlacing. */
        InterlaceType interlacing;

        /** Specifies the audio sample rate */
        unsigned int samplerate;

        /** Specifies the audio sample size */
        AudioBitSize samplesize;

        /** Specifies the surround type */
        SurroundType surround;

        /** Specifies the number of audio channels (depens on the surround field above) */
        unsigned int channels;

        /** Specifies the container format of the video. */
        std::string vidformat;

        /** Specifies the codec in which the video is encoded. */
        std::string videocodec;

        /** Specifies the codec in which the audio is encoded. */
        std::string audiocodec;

        /** Specifies the settings for the video codec. */
        PropertyMap* videocodecsettings;

        /** Specifies the settings for the audio codec. */
        PropertyMap* audiocodecsettings;

        /** Specifies the settings for the container format. */
        PropertyMap* formatsettings;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
