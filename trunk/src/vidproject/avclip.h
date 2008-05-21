#ifndef AVCLIP_H
#define AVCLIP_H

/***************************************************************
 * Name:      avclip.cpp
 * Purpose:   Definition for Timeline clips and tracks
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <wx/string.h>
#include <vector>
#include <map>
#include <deque>
#include "avcommon.h"
#include "aveffect.h"

class AVSequence;
class AVTrack;
class AVTimeline;
class AVClip;
class AVClipboard;
class AVResource;

enum ClipType   /** Used for clips in the timeline
                  *  For now I don't know how subtitle tracks work, so this may not be implemented */
{
    CTVideo = 0, /**<Enum value CTVideo. */
    CTAudio,     /**<Enum value CTAudio. */
    CTSubtitle   /**<Enum value CTSubtitle. */
};

enum ResourceType   /** Used for the video resources. */
{
    RTSequence = 0, /**<Enum value RTSequence. */
    RTVideoFile,    /**<Enum value RTVideoFile. */
    RTAudioFile,    /**<Enum value RTAudioFile. */
    RTImageFile,    /**<Enum value RTImageFile. */
    RTOfflineFile,  /**<Enum value RTOfflineFile. */
    RTTitle,        /**<Enum value RTTitle. */
    RTBarsAndTone,  /**<Enum value RTBarsAndTone. */
    RTBlackVideo,   /**<Enum value RTBlackVideo. */
    RTColorMatte,   /**<Enum value RTColorMatte. */
    RTUCLeader      /**<Enum value RTUCLeader. */
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


/*! @class AVClip avclip.h
 *  @brief Defines a clip in the timeline.
 *
 *  AVClip is the base of our whole video editing framework.
 *  It contains a numeric link to the resource used,
 *  which effects and transitions are applied,
 *  whether it's video or audio, the loop count, start and ending frames (actually time),
 *  etc.
 */

class AVClip: public serializable {
    public:

        /** Default constructor */
        AVClip();
        /** Default destructor */
        virtual ~AVClip();

        /** @brief Loads serialized data. @see serializable::unserialize() */
        virtual bool unserialize(const wxString& data);

        /** @brief Saves serialized data. @see serializable::serialize() */
        virtual wxString serialize();


        ClipType m_ClipType;       /// The type of clip we're storing in the timeline. @see ClipType


        // Resource information

        unsigned int m_ResourceId; /// Resource from which this clip takes the info.
        unsigned int m_SourceStartTime;  /// Beginning source time (zero-based; in milliseconds)
        unsigned int m_SourceEndTime;    /// End source time (inclusive - if it's 7, millisecond #7 is included)

        /** @brief Resource's track used for the clip
          *
          * Resouces (sequences) with more than one track can be used for multicam clips.
          * If you assign a number greater than 0 to m_SourceTrack, the clip's source is taken
          * @b ONLY from the resource's specified track.
          * @note Audio clips need to specify this, too!
          */
        unsigned int m_SourceTrack;


        /** @brief Specifies a moment to start loops in a clip
          *
          * Sometimes you wish to repeat certain part of a clip a certain number of times, or even
          * infinite times. Instead of having to copy/paste the clip over and over, you only have to
          * set the loop start/end, the loopcount, and enable looping. This way you can expand the clip using
          * the ripple edit or the rolling edit tool.
          * The clip's default duration will be recalculated to reflect this. @see AVClip::GetDefaultDuration()
          * @note In reversed clips, m_LoopStart and m_LoopEnd are switched automatically.
          */
        unsigned int m_LoopStart;

        unsigned int m_LoopEnd;

        /** @brief Set to true to enable looping. */
        bool m_Loop;

        /** @brief  Determines the number of passes that a loop contains.
         *
         *  0 equals infinite passes,
         *  1 sets one pass (same as no looping)
         *  2 sets two passes (one repetition)
         * Example: With looping disabled:
         * - Vegeta: What is Kakaroto's power level?
         * - It's more than 9000!!!
         * - WHAT, nine thousand!?!?
         * With looping enabled, and m_LoopCount set to 3:
         * - Vegeta: What is Kakaroto's power level?
         * - It's more than 9000!!! It's more than 9000!!! It's more than 9000!!!
         * - WHAT, nine thousand!?!?
         */
        unsigned int m_LoopCount;

        /**  @brief Set to true to make the loops go in a zig-zag sequence.
         *
         *   VJs will appreciate this feature. Instead of looping normally, even loops
         *   do a reverse iteration. If you loop a person turning his head from left
         *   to right, the loop will make the person turn his head right (first pass, no loop),
         *   then left (second pass), and then right again (third pass).
         *   @note If the number of loops is even, the clip will continue in reverse from
         *   m_LoopStart down to 0.
         */
        bool m_LoopZigZag;

        // Speed control

        /** @brief Clip's Post-loop start time
          *
          * After the clip loops have been processed, the clip's duration has changed.
          * To successfully split clips, we need to have a post-loop start time and end time.
          * These are the variables that are modified when using the rolling / ripple edit tool.
          */
        unsigned int m_PostLoopStartTime;
        unsigned int m_PostLoopEndTime;


