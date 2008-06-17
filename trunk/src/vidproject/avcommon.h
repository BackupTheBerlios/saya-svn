#ifndef AVCOMMON_H
#define AVCOMMON_H

#include <map>
#include <string>
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

enum SurroundType   /** Used for audio tracks */
{
    STMono = 0,     /**<Enum value STMono. Mono (1 channel) */
    STStereo,       /**<Enum value STStereo. Stereo (Left, right) */
    STSurround51,   /**<Enum value STSurround51. 5.1 surround */
    STSurround71,   /**<Enum value STSurround71. 7.1 surround */
    STSurround102,  /**<Enum value STSurround102. 10.2 surround */
    STOther,        /**<Enum value STOther. Other. */
    STReserved6, STReserved7, STReserved8, STReserved9,STReserved10,
    STReserved11,STReserved12,STReserved13,STReserved14,STReserved15
};

enum InterlaceType  /** Used for video clips and resources */
{
    ITProgressive = 0,
    ITBottomFirst,
    ITTopFirst
};

enum AspectRatioType /** Aspect ratio */
{
    ARStandard = 0, /** Standard (4:3) */
    ARWide, /** Wide screen (16:9) */
    ARAgnostic, /** Aspect-ratio agnostic (square pixels) */
    ARCustom /** Determined by pixel aspect ratio */
};

enum AudioBitSize /** Audio sample size */
{
    ABS8bit = 0,
    ABS16bit
};

typedef std::map<std::string,std::string> propertymap;

class serializable {
    public:
        serializable() {}

        /** Loads serialized data into the object.
         *  @param data The data to unserialize into the object
         *  @return the result. true if successful.
         */
        virtual bool unserialize(const std::string& data);

        /** Compresses the object data into a serialized string format.
         *  @param data The data to unserialize into the object
         *  @return The serialized data
         */
        virtual std::string serialize();

        virtual ~serializable() {}
};


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

        /** Specifies the aspect ratio of the video. */
        AspectRatioType aspectratio;

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
        propertymap videocodecsettings;

        /** Specifies the settings for the audio codec. */
        propertymap audiocodecsettings;

        /** Specifies the settings for the container format. */
        propertymap formatsettings;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& data);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
