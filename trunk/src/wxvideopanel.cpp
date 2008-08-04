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
m_BackupBitmap(NULL),
m_Panel(panel)
{
    m_Bitmap = new syVODBitmap();
    m_BackupBitmap = new syBitmap();
    m_Bitmap->SetVOD(this);
}

wxVideoOutputDevice::~wxVideoOutputDevice() {
    ShutDown();
    delete m_BackupBitmap;
    delete m_Bitmap;
}

bool wxVideoOutputDevice::InitializeOutput() {
    // First, let's set the width and height according to the panel's data
    wxSize tmpsize = m_Panel->GetSize();
    m_ColorFormat = vcfRGB24;
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
    m_Bitmap->Lock(0,8); // unlimited lock attempts, 8ms each

    // Copy the current data into the backup bitmap
    m_BackupBitmap->CopyFrom(m_Bitmap);

    // Reallocate the current bitmap's buffer, if necessary
    m_Bitmap->Realloc(m_Width,m_Height,m_ColorFormat);

    // Now paste the old data
    m_Bitmap->PasteFrom(m_BackupBitmap);

    // And unlock
    m_Bitmap->Unlock();

    return true;
}

syVODBitmap* wxVideoOutputDevice::GetBitmap() {
    return m_Bitmap;
}

void wxVideoOutputDevice::LoadDeviceVideoData(syBitmap* bitmap) {
    if(!m_Width || !m_Height) {
        return;
    }

    m_Bitmap->Lock(0,10); // Lock the buffer with 10 msecs between lock attempts; Never stop trying.

    m_Bitmap->PasteFrom(bitmap,sy_stkeepaspectratio);
    // Copy from the source bitmap, stretching it as much as possible without distorting it.
    // Note: Since syBitmap checks our MustAbortPlayback() method, we don't need to worry about it

    m_Bitmap->Unlock(); // Don't forget to unlock the buffer

    if(!MustAbortPlayback()) { // If we're called to abort, skip this step
        RenderData(); // We're clear. Now copy the data to m_Panel.
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
    m_PaintingDemo = false;
    m_DemoBitmap = new syBitmap(200,100,vcfRGB24);
    m_Video = new wxVideoOutputDevice(this);
    m_Video->Init();
    Demo();
}

wxVideoPanel::~wxVideoPanel() {
    delete m_Video;
    delete m_DemoBitmap;
    m_Video = NULL;
}

void wxVideoPanel::OnPaint(wxPaintEvent &event) {
    if(m_SizeChanging) {
        // Do not try to repaint screen while resizing
        return;
    }
    if(m_Video && m_Video->IsOk()) {
        // Video is created and active. Let's copy it.
        m_Video->GetBitmap()->Lock(0,10); // Unlimited attempts, 10 ms each.
        {
            wxSize size = GetSize();
            unsigned int w = size.GetWidth();
            unsigned int h = size.GetHeight();
            wxBitmap* bmp = NULL;

            if(w == m_Video->GetWidth() && h == m_Video->GetHeight()) {
                // Same size, no problem
                bmp = new wxBitmap(wxImage(w,h,m_Video->GetBitmap()->GetBuffer(), true));
            } else {
                // Size has changed! We must resize the video.
                syBitmap sybmp(w,h,vcfRGB24);
                sybmp.PasteFrom(m_Video->GetBitmap(),sy_stkeepaspectratio);
                bmp = new wxBitmap(wxImage(w,h,sybmp.GetBuffer(), true));
            }
            m_Video->GetBitmap()->Unlock();
            { // We add another stack layer (the brace) for the dc so it gets deleted BEFORE our bitmap.
              // Otherwise, we'll get a segfault.
                wxBufferedPaintDC dc(this, *bmp);
            }
            delete bmp;
        }
    } else {
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
        event.RequestMore();
    }
//    Demo();
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
        for(y = 0; y < (int)(m_DemoBitmap->GetHeight()); ++y) {
            for(x = 0; x < (int)(m_DemoBitmap->GetWidth()); ++x) {
                tick = wxDateTime::UNow().GetMillisecond();
                pixel = y*y+(x*x) + tick;
                m_DemoBitmap->SetPixel(x,y,pixel);
            }
        }
        m_Video->LoadVideoData(m_DemoBitmap);
        m_PaintingDemo = false;
        syMilliSleep(10);
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
        Refresh();
    }
}

void wxVideoPanel::FlagForRepaint() {
    m_BufferChanged = true;
}
