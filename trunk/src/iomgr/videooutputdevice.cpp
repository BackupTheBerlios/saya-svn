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

bool VideoOutputDevice::IsPlaying() {
    return m_playing;
}
bool VideoOutputDevice::MustAbort() {
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
        if(m_playing || MustAbort()) {
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
