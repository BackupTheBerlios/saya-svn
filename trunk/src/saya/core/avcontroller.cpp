/***************************************************************
 * Name:      avcontroller.cpp
 * Purpose:   Implementation of the AVController class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avcontroller.h"
#include "sythread.h"
#include "videoinputdevice.h"
#include "videooutputdevice.h"
#include "audioinputdevice.h"
#include "audiooutputdevice.h"

#include "debuglog.h" // Remove when debugging is finished
#include "systring.h" // Remove when debugging is finished

#include <cstddef>
#include <math.h>

namespace AVControllerConsts {

    const float MinimumPlaybackSpeed = 0.01;
    const float MinimumFramerate = 1.0;
    const float MaximumFramerate = 300.0;
    const float DefaultMaximumFramerate = 75.0;
    float CurrentMaximumFramerate = 75.0;
    unsigned int MinimumPlaybackDelay = 1000 / static_cast<unsigned int>(DefaultMaximumFramerate);
    const unsigned long DefaultAudioGranularity = 100;
    const unsigned long MinimumAudioGranularity = 10;

    // 1 meg for stack is quite a good size. We're using 4 threads max, so 4 megs isn't a bad size.
    const unsigned long AVThreadStackSize = 1048576;
};

using namespace AVControllerConsts;

class AVControllerData {
    public:
        /** Constructor */
        AVControllerData(AVController* parent);

        /** Destructor */
        ~AVControllerData();

        /** @brief Starts the playback/encoding thread for encoding only. Called by the Play*() functions.
         *
         *  @note Will do nothing if any of the Output Devices is for encoding.
         */
        void StartPlayback();

        /** Starts the playback/encoding thread.
         *  StartEncoding() sets the playback data and proceeds to call StartWorkerThreads().
         *
         *  @note Will do nothing if any of the Output Devices is for playback.
         */
        void StartEncoding();

        /** @brief Pauses playback/encoding. Extra frames are NOT sent to the output. */
        void Pause();

        /** @brief Stops playback/encoding. Extra frames are NOT sent to the output. */
        void Stop();

        /** @brief Starts the playback/encoding threads. Called by StartEncoding().
         *  @see AVController::StartEncoding()
         *  @return true on success; false otherwise.
         */
        bool StartWorkerThreads();

        /** @brief Shuts down the A/V devices without setting them to null.
         *  This is required for Init().
         */
        void ShutdownDevices();


        /** Loop for Audio In. Returns false if the thread must exit. */
        bool AudioInLoop();
        /** Loop for Video In. Returns false if the thread must exit. */
        bool VideoInLoop();
        /** Loop for Audio Out. Returns false if the thread must exit. */
        bool AudioOutLoop();
        /** Loop for Video Out. Returns false if the thread must exit. */
        bool VideoOutLoop();

        /** Playback Loop for Audio In. */
        void PlaybackAudioInLoop();
        /** Playback Loop for video In. */
        void PlaybackVideoInLoop();
        /** Playback Loop for Audio Out. */
        void PlaybackAudioOutLoop();
        /** Playback Loop for Video Out. */
        void PlaybackVideoOutLoop();

        /** Encoding Loop for Audio In. */
        void EncodingAudioInLoop();
        /** Encoding Loop for video In. */
        void EncodingVideoInLoop();
        /** Encoding Loop for Audio Out. */
        void EncodingAudioOutLoop();
        /** Encoding Loop for Video Out. */
        void EncodingVideoOutLoop();

        /** Returns true if the Video In thread is running. */
        bool IsVideoInRunning();
        /** Returns true if the Audio In thread is running. */
        bool IsAudioInRunning();
        /** Returns true if the Video Out thread is running. */
        bool IsVideoOutRunning();
        /** Returns true if the Audio Out thread is running. */
        bool IsAudioOutRunning();

        /** Returns true if any of the threads are running, false otherwise. */
        bool IsPlaying();

        /** Current playback framerate. */
        float m_FrameRate;

        /** Flag for video-only or audio-only playback/encoding. */
        bool m_VideoEnabled;

        /** Flag for video-only or audio-only playback/encoding. */
        bool m_AudioEnabled;

        /** Flag for not-skipping video frames on playback. */
        bool m_StutterMode;

        /** @brief Flag determining whether we're currently playing or not.
         *
         *  @note For checking paused states, use the IsPlaying() function.
         */
        volatile bool m_IsPlaying;

        /** Flag indicating that playback / encoding must be stopped. */
        volatile bool m_Stop;

        /** Flag indicating that playback / encoding must be paused. */
        volatile bool m_Pause;

        /** Playback/encoding speed. 1.0 = normal speed; -1.0 = normal speed, reversed. */
        volatile float m_PlaybackSpeed;

        /** Playback/encoding duration, in nanoseconds. */
        volatile avtime_t m_PlaybackDuration;

        /** Video In */
        VideoInputDevice* m_VideoIn;

        /** Audio In */
        AudioInputDevice* m_AudioIn;

        /** Video Out */
        VideoOutputDevice* m_VideoOut;

        /** Audio Out */
        AudioOutputDevice* m_AudioOut;

        /** Sets the minimum quantity of data that will be sent to the AudioOutputDevice.
         *
         *  @see AVController::SetAudioGranularity
         */
        volatile unsigned long m_AudioGranularity;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        volatile avtime_t m_StartVideoPos;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        volatile avtime_t m_StartAudioPos;

        /** The current position for video in the current playback. Used for relative seeking. */
        volatile avtime_t m_CurrentVideoPos;

        /** The current position for audio in the current playback. Used for relative seeking. */
        volatile avtime_t m_CurrentAudioPos;

        AVController* m_Parent;

        /** Thread for handling Audio Input. */
        syThread* m_AudioInThread;
        /** Thread for handling Video Input. */
        syThread* m_VideoInThread;
        /** Thread for handling Audio Output. */
        syThread* m_AudioOutThread;
        /** Thread for handling Video Output. */
        syThread* m_VideoOutThread;
};

