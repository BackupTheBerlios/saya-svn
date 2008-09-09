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

const float MinimumPlaybackSpeed = 0.01;
const float MinimumFramerate = 1.0;
const unsigned long DefaultAudioGranularity = 100;
const unsigned long MinimumAudioGranularity = 10;

AVController::AVController() :
m_FrameRate(30),
m_PlaybackSpeed(1.0),
m_PlaybackDuration(0),
m_VideoIn(NULL),
m_AudioIn(NULL),
m_VideoOut(NULL),
m_AudioOut(NULL),
m_Mutex(NULL),
m_WorkerThread(0),
m_AudioGranularity(DefaultAudioGranularity),
m_StartVideoPos(0),
m_StartAudioPos(0)
{
    m_Mutex = new syMutex;
}

AVController::~AVController() {
    ShutDown();
    delete m_Mutex;
}

void AVController::Init(VideoInputDevice* videoin,AudioInputDevice* audioin,
                    VideoOutputDevice* videoout,AudioOutputDevice* audioout) {
    m_VideoIn = videoin;
    m_AudioIn = audioin;
    m_VideoOut = videoout;
    m_AudioOut = audioout;
}

void AVController::ShutDown() {
    if(!syThread::IsMain()) { return; }
    Stop();

    if(m_VideoOut) {
        m_VideoOut->ShutDown();
        m_VideoOut = NULL;
    }

    if(m_VideoIn) {
        m_VideoIn->ShutDown();
        m_VideoIn = NULL;
    }

    if(m_AudioOut) {
        m_AudioOut->ShutDown();
        m_AudioOut = NULL;
    }

    if(m_AudioIn) {
        m_AudioIn->ShutDown();
        m_AudioIn = NULL;
    }
}

void AVController::SetPlaybackFramerate(float framerate) {
    if(framerate < MinimumFramerate) { framerate = MinimumFramerate; }
    m_FrameRate = framerate;
}

