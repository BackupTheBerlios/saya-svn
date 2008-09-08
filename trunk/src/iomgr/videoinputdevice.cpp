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

VideoInputDevice::VideoInputDevice() :
m_Bitmap(NULL),
m_CurrentTime(0),
m_Length(0),
m_Width(0),
m_Height(0),
m_ColorFormat(vcfBGR24),
m_PixelAspect(1.0),
m_IsOk(false),
m_IsShuttingDown(false)
{
    m_Busy = new sySafeMutex;
    m_Bitmap = new syBitmap();
    m_Bitmap->SetAborter(this);
}

VideoInputDevice::~VideoInputDevice() {
    delete m_Bitmap;
    delete m_Busy;
}

bool VideoInputDevice::Init() {
    bool result = ConnectInput();
    if(result) {
        AllocateBitmap();
    }
    m_IsOk = result;
    return m_IsOk;
}

bool VideoInputDevice::IsOk() {
    return m_IsOk;
}

bool VideoInputDevice::InnerMustAbort() {
    return !m_IsOk || m_IsShuttingDown;
}


void VideoInputDevice::ShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    m_Busy->Wait();
    DisconnectInput();
    ReleaseBitmap();
    m_IsOk = false;
    m_IsShuttingDown = false;
}

void VideoInputDevice::StartShutDown() {
    m_IsShuttingDown = true;
}

void VideoInputDevice::WaitForShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    m_Busy->Wait();
}

void VideoInputDevice::FinishShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    if(m_Busy->IsUnlocked()) {
        DisconnectInput();
        ReleaseBitmap();
        m_IsOk = false;
        m_IsShuttingDown = false;
    }
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


void VideoInputDevice::AllocateBitmap() {
    m_Bitmap->Realloc(m_Width, m_Height, m_ColorFormat);
}


void VideoInputDevice::ReleaseBitmap() {
    m_Bitmap->ReleaseBuffer();
}