// ------------------------------
// Begin Auxiliary thread classes
// ------------------------------

class syAudioInThread : public syThread {
    friend class AVControllerData;
    public:
        syAudioInThread(AVControllerData* parent);
        virtual int Entry();

    private:
        AVControllerData* m_Parent;
};

class syVideoInThread : public syThread {
    friend class AVControllerData;
    public:
        syVideoInThread(AVControllerData* parent);
        virtual int Entry();

    private:
        AVControllerData* m_Parent;
};

class syAudioOutThread : public syThread {
    friend class AVControllerData;
    public:
        syAudioOutThread(AVControllerData* parent);
        virtual int Entry();

    private:
        AVControllerData* m_Parent;
};

class syVideoOutThread : public syThread {
    friend class AVControllerData;
    public:
        syVideoOutThread(AVControllerData* parent);
        virtual int Entry();

    private:
        AVControllerData* m_Parent;
};

syAudioInThread::syAudioInThread(AVControllerData* parent) :
syThread(syTHREAD_JOINABLE),
m_Parent(parent)
{
}

syVideoInThread::syVideoInThread(AVControllerData* parent) :
syThread(syTHREAD_JOINABLE),
m_Parent(parent)
{
}

syAudioOutThread::syAudioOutThread(AVControllerData* parent) :
syThread(syTHREAD_JOINABLE),
m_Parent(parent)
{
}

syVideoOutThread::syVideoOutThread(AVControllerData* parent) :
syThread(syTHREAD_JOINABLE),
m_Parent(parent)
{
}

int syAudioInThread::Entry() {
    while(!MustAbort()) {
        if(!m_Parent->m_AudioEnabled) {
            SelfPause();
        }
        if(!m_Parent->AudioInLoop()) break;
    }
    return 0;
}

int syVideoInThread::Entry() {
    while(!MustAbort()) {
        if(!m_Parent->m_VideoEnabled) {
            SelfPause();
        }
        if(!m_Parent->VideoInLoop()) break;
    }
    return 0;
}

int syAudioOutThread::Entry() {
    while(!MustAbort()) {
        if(!m_Parent->m_AudioEnabled) {
            SelfPause();
        }
        if(!m_Parent->AudioOutLoop()) break;
    }
    return 0;
}

int syVideoOutThread::Entry() {
    while(!MustAbort()) {
        if(!m_Parent->m_VideoEnabled) {
            SelfPause();
        }
        if(!m_Parent->VideoOutLoop()) break;
    }
    return 0;
}

// ----------------------------
// End Auxiliary thread classes
// ----------------------------

// ----------------------
// begin AVControllerData
// ----------------------

