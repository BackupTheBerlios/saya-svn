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
#include "avcommon.h"
#include "aveffect.h"


enum ClipType {
    CTVideo = 0, // The default
    CTAudio,
    CTSubtitle
};


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
