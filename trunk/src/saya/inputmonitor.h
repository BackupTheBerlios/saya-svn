/*******************************************************************
 * Name:      inputmonitor.h
 * Purpose:   Declaration for the AVPlayer and InputMonitor classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ******************************************************************/

#ifndef inputmonitor_h
#define inputmonitor_h

class syString;

#include "core/avcontroller.h"
#include "core/events.h"

class AVPlayerEvent : public syEvent {
    public:
        enum PlayerEventId {
            idNone = 0,
            idStop = 1, /**< Stops playback. */
            idPlay = 2, /**< Starts/Resumes playback. */
            idPause = 3, /**< Pauses playback. */
            idGotoFirstFrame = 4, /**< Goes to the clip's first frame. */
            idGotoLastFrame = 5, /**< Goes to the clip's last frame. */
            idGotoNextFrame = 6, /**< Skips forward one frame. */
            idGotoPrevFrame = 7, /**< Skips back one frame. */
            idFastForward = 8, /**< Fast Forwards at 2X. */
            idFastRewind = 9, /** < Fast Rewinds at 2X. */
            idGotoSpecificFrame = 10, /**< Jumps to a specific frame. The parameter indicates the frame to go to. */
            idGotoSpecificTime = 11, /**< Jumps to a specific time. The parameter is the nanoseconds from the start. */
            idSetSpeed = 12 /**< Sets speed. The parameter is a fixed point integer, where 1000 = 1.0x. */
        };
        long long ExtraParam;
        AVPlayerEvent(PlayerEventId id, long long extra = 0) : syEvent(id), ExtraParam(extra) {}
        AVPlayerEvent* clone() { return new AVPlayerEvent(*this); }
        virtual ~AVPlayerEvent() {}
};

class AVPlayer : public AVController, public syEvtHandler {
    public:
        AVPlayer();
        virtual ~AVPlayer();
    private:
        class Data;
        friend class Data;
        Data* m_Data;

};

class InputMonitor : public AVPlayer {
    public:

        /** Default constructor. */
        InputMonitor();

        /** Destructor. */
        virtual ~InputMonitor();

        /** @brief Sets the file to read.
         *  @param filename The file to be read.
         *  @return true on success; false if the file couldn't be set due to playback.
         *  @warning This function must be called ONLY by the main thread.
         */
        bool SetFile(syString filename);

        const syString GetFile() const;

        void Init(VideoOutputDevice* videoout, AudioOutputDevice* audioout);

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
