/***************************************************************
 * Name:      avsource.h
 * Purpose:   Implementation of the AVSource class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  This class supercedes the old classes
 *            AudioInputDevice and VideoInputDevice.
 **************************************************************/

#include "sybitmap.h"
#include "audiobuffer.h"
#include "avsource.h"
//#include "videooutputdevice.h"
#include "sythread.h"
#include "systring.h"
#include <cmath>
#include <cstddef>
#include <map>

const unsigned int DefaultAVSrcFrequency = 44100;
const unsigned int DefaultAVSrcChannels = 2;
const unsigned int DefaultAVSrcBufferSize = 88200;
const unsigned int DefaultAVSrcPrecision = 16;

// ------------------------
// begin register functions
// ------------------------

class AVSourceFactory  {
    public:
        typedef std::map<syString, AVSourceFactoryFunction, ltsystr> AVSourceFactoryMap;
        AVSourceFactoryMap m_Map;
        static bool Register(const char* url, AVSourceFactoryFunction func);
        static void Unregister(const char* url);
        static AVSource* Create(const char* url);

        static AVSourceFactory* s_self;
        class StaticDestructor {
            public:
                ~StaticDestructor() {
                    delete AVSourceFactory::s_self;
                    s_self = 0;
                }
        };
        static StaticDestructor s_Destructor;
};

AVSourceFactory* AVSourceFactory::s_self = 0;
AVSourceFactory::StaticDestructor AVSourceFactory::s_Destructor;


bool AVSourceFactory::Register(const char* url, AVSourceFactoryFunction func) {
    if(!s_self) {
        s_self = new AVSourceFactory;
    }
    syString tmp(url);
    s_self->m_Map[tmp] = func;
    return true;
}

void AVSourceFactory::Unregister(const char* url) {
    s_self->m_Map.erase(syString(url));
}

AVSource* AVSourceFactory::Create(const char* url) {
    if(!s_self) return 0;
    AVSourceFactoryMap::const_iterator it = s_self->m_Map.find(syString(url));
    if(it != s_self->m_Map.end()) {
        return it->second();
    }
    return 0;
}

bool AVSource::RegisterSource(const char* url, AVSourceFactoryFunction func) {
    return AVSourceFactory::Register(url, func);
}

void AVSource::UnregisterSource(const char* url) {
    AVSourceFactory::Unregister(url);
}

AVSource* AVSource::CreateSource(const char* url) {
    return AVSourceFactory::Create(url);
}

// ----------------------
// end register functions
// ----------------------

AVSource::AVSource() : AVDevice(),
m_Bitmap(NULL),
m_AudioBuffer(NULL),
m_CurrentVideoTime(0),
m_CurrentAudioTime(0),
m_VideoLength(0),
m_AudioLength(0),
m_Width(0),
m_Height(0),
m_ColorFormat(vcfBGR24),
m_PixelAspect(1.0),
m_FramesPerSecond(30),
m_NumAudioChannels(2)
{
    m_IsVideo = true;
    m_IsAudio = false; // No audio by default
    m_IsInput = true;
    m_IsOutput = false;
    m_Bitmap = new syBitmap();
    m_Bitmap->SetAborter(this);

    m_NumAudioChannels = DefaultAVSrcChannels;
    m_AudioFrequency = DefaultAVSrcFrequency;
    m_AudioPrecision = DefaultAVSrcPrecision;
    m_AudioBufferSize = DefaultAVSrcBufferSize;
}

AVSource::~AVSource() {
    delete m_Bitmap;
}

// ----------------------
// begin public functions
// ----------------------

avtime_t AVSource::SeekVideoFrame(unsigned long frame,bool fromend) {
    avtime_t thetime = GetTimeFromFrameIndex(frame, fromend);
    return SeekVideo(thetime, false);
}

avtime_t AVSource::SeekVideo(avtime_t time,bool fromend) {
    sySafeMutexLocker lock(*m_InputVideoMutex, this);
    bool result = false;
    if(lock.IsLocked()) {
        result = InternalVideoSeek(time, fromend);
    } else {
        result = m_CurrentVideoTime;
    }
    return result;
}

avtime_t AVSource::SeekAudio(avtime_t time,bool fromend) {
    sySafeMutexLocker lock(*m_InputAudioMutex, this);
    bool result = false;
    if(lock.IsLocked()) {
        result = InternalAudioSeek(time, fromend);
    } else {
        result = m_CurrentAudioTime;
    }
    return result;
}


avtime_t AVSource::GetVideoPos() const {
    return m_CurrentVideoTime;
}

avtime_t AVSource::GetAudioPos() const {
    return m_CurrentAudioTime;
}

avtime_t AVSource::GetVideoLength() const {
    return m_VideoLength;
}

avtime_t AVSource::GetAudioLength() const {
    return m_AudioLength;
}

avtime_t AVSource::GetLength() const {
    return (m_VideoLength > m_AudioLength) ? m_VideoLength : m_AudioLength;
}

VideoColorFormat AVSource::GetColorFormat() const {
    return m_ColorFormat;
}

unsigned long AVSource::GetWidth() const {
    return m_Width;
}

unsigned long AVSource::GetHeight() const {
    return m_Height;
}

float AVSource::GetPixelAspect() const {
    return m_PixelAspect;
}

float AVSource::GetFramesPerSecond() const {
    return m_FramesPerSecond;
}