void AVController::Play(float speed,unsigned long duration,bool muted) {
    if(IsEncoder()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = m_AudioEnabled = true;
    StartPlayback();
}

void AVController::PlayVideo(float speed,unsigned long duration) {
    if(IsVideoEncoder()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = true;
    m_AudioEnabled = false;
    StartPlayback();
}

void AVController::PlayAudio(float speed,unsigned long duration) {
    if(IsAudioEncoder()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = false;
    m_AudioEnabled = true;
    StartPlayback();
}

void AVController::Snapshot() {
    if(IsVideoEncoder()) { return; }
    if(m_IsPlaying) { return; }
    if(m_VideoIn && m_VideoOut) {
        m_VideoIn->SendCurrentFrame(m_VideoOut);
    }
}


void AVController::Encode(float speed,unsigned long duration,bool muted) {
    if(!IsEncoderOnly()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = m_AudioEnabled = true;
    StartEncoding();
}

void AVController::EncodeVideo(float speed,unsigned long duration) {
    if(!IsVideoEncoder()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = true;
    m_AudioEnabled = false;
    StartEncoding();
}

void AVController::EncodeAudio(float speed,unsigned long duration) {
    if(!IsAudioEncoder()) { return; }
    if(m_IsPlaying) { return; }
    m_PlaybackSpeed = speed;
    m_PlaybackDuration = duration;
    m_VideoEnabled = false;
    m_AudioEnabled = true;
    StartEncoding();
}

// IMPORTANT! Before this function returns, all threads must have already been stopped!
void AVController::Pause() {
    if(!syThread::IsMain()) { return; } // This function must be called by the main thread ONLY!
    m_Pause = true;
    // IMPORTANT! If m_Pause is true, the playing thread MUST consider per-frame operations as atomic, as to not corrupt
    // the data being sent to the Output Devices.
    while(m_IsPlaying) {
        syThread::Yield();
    }
    if(!IsVideoEncoder()) {
        if(m_VideoIn && m_VideoOut) {
            // After pausing, always send a snapshot of the current frame to the screen
            m_VideoIn->SendCurrentFrame(m_VideoOut);
        }
    }
}

void AVController::Stop() {
    if(!syThread::IsMain()) { return; } // This function must be called by the main thread ONLY!
    m_Stop = true;
    while(m_IsPlaying) {
        syThread::Yield();
    }
}

unsigned long AVController::Seek(unsigned long time,bool fromend) {
    if(IsEncoder()) { return 0; }
    unsigned long videoresult = 0, audioresult = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_VideoIn) {
        videoresult = m_VideoIn->Seek(time, fromend);
    }
    if(m_AudioIn) {
        audioresult = m_AudioIn->Seek(time, fromend);
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
    if(m_VideoIn) {
        result = m_VideoIn->Seek(time, fromend);
    }
    return result;
}

unsigned long AVController::SeekAudio(unsigned long time,bool fromend) {
    if(IsEncoder()) { return 0; }
    unsigned long result = 0;
    if(syThread::IsMain()) {
        Pause();
    }
    if(m_AudioIn) {
        result = m_AudioIn->Seek(time, fromend);
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
    if(m_VideoIn) {
        result = m_VideoIn->GetLength();
    }
    return result;
}

unsigned long AVController::GetAudioLength() {
    unsigned long result = 0;
    if(m_AudioIn) {
        result = m_AudioIn->GetLength();
    }
    return result;
}

bool AVController::IsEof() {
    bool videoeof = true;
    bool audioeof = true;
    if(m_VideoIn) {
        videoeof = m_VideoIn->GetPos() >= m_VideoIn->GetLength();
    }

    if(m_AudioIn) {
        audioeof = m_AudioIn->GetPos() >= m_AudioIn->GetLength();
    }
    return videoeof && audioeof;

}

bool AVController::IsVideoEof() {
    bool videoeof = true;
    if(m_VideoIn) {
        videoeof = (m_VideoIn->GetPos() >= m_VideoIn->GetLength());
    }
    return videoeof;
}

bool AVController::IsAudioEof() {
    bool audioeof = true;
    if(m_AudioIn) {
        audioeof = (m_AudioIn->GetPos() >= m_AudioIn->GetLength());
    }
    return audioeof;
}

bool AVController::IsVideoEncoder() {
    bool result = false;
    if(m_VideoOut) {
        result = m_VideoOut->IsEncoder();
    }
    return result;
}

bool AVController::IsAudioEncoder() {
    bool result = false;
    if(m_AudioOut) {
        result = m_AudioOut->IsEncoder();
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
    return (!m_VideoOut || IsVideoEncoder()) && (!m_AudioOut || IsAudioEncoder());
}

bool AVController::IsPlaying() {
    return m_IsPlaying;
}

void AVController::StartPlayback() {
    if(IsEncoder()) { return; } // Encoder streams do not concern us.
    if(m_PlaybackSpeed < MinimumPlaybackSpeed) { return; } // Consider it a pause
    if(m_PlaybackDuration == 0) {
        m_PlaybackDuration = GetLength(); // Maximum length possible
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
    StartWorkerThread();
}

void AVController::StartEncoding() {
    if(!IsEncoderOnly()) { return; } // Playback streams do not concern us.
    m_StartVideoPos = 0;
    m_StartAudioPos = 0;
    if(m_VideoIn) {
        m_StartVideoPos = m_VideoIn->GetPos();
    }
    if(m_AudioIn) {
        m_StartAudioPos = m_AudioIn->GetPos();
    }
    // And start!
    StartWorkerThread();
}

void AVController::PlaybackOrEncodingLoop() {
    if(syThread::GetThreadId() != m_WorkerThread) { return; }
}

void AVController::PlaybackLoop() {
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
    while(!IsEof() && m_PlaybackDuration > 0) {
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

void AVController::EncodingLoop() {
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
    m_AudioGranularity = granularity;
}

bool AVController::StartWorkerThread() {
    if(!syThread::IsMain()) { return false; }
    bool result = false;
    // TODO: Implement AVController::StartWorkerThread
    return result;
}
