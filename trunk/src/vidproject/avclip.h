#ifndef AVCLIP_H
#define AVCLIP_H

#include <wx/string.h>
#include <vector>
#include <map>


enum ClipType {
    CTVideo = 0, // The default
    CTAudio,
    CTSubtitle
};

enum AVType {
    AVTVideo = 0,
    AVTAudio
};

typedef std::map<wxString,wxString> FXParameterList; // parameter list for Effect stack.
// For simplicity, we'll use strings. At rendering, we'll find out a way to optimize this,
// like precalculating the parameter values or keeping a cache, so we only have to parse the values ONCE.
// Remember that all these data structures are only for information storage; rendering will use more advanced
// data structures independent from these.


class FXKeyFrame {
    FXParameterList m_ParameterList;
    signed char m_inSlope;
    signed char m_outSlope;

    // Slope of the curve before arriving to the inpoint (or after leaving the outpoint).
    // -128 = 0 degrees. 127 = MAX (nearly 90 degrees). Default = 0.

    // NOTE:
    // If we have keyframes A and B, and A has parameter "x" 0, and B has parameter "x" 100, if the slope for both
    // keyframes is 127, the curve to both points would be a straight line, meaning that between those two keyframes,
    // parameter "x" would be 50.

    //  outSlope(A) = 0; inSlope(B) = 0
    //       B
    //      /
    //     /
    //   A/

    //  outSlope(A) = 127; inSlope(B) = -128
    //      ____ B
    //     /
    //   A |


    //  outSlope(A) = -128; inSlope(B) = 127
    //           B
    //           |
    //   A ______/

    //  outSlope(A) = 127; inSlope(B) = 127. Note that in the middle point, the curve becomes a straight line.
    //        |B
    //        |
    //       /
    //      /
    //     /
    //    |
    //   A|

    //  outSlope(A) = -128; inSlope(B) = -128. Note that in the middle point, the curve becomes a straight line.
    //          ___B
    //         /
    //        /
    //   A___/

    // IMPORTANT NOTE!!! The algorithm for calculating the actual points has yet to be designed!!!

};

typedef std::map<unsigned int,FXKeyFrame> FXTimeline;
// Parameters have a timeline.
// The key is a frame index, where the first point is the clip's first frame.
// IMPORTANT NOTE! If a clip is reversed, the reversal will be interpreted as the first "effect" (before transitions
// are applied). If you want to reverse a final processed clip, you'd have to add a "reverse" effect at the end.

enum SurroundType {
    STMono = 0, // Mono (1 channel)
    STStereo,   // Stereo (Left, right)
    STSurround51, // (5.1 surround)
    STSurround71, // (7.1 surround)
    STSurround102, // (10.2 surround)
    STOther,
    STReserved6, STReserved7, STReserved8, STReserved9,STReserved10,
    STReserved11,STReserved12,STReserved13,STReserved14,STReserved15
};

class AVTransition {
    public:
        AVType m_TransitionType;
    // TODO (rick#1#): Design the transition class

    // IMPORTANT!!! Transitions MUST be applied *AFTER* all the effects have been done! Failing to do so
    // Would result in defective transitions, i.e. a resized/rotated clip would suddenly be of a wrong size.

};

class AVEffect {
    public:
        AVType m_EffectType;
        wxString EffectName; // Standard effect name; all lowercase. Only numbers, letters and underscore allowed.
        FXTimeline m_Timeline; // remember that this timeline is on THE CLIP, not on the track!
                               // (And yet, it uses the track's fps)
        bool m_Enabled;
        unsigned int m_StartFrame; // Beginning frame for the effect (this is independent from the timeline)
        unsigned int m_EndFrame; // Ending frame for the effect (independent from the timeline)
        AVEffect();
        virtual ~AVEffect();
};

class AVClip
{
    public:


        /** Default constructor */
        AVClip();
        /** Default destructor */
        ~AVClip();

        ClipType m_ClipType;


        // Resource information

        unsigned int m_ResourceId; // Resource from which this clip takes the info.
        unsigned int m_StartFrame; // Beginning source frame (zero-based; zero is first frame)
        unsigned int m_EndFrame;   // End source frame (inclusive - if it's 7, frame #7 is included)
        unsigned int m_SourceTrack;// 0 for all; 1 for specific track in multi-track (multicam) sources.
                                   // Note that this has to be set up for the audio track, too!

        // Loop control
        unsigned int m_LoopFrame;  // NOTE: In case of reversed clips, the beginning of the loop would be
                                   // m_EndFrame, and the end of the loop would be m_LoopFrame.
                                   // To be consistent with loops in forward clips, the last loop would include
                                   // frames from (m_LoopFrame-1) to 0.

        bool m_Loop;               // Does the clip loop?
        unsigned int m_LoopCount;  // Does the clip loop infinitely?
                                   // 0 for infinity, 1 for once (one playback only, no repeat),
                                   // 2 for 2 repetitions (one normal, one loop), and so on.

        // Speed control
        bool m_reverse;            // The reverse flag states if we start by the last frame.
                                   // IMPORTANT!!! Reverse is applied *BEFORE* the effects!
                                   // To reverse a clip after the effects have been done, you need to add
                                   // a reverse effect at the top of the effect stack.

        unsigned int m_Duration;   // Duration in timeline frames (for stretching / compressing)
                                   // Note that this duration will be used with the fps of the timeline,
                                   // Duration is applied also before any special effects.

        std::vector<AVEffect> m_Effects; // The effects for the clip

        // Video-specific information

        unsigned int m_AudioClip;  // Link to audio clip; 0 for none. Ignored if this is an audio clip.
        unsigned int m_BeginningTransition; // id for the beginning transition; 0 for none.
        unsigned int m_EndingTransition; // id for the ending transition; 0 for none.
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

class AVSequence;
class AVTrack;
class AVTimeline;

class AVSequence {
    public:
        wxString m_SeqName;
        unsigned int m_ResourceId; // Sequences are their own resources, they can be inserted into clips!
        std::vector<unsigned int> m_ChildrenTracks;
        std::vector<AVTrack> m_Tracks; // the Actual track data
};

class AVTrack {
    public:
        unsigned int m_TrackId;
        wxString m_TrackName;
        bool m_Locked;
        bool m_Hidden;
        unsigned int m_ParentTrack; // Tracks can be nested! Parent 0 means the parent is the sequence itself.
        std::vector<unsigned int> m_ChildrenTracks;
        // Tracks of different sequences can have the same ID#s. But tracks cannot be moved from a Sequence
        // to another. This is by design so that links between Audio and Video clips cannot be broken.

        // TODO (rick#2#): Add the data structures for containing the clips in the tracks.

};

class AVTimeline {
    public:
        std::vector<AVSequence> m_Sequences;
};

#endif // AVCLIP_H
