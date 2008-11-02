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

// 1 meg for stack is quite a good size. We're using 4 threads max, so 4 megs isn't a bad size.
const unsigned long AVThreadStackSize = 1048576;

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

        /** @brief Starts the playback/encoding threads. Called by StartEncoding().
         *  @see AVController::StartEncoding()
         *  @return true on success; false otherwise.
         */
        bool StartWorkerThreads();


        /** Loop for Audio In. */
        void AudioInLoop();
        /** Loop for Video In. */
        void VideoInLoop();
        /** Loop for Audio Out. */
        void AudioOutLoop();
        /** Loop for Video Out. */
        void VideoOutLoop();

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
         *  @note For paused states, it will return false.
         */
        volatile bool m_IsPlaying;

        /** Flag indicating that playback / encoding must be stopped. */
        volatile bool m_Stop;

        /** Flag indicating that playback / encoding must be paused. */
        volatile bool m_Pause;

        /** Playback/encoding speed. 1.0 = normal speed; -1.0 = normal speed, reversed. */
        volatile float m_PlaybackSpeed;

        /** Playback/encoding duration, in milliseconds. */
        volatile unsigned long m_PlaybackDuration;

        /** Video In */
        VideoInputDevice* m_VideoIn;

        /** Audio In */
        AudioInputDevice* m_AudioIn;

        /** Video Out */
        VideoOutputDevice* m_VideoOut;

        /** Audio Out */
        AudioOutputDevice* m_AudioOut;

        syMutex m_Mutex;


        /** Sets the minimum quantity of data that will be sent to the AudioOutputDevice.
         *
         *  @see AVController::SetAudioGranularity
         */
        volatile unsigned long m_AudioGranularity;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        volatile unsigned long m_StartVideoPos;

        /** The starting position for video in the current playback. Used for video/audio sync. */
        volatile unsigned long m_StartAudioPos;

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
        m_Parent->AudioInLoop();
    }
    return 0;
}

int syVideoInThread::Entry() {
    while(!MustAbort()) {
        m_Parent->VideoInLoop();
    }
    return 0;
}

int syAudioOutThread::Entry() {
    while(!MustAbort()) {
        m_Parent->AudioOutLoop();
    }
    return 0;
}

int syVideoOutThread::Entry() {
    while(!MustAbort()) {
        m_Parent->VideoOutLoop();
    }
    return 0;
}

// ----------------------------
// End Auxiliary thread classes
// ----------------------------

