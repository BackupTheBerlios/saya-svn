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

enum ClipType {
    CTVideo = 0, // The default
    CTAudio,
    CTSubtitle
};

enum ResourceType {
    RTSequence = 0,
    RTVideoFile,
    RTAudioFile,
    RTImageFile,
    RTOfflineFile,
    RTTitle,
    RTBarsAndTone,
    RTBlackVideo,
    RTColorMatte,
    RTUCLeader
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



class AVClip: public serializable {
    public:

        /** Default constructor */
        AVClip();
        /** Default destructor */
        virtual ~AVClip();

        virtual bool unserialize(const wxString& data);
        virtual wxString serialize();


        ClipType m_ClipType;


        // Resource information

        unsigned int m_ResourceId; // Resource from which this clip takes the info.
        unsigned int m_StartTime;  // Beginning source time (zero-based; in milliseconds)
        unsigned int m_EndTime;    // End source time (inclusive - if it's 7, millisecond #7 is included)
        unsigned int m_SourceTrack;// 0 for all; 1 for specific track in multi-track (multicam) sources.
                                   // Note that this has to be set up for the audio track, too!

        // Loop control
        unsigned int m_LoopTime;   // NOTE: In case of reversed clips, the beginning of the loop would be
                                   // m_EndTime, and the end of the loop would be m_LoopTime.
                                   // To be consistent with loops in forward clips, the last loop would include
                                   // frames from and including (m_LoopTime-1) to 0.

        bool m_Loop;               // Does the clip loop?
        unsigned int m_LoopCount;  // Does the clip loop infinitely?
                                   // 0 for infinity, 1 for once (one playback only, no repeat),
                                   // 2 for 2 repetitions (one normal, one loop), and so on.

        // Speed control
        bool m_reverse;            // The reverse flag states if we start by the last frame.
                                   // IMPORTANT!!! Reverse is applied *BEFORE* the effects!
                                   // To reverse a clip after the effects have been done, you need to add
                                   // a reverse effect at the top of the effect stack.

        unsigned int m_Duration;   // Duration in milliseconds (for stretching / compressing)

        // Effects and transitions
        std::vector<AVEffect> m_Effects; // The effects for the clip
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
