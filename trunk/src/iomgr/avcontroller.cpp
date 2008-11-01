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

#include <cstddef>

const float MinimumPlaybackSpeed = 0.01;
const float MinimumFramerate = 1.0;
const unsigned long DefaultAudioGranularity = 100;
const unsigned long MinimumAudioGranularity = 10;

class AVControllerData {
    public:
        /** Constructor */
        AVControllerData(AVController* parent);

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

        /** @brief Starts the playback/encoding threads. Called by StartEncoding().
         *  @see AVController::StartEncoding()
         *  @return true on success; false otherwise.
         */
        bool StartWorkerThreads();

        /** Main playback-only loop.
         *
         *  @warning Only for use by the worker thread.
         */
        void PlaybackLoop();

        /** Main encoding-only loop.
         *
         *  @warning Only for use by the worker thread.
         */
        void EncodingLoop();

        /** Current playback framerate. */
        float m_FrameRate;

        /** Flag for video-only or audio-only playback/encoding. */
        bool m_VideoEnabled;

        /** Flag for video-only or audio-only playback/encoding. */
        bool m_AudioEnabled;

        /** @brief Flag determining whether we're currently playing or not.
         *
         *  @note For paused states, it will return false.
         */
        bool m_IsPlaying;

        /** Flag indicating that playback / encoding must be stopped. */
        bool m_Stop;

        /** Flag indicating that playback / encoding must be paused. */
        bool m_Pause;

        /** Playback/encoding speed. 1.0 = normal speed; -1.0 = normal speed, reversed. */
        float m_PlaybackSpeed;

        /** Playback/encoding duration, in milliseconds. */
        unsigned long m_PlaybackDuration;

        /** Video In */
        VideoInputDevice* m_VideoIn;

        /** Audio In */
        AudioInputDevice* m_AudioIn;

        /** Video Out */
        VideoOutputDevice* m_VideoOut;

        /** Audio Out */
        AudioOutputDevice* m_AudioOut;

        syMutex m_Mutex;

        // TODO: Include threads for video in, audio in, video out and audio out.

        unsigned long m_WorkerThread;

        /** Sets the minimum quantity of data that will be sent to the AudioOutputDevice.
         *
         *  @see AVController::SetAudioGranularity
         */
        unsigned long m_AudioGranularity;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        unsigned long m_StartVideoPos;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        unsigned long m_StartAudioPos;

        AVController* m_Parent;
};

AVControllerData::AVControllerData(AVController* parent) :
m_FrameRate(30),
m_PlaybackSpeed(1.0),
m_PlaybackDuration(0),
m_VideoIn(NULL),
m_AudioIn(NULL),
m_VideoOut(NULL),
m_AudioOut(NULL),
m_WorkerThread(0),
m_AudioGranularity(DefaultAudioGranularity),
m_StartVideoPos(0),
m_StartAudioPos(0),
m_Parent(parent)
{
}



AVController::AVController() :
m_Data(NULL)
{
    m_Data = new AVControllerData(this);
}

AVController::~AVController() {
    ShutDown();
    delete m_Data;
}

void AVController::Init(VideoInputDevice* videoin,AudioInputDevice* audioin,
                    VideoOutputDevice* videoout,AudioOutputDevice* audioout) {
    m_Data->m_VideoIn = videoin;
    m_Data->m_AudioIn = audioin;
    m_Data->m_VideoOut = videoout;
    m_Data->m_AudioOut = audioout;
}

void AVController::ShutDown() {
    if(!syThread::IsMain()) { return; }
    Stop();

    if(m_Data->m_VideoOut) {
        m_Data->m_VideoOut->ShutDown();
        m_Data->m_VideoOut = NULL;
    }

    if(m_Data->m_VideoIn) {
        m_Data->m_VideoIn->ShutDown();
        m_Data->m_VideoIn = NULL;
    }

    if(m_Data->m_AudioOut) {
        m_Data->m_AudioOut->ShutDown();
        m_Data->m_AudioOut = NULL;
    }

    if(m_Data->m_AudioIn) {
        m_Data->m_AudioIn->ShutDown();
        m_Data->m_AudioIn = NULL;
    }
}

void AVController::SetPlaybackFramerate(float framerate) {
    if(framerate < MinimumFramerate) { framerate = MinimumFramerate; }
    m_Data->m_FrameRate = framerate;
}