AVControllerData::AVControllerData(AVController* parent) :
m_FrameRate(30),
m_VideoEnabled(true),
m_AudioEnabled(true),
m_StutterMode(false),
m_IsPlaying(false),
m_PlaybackSpeed(1.0),
m_PlaybackDuration(0),
m_VideoIn(NULL),
m_AudioIn(NULL),
m_VideoOut(NULL),
m_AudioOut(NULL),
m_AudioGranularity(DefaultAudioGranularity),
m_StartVideoPos(0),
m_StartAudioPos(0),
m_CurrentVideoPos(0),
m_CurrentAudioPos(0),
m_Parent(parent),
m_AudioInThread(new syAudioInThread(this)),
m_VideoInThread(new syVideoInThread(this)),
m_AudioOutThread(new syAudioOutThread(this)),
m_VideoOutThread(new syVideoOutThread(this))
{
    m_AudioInThread->Create(AVThreadStackSize);
    m_VideoInThread->Create(AVThreadStackSize);
    m_AudioOutThread->Create(AVThreadStackSize);
    m_VideoOutThread->Create(AVThreadStackSize);
}

AVControllerData::~AVControllerData() {
    m_VideoOutThread->Delete();
    m_AudioOutThread->Delete();
    m_VideoInThread->Delete();
    m_AudioInThread->Delete();
}

void AVControllerData::StartEncoding() {
    if(!m_Parent->IsEncoderOnly()) { return; } // Playback streams do not concern us.
    m_StartVideoPos = 0;
    m_StartAudioPos = 0;
    if(m_VideoIn) {
        m_StartVideoPos = m_VideoIn->GetPos();
    }
    if(m_AudioIn) {
        m_StartAudioPos = m_AudioIn->GetPos();
    }
    // And start!
    StartWorkerThreads();
}

inline bool AVControllerData::IsVideoInRunning() {
    return m_VideoInThread && m_VideoInThread->IsRunning();
}
inline bool AVControllerData::IsAudioInRunning() {
    return m_AudioInThread && m_AudioInThread->IsRunning();
}
inline bool AVControllerData::IsVideoOutRunning() {
    return m_VideoOutThread && m_VideoOutThread->IsRunning();
}
inline bool AVControllerData::IsAudioOutRunning() {
    return m_AudioOutThread && m_AudioOutThread->IsRunning();
}

bool AVControllerData::IsPlaying() {
    if(IsVideoInRunning()) return true;
    if(IsAudioInRunning()) return true;
    if(IsVideoOutRunning()) return true;
    if(IsAudioOutRunning()) return true;
    return false;
}

inline void AVControllerData::Pause() {
    m_Pause = true;
    if(syThread::IsMain()) {
        while(IsPlaying()) {
            syMilliSleep(1);
        }
    }
    m_IsPlaying = false;
}

inline void AVControllerData::Stop() {
    m_Stop = true;
    if(syThread::IsMain()) {
        while(IsPlaying()) {
            syMilliSleep(1);
        }
    }
    m_IsPlaying = false;
}

void AVControllerData::ShutdownDevices() {
    if(!syThread::IsMain()) { return; }
    Stop();

    if(m_AudioOut) {
        m_AudioOut->ShutDown();
    }
    if(m_VideoOut) {
        m_VideoOut->ShutDown();
    }

    if(m_AudioIn) {
        m_AudioIn->ShutDown();
    }
    if(m_VideoIn) {
        m_VideoIn->ShutDown();
    }
}

//// ---------------------------
//// Begin Generic Loop Wrappers
//// ---------------------------

bool AVControllerData::AudioInLoop() {
    while(m_Pause || m_Stop || !m_AudioEnabled) {
        if(syThread::MustAbort() || !m_AudioEnabled) return false;
        if(!m_AudioInThread->SelfPause()) return false;
    }
    if(m_Parent->IsAudioEncoder()) {
        EncodingAudioInLoop();
    } else {
        PlaybackAudioInLoop();
    }
    return true;
}

bool AVControllerData::VideoInLoop() {
    while(m_Pause || m_Stop || !m_VideoEnabled) {
        if(syThread::MustAbort() || !m_VideoEnabled) return false;
        if(!m_VideoInThread->SelfPause()) return false;
    }
    if(m_Parent->IsVideoEncoder()) {
        EncodingVideoInLoop();
    } else {
        PlaybackVideoInLoop();
    }
    return true;
}

bool AVControllerData::AudioOutLoop() {
    while(m_Pause || m_Stop || !m_AudioEnabled) {
        if(syThread::MustAbort() || !m_AudioEnabled) return false;
        if(!m_AudioOutThread->SelfPause()) return false;
    }
    if(m_Parent->IsAudioEncoder()) {
        EncodingAudioOutLoop();
    } else {
        PlaybackAudioOutLoop();
    }
    return true;
}

bool AVControllerData::VideoOutLoop() {
    while(m_Pause || m_Stop || !m_VideoEnabled) {
        if(syThread::MustAbort() || !m_VideoEnabled) return false;
        if(!m_VideoOutThread->SelfPause()) return false;
    }
    if(m_Parent->IsVideoEncoder()) {
        EncodingVideoOutLoop();
    } else {
        PlaybackVideoOutLoop();
    }
    return true;
}

