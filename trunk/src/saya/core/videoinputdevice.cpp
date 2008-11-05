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
#include <cstddef>

VideoInputDevice::VideoInputDevice() : AVDevice(),
m_Bitmap(NULL),
m_CurrentTime(0),
m_VideoLength(0),
m_Width(0),
m_Height(0),
m_ColorFormat(vcfBGR24),
m_PixelAspect(1.0)
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


void VideoInputDevice::SendCurrentFrame(VideoOutputDevice* device) {
    sySafeMutexLocker lock1(*m_InputMutex, this);
    sySafeMutexLocker lock2(*m_OutputMutex, this);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        LoadCurrentFrame();
        if(device) {
            device->LoadVideoData(this->m_Bitmap);
        }
    }
}

void VideoInputDevice::SendCurrentFrame(syBitmap* bitmap) {
    sySafeMutexLocker lock1(*m_InputMutex, this);
    sySafeMutexLocker lock2(*m_OutputMutex, this);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        LoadCurrentFrame();
        if(bitmap) {
            bitmap->CopyFrom(this->m_Bitmap); // And copy to the destination bitmap
        }
    }
}


unsigned long VideoInputDevice::GetFrameIndex(avtime_t time) {
    return 0; // This is a Stub. You must override it in your subclass.
}

avtime_t VideoInputDevice::GetTimeFromFrameIndex(unsigned long  frame,bool fromend) {
    return 0; // This is a Stub. You must override it in your subclass.
}


void VideoInputDevice::LoadCurrentFrame() {
    m_Bitmap->Clear(); // This is a Stub. You must override it in your subclass.
}


bool VideoInputDevice::AllocateResources() {
    m_Bitmap->Realloc(m_Width, m_Height, m_ColorFormat);
    return true;
}


void VideoInputDevice::FreeResources() {
    m_Bitmap->ReleaseBuffer();
}