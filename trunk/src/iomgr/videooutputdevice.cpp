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
#include "sybitmap.h"
#include "sentryfuncs.h"

const unsigned int VideoOutputDevice::MaxWidth = 10240;
const unsigned int VideoOutputDevice::MaxHeight = 10240;

class VideoOutputDeviceData {

    public:

        syBitmap* m_InputBitmap;
        syBitmap* m_OutputBitmap;
        syBitmap* m_ExtraBitmap;

        VideoOutputDeviceData() {
            m_InputBitmap = new syBitmap;
            m_OutputBitmap = new syBitmap;
            m_ExtraBitmap = new syBitmap;
        }

        ~VideoOutputDeviceData() {
            delete m_ExtraBitmap;
            delete m_OutputBitmap;
            delete m_InputBitmap;
        }
};

VideoOutputDevice::VideoOutputDevice() : AVDevice(),
m_ColorFormat(vcfRGB32),
m_Width(0),
m_Height(0),
m_ChangingSize(false)
{
    m_IsVideo = true;
    m_IsOutput = true;
    m_Data = new VideoOutputDeviceData;
}

VideoOutputDevice::~VideoOutputDevice() {
    delete m_Data;
}

bool VideoOutputDevice::InnerMustAbort() const {
    return (AVDevice::InnerMustAbort() || m_ChangingSize);
}

unsigned int VideoOutputDevice::GetWidth() const {
    return m_Width;
}

unsigned int VideoOutputDevice::GetHeight() const {
    return m_Height;
}

bool VideoOutputDevice::ChangeSize(unsigned int newwidth,unsigned int newheight) {
    if(!syThread::IsMain()) { return false; } // Can only be called from the main thread!
    if(!IsOk()) { return false; }
    syBoolSetter setter(m_ChangingSize, true);
    sySafeMutexLocker lockin(*m_InputMutex);
    sySafeMutexLocker lockout(*m_OutputMutex);
    bool result = false;
    if(lockin.IsLocked() && lockout.IsLocked()) {
        if(newwidth > MaxWidth || newheight > MaxHeight) {
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

void VideoOutputDevice::LoadVideoData(const syBitmap* bitmap) {
    if(!IsOk()) return;
    if(MustAbort()) return;

    sySafeMutexLocker lock(*m_InputMutex, this);
    if(lock.IsLocked()) {
        m_Data->m_InputBitmap->PasteFrom(bitmap,sy_stkeepaspectratio);
        // TODO: Read data from the source bitmap.
    }
}

void VideoOutputDevice::FlushVideoData() {
    if(!IsOk()) return;
    if(MustAbort()) return;
    sySafeMutexLocker lock(*m_OutputMutex, this);
    if(lock.IsLocked()) {
        RenderVideoData(m_Data->m_OutputBitmap);
    }
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

void VideoOutputDevice::RenderVideoData(const syBitmap* bitmap) {
    // This is a stub
}

bool VideoOutputDevice::IsEncoder() const {
    return false;
}
