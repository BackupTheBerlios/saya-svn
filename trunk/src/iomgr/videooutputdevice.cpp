/***************************************************************
 * Name:      videooutputdevice.h
 * Purpose:   Implementation of the VideoOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "sythread.h"
#include "videooutputdevice.h"

/** Maximum size for the rendered window. 100 megapixels ought to be enough for anyone. */
const unsigned int MaxVideoOutputDeviceWidth = 10240;
const unsigned int MaxVideoOutputDeviceHeight = 10240;

VideoOutputDevice::VideoOutputDevice() : AVDevice(),
m_ColorFormat(vcfRGB32),
m_Width(0),
m_Height(0),
m_Playing(false),
m_ChangingSize(false)
{
    m_IsVideo = true;
    m_IsOutput = true;
}

VideoOutputDevice::~VideoOutputDevice() {
}

bool VideoOutputDevice::IsPlaying() {
    return m_Playing;
}

bool VideoOutputDevice::InnerMustAbort() {
    return (AVDevice::InnerMustAbort() || m_ChangingSize);
}

unsigned int VideoOutputDevice::GetWidth() {
    return m_Width;
}

unsigned int VideoOutputDevice::GetHeight() {
    return m_Height;
}

bool VideoOutputDevice::ChangeSize(unsigned int newwidth,unsigned int newheight) {
    if(!syThread::IsMain()) { return false; } // Can only be called from the main thread!
    if(!IsOk()) { return false; }
    syBoolSetter setter(m_ChangingSize, true);
    sySafeMutexLocker lock(*m_Busy);
    bool result = false;
    if(lock.IsLocked()) {
        if(newwidth > MaxVideoOutputDeviceWidth || newheight > MaxVideoOutputDeviceHeight) {
            return false;
        }
        result = ChangeDeviceSize(newwidth, newheight);
        if(result) {
            m_Width = newwidth;
            m_Height = newheight;
        }
    }
    return result;
}

bool VideoOutputDevice::ChangeDeviceSize(unsigned int newwidth,unsigned int newheight) {
    // This is a stub
    return false;
}

void VideoOutputDevice::LoadVideoData(syBitmap* bitmap) {
    sySafeMutexLocker lock(*m_Busy, this);
    if(lock.IsLocked()) {
        syBoolSetter setter(m_Playing, true);
        LoadDeviceVideoData(bitmap);
    }
}

void VideoOutputDevice::LoadDeviceVideoData(syBitmap* bitmap) {
    // This is a stub
}

bool VideoOutputDevice::Connect() {
    // This is a stub
    m_Width = 640;
    m_Height = 480;
    m_ColorFormat = vcfRGB32;
    return true;
}

void VideoOutputDevice::Clear() {
    // This is a stub
}

void VideoOutputDevice::RenderData() {
    // This is a stub
}

bool VideoOutputDevice::IsEncoder() {
    return false;
}