void AVController::Play(float speed,unsigned long duration,bool muted) {
    if(IsEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = m_Data->m_AudioEnabled = true;
    m_Data->StartPlayback();
}

void AVController::PlayVideo(float speed,unsigned long duration) {
    if(IsVideoEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = true;
    m_Data->m_AudioEnabled = false;
    m_Data->StartPlayback();
}

void AVController::PlayAudio(float speed,unsigned long duration) {
    if(IsAudioEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = false;
    m_Data->m_AudioEnabled = true;
    m_Data->StartPlayback();
}

void AVController::Snapshot() {
    if(IsVideoEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    if(m_Data->m_VideoIn && m_Data->m_VideoOut) {
        m_Data->m_VideoIn->SendCurrentFrame(m_Data->m_VideoOut);
    }
}


void AVController::Encode(float speed,unsigned long duration,bool muted) {
    if(!IsEncoderOnly()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = m_Data->m_AudioEnabled = true;
    m_Data->StartEncoding();
}

void AVController::EncodeVideo(float speed,unsigned long duration) {
    if(!IsVideoEncoder()) { return; }
    if(m_Data->m_IsPlaying) { return; }
    m_Data->m_PlaybackSpeed = speed;
    m_Data->m_PlaybackDuration = duration;
    m_Data->m_VideoEnabled = true;
    m_Data->m_AudioEnabled = false;
    m_Data->StartEncoding();
}

void AVController::EncodeAudio(float speed,unsigned long duration) {
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
    if(!syThread::IsMain()) { return; } // This function must be called by the main thread ONLY!
    m_Data->m_Pause = true;
    // IMPORTANT! If m_Data->m_Pause is true, the playing thread MUST consider per-frame operations as atomic, as to not corrupt
    // the data being sent to the Output Devices.
    while(m_Data->m_IsPlaying) {
        syMicroSleep(1);
    }
    if(!IsVideoEncoder()) {
        if(m_Data->m_VideoIn && m_Data->m_VideoOut) {
            // After pausing, always send a snapshot of the current frame to the screen
            m_Data->m_VideoIn->SendCurrentFrame(m_Data->m_VideoOut);
        }
    }
}

void AVController::Stop() {
    if(!syThread::IsMain()) { return; } // This function must be called by the main thread ONLY!
    m_Data->m_Stop = true;
    while(m_Data->m_IsPlaying) {
        syMicroSleep(1);
    }
}

unsigned long AVController::Seek(unsigned long time,bool fromend) {
    if(IsEncoder()) { return 0; }
    unsigned long videoresult = 0, audioresult = 0;
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

unsigned long AVController::SeekVideo(unsigned long time,bool fromend) {
    if(IsEncoder()) { return 0; }
    unsigned long result = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_Data->m_VideoIn) {
        result = m_Data->m_VideoIn->Seek(time, fromend);
    }
    return result;
}

unsigned long AVController::SeekAudio(unsigned long time,bool fromend) {
    if(IsEncoder()) { return 0; }
    unsigned long result = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_Data->m_AudioIn) {
        result = m_Data->m_AudioIn->Seek(time, fromend);
    }
    return result;
}

unsigned long AVController::GetLength() {
    unsigned long videolength = GetVideoLength();
    unsigned long audiolength = GetAudioLength();
    if(videolength < audiolength) { // Gets the length of the longest data stream.
        videolength = audiolength;
    }
    return videolength;
}

unsigned long AVController::GetVideoLength() {
    unsigned long result = 0;
    if(m_Data->m_VideoIn) {
        result = m_Data->m_VideoIn->GetLength();
    }
    return result;
}

unsigned long AVController::GetAudioLength() {
    unsigned long result = 0;
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

void AVControllerData::StartPlayback() {
    if(m_Parent->IsEncoder()) { return; } // Encoder streams do not concern us.
    if(m_PlaybackSpeed < MinimumPlaybackSpeed) { return; } // Consider it a pause
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
    StartWorkerThreads();
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

void AVController::PlaybackOrEncodingLoop() {
    if(syThread::GetThreadId() != m_Data->m_WorkerThread) { return; }
}

void AVControllerData::PlaybackLoop() {
    if(syThread::GetThreadId() != m_WorkerThread) { return; }
    unsigned long curvideopos, curaudiopos, starttime;
    unsigned long audiochunklength;
    curvideopos = m_StartVideoPos;
    curaudiopos = m_StartAudioPos;
    starttime = syGetTicks();

    // FIXME: Implement AVController::PlaybackLoop()
    // Currently, we're facing three problems:
    // 1. We need to moderate our framerate according to our current parameters.
    //    To do that, we need to calculate 1000 / m_Framerate and have that for the video delay.
    // 2. We need to stay alert for when the device audio buffer gets empty. So we need to be careful
    //    with sleep. Oh yeah, that part of the device audio buffer notifying before reaching an empty state,
    //    hasn't been implemented yet.
    // 3. We need a similar measure for "buffer full".

    unsigned long curvideoframe = 0,nextvideoframe = 0;
    if(m_VideoIn) {
        if(m_VideoIn && m_VideoOut && m_VideoEnabled) {
            m_VideoIn->SendCurrentFrame(m_VideoOut);
            curvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
        }

    }
    while(!m_Parent->IsEof() && m_PlaybackDuration > 0) {
        audiochunklength = (m_PlaybackDuration < m_AudioGranularity) ? m_PlaybackDuration : m_AudioGranularity;
        if(m_Stop || m_Pause) { break; }
        if(m_AudioIn && m_AudioOut && m_AudioEnabled) {
            m_AudioIn->SendAudioData(m_AudioOut);
        }
        curaudiopos += audiochunklength;
        if(m_VideoIn && m_VideoOut && m_VideoEnabled) {
            nextvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
            if(curvideoframe != nextvideoframe) {
                m_VideoIn->SendCurrentFrame(m_VideoOut);
                curvideoframe = nextvideoframe;
            }
        }
        curvideopos = m_StartVideoPos + (syGetTicks() - starttime);
        if(m_AudioIn) {
            m_AudioIn->Seek(curaudiopos);
        }
        if(m_VideoIn) {
            m_VideoIn->Seek(curvideopos);
        }
        m_PlaybackDuration -= audiochunklength;
        if(m_PlaybackDuration > 0) {
            if(audiochunklength > 20) {
                syMilliSleep(audiochunklength - 20);
            }
        }
    }
}

void AVControllerData::EncodingLoop() {
    if(syThread::GetThreadId() != m_WorkerThread) { return; }

    // TODO: Implement AVController::EncodingLoop()

//
//    while(!IsEof() && m_PlaybackDuration > 0) {
//        if(m_Stop || m_Pause) { break; }
//
//
//    }
}

void AVController::SetAudioGranularity(unsigned long granularity) {
    if(granularity < MinimumAudioGranularity) {
        granularity = MinimumAudioGranularity;
    }
    m_Data->m_AudioGranularity = granularity;
}

bool AVControllerData::StartWorkerThreads() {
    if(!syThread::IsMain()) { return false; }
    bool result = false;
    // TODO: Implement AVController::StartWorkerThreads
    return result;
}