//// -------------------------
//// End Generic Loop Wrappers
//// -------------------------

//// --------------------
//// Begin Playback Loops
//// --------------------

void AVControllerData::PlaybackAudioInLoop() {
    if(m_StutterMode) return; // In stutter mode, Audio Input will be handled by the video thread.
    while(!syThread::MustAbort() && m_AudioEnabled && !m_Stop) {
        while(m_Pause) {
            if(m_Stop || syThread::MustAbort() || !m_AudioEnabled) return;
            if(!m_AudioInThread->SelfPause()) return;
        }
        #warning TODO: Implement AVControllerData::PlaybackAudioInLoop()
        syMicroSleep(10); // Remove this line after PlaybackAudioInLoop() has been implemented.
    }
}

void AVControllerData::PlaybackVideoInLoop() {
    avtime_t curvideopos, curaudiopos, nextaudiopos, starttime;
    unsigned long curvideoframe,nextvideoframe;

    starttime = syGetNanoTicks();
    m_CurrentVideoPos = curaudiopos = curvideopos = m_StartVideoPos;
    nextaudiopos = curaudiopos;
    nextvideoframe = m_VideoIn->GetFrameIndex(m_StartVideoPos);
    curvideoframe = nextvideoframe;

    if(!syThread::MustAbort() && m_VideoEnabled && !m_Stop && m_VideoIn && m_VideoOut) {
        m_VideoIn->SendCurrentFrame(m_VideoOut);
    }

    if(m_StutterMode && m_AudioEnabled) {
        m_CurrentAudioPos = curaudiopos = m_AudioIn->Seek(curvideopos);
    }

    while(!syThread::MustAbort() && m_VideoEnabled && !m_Stop && m_VideoIn && m_VideoOut) {
        while(m_Pause) {
            if(m_Stop || syThread::MustAbort() || !m_VideoEnabled || !m_VideoIn || !m_VideoOut) return;
            if(!m_VideoInThread->SelfPause()) return;
        }
        curvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
        if(curvideoframe != nextvideoframe) {
            if(m_StutterMode && m_AudioEnabled) {
                // In "stutter mode", we play back the audio using the video thread.

                // Calculate the length of the audio, in seconds.
                nextaudiopos = m_VideoIn->GetTimeFromFrameIndex(curvideoframe + 1, false);

                avtime_t audiolen = nextaudiopos - curaudiopos;
                if(audiolen > 0) {
                    m_AudioIn->SendAudioData(m_AudioOut,audiolen);
                }
            }
            m_VideoIn->SendCurrentFrame(m_VideoOut);
            curvideoframe = nextvideoframe;
        } else {
            // syMilliSleep(MinimumPlaybackDelay); // This will prevent us from surpassing the maximum framerate.
            // syMilliSleep(1); // Until the framerate/timing issue is fixed, this will have to suffice.
        }

        // Calculate the next video position based on the current time.
        curvideopos = m_StartVideoPos + (syGetNanoTicks() - starttime);
        if(m_StutterMode && nextvideoframe > curvideoframe + 1) {
            // Don't skip video frames in stutter mode.
            nextvideoframe = curvideoframe + 1;
            curvideopos = m_VideoIn->GetTimeFromFrameIndex(nextvideoframe, false);
        } else {
            nextvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
        }

        // Seek to the calculated video position.
        m_CurrentVideoPos = curvideopos = m_VideoIn->Seek(curvideopos);

        if(m_StutterMode && m_AudioEnabled) {
            // Seek to the calculated audio position.
            m_CurrentAudioPos = curaudiopos = m_AudioIn->Seek(curvideopos);
        }
    }
}

void AVControllerData::PlaybackAudioOutLoop() {
    while(!syThread::MustAbort() && m_AudioEnabled && !m_Stop) {
        while(m_Pause) {
            if(m_Stop || syThread::MustAbort() || !m_AudioEnabled) return;
            if(!m_AudioOutThread->SelfPause()) return;
        }
    }
}

void AVControllerData::PlaybackVideoOutLoop() {
    while(!syThread::MustAbort() && m_VideoEnabled && !m_Stop) {
        while(m_Pause) {
            if(m_Stop || syThread::MustAbort() || !m_VideoEnabled) return;
            if(!m_VideoOutThread->SelfPause()) return;
        }
        m_VideoOut->FlushVideoData();
        syMilliSleep(1); // FIXME: Sleep using the maximum framerate when the timing issue gets fixed.
    }
}

