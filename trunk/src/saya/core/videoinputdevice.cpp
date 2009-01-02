/***************************************************************
 * Name:      videoinputdevice.h
 * Purpose:   Implementation of the VideoInputDevice class
 *            and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "sybitmap.h"
#include "videoinputdevice.h"
#include "videooutputdevice.h"
#include "sythread.h"
#include "systring.h"
#include <cmath>
#include <cstddef>
#include <map>

// ------------------------
// begin register functions
// ------------------------

class VIDFactory  {
    public:
        typedef std::map<syString, VIDFactoryFunction, ltsystr> VIDFactoryMap;
        VIDFactoryMap m_Map;
        static bool Register(const char* url, VIDFactoryFunction func);
        static void Unregister(const char* url);
        static VideoInputDevice* Create(const char* url);

        static VIDFactory* s_self;
        class StaticDestructor {
            public:
                ~StaticDestructor() {
                    delete VIDFactory::s_self;
                    s_self = 0;
                }
        };
        static StaticDestructor s_Destructor;
};

VIDFactory* VIDFactory::s_self = 0;
VIDFactory::StaticDestructor VIDFactory::s_Destructor;


bool VIDFactory::Register(const char* url, VIDFactoryFunction func) {
    if(!s_self) {
        s_self = new VIDFactory;
    }
    syString tmp(url);
    s_self->m_Map[tmp] = func;
    return true;
}

void VIDFactory::Unregister(const char* url) {
    s_self->m_Map.erase(syString(url));
}

VideoInputDevice* VIDFactory::Create(const char* url) {
    if(!s_self) return 0;
    VIDFactoryMap::const_iterator it = s_self->m_Map.find(syString(url));
    if(it != s_self->m_Map.end()) {
        return it->second();
    }
    return 0;
}

bool VideoInputDevice::RegisterVID(const char* url, VIDFactoryFunction func) {
    return VIDFactory::Register(url, func);
}

void VideoInputDevice::UnregisterVID(const char* url) {
    VIDFactory::Unregister(url);
}

VideoInputDevice* VideoInputDevice::CreateVID(const char* url) {
    return VIDFactory::Create(url);
}

// ----------------------
// end register functions
// ----------------------

VideoInputDevice::VideoInputDevice() : AVDevice(),
m_Bitmap(NULL),
m_CurrentTime(0),
m_VideoLength(0),
m_Width(0),
m_Height(0),
m_ColorFormat(vcfBGR24),
m_PixelAspect(1.0),
m_FramesPerSecond(30)
{
    m_IsVideo = true;
    m_IsInput = true;
    m_Bitmap = new syBitmap();
    m_Bitmap->SetAborter(this);
}

VideoInputDevice::~VideoInputDevice() {
    delete m_Bitmap;
}

avtime_t VideoInputDevice::InternalSeek(avtime_t time, bool fromend) {
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
    unsigned long result;
    if(!MustAbort()) {
        m_CurrentTime = result = SeekResource(time);
    } else {
        result = m_CurrentTime;
    }
    return result;
}

avtime_t VideoInputDevice::SeekFrame(unsigned long frame,bool fromend) {
    avtime_t thetime = GetTimeFromFrameIndex(frame, fromend);
    return Seek(thetime, false);
}

avtime_t VideoInputDevice::Seek(avtime_t time,bool fromend) {
    sySafeMutexLocker lock(*m_InputMutex, this);
    bool result = false;
    if(lock.IsLocked()) {
        result = InternalSeek(time, fromend);
    } else {
        result = m_CurrentTime;
    }
    return result;
}


avtime_t VideoInputDevice::GetLength() const {
    return m_VideoLength;
}

avtime_t VideoInputDevice::GetPos() const {
    return m_CurrentTime;
}

VideoColorFormat VideoInputDevice::GetColorFormat() const {
    return m_ColorFormat;
}

unsigned long VideoInputDevice::GetWidth() const {
    return m_Width;
}


unsigned long VideoInputDevice::GetHeight() const {
    return m_Height;
}

float VideoInputDevice::GetPixelAspect() const {
    return m_PixelAspect;
}

/** Gets the resource's framerate. */
float VideoInputDevice::GetFramesPerSecond() const {
    return m_FramesPerSecond;
}


void VideoInputDevice::SendCurrentFrame(VideoOutputDevice* device) {
    sySafeMutexLocker lock1(*m_InputMutex, this);
    sySafeMutexLocker lock2(*m_OutputMutex, this);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        LoadCurrentFrame();
        if(device) {
            device->LoadVideoData(this->GetBitmap());
        }
    }
}

void VideoInputDevice::SendCurrentFrame(syBitmap* bitmap) {
    sySafeMutexLocker lock1(*m_InputMutex, this);
    sySafeMutexLocker lock2(*m_OutputMutex, this);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        LoadCurrentFrame();
        if(bitmap) {
            bitmap->CopyFrom(this->GetBitmap()); // And copy to the destination bitmap
        }
    }
}


unsigned long VideoInputDevice::GetFrameIndex(avtime_t time) {

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

avtime_t VideoInputDevice::GetTimeFromFrameIndex(unsigned long  frame,bool fromend) {
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


void VideoInputDevice::LoadCurrentFrame() {
    m_Bitmap->Clear(); // This is a Stub. You must override it in your subclass.
}


bool VideoInputDevice::AllocateResources() {
    if(m_Bitmap) {
        m_Bitmap->Realloc(m_Width, m_Height, m_ColorFormat);
    }
    return true;
}


void VideoInputDevice::FreeResources() {
    if(m_Bitmap) {
        m_Bitmap->ReleaseBuffer();
    }
}

const syBitmap* VideoInputDevice::GetBitmap() {
    return m_Bitmap;
}
