/***************************************************************
 * Name:      avcommon.h
 * Purpose:   Common classes and definitions for timeline handling
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avcommon_h
#define avcommon_h

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

enum AudioBitSize /** Audio sample size */
{
    ABS8bit = 0,
    ABS16bit
};

#endif