//// ------------------
//// End Playback Loops
//// ------------------

//// --------------------
//// Begin Encoding Loops
//// --------------------

void AVControllerData::EncodingAudioInLoop() {
    // TODO: Implement AVControllerData::EncodingAudioInLoop()
}

void AVControllerData::EncodingVideoInLoop() {
    // TODO: Implement AVControllerData::EncodingVideoInLoop()
}

void AVControllerData::EncodingAudioOutLoop() {
    // TODO: Implement AVControllerData::EncodingAudioOutLoop()
}

void AVControllerData::EncodingVideoOutLoop() {
    // TODO: Implement AVControllerData::EncodingVideoOutLoop()
}

//// ------------------
//// End Encoding Loops
//// ------------------

void AVControllerData::StartPlayback() {
    if(m_Parent->IsEncoder()) { return; } // Encoder streams do not concern us.
    if(fabs(m_PlaybackSpeed) < MinimumPlaybackSpeed) { return; } // Consider it a pause
    if(m_PlaybackDuration == 0) {
        m_PlaybackDuration = m_Parent->GetLength(); // Maximum length possible
    }
    // Always start at 0 by default
    m_StartVideoPos = 0;
    m_StartAudioPos = 0;

    // Get the initial positions
    if(m_VideoIn) {
        m_StartVideoPos = m_VideoIn->GetPos();
    }
    if(m_AudioIn) {
        m_StartAudioPos = m_AudioIn->GetPos();
        if(m_AudioOut) {
            // We MUST clear the audio buffer, otherwise we'll lose sync!
            m_AudioOut->Clear();
        }
    }
    // And start!
    m_IsPlaying = StartWorkerThreads();
}

bool AVControllerData::StartWorkerThreads() {
    if(!syThread::IsMain()) { return false; }
    bool result = false;
    // First, create the threads if they haven't been created already (the threads die on abort).
    // Failing to re-create the threads will end up in a "no video / audio after abort" bug.
    do {
        if(!m_AudioInThread->IsAlive()) {
            if(m_AudioInThread->Create(AVThreadStackSize) != syTHREAD_NO_ERROR) {
                break;
            }
        }
        if(!m_VideoInThread->IsAlive()) {
            if(m_VideoInThread->Create(AVThreadStackSize) != syTHREAD_NO_ERROR) {
                break;
            }
        }
        if(!m_AudioOutThread->IsAlive()) {
            if(m_AudioOutThread->Create(AVThreadStackSize) != syTHREAD_NO_ERROR) {
                break;
            }
        }
        if(!m_VideoOutThread->IsAlive()) {
            if(m_VideoOutThread->Create(AVThreadStackSize) != syTHREAD_NO_ERROR) {
                break;
            }
        }

        m_Stop = false;
        m_Pause = false;
        if(!m_AudioOutThread->IsRunning() && m_AudioOutThread->Run() != syTHREAD_NO_ERROR) {
            break;
        }
        if(!m_VideoOutThread->IsRunning() && m_VideoOutThread->Run() != syTHREAD_NO_ERROR) {
            break;
        }
        if(!m_AudioInThread->IsRunning() && m_AudioInThread->Run() != syTHREAD_NO_ERROR) {
            break;
        }
        if(!m_VideoInThread->IsRunning() && m_VideoInThread->Run() != syTHREAD_NO_ERROR) {
            break;
        }

        result = true;
        break;
    }while(false);

    return result;
}

// --------------------
// End AVControllerData
// --------------------

// ------------------
// Begin AVController
// ------------------

AVController::AVController() :
m_ReservedVideoIn(false),
m_ReservedAudioIn(false),
m_ReservedVideoOut(false),
m_ReservedAudioOut(false),
m_Data(new AVControllerData(this))
{
}

AVController::~AVController() {
    ShutDown();
    delete m_Data;
}

bool AVController::SetVideoIn(VideoInputDevice* device) {
    if(m_ReservedVideoIn) { return false; }
    return InnerSetVideoIn(device);
}

bool AVController::SetAudioIn(AudioInputDevice* device) {
    if(m_ReservedAudioIn) { return false; }
    return InnerSetAudioIn(device);
}

bool AVController::SetVideoOut(VideoOutputDevice* device) {
    if(m_ReservedVideoOut) { return false; }
    return InnerSetVideoOut(device);
}

bool AVController::SetAudioOut(AudioOutputDevice* device) {
    if(m_ReservedAudioOut) { return false; }
    return InnerSetAudioOut(device);
}