void AVSource::SendCurrentFrame(syBitmapSink* sink) {
    sySafeMutexLocker lock1(*m_InputVideoMutex, this);
    sySafeMutexLocker lock2(*m_OutputVideoMutex, this);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        LoadCurrentFrame();
        if(sink) {
            sink->LoadData(this->GetBitmap());
        }
    }
}

unsigned long AVSource::GetFrameIndex(avtime_t time) {

    if(m_FramesPerSecond == 0) {
        return 0;
    }
    avtime_t unitsperframe = (avtime_t)floor(((double)AVTIME_T_SCALE) / m_FramesPerSecond);
    time /= unitsperframe;
    unsigned long frame = (unsigned long)(time & 0xFFFFFFFF);
    unsigned long length = GetLength();
    if(length && frame > length) { frame = length - 1; }
    return frame;
}

avtime_t AVSource::GetTimeFromFrameIndex(unsigned long  frame,bool fromend) {
    if(m_FramesPerSecond == 0) {
        return 0;
    }
    unsigned long length = GetLength();
    if(length && frame >= length) { frame = length - 1; }
    if(fromend) {
        if(!length) {
            frame = 0;
        } else {
            frame = length - 1 - frame;
        }
    }

    avtime_t unitsperframe = (avtime_t)floor(((double)AVTIME_T_SCALE) / m_FramesPerSecond);
    unitsperframe *= frame;
    return unitsperframe;
}

unsigned long AVSource::GetSampleIndex(avtime_t time) {
    return 0; // This is a stub.
}

const syBitmap* AVSource::GetBitmap() {
    return m_Bitmap;
}

void AVSource::SendAudioData(AudioOutputDevice* device,unsigned long numsamples) {
    // TODO: Implement AVSource::SendAudioData
    // Here we have to implement a loop to make sure the desired quantity of numsamples
    // has been sent.
}

void AVSource::SendAudioData(AudioOutputDevice* device,avtime_t duration) {
    unsigned long numsamples = 0;
    if(duration) {
        unsigned long begin_index = GetSampleIndex(m_CurrentAudioTime);
        unsigned long end_index = GetSampleIndex(m_CurrentAudioTime + duration);
        if(end_index <= begin_index) {
            return;
        } else {
            numsamples = end_index + 1- begin_index;
        }
    }
    SendAudioData(device, numsamples);
}



// --------------------
// end public functions
// --------------------

// -------------------------
// begin protected functions
// -------------------------

void AVSource::LoadCurrentFrame() {
    m_Bitmap->Clear(); // This is a Stub. You must override it in your subclass.
}

avtime_t AVSource::InternalVideoSeek(avtime_t time, bool fromend) {
    if(fromend) {
        if(time >= m_VideoLength) {
            time = 0;
        } else {
            time = (m_VideoLength) - time;
        }
    } else {
        if(time >= m_VideoLength) {
            if(m_VideoLength > 0) {
                time = m_VideoLength;
            } else {
                time = 0;
            }
        }
    }
    avtime_t result;
    if(!MustAbort()) {
        m_CurrentVideoTime = result = SeekVideoResource(time);
    } else {
        result = m_CurrentVideoTime;
    }
    return result;
}

void AVSource::LoadAudioBuffer(unsigned long numsamples) {
    // This is a Stub. You must override it in your subclass.
    m_CurrentAudioTime = 0;
    m_AudioBuffer->Clear();
}

avtime_t AVSource::InternalAudioSeek(avtime_t time, bool fromend) {
    if(fromend) {
        if(time >= m_AudioLength) {
            time = 0;
        } else {
            time = (m_AudioLength) - time;
        }
    } else {
        if(time >= m_AudioLength) {
            if(m_AudioLength > 0) {
                time = m_AudioLength;
            } else {
                time = 0;
            }
        }
    }
    avtime_t result;
    if(!MustAbort()) {
        m_CurrentAudioTime = result = SeekAudioResource(time);
    } else {
        result = m_CurrentAudioTime;
    }
    return result;
}

bool AVSource::AllocateResources() {
    // Allocate Video Resources
    if(m_IsVideo && m_Bitmap) {
        m_Bitmap->Realloc(m_Width, m_Height, m_ColorFormat);
    }
    m_CurrentVideoTime = 0;

    if(m_IsAudio) {
        if(m_AudioBuffer) {
            // Check if the Buffer needs to be resized.
            if(m_AudioBuffer->GetSize() != m_AudioBufferSize || m_AudioBuffer->GetNumChannels() != m_NumAudioChannels) {
                delete m_AudioBuffer;
                m_AudioBuffer = 0;
            }
        }
        if(!m_AudioBuffer) {
            m_AudioBuffer = new syAudioBuffer(m_AudioBufferSize,m_NumAudioChannels,m_AudioPrecision,m_AudioFrequency);
        }
        m_AudioBuffer->SetSamplePrecision(m_AudioPrecision);
        m_AudioBuffer->SetSampleFrequency(m_AudioFrequency);
    }
    m_CurrentAudioTime = 0;
    return true;
}


void AVSource::FreeResources() {

    if(m_AudioBuffer) {
        delete m_AudioBuffer;
        m_AudioBuffer = 0;
    }

    if(m_Bitmap) {
        m_Bitmap->ReleaseBuffer();
    }
}

// -----------------------
// end protected functions
// -----------------------
