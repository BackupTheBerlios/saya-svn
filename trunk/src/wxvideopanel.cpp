/***************************************************************************
 * Name:      wxvideopanel.cpp
 * Purpose:   Implementation of classes wxVideoPanel and wxVideoOutputDevice
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-07-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code from sdlpanel.cc found at
 *            http://code.technoplaza.net/wx-sdl/part1/
 *            (LGPL licensed)
 ***************************************************************************/

// #include <wx/dcbuffer.h>
// #include <wx/image.h>
#include "wxvideopanel.h"
#include "iomgr/mutex.h"

IMPLEMENT_CLASS(wxVideoPanel, wxPanel)

BEGIN_EVENT_TABLE(wxVideoPanel, wxPanel)
    EVT_PAINT(wxVideoPanel::OnPaint)
    EVT_ERASE_BACKGROUND(wxVideoPanel::OnEraseBackground)
    EVT_SIZE(wxVideoPanel::OnSize)
    EVT_IDLE(wxVideoPanel::OnIdle)
END_EVENT_TABLE()

// *** Begin wxVideoOutputDevice code ***

wxVideoOutputDevice::wxVideoOutputDevice(wxVideoPanel* panel) : VideoOutputDevice(),
m_Buffer(NULL),
m_BufferSize(0),
m_BufferLength(0),
m_BufferOwner(0),
m_BufferLockCount(0),
m_BufferMutex(NULL),
m_Panel(panel)
{
    m_BufferMutex = new syMutex;

}

wxVideoOutputDevice::~wxVideoOutputDevice() {
    m_BufferOwner = 0;
    m_BufferLockCount = 0;
    ShutDown();
    delete m_BufferMutex;
    m_BufferMutex = NULL;
}

unsigned long wxVideoOutputDevice::CalculateBufferLength(unsigned int width,unsigned int height) {
    unsigned long linesize = 3*width;
    if(linesize > 0) {
        while(linesize & 3 > 0) ++linesize; // Round to 4 bytes
    }
    return linesize * height;
}

bool wxVideoOutputDevice::InitializeOutput() {
    // First, let's set the width and height according to the panel's data
    wxSize tmpsize = m_Panel->GetSize();
    m_ColorFormat = vcfRGB24;
    m_Width = tmpsize.GetWidth();
    m_Height = tmpsize.GetHeight();
    ReAllocBuffer(m_Width,m_Height);
    return true;
}

void wxVideoOutputDevice::ReAllocBuffer(unsigned int newwidth,unsigned int newheight) {
    m_BufferLength = CalculateBufferLength(newwidth,newheight);
    if(m_Buffer==NULL || m_BufferSize < m_BufferLength) {
        if(m_Buffer!=NULL) {
            delete[] m_Buffer;
            m_Buffer = NULL;
        }
        m_Buffer = new char[m_BufferLength];
        m_BufferSize = m_BufferLength;
    }
}

void wxVideoOutputDevice::Clear() {
    if(!m_Buffer || m_Width == 0 || m_Height == 0) {
        return;
    }

    // Clear the buffer...
    unsigned long i;
    for(i = 0; i < m_BufferSize; ++i) {
        // We clear all the buffer and not just the visible part
        m_Buffer[i] = 0;
    }
    // ... and repaint the panel.
    RenderData();
}

void wxVideoOutputDevice::DisconnectOutput() {
    if(m_Buffer != NULL) {
        delete[] m_Buffer;
        m_Buffer = NULL;
    }
    m_BufferSize = 0;
    m_BufferLength = 0;
    m_BufferOwner = 0;
    m_BufferLockCount = 0;
}

bool wxVideoOutputDevice::ChangeDeviceSize(unsigned int newwidth,unsigned int newheight) {
    if(!LockBuffer()) {
        return false; // Error!
    }
    ReAllocBuffer(newwidth,newheight);
    // m_Width and m_Height change handled by VideoOutputDevice::ChangeSize(), no need to change here
    UnlockBuffer();
    return true;
}

bool wxVideoOutputDevice::LockBuffer() {
    syMutexLocker tmplock(*m_BufferMutex);
    bool result = (m_BufferOwner == 0 || (m_BufferOwner == syMutex::GetThreadId()));
    if(result) {
        if(!m_BufferLockCount) {
            m_BufferOwner = syMutex::GetThreadId();
        }
        ++m_BufferLockCount;
    }
    return result;
}

bool wxVideoOutputDevice::UnlockBuffer() {
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

char* wxVideoOutputDevice::GetBuffer() {
    return m_Buffer;
}

unsigned long wxVideoOutputDevice::GetBufferSize() {
    return m_BufferSize;
}

unsigned long wxVideoOutputDevice::GetBufferLength() {
    return m_BufferLength;
}

void wxVideoOutputDevice::LoadDeviceVideoData(VideoColorFormat colorformat, const char *buf,unsigned int buflen) {
    if(m_Width == 0 || m_Height == 0) {
        return;
    }
    bool abort = false;

    while(!LockBuffer()) {
        syMilliSleep(10); // Sleep for 10 msecs and try again
    }
    // Copy the data to our buffer
    if(buflen > m_BufferLength) {
        buflen = m_BufferLength;
    }
    unsigned int i,j;
    for(i = 0; i < buflen; ++i,++j) {
        if(j & 32768 == 0) { // Check every 32K
            j = 0;
            if(MustAbortPlayback()) {
                abort = true;
                break;
            }
        }
        m_Buffer[i] = buf[i];
    }
    UnlockBuffer();

    // Check again after we're finished
    if(!abort) {
        abort = MustAbortPlayback();
    }

    if(!abort) {
        // Copy the data to the Panel
        RenderData();
    }
}

void wxVideoOutputDevice::RenderData() {
    if(m_Width == 0 || m_Height == 0) {
        return;
    }
    m_Panel->FlagForRepaint(); // Just tell the Panel that the buffer has changed
}

// *** Begin wxVideoPanel code ***

wxVideoPanel::wxVideoPanel(wxWindow *parent) : wxPanel(parent),
m_Video(NULL),
m_IsPlaying(false),
m_SizeChanging(false),
m_BufferChanged(false)
{
    m_Video = new wxVideoOutputDevice(this);
    m_Video->Init();
}

wxVideoPanel::~wxVideoPanel() {
    delete m_Video;
    m_Video = NULL;
}

void wxVideoPanel::OnPaint(wxPaintEvent &event) {
    if(!m_Video || m_SizeChanging) {
        return;
    }
    if(!m_Video->IsOk()) {
        return;
    }
    if(!m_Video->LockBuffer()) {
        return;
    }

    // TODO: Draw screen with the buffer's pixel data. Note: If the buffer is NULL, then we must
    // repaint everything in black.


    m_Video->UnlockBuffer();
}

void wxVideoPanel::OnEraseBackground(wxEraseEvent &event) {
    // Do Nothing (this way we'll avoid flicker)
}

void wxVideoPanel::OnIdle(wxIdleEvent &event) {
    if(m_BufferChanged) {
        Refresh();
        m_BufferChanged = false;
    }
}

void wxVideoPanel::OnSize(wxSizeEvent& event) {
    if(m_Video == NULL) {
        return;
    }
    if(!m_Video->IsOk()) {
        return;
    }
    m_SizeChanging = true;
    wxSize newsize = event.GetSize();
    bool result = m_Video->ChangeSize(newsize.GetWidth(),newsize.GetHeight());
    m_SizeChanging = false;
    if(result) {
        FlagForRepaint();
    }
}

void wxVideoPanel::FlagForRepaint() {
    m_BufferChanged = true;
}