void AVController::Init() {
    if(!syThread::IsMain()) { return; }
    m_Data->ShutdownDevices();

    if(m_Data->m_AudioOut) {
        m_Data->m_AudioOut->Init();
    }
    if(m_Data->m_VideoOut) {
        m_Data->m_VideoOut->Init();
    }

    if(m_Data->m_AudioIn) {
        m_Data->m_AudioIn->Init();
    }
    if(m_Data->m_VideoIn) {
        m_Data->m_VideoIn->Init();
    }
}

bool AVController::MustPause() {
    return m_Data->m_Pause;
}

bool AVController::MustStop() {
    return m_Data->m_Stop;
}

void AVController::ShutDown() {
    if(!syThread::IsMain()) { return; }
    Stop();

    // We don't need to wait for the threads to stop because they're already stopped;
    // so shutdown will have an immediate effect. Otherwise, it would be better to
    // call StartShutDown(); WaitForShutDown() and FinishShutDown() for each one of the
    // threads.

    // We first shut down the output device objects because they're the ones that write the data.
    // Even if they're already stopped, it's better to take precautions in case of further changes
    // in the code.
    // Later, we'll shut down the input device objects.

    if(m_Data->m_AudioOut) {
        m_Data->m_AudioOut->ShutDown();
    }
    if(m_Data->m_VideoOut) {
        m_Data->m_VideoOut->ShutDown();
    }

    if(m_Data->m_AudioIn) {
        m_Data->m_AudioIn->ShutDown();
    }
    if(m_Data->m_VideoIn) {
        m_Data->m_VideoIn->ShutDown();
    }

    // All the device objects have been stopped. Now we set them to NULL.
    m_Data->m_AudioIn = NULL;
    m_Data->m_VideoIn = NULL;
    m_Data->m_AudioOut = NULL;
    m_Data->m_VideoOut = NULL;
}

void AVController::SetPlaybackFramerate(float framerate) {
    if(framerate < MinimumFramerate) { framerate = MinimumFramerate; }
    m_Data->m_FrameRate = framerate;
}

void AVController::Play(float speed, avtime_t duration,bool muted) {
    if(IsEncoder()) { return; }
    if(m_Data->IsPlaying()) { m_Data->Pause(); }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = m_Data->m_AudioEnabled = true;
    m_Data->StartPlayback();
}

void AVController::PlayVideo(float speed, avtime_t duration) {
    if(IsVideoEncoder()) { return; }
    if(m_Data->IsPlaying()) { m_Data->Pause(); }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = true;
    m_Data->m_AudioEnabled = false;
    m_Data->StartPlayback();
}

void AVController::PlayAudio(float speed, avtime_t duration) {
    if(IsAudioEncoder()) { return; }
    if(m_Data->IsPlaying()) { m_Data->Pause(); }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = false;
    m_Data->m_AudioEnabled = true;
    m_Data->StartPlayback();
}

void AVController::Snapshot() {
    if(IsVideoEncoder()) { return; }
    if(m_Data->IsPlaying()) { m_Data->Pause(); }
    if(m_Data->m_VideoIn && m_Data->m_VideoOut) {
        m_Data->m_VideoIn->SendCurrentFrame(m_Data->m_VideoOut);
    }
    if(m_Data->m_VideoOut) {
        m_Data->m_VideoOut->FlushVideoData();
    }
}

