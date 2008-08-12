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

#include <wx/dcbuffer.h>
#include <wx/image.h>
#include <wx/bitmap.h>

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
m_Bitmap(NULL),
m_Panel(panel)
{
    m_Bitmap = new syBitmap();
    m_Bitmap->SetAborter(this);
}

wxVideoOutputDevice::~wxVideoOutputDevice() {
    ShutDown();
    delete m_Bitmap;
}

bool wxVideoOutputDevice::InitializeOutput() {
    // First, let's set the width and height according to the panel's data
    wxSize tmpsize = m_Panel->GetSize();
    m_ColorFormat = vcfBGR24;
    m_Width = tmpsize.GetWidth();
    m_Height = tmpsize.GetHeight();
    m_Bitmap->Realloc(m_Width,m_Height,m_ColorFormat);
    return true;
}

void wxVideoOutputDevice::Clear() {
    m_Bitmap->Clear(); // Clear the bitmap...
    // ... and repaint the panel.
    RenderData();
}

void wxVideoOutputDevice::DisconnectOutput() {
    m_Bitmap->ReleaseBuffer(false);
}

bool wxVideoOutputDevice::ChangeDeviceSize(unsigned int newwidth,unsigned int newheight) {
    m_Bitmap->Lock(0,1); // unlimited lock attempts, 1ms each

    // Reallocate the current bitmap's buffer, if necessary
    m_Bitmap->Realloc(m_Width,m_Height,m_ColorFormat);

    // And unlock
    m_Bitmap->Unlock();

    return true;
}

syBitmap* wxVideoOutputDevice::GetBitmap() {
    return m_Bitmap;
}

void wxVideoOutputDevice::LoadDeviceVideoData(syBitmap* bitmap) {
    if(!m_Width || !m_Height) {
        return;
    }

    m_Bitmap->Lock(0,1); // Lock the buffer with 1 msec between lock attempts; Never stop trying.

    m_Bitmap->PasteFrom(bitmap,sy_stkeepaspectratio);
    // Copy from the source bitmap, stretching it as much as possible without distorting it.
    // Note: Since syBitmap checks our MustAbort() method, we don't need to worry about it

    m_Bitmap->Unlock(); // Don't forget to unlock the buffer

    if(!MustAbort()) { // If we're called to abort, skip this step
        RenderData(); // We're clear. Now copy the data to m_Panel.
    }
}

void wxVideoOutputDevice::RenderData() {
    if(m_Width == 0 || m_Height == 0) {
        return;
    }
    m_Panel->LoadData(); // Copy the data to the wxPanel's internal bitmap
}

// *** Begin wxVideoPanel code ***

wxVideoPanel::wxVideoPanel(wxWindow *parent) : wxPanel(parent),
m_Video(NULL),
m_IsPlaying(false),
m_SizeChanging(false),
m_BufferChanged(false)
{
    m_PaintingDemo = false;
    m_DemoBitmap = new syBitmap(200,100,vcfBGR24);
    m_Bitmap = new syBitmap();
    m_Video = new wxVideoOutputDevice(this);
    m_Video->Init();
    Demo();
}

wxVideoPanel::~wxVideoPanel() {
    delete m_Video;
    delete m_DemoBitmap;
    delete m_Bitmap;
    m_Video = NULL;
}

void wxVideoPanel::OnPaint(wxPaintEvent &event) {
    if(m_SizeChanging) {
        // Do not try to repaint screen while resizing
        return;
    }
    wxSize size = GetSize();
    unsigned int w = size.GetWidth();
    unsigned int h = size.GetHeight();
    m_Bitmap->Lock(0,1);
    if(w > 0 && h > 0 && m_Video && m_Video->IsOk() && m_Bitmap->GetBuffer()) {
        // Video is created, active and available. Let's play our current bitmap.
        wxBitmap bmp(wxImage(w,h,m_Bitmap->GetBuffer(), true));
        m_Bitmap->Unlock(); // Unlock our own bitmap
        wxBufferedPaintDC dc(this, bmp);
    } else {
        m_Bitmap->Unlock(); // Unlock our own bitmap
        // Video has not yet been created. Let's paint a black bitmap.
        wxPaintDC pdc(this);
        wxBrush mybrush(*wxBLACK, wxSOLID);
        pdc.SetBackground(mybrush);
        pdc.Clear();
    }
}

void wxVideoPanel::OnEraseBackground(wxEraseEvent &event) {
    // Do Nothing (this way we'll avoid flicker)
}

void wxVideoPanel::OnIdle(wxIdleEvent &event) {
    if(m_BufferChanged) {
        m_BufferChanged = false;
        Refresh();
    }
    Demo();
}

void wxVideoPanel::LoadData() {
    if(m_SizeChanging) {
        // Do not load the new data while resizing
        return;
    }
    if(m_Video && m_Video->IsOk()) {
        m_Bitmap->Lock(0,1);
        m_Bitmap->PasteFrom(m_Video->GetBitmap());
        m_Bitmap->Unlock();
        FlagForRepaint();
    }
}

void wxVideoPanel::Demo() {
    if(!m_Video || m_PaintingDemo) {
        return;
    }
    if(m_Video->GetWidth() !=0 && m_Video->GetHeight() !=0) {
        m_PaintingDemo = true;
        // Let's paint some pretty colors!

        int x,y;
        unsigned long pixel,tick;
        tick = syGetTicks();
        // tick = wxDateTime::UNow().GetSecond() * 1000 + wxDateTime::UNow().GetMillisecond();
        tick /= 5;
        for(y = 0; y < (int)(m_DemoBitmap->GetHeight()); ++y) {
            for(x = 0; x < (int)(m_DemoBitmap->GetWidth()); ++x) {
                pixel = ((y*y+(x*x) + tick) & 255);
                m_DemoBitmap->SetPixel(x,y,pixel);
            }
        }
        m_Video->LoadVideoData(m_DemoBitmap);
        m_PaintingDemo = false;
        syMilliSleep(10);
    }
}

void wxVideoPanel::OnSize(wxSizeEvent& event) {
    bool result = false;
    m_SizeChanging = true;
    wxSize newsize = event.GetSize();
    if(m_Video && m_Video->IsOk()) {
        result = m_Video->ChangeSize(newsize.GetWidth(),newsize.GetHeight());
    }
    m_Bitmap->Lock(0,1);
    m_Bitmap->Realloc(newsize.GetWidth(),newsize.GetHeight(),vcfBGR24);
    m_Bitmap->Unlock();
    m_SizeChanging = false;
    if(result) {
        Refresh();
    }
}

void wxVideoPanel::FlagForRepaint() {
    m_BufferChanged = true;
}
