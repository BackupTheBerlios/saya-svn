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

VideoInputDevice::VideoInputDevice() : AVDevice(),
m_Bitmap(NULL),
m_CurrentTime(0),
m_Length(0),
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

unsigned long VideoInputDevice::InternalSeek(unsigned long time, bool fromend) {
    if(fromend) {
        if(time >= m_Length) {
            time = 0;
        } else {
            time = (m_Length) - time;
        }
    } else {
        if(time >= m_Length) {
            if(m_Length > 0) {
                time = m_Length;
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

unsigned long VideoInputDevice::Seek(unsigned long time,bool fromend) {
    sySafeMutexLocker lock(*m_Busy, this);
    bool result = false;
    if(lock.IsLocked()) {
        result = InternalSeek(time, fromend);
    } else {
        result = m_CurrentTime;
    }
    return result;
}


unsigned long VideoInputDevice::GetLength() {
    return m_Length;
}

unsigned long VideoInputDevice::GetPos() {
    return m_CurrentTime;
}

VideoColorFormat VideoInputDevice::GetColorFormat() {
    return m_ColorFormat;
}

unsigned long VideoInputDevice::GetWidth() {
    return m_Width;
}


unsigned long VideoInputDevice::GetHeight() {
    return m_Height;
}

float VideoInputDevice::GetPixelAspect() {
    return m_PixelAspect;
}


void VideoInputDevice::SendCurrentFrame(VideoOutputDevice* device) {
    sySafeMutexLocker lock(*m_Busy, this);
    if(lock.IsLocked()) {
        LoadCurrentFrame();
        if(device) {
            device->LoadVideoData(this->m_Bitmap);
        }
    }
}

void VideoInputDevice::SendCurrentFrame(syBitmap* bitmap) {
    sySafeMutexLocker lock(*m_Busy, this);
    if(lock.IsLocked()) {
        LoadCurrentFrame();
        if(bitmap) {
            bitmap->CopyFrom(this->m_Bitmap); // And copy to the destination bitmap
        }
    }
}


unsigned long VideoInputDevice::GetFrameIndex(unsigned long time) {
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