AVControllerData::AVControllerData(AVController* parent) :
m_FrameRate(30),
m_VideoEnabled(true),
m_AudioEnabled(true),
m_StutterMode(false),
m_PlaybackSpeed(1.0),
m_PlaybackDuration(0),
m_VideoIn(NULL),
m_AudioIn(NULL),
m_VideoOut(NULL),
m_AudioOut(NULL),
m_AudioGranularity(DefaultAudioGranularity),
m_StartVideoPos(0),
m_StartAudioPos(0),
m_Parent(parent),
m_AudioInThread(NULL),
m_VideoInThread(NULL),
m_AudioOutThread(NULL),
m_VideoOutThread(NULL)
{
    m_AudioInThread = new syAudioInThread(this);
    m_VideoInThread = new syVideoInThread(this);
    m_AudioOutThread = new syAudioOutThread(this);
    m_VideoOutThread = new syVideoOutThread(this);

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
    ShutDown();
    m_Data->m_VideoIn = videoin;
    m_Data->m_AudioIn = audioin;
    m_Data->m_VideoOut = videoout;
    m_Data->m_AudioOut = audioout;

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

// ---------------------------
// Begin Generic Loop Wrappers
// ---------------------------

void AVControllerData::AudioInLoop() {
    if(m_Parent->IsAudioEncoder()) {
        EncodingAudioInLoop();
    } else {
        PlaybackAudioInLoop();
    }
}

void AVControllerData::VideoInLoop() {
    if(m_Parent->IsVideoEncoder()) {
        EncodingVideoInLoop();
    } else {
        PlaybackVideoInLoop();
    }
}

void AVControllerData::AudioOutLoop() {
    if(m_Parent->IsAudioEncoder()) {
        EncodingAudioOutLoop();
    } else {
        PlaybackAudioOutLoop();
    }
}

void AVControllerData::VideoOutLoop() {
    if(m_Parent->IsVideoEncoder()) {
        EncodingVideoOutLoop();
    } else {
        PlaybackVideoOutLoop();
    }
}

// -------------------------
// End Generic Loop Wrappers
// -------------------------

// --------------------
// Begin Playback Loops
// --------------------

void AVControllerData::PlaybackAudioInLoop() {
    while(!syThread::MustAbort() && m_AudioEnabled && !m_Stop) {
        while(m_Pause) {
            if(!m_AudioInThread->SelfPause()) return;
        }
    }
}

void AVControllerData::PlaybackVideoInLoop() {
    avtime_t curvideopos, starttime;
    unsigned long curvideoframe,nextvideoframe;
    bool stuttermode = m_StutterMode;

    starttime = syGetTicks();
    curvideopos = m_StartVideoPos;
    nextvideoframe = 0;

    while(!syThread::MustAbort() && m_VideoEnabled && !m_Stop && m_VideoIn && m_VideoOut) {
        while(m_Pause) {
            if(!m_VideoInThread->SelfPause()) return;
        }
        m_VideoIn->SendCurrentFrame(m_VideoOut);
        curvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
        if(curvideoframe != nextvideoframe) {

            m_VideoIn->SendCurrentFrame(m_VideoOut);
            curvideoframe = nextvideoframe;
        }
        // Here we calculate the next video position based on the current time.

        nextvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
        // stuttermode: The case where we don't want to skip the video, even if audio will stutter.
        if(stuttermode && nextvideoframe > curvideoframe + 1) {
            // Don't skip video frames in stutter mode.
            nextvideoframe = curvideoframe + 1;
        }

        curvideopos = m_StartVideoPos + ((syGetTicks() - starttime)*(AVTIME_T_SCALE / 1000));
        m_VideoIn->Seek(curvideopos);
        if(stuttermode) {
            // TODO: Signal the audio thread to continue sending the audio if we're in "stuttering" mode.
        }
    }
}

void AVControllerData::PlaybackAudioOutLoop() {
    while(!syThread::MustAbort() && m_AudioEnabled && !m_Stop) {
        while(m_Pause) {
            if(!m_AudioOutThread->SelfPause()) return;
        }
    }
}

void AVControllerData::PlaybackVideoOutLoop() {
    while(!syThread::MustAbort() && m_VideoEnabled && !m_Stop) {
        while(m_Pause) {
            if(!m_VideoOutThread->SelfPause()) return;
        }
    }
}

// ------------------
// End Playback Loops
// ------------------

// --------------------
// Begin Encoding Loops
// --------------------

void AVControllerData::EncodingAudioInLoop() {
}

void AVControllerData::EncodingVideoInLoop() {
}

void AVControllerData::EncodingAudioOutLoop() {
}

void AVControllerData::EncodingVideoOutLoop() {
}

// ------------------
// End Encoding Loops
// ------------------

//void AVControllerData::PlaybackLoop() {
//    if(syThread::GetThreadId() != m_WorkerThread) { return; }
//    unsigned long curvideopos, curaudiopos, starttime;
//    unsigned long audiochunklength;
//    curvideopos = m_StartVideoPos;
//    curaudiopos = m_StartAudioPos;
//    starttime = syGetTicks();
//
//    // FIXME: Implement AVController::PlaybackLoop()
//    // Currently, we're facing three problems:
//    // 1. We need to moderate our framerate according to our current parameters.
//    //    To do that, we need to calculate 1000 / m_Framerate and have that for the video delay.
//    // 2. We need to stay alert for when the device audio buffer gets empty. So we need to be careful
//    //    with sleep. Oh yeah, that part of the device audio buffer notifying before reaching an empty state,
//    //    hasn't been implemented yet.
//    // 3. We need a similar measure for "buffer full".
//
//    unsigned long curvideoframe = 0,nextvideoframe = 0;
//    if(m_VideoIn) {
//        if(m_VideoIn && m_VideoOut && m_VideoEnabled) {
//            m_VideoIn->SendCurrentFrame(m_VideoOut);
//            curvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
//        }
//
//    }
//    while(!m_Parent->IsEof() && m_PlaybackDuration > 0) {
//        audiochunklength = (m_PlaybackDuration < m_AudioGranularity) ? m_PlaybackDuration : m_AudioGranularity;
//        if(m_Stop || m_Pause) { break; }
//        if(m_AudioIn && m_AudioOut && m_AudioEnabled) {
//            m_AudioIn->SendAudioData(m_AudioOut);
//        }
//        curaudiopos += audiochunklength;
//        if(m_VideoIn && m_VideoOut && m_VideoEnabled) {
//            nextvideoframe = m_VideoIn->GetFrameIndex(curvideopos);
//            if(curvideoframe != nextvideoframe) {
//                m_VideoIn->SendCurrentFrame(m_VideoOut);
//                curvideoframe = nextvideoframe;
//            }
//        }
//        curvideopos = m_StartVideoPos + (syGetTicks() - starttime);
//        if(m_AudioIn) {
//            m_AudioIn->Seek(curaudiopos);
//        }
//        if(m_VideoIn) {
//            m_VideoIn->Seek(curvideopos);
//        }
//        m_PlaybackDuration -= audiochunklength;
//        if(m_PlaybackDuration > 0) {
//            if(audiochunklength > 20) {
//                syMilliSleep(audiochunklength - 20);
//            }
//        }
//    }
//}
//
//void AVControllerData::EncodingLoop() {
//    if(syThread::GetThreadId() != m_WorkerThread) { return; }
//
//    // TODO: Implement AVController::EncodingLoop()
//
////
////    while(!IsEof() && m_PlaybackDuration > 0) {
////        if(m_Stop || m_Pause) { break; }
////
////
////    }
//}

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