void AVController::Encode(float speed, avtime_t duration,bool muted) {
    if(!IsEncoderOnly()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = m_Data->m_AudioEnabled = true;
    m_Data->StartEncoding();
}

void AVController::EncodeVideo(float speed, avtime_t duration) {
    if(!IsVideoEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = true;
    m_Data->m_AudioEnabled = false;
    m_Data->StartEncoding();
}

void AVController::EncodeAudio(float speed, avtime_t duration) {
    if(!IsAudioEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = false;
    m_Data->m_AudioEnabled = true;
    m_Data->StartEncoding();
}

// IMPORTANT! Before this function returns, all threads must have already been stopped!
void AVController::Pause() {
    m_Data->Pause();
    if(!syThread::IsMain()) { return; } // This function must be called by the main thread ONLY!
    if(!IsVideoEncoder()) {
        if(m_Data->m_VideoIn && m_Data->m_VideoOut) {
            // After pausing, always send a snapshot of the current frame to the screen
            m_Data->m_VideoIn->SendCurrentFrame(m_Data->m_VideoOut);
        }
    }
}

void AVController::Stop() {
    m_Data->Stop();
}

unsigned long AVController::GetVideoFrameIndex(avtime_t time) {
    if(m_Data->m_VideoIn) {
        return m_Data->m_VideoIn->GetFrameIndex(time);
    }

    // Video input device not found, we calculate manually.
    if(m_Data->m_FrameRate == 0) {
        return 0;
    }
    avtime_t unitsperframe = (avtime_t)floor(((double)AVTIME_T_SCALE) / m_Data->m_FrameRate);
    time /= unitsperframe;
    unsigned long frame = (unsigned long)(time & 0xFFFFFFFF);
    return frame;
}

avtime_t AVController::GetTimeFromVideoFrameIndex(unsigned long  frame, bool fromend) {
    if(m_Data->m_VideoIn) {
        return m_Data->m_VideoIn->GetTimeFromFrameIndex(frame, fromend);
    }
    // Manual calculation based on our current framerate.
    if(m_Data->m_FrameRate == 0 || fromend) {
        return 0;
    }
    avtime_t unitsperframe = (avtime_t)floor(((double)AVTIME_T_SCALE) / m_Data->m_FrameRate);
    unitsperframe *= frame;
    return unitsperframe;
}

avtime_t AVController::GetCurrentVideoTime() {
    return m_Data->m_CurrentVideoPos;
}

avtime_t AVController::GetCurrentAudioTime() {
    return m_Data->m_CurrentAudioPos;
}

unsigned long AVController::GetCurrentVideoFrame() {
    return GetVideoFrameIndex(GetCurrentVideoTime());
}

avtime_t AVController::Seek(avtime_t time,bool fromend) {
    if(IsEncoder()) { return 0; }
    avtime_t videoresult = 0, audioresult = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_Data->m_VideoIn) {
        videoresult = m_Data->m_VideoIn->Seek(time, fromend);
    }
    if(m_Data->m_AudioIn) {
        audioresult = m_Data->m_AudioIn->Seek(time, fromend);
    }
    if(videoresult < audioresult) {
        // If seek results differ, it means one of the streams already reached EOF. This is the case
        // of a video with incomplete audio, or viceversa. In any case, there's still data, so we must
        // follow it by always using the longest result.
        videoresult = audioresult;
    }
    return videoresult;
}

avtime_t AVController::SeekVideo(avtime_t time,bool fromend) {
    if(IsEncoder()) { return 0; }
    avtime_t result = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_Data->m_VideoIn) {
        result = m_Data->m_VideoIn->Seek(time, fromend);
    }
    return result;
}

avtime_t AVController::SeekAudio(avtime_t time,bool fromend) {
    if(IsEncoder()) { return 0; }
    avtime_t result = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_Data->m_AudioIn) {
        result = m_Data->m_AudioIn->Seek(time, fromend);
    }
    return result;
}

avtime_t AVController::SeekFrame(unsigned long frame,bool fromend) {
    if(IsEncoder()) { return 0; }
    avtime_t result = Seek(GetTimeFromVideoFrameIndex(frame, fromend));
    return result;
}

avtime_t AVController::SeekVideoFrame(unsigned long frame,bool fromend) {
    if(IsEncoder()) { return 0; }
    avtime_t result = SeekVideo(GetTimeFromVideoFrameIndex(frame, fromend));
    return result;
}

avtime_t AVController::SeekFrameRelative(long frame) {
    if(IsEncoder()) { return 0; }
    Pause();
    frame += GetCurrentVideoFrame();
    if(frame < 0) { frame = 0; }
    avtime_t result = Seek(GetTimeFromVideoFrameIndex(frame));
    return result;
}

avtime_t AVController::SeekVideoFrameRelative(long frame) {
    if(IsEncoder()) { return 0; }
    Pause();
    frame += GetCurrentVideoFrame();
    if(frame < 0) { frame = 0; }
    avtime_t result = SeekVideo(GetTimeFromVideoFrameIndex(frame));
    return result;
}

avtime_t AVController::GetLength() {
    avtime_t videolength = GetVideoLength();
    avtime_t audiolength = GetAudioLength();
    if(videolength < audiolength) { // Gets the length of the longest data stream.
        videolength = audiolength;
    }
    return videolength;
}

avtime_t AVController::GetVideoLength() {
    avtime_t result = 0;
    if(m_Data->m_VideoIn) {
        result = m_Data->m_VideoIn->GetLength();
    }
    return result;
}

avtime_t AVController::GetAudioLength() {
    avtime_t result = 0;
    if(m_Data->m_AudioIn) {
        result = m_Data->m_AudioIn->GetLength();
    }
    return result;
}

