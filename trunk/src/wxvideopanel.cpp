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
m_Panel(panel),
m_Connected(false)
{

}

wxVideoOutputDevice::~wxVideoOutputDevice() {
    m_Connected = false;
    delete[] m_Buffer;
    m_Buffer = NULL;
    m_BufferSize = 0;
    m_BufferLength = 0;
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
    m_Connected = true;
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
    if(!m_Connected || m_Width == 0 || m_Height == 0) {
        return;
    }

    // Clear the buffer...
    unsigned long i;
    for(i = 0; i < m_BufferLength; ++i) {
        m_Buffer[i] = 0;
    }
    // ... and repaint the panel.
    RenderData();
}

void wxVideoOutputDevice::DisconnectOutput() {
    m_Connected = false;
}

bool wxVideoOutputDevice::ChangeDeviceSize(unsigned int newwidth,unsigned int newheight) {
    if(!m_Connected) {
        return false;
    }
    ReAllocBuffer(newwidth,newheight);
    m_Width = newwidth;
    m_Height = newheight;

    return true;
}

void wxVideoOutputDevice::LoadDeviceVideoData(VideoColorFormat colorformat, const char *buf,unsigned int buflen) {
    if(!m_Connected || m_Width == 0 || m_Height == 0) {
        return;
    }
    bool abort = false;
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
    }

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
    if(!m_Connected || m_Width == 0 || m_Height == 0) {
        return;
    }
    // TODO: Render data into the panel
}

// *** Begin wxVideoPanel code ***

wxVideoPanel::wxVideoPanel(wxWindow *parent) : wxPanel(parent),
m_Video(NULL),
m_SizeChanged(false),
m_IsPlaying(false)
{
    m_Video = new wxVideoOutputDevice(this);
}

wxVideoPanel::~wxVideoPanel() {
    delete m_Video;
    m_Video = NULL;
}

void wxVideoPanel::OnPaint(wxPaintEvent &event) {
}

void wxVideoPanel::OnEraseBackground(wxEraseEvent &event) {
}

void wxVideoPanel::OnIdle(wxIdleEvent &event) {
}

void wxVideoPanel::OnSize(wxSizeEvent& event) {
}
