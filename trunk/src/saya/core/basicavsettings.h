/*****************************************************************
 * Name:      basicavsettings.h
 * Purpose:   Definition for the BasicAVSettings class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   LGPL version 3 or later
 ****************************************************************/
#ifndef basicavsettings_h
#define basicavsettings_h

#include "avcommon.h"

class BasicAVSettings {
    public:
        BasicAVSettings() {}
        virtual ~BasicAVSettings() {}

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

};

#endif
