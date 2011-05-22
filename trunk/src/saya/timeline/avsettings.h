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

#include <saya/core/basicavsettings.h>
#include "serializable.h"
#include <saya/core/systring.h>

class SMapStrStr;

class AVSettings: public serializable, public BasicAVSettings {

    public:

        /** Standard constructor. */
        AVSettings();
        /** Standard destructor. */
        virtual ~AVSettings();

        /** Resets the settings to their default values. */
        void ResetToDefaults();

        /** Specifies the container format of the video. */
        syString vidformat;

        /** Specifies the codec in which the video is encoded. */
        syString videocodec;

        /** Specifies the codec in which the audio is encoded. */
        syString audiocodec;

        /** Specifies the settings for the video codec. */
        SMapStrStr* videocodecsettings;

        /** Specifies the settings for the audio codec. */
        SMapStrStr* audiocodecsettings;

        /** Specifies the settings for the container format. */
        SMapStrStr* formatsettings;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;
};

#endif