bool AVController::IsEof() {
    bool videoeof = true;
    bool audioeof = true;
    if(m_Data->m_VideoIn) {
        videoeof = m_Data->m_VideoIn->GetPos() >= m_Data->m_VideoIn->GetLength();
    }

    if(m_Data->m_AudioIn) {
        audioeof = m_Data->m_AudioIn->GetPos() >= m_Data->m_AudioIn->GetLength();
    }
    return videoeof && audioeof;

}

bool AVController::IsVideoEof() {
    bool videoeof = true;
    if(m_Data->m_VideoIn) {
        videoeof = (m_Data->m_VideoIn->GetPos() >= m_Data->m_VideoIn->GetLength());
    }
    return videoeof;
}

bool AVController::IsAudioEof() {
    bool audioeof = true;
    if(m_Data->m_AudioIn) {
        audioeof = (m_Data->m_AudioIn->GetPos() >= m_Data->m_AudioIn->GetLength());
    }
    return audioeof;
}

bool AVController::IsVideoEncoder() {
    bool result = false;
    if(m_Data->m_VideoOut) {
        result = m_Data->m_VideoOut->IsEncoder();
    }
    return result;
}

bool AVController::IsAudioEncoder() {
    bool result = false;
    if(m_Data->m_AudioOut) {
        result = m_Data->m_AudioOut->IsEncoder();
    }
    return result;
}

bool AVController::IsEncoder() {
    // Returns true if any of both streams is an encoder. This is important to know since we send an extra
    // frame on pause after playback.
    return IsVideoEncoder() || IsAudioEncoder();
}

bool AVController::IsEncoderOnly() {
    // Returns true if both streams are encoders or null.
    return (!m_Data->m_VideoOut || IsVideoEncoder()) && (!m_Data->m_AudioOut || IsAudioEncoder());
}

bool AVController::IsPlaying() {
    return m_Data->m_IsPlaying;
}

void AVController::SetAudioGranularity(unsigned long granularity) {
    if(granularity < MinimumAudioGranularity) {
        granularity = MinimumAudioGranularity;
    }
    m_Data->m_AudioGranularity = granularity;
}

unsigned long AVController::GetAudioInThreadId() {
    return m_Data->m_AudioInThread->GetCurrentId();
}

unsigned long AVController::GetVideoInThreadId() {
    return m_Data->m_VideoInThread->GetCurrentId();
}

unsigned long AVController::GetAudioOutThreadId() {
    return m_Data->m_AudioOutThread->GetCurrentId();
}

unsigned long AVController::GetVideoOutThreadId() {
    return m_Data->m_VideoOutThread->GetCurrentId();
}

void AVController::DontSkipVideoFrames(bool dontskip) {
    m_Data->m_StutterMode = dontskip;
}

bool AVController::GetDontSkipVideoFrames() {
    return m_Data->m_StutterMode;
}

bool AVController::InnerSetVideoIn(VideoInputDevice* device) {
    if(!syThread::IsMain() || m_Data->m_IsPlaying) { return false; }
    if(m_Data->m_VideoIn) {
        m_Data->m_VideoIn->ShutDown();
    }
    m_Data->m_VideoIn = device;
    return true;
}

bool AVController::InnerSetVideoOut(VideoOutputDevice* device) {
    if(!syThread::IsMain() || m_Data->m_IsPlaying) { return false; }
    if(m_Data->m_VideoOut) {
        m_Data->m_VideoOut->ShutDown();
    }
    m_Data->m_VideoOut = device;
    return true;
}

bool AVController::InnerSetAudioIn(AudioInputDevice* device) {
    if(!syThread::IsMain() || m_Data->m_IsPlaying) { return false; }
    if(m_Data->m_AudioIn) {
        m_Data->m_AudioIn->ShutDown();
    }
    m_Data->m_AudioIn = device;
    return true;
}

bool AVController::InnerSetAudioOut(AudioOutputDevice* device) {
    if(!syThread::IsMain() || m_Data->m_IsPlaying) { return false; }
    if(m_Data->m_AudioOut) {
        m_Data->m_AudioOut->ShutDown();
    }
    m_Data->m_AudioOut = device;
    return true;
}

void AVController::SetMaximumFrameRate(float maxframerate) {
    if(maxframerate < 0 || maxframerate > MaximumFramerate) {
        maxframerate = MaximumFramerate;
    }
    if(maxframerate < 1) {
        maxframerate = 1;
    }
    unsigned int tmpdelay = 1000 / static_cast<unsigned int>(maxframerate);
    if(tmpdelay < 1) {
        tmpdelay = 1;
    }
    MinimumPlaybackDelay = tmpdelay;
    CurrentMaximumFramerate = maxframerate;
}

