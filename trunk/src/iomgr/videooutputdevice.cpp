/***************************************************************
 * Name:      videooutputdevice.h
 * Purpose:   Implementation of the VideoOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "sythread.h"
#include "atomic.h"
#include "videooutputdevice.h"
#include "sybitmap.h"
#include "sentryfuncs.h"

const unsigned int VideoOutputDevice::MaxWidth = 10240;
const unsigned int VideoOutputDevice::MaxHeight = 10240;

// ---------------------------
// Begin VideoOutputDeviceData
// ---------------------------

class VideoOutputDeviceData {

    public:

        syBitmap* volatile m_InputBitmap;
        syBitmap* volatile m_OutputBitmap;
        syBitmap* volatile m_ExtraBitmap;
        // Using the form "volatile syBitmap* m_InputBitmap" would make the contents volatile.
        // To make the pointer volatile we use the form  "syBitmap* volatile m_InputBitmap".

        /** Flag indicating that there's new data present in m_ExtraBitmap */
        volatile bool m_NewInput;

        // So, we have m_InputBitmap, m_OutputBitmap, and m_ExtraBitmap.
        // Our current model for VideoOutputDevice is that we have only one producer and only one consumer.
        // This will simplify handling the buffer. Besides, we have m_InputMutex and m_OutputMutex inherited
        // from AVDevice.

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

        void SwapInputAndExtraBitmaps();
        void CheckForNewData();
        void SwapOutputAndExtraBitmaps();
};

void VideoOutputDeviceData::SwapInputAndExtraBitmaps() {
    syBitmap* tmpbmp;
    // We need to spin for this operation since the output thread might swap the output and the extra
    // bitmaps.
    for(;;) {
        tmpbmp =  m_ExtraBitmap;
        if(syAtomic::bool_CAS((void**)(&m_ExtraBitmap),(void*)tmpbmp,(void*)(m_InputBitmap))) break;
    }
    m_InputBitmap = tmpbmp;
    m_NewInput = true;
    syAtomic::MemoryBarrier();
}

void VideoOutputDeviceData::SwapOutputAndExtraBitmaps() {
    syBitmap* tmpbmp;
    // We need to spin for this operation since the input thread might swap the input and extra
    // bitmaps.
    for(;;) {
        tmpbmp =  m_ExtraBitmap;
        if(syAtomic::bool_CAS((void**)(&m_ExtraBitmap),(void*)tmpbmp,(void*)(m_OutputBitmap))) break;
    }
    m_OutputBitmap = tmpbmp;
    syAtomic::MemoryBarrier();
}

void VideoOutputDeviceData::CheckForNewData() {
    if(syAtomic::bool_CAS((bool*)&m_NewInput,true,false)) {
        // The input thread wrote into m_ExtraBitmap; we must swap and process the output.
        SwapOutputAndExtraBitmaps();
    }
}

// -------------------------
// End VideoOutputDeviceData
// -------------------------

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
            m_Data->m_InputBitmap->Realloc(newwidth,newheight,m_ColorFormat);
            m_Data->m_OutputBitmap->Realloc(newwidth,newheight,m_ColorFormat);
            m_Data->m_ExtraBitmap->Realloc(newwidth,newheight,m_ColorFormat);
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
        // Step one: Read data from bitmap into m_InputBitmap.
        m_Data->m_InputBitmap->PasteFrom(bitmap,sy_stkeepaspectratio);

        // Step two: Swap m_InputBitmap and m_ExtraBitmap.
        m_Data->SwapInputAndExtraBitmaps();
    }
}

void VideoOutputDevice::FlushVideoData() {
    if(!IsOk()) return;
    if(MustAbort()) return;
    sySafeMutexLocker lock(*m_OutputMutex, this);
    if(lock.IsLocked()) {
        // Step one: Swap the Extra bitmap and the output bitmap, if necessary
        m_Data->CheckForNewData();

        // Step two: Render the data from m_OutputBitmap into the actual device.
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

bool VideoOutputDevice::AllocateResources() {
    m_Data->m_InputBitmap->Realloc(m_Width,m_Height,m_ColorFormat);
    m_Data->m_OutputBitmap->Realloc(m_Width,m_Height,m_ColorFormat);
    m_Data->m_ExtraBitmap->Realloc(m_Width,m_Height,m_ColorFormat);
    return true;
}

void VideoOutputDevice::FreeResources() {
    m_Data->m_InputBitmap->ReleaseBuffer(false);
    m_Data->m_OutputBitmap->ReleaseBuffer(false);
    m_Data->m_ExtraBitmap->ReleaseBuffer(false);
}


void VideoOutputDevice::Clear() {
    sySafeMutexLocker lockout(*m_OutputMutex);
    if(lockout.IsLocked()) {
        m_Data->m_OutputBitmap->Clear();
        RenderVideoData(m_Data->m_OutputBitmap);
    }
}

void VideoOutputDevice::RenderVideoData(const syBitmap* bitmap) {
    // This is a stub
}

bool VideoOutputDevice::IsEncoder() const {
    return false;
}