        /** @brief Obtains the clip's default duration.
         *  The clip's default duration (in milliseconds) is obtained by first dividing the resource's
         *  number of frames over the resource's fps, and then adding the number of loops in the clip.
         *  For infinite loops, only one pass will be accounted.
         *  @return The clip's default duration.
         */
        unsigned int GetDefaultDuration();

        /** @brief Set to true to make the clip go from the last frame to the first frame.
         *
         *  The reverse flag is useful to make a clip go backwards.
         *  The order of processing is the following:
         *  First, the loop variables are processed.
         *  After the loop count has been established, the m_reverse flag is analyzed.
         *  Finally, the video effects and transitions are applied.
         *  @note To make a clip go in reverse after some effects have been applied, apply
         *  a reverse effect on the clip.
         */
        bool m_reverse;

        /** @brief Sets the clip duration for speeding up / slowing down purposes.
          *
          * The m_Duration variable sets the clip duration in milliseconds, speeding it up
          * or slowing it down in consequence.
          * @note The duration is set after looping has been processed.
          */
        unsigned int m_Duration;

        // Effects and transitions

        /** @brief Contains the stack of video/audio effects for the clip
          *
          * All the special effects that are applied to the clip are contained in this variable.
          * It's an STL vector, so whenever you copy / paste into another clip, the original effects
          * are unmodified.
          */
        std::vector<AVEffect> m_Effects;
        AVTransition m_EndingTransition; // Ending transition


        // Label and markers
        unsigned short m_Label;    // Color label for the track

        std::map<unsigned int,wxString> m_Markers; // A map that goes time => marker_id.

        // Video-specific information

        unsigned int m_AudioClip;  // Link to audio clip; 0 for none. Ignored if this is an audio clip.

                                   // NOTE: Video and audio transitions are separate!
        bool m_Hide;               // Hides the video

        // Audio-specific information

        unsigned int m_VideoClip;  // Link to video clip; 0 for none. Ignored if this is a video clip.
        // Unlinking is done first by the Video clip; therefore, the audio clip must always check if it's linked
        // by the video. In other words, the m_VideoClip link is provided only for a faster access, but the
        // video -> audio link is normative. Without the audio->video link, we'd have to search for all video clips
        // to see which one would link to the audio clip.


        unsigned short m_AudioChannelCount; // How many channels does this track have?
        SurroundType m_SurroundType; // What type of sound configuration does this channel have?
        unsigned int m_SoloChannel;  // 0 for no solo; otherwise the number of the source
        bool m_Mute;                 // Mutes the audio
        double m_AudioGain;           // Generalized Gain in Decibels; default is 0.0.

    protected:
    private:
};

// TODO (rick#1#): Finish designing the AVSequence, AVTrack and AVTimeline classes.


class AVSequence: public serializable {
    public:
        AVSequence();
        wxString m_SeqName;
        unsigned int m_ResourceId; // Sequences are their own resources, they can be inserted into clips!
        // Since tracks are only maps of integers, copying/moving them isn't time critical, and is seldom
        // performed (i.e. when dragging a track, adding / removing tracks). Having pointers or indexes for
        // tracks isn't just worth it.

        std::vector<AVTrack> m_VideoTracks;
        std::vector<AVTrack> m_AudioTracks;
        unsigned int m_CursorPosition;
        unsigned int m_BeginWorkArea;
        unsigned int m_EndWorkArea;

        virtual bool unserialize(const wxString& data);
        virtual wxString serialize();

        virtual ~AVSequence() {}
};

class AVClipboard: public AVSequence {
        AVClipboard() {}
        unsigned int m_StartFrame; // Beginning timeline frame (zero-based)
        unsigned int m_EndFrame;   // End timeline frame (inclusive)
        bool m_Singleclip;
        virtual ~AVClipboard() {}
};

class AVTrack:public serializable {
    public:
        AVType m_AVType;
        bool m_Readonly;
        bool m_Hidden;
        std::map<unsigned int,AVClip> m_Clips;
        AVTrack();
        virtual ~AVTrack();
        virtual bool unserialize(const wxString& data);
        virtual wxString serialize();
};

class AVTimeline:public serializable {
    public:
        AVTimeline();
        virtual ~AVTimeline();
        std::vector<AVSequence> m_Sequences;
        virtual bool unserialize(const wxString& data);
        virtual wxString serialize();
};

class AVResource:public serializable {
    public:
        AVResource();
        virtual ~AVResource();
        unsigned int m_ResourceId;
        ResourceType m_ResourceType;
        wxString m_Filename;
        wxString m_RelativeFilename;
        wxString m_Icon; // 64x64 JPEG icon encoded with base64
        VideoSettings m_VideoSettings;
        virtual bool unserialize(const wxString& data);
        virtual wxString serialize();
};

typedef std::map<unsigned int, AVResource> AVResources;

#endif // AVCLIP_H
