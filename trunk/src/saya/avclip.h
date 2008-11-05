/***************************************************************
 * Name:      avclip.cpp
 * Purpose:   Declaration for the Timeline clips class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avclip_h
#define avclip_h

#include "avcommon.h"
#include "serializable.h"

class AVClipboard;
class AVTransition;
class AVEffects;
class SMapUintUint;

enum ClipType   /** Used for clips in the timeline
                  *  For now I don't know how subtitle tracks work, so this may not be implemented */
{
    CTVideo = 0, /**<Enum value CTVideo. */
    CTAudio,     /**<Enum value CTAudio. */
    CTSubtitle   /**<Enum value CTSubtitle. */
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
        virtual bool unserialize(const std::string& src);

        /** @brief Saves serialized data. @see serializable::serialize() */
        virtual std::string serialize();


        ClipType m_ClipType;       /// The type of clip we're storing in the timeline. @see ClipType

        /** @brief The clip's unique identifier.
          *
          * Each time a Clip is created, it's given a new clip ID, which is used for linking between
          * Video and audio.
          */

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
         * - Vegeta: What does the scouter say about his power level?
         * - It's over 9000!!!
         * - WHAT, nine thousand!?!?
         * With looping enabled, and m_LoopCount set to 3:
         * - Vegeta: What does the scouter say about his power level?
         * - It's over 9000!!! It's over 9000!!! It's over 9000!!!
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

        /** @brief Clip's Post-loop start time.
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

        /** @brief Contains the stack of video/audio effects for the clip.
          *
          * All the special effects that are applied to the clip are contained in this variable.
          * It's an STL vector, so whenever you copy / paste into another clip, the original effects
          * are unmodified.
          */
        AVEffects* m_Effects;

        /** @brief Sets the ending transition for the next clip.
          *
          * Transitions are handled differently than in other editors. Transitions are created by moving
          * adjacent clips in a track, so that they overlap. Then you right-click and edit the transition's
          * properties. For this reason, transitions are needed to be stored only in the ending track.
          * When clips don't overlap, no transition is applied.
          */
        AVTransition* m_EndingTransition;

        /** @brief Hides the clip from the rendering.
          *
          * For video clips, they're made invisible. For audio clips, they're muted.
          */
        bool m_Hide;

        // Label and markers

        /** @brief Sets a color label for the track.
          *
          * Labels are simply a color index so that you can "paint" your tracks in the timeline.
          */
        unsigned short m_Label;

        /** @brief The markers for time-sync'ing the video.
          *
          * Markers are stored as an STL map of strings. The index corresponds to a moment in time,
          * while the string is the desired marker.
          */
        SMapUintUint* m_Markers; // A map that goes time => marker_id.

        /** @brief Links to the synchronized video/audio clip. Use 0 for no link.
          *
          * Synchronized video and audio clips share the same starting and ending points in the timeline.
          * When split, the resulting clips are also split. To be able to handle this, each clip has a
          * link to the other.
          * @note Audio-to-video links are weak, while video-to-audio links are strong. This means that
          * in the case of a coherency error, the audio link always follows the video link.
          * @note Important care has to be given to the process of copying clips, since created clips have
          * a new id. The link needs to be copied, too, by sinchronizing the new video clip with the new
          * audio clip.
          */
        unsigned int m_Link;

        // Video-specific information


        // Audio-specific information

        /** @brief How many channels does this track have?
          *
          * Audio clips can be altered in the number of tracks they contain. Therefore,
          * we need to specify these settings in the clip, too.
          * @see SurroundType
          */
        unsigned short m_AudioChannelCount;

        /** @brief What type of surround does this track have (Mono, Stereo, 5.1,...)?
          *
          * @see SurroundType
          */
        SurroundType m_SurroundType;

        /** @brief Solo channel configuration
          *
          * Since an audio clip can have more than one channel, you can choose to
          * mute all channels except one. Use 0 enable all channels.
          * (More advanced audio effects like mixing will be done with the effect stack)
          */
        unsigned int m_SoloChannel;  // 0 for no solo; otherwise the number of the source

        /** @brief Generalized Gain in Decibels; default is 0.0.
          *
          * Sets the default audio gain for the clip, before all the effects are processed.
          */
        double m_AudioGain;

    protected:
    private:
};

#endif
