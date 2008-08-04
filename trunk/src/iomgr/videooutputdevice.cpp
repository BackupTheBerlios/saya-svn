/***************************************************************
 * Name:      videooutputdevice.h
 * Purpose:   Implementation of the VideoOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "mutex.h"
#include "videooutputdevice.h"

/** Maximum size for the rendered window. 100 megapixels ought to be enough for anyone. */
const unsigned int MaxVideoOutputDeviceWidth = 10240;
const unsigned int MaxVideoOutputDeviceHeight = 10240;

VideoOutputDevice::VideoOutputDevice() :
m_ColorFormat(vcfRGB32),
m_Width(0),
m_Height(0),
m_playing(false),
m_changingsize(false),
m_shuttingdown(false),
m_ok(false)
{
    m_mutex = new syMutex;
}

VideoOutputDevice::~VideoOutputDevice() {
    ShutDown();
    delete m_mutex;
}

bool VideoOutputDevice::Init() {
    if(!m_ok) {
        m_ok = InitializeOutput();
    }
    return m_ok;
}

bool VideoOutputDevice::IsOk() {
    return m_ok;
}

bool VideoOutputDevice::MustAbortPlayback() {
    return (!m_ok || m_shuttingdown || m_changingsize);
}

void VideoOutputDevice::ShutDown() {
    m_shuttingdown = true;
    while(m_playing) {
        syMilliSleep(10); // Sleep for 10 milliseconds
    }
    Clear();
    DisconnectOutput();
    m_ok = false;
    m_shuttingdown = false;
}

unsigned int VideoOutputDevice::GetWidth() {
    return m_Width;
}

unsigned int VideoOutputDevice::GetHeight() {
    return m_Height;
}

bool VideoOutputDevice::ChangeSize(unsigned int newwidth,unsigned int newheight) {
    bool result = true;

    {
        syMutexLocker mylocker(*m_mutex);
        if(m_shuttingdown || m_changingsize || m_playing || newwidth > MaxVideoOutputDeviceWidth || newheight > MaxVideoOutputDeviceHeight) {
            result = false;
        } else {
            m_changingsize = true;
        }
    }

    if(result) {
        result = ChangeDeviceSize(newwidth, newheight);
        if(result) {
            m_Width = newwidth;
            m_Height = newheight;
        }
        m_changingsize = false;
    }

    return result;
}

bool VideoOutputDevice::ChangeDeviceSize(unsigned int newwidth,unsigned int newheight) {
    // This is a stub
    return false;
}

void VideoOutputDevice::LoadVideoData(syBitmap* bitmap) {
    bool result = true;

    {
        syMutexLocker mylocker(*m_mutex);
        if(m_playing || MustAbortPlayback()) {
            result = false;
        } else {
            m_playing = true;
        }
    }

    if(result) {
        LoadDeviceVideoData(bitmap);
        m_playing = false;
    }
}

void VideoOutputDevice::LoadDeviceVideoData(syBitmap* bitmap) {
    // This is a stub
}

bool VideoOutputDevice::InitializeOutput() {
    // This is a stub
    m_Width = 640;
    m_Height = 480;
    m_ColorFormat = vcfRGB32;
    return false;
}

void VideoOutputDevice::Clear() {
    // This is a stub
}

void VideoOutputDevice::DisconnectOutput() {
    // This is a stub
}

void VideoOutputDevice::RenderData() {
    // This is a stub
}

/** syBitmap subclass for use with VideoOutputDevice */

syVODBitmap::syVODBitmap() :
m_VOD(NULL),
m_BufferMutex(NULL),
m_BufferOwner(0),
m_BufferLockCount(0)
{
    m_BufferMutex = new syMutex();
}

/** Constructor with incorporated bitmap creation */
syVODBitmap::syVODBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat) :
syBitmap(width,height,colorformat),
m_VOD(NULL),
m_BufferMutex(NULL),
m_BufferOwner(0),
m_BufferLockCount(0)
{
    m_BufferMutex = new syMutex();
}

syVODBitmap::~syVODBitmap() {
    delete m_BufferMutex;
}

void syVODBitmap::SetVOD(VideoOutputDevice* device) {
    m_VOD = device;
}

bool syVODBitmap::MustAbort() {
    if(!m_VOD) {
        return false;
    }
    return m_VOD->MustAbortPlayback();
}

bool syVODBitmap::Lock(unsigned int tries,unsigned delay) {
    bool result = false;
    unsigned i = 0;
    if(delay < 10) {
        delay = 10;
    }
    do {
        {   // The extra braces are a stack spaceholder for tmplock
            syMutexLocker tmplock(*m_BufferMutex);
            result = (m_BufferOwner == 0 || (m_BufferOwner == syMutex::GetThreadId()));
            if(result) {
                if(!m_BufferLockCount) {
                    m_BufferOwner = syMutex::GetThreadId();
                }
                ++m_BufferLockCount;
            }
        }   // After closing this brace, the mutexlocker is released from the stack, and the mutex is unlocked.
        if(!result) {
            if(tries > 0) {
                ++i;
            }
            syMilliSleep(delay);
        }
    }while(!result && (tries==0 || i < tries));
    return result;
}

bool syVODBitmap::Unlock() {
    bool result = false;
    do {
        syMutexLocker tmplock(*m_BufferMutex);
        if(!m_BufferOwner) {
            result = true;
            break;
        }
        if(m_BufferOwner == syMutex::GetThreadId()) {
            if(m_BufferLockCount > 0) {
                --m_BufferLockCount;
            }
            if(!m_BufferLockCount) {
                m_BufferOwner = 0;
            }
            result = true;
            break;
        }
    }while(false);

    return result;
}

bool syVODBitmap::ReleaseBuffer(bool force) {
    bool isLockedNow = Lock(10,10);
    bool result = isLockedNow;
    if(isLockedNow || force) {
        delete[] m_Buffer;
        m_Buffer = NULL;
        m_BufferLength = 0;
        m_BufferSize = 0;
        result = true;
    }
    if(isLockedNow) {
        Unlock();
    }
    return result;
}
