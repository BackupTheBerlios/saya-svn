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
#include "mutex.h"

VideoInputDevice::VideoInputDevice() :
m_Bitmap(NULL),
m_CurrentTime(0),
m_Length(0),
m_Width(0),
m_Height(0),
m_ColorFormat(vcfBGR24),
m_PixelAspect(1.0),
m_IsOk(false),
m_IsBusy(false),
m_IsShuttingDown(false)
{
    m_Mutex = new syMutex;
    m_Bitmap = new syBitmap();
    m_Bitmap->SetAborter(this);
}

VideoInputDevice::~VideoInputDevice() {
    delete m_Bitmap;
    delete m_Mutex;
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

bool VideoInputDevice::MustAbort() {
    return !m_IsOk || m_IsShuttingDown;
}

void VideoInputDevice::ShutDown() {
    if(!syMutex::IsMainThread()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    while(m_IsBusy) {
        syMilliSleep(1); // Sleep for 1 millisecond at least
    }
    DisconnectInput();
    ReleaseBitmap();
    m_IsOk = false;
    m_IsShuttingDown = false;
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
    bool result,abort,oldbusy,ok;
    ok = false;
    abort = false;
    while(!ok) {
        {
            syMutexLocker locker(*m_Mutex);
            oldbusy = m_IsBusy;
            if(!oldbusy) {
                ok = true;
                m_IsBusy = true;
            }
        }
        if(MustAbort()) {
            break;
        }
        if(!ok) {
            syMilliSleep(1);
        }
    }
    if(!MustAbort()) {
        return InternalSeek(time, fromend);
    } else {
        result = m_CurrentTime;
    }
    {
        syMutexLocker locker(*m_Mutex);
        m_IsBusy = oldbusy;
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
    bool abort,oldbusy,ok;
    ok = false;
    abort = false;
    while(!ok) {
        {
            syMutexLocker locker(*m_Mutex);
            oldbusy = m_IsBusy;
            if(!oldbusy) {
                ok = true;
                m_IsBusy = true;
            }
        }
        if(MustAbort()) {
            break;
        }
        if(!ok) {
            syMilliSleep(1);
        }
    }
    if(!MustAbort()) {
        LoadCurrentFrame();
        if(device) {
            device->LoadVideoData(this->m_Bitmap);
        }
        {
            syMutexLocker locker(*m_Mutex);
            m_IsBusy = oldbusy;
        }
    }
}

void VideoInputDevice::SendCurrentFrame(syBitmap* bitmap) {
    bool abort,oldbusy,ok;
    ok = false;
    abort = false;
    while(!ok) {
        {   // Lock the mutex to try to get the Busy status.
            syMutexLocker locker(*m_Mutex);
            oldbusy = m_IsBusy;
            if(!oldbusy) {
                ok = true;
                m_IsBusy = true;
            }
        }
        if(MustAbort()) {
            break;
        }
        if(!ok) {
            syMilliSleep(1); // Sleep until we get it.
        }
    }
    if(!MustAbort()) { // Always check the MustAbort() flag.
        LoadCurrentFrame(); // Loads frame from the input resouce
        if(bitmap) {
            bitmap->CopyFrom(this->m_Bitmap); // And copy to the destination bitmap
        }
        {
            syMutexLocker locker(*m_Mutex);
            m_IsBusy = oldbusy;
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
