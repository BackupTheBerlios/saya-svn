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

#include "../saya/core/sythread.h"
#include "../saya/core/sybitmap.h"
#include "../saya/core/sentryfuncs.h"
#include "../saya/core/videoinputdevice.h"
#include "../saya/core/app.h"

IMPLEMENT_CLASS(wxVideoPanel, wxPanel)

BEGIN_EVENT_TABLE(wxVideoPanel, wxPanel)
    EVT_PAINT(wxVideoPanel::OnPaint)
    EVT_ERASE_BACKGROUND(wxVideoPanel::OnEraseBackground)
    EVT_SIZE(wxVideoPanel::OnSize)
    EVT_IDLE(wxVideoPanel::OnIdle)
END_EVENT_TABLE()

// *** Begin DemoVideo1 code ***

class DemoVideo1 : public VideoInputDevice {
    public:
        DemoVideo1();
        virtual ~DemoVideo1();

    protected:

        /** @brief Loads the current frame into m_Bitmap.
         *
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadCurrentFrame(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         */
        void LoadCurrentFrame();
};

VideoInputDevice* CreateDemoVID() {
    return new DemoVideo1;
}

namespace DummyDemoVideo1 {
    bool dummybool = VideoInputDevice::RegisterVID("VID://Demo", &CreateDemoVID);
};

DemoVideo1::DemoVideo1() {
    m_Width = 200;
    m_Height = 100;
    m_ColorFormat = vcfBGR24;
}

DemoVideo1::~DemoVideo1() {
}

void DemoVideo1::LoadCurrentFrame() {
    unsigned long ticks = syGetTicks() / 5;
    long x, y;
    for (y = 0; y < (int)(m_Bitmap->GetHeight()); ++y) {
        for (x = 0; x < (int)(m_Bitmap->GetWidth()); ++x) {
            unsigned long pixel = (y * y + (x * x) + ticks) & 255;
            m_Bitmap->SetPixel(x, y, pixel);
        }
    }
}

// *** End DemoVideo1 code ***

// *** Begin wxVideoOutputDevice code ***

wxVideoOutputDevice::wxVideoOutputDevice(wxVideoPanel* panel) : VideoOutputDevice(),
        m_Panel(panel)
{
}

wxVideoOutputDevice::~wxVideoOutputDevice() {
}

bool wxVideoOutputDevice::Connect() {
    // First, let's set the width and height according to the panel's data
    wxSize tmpsize = m_Panel->GetSize();

    m_ColorFormat = vcfBGR24;
    m_Width = tmpsize.GetWidth();
    m_Height = tmpsize.GetHeight();
    return true;
}

bool wxVideoOutputDevice::AllocateResources() {
    return VideoOutputDevice::AllocateResources();
}

void wxVideoOutputDevice::Clear() {
    VideoOutputDevice::Clear();
}

void wxVideoOutputDevice::FreeResources() {
    VideoOutputDevice::FreeResources();
}

bool wxVideoOutputDevice::ChangeDeviceSize(unsigned int newwidth, unsigned int newheight) {
    return true;
}

void wxVideoOutputDevice::RenderVideoData(const syBitmap* bitmap) {
    if (m_Width == 0 || m_Height == 0) {
        return;
    }
    if(m_Panel) {
        m_Panel->LoadData(bitmap); // Copy the data to the wxPanel's internal bitmap
    }
}

void wxVideoOutputDevice::Detach() {
    ShutDown();
    m_Panel = 0;
}

// *** Begin wxVideoPanel code ***

wxVideoPanel::wxVideoPanel(wxWindow *parent) : wxPanel(parent),
        m_Video(NULL),
        m_IsPlaying(false),
        m_SizeChanging(false),
        m_BufferChanged(false),
        m_PaintingDemo(false),
        m_Bitmap(NULL),
        m_NativeFormat(
#ifdef __WIN32__
        vcfRGB24
#else
        vcfBGR24
#endif
        )
{
    m_Bitmap = new syBitmap();
    m_Video = new wxVideoOutputDevice(this);
    m_Video->Init();
}

wxVideoPanel::~wxVideoPanel() {
    m_Video->Detach();
    m_Video = 0;
    // We'll let the static class AVDeviceRegistry take care of the deletion.
    // We do this because the InputMonitor is managed by PlaybackManager, and it still has access to
    // the devices. And since this object will be deleted first, we need to shut down m_Video but not delete it,
    // to avoid leaving PlaybackManager with a dangling pointer.
    delete m_Bitmap;
}

void wxVideoPanel::OnPaint(wxPaintEvent &event) {
    if (m_SizeChanging) {
        // Do not try to repaint screen while resizing
        return;
    }
    // m_Video->FlushVideoData();
    // FlushVideoData must enabled only for testing; normally it must be called via AVController.
    // m_BufferChanged = false;
    // If we call FlushVideoData, we must set m_BufferChanged to false, or we'll continually
    // consume the CPU with an endless paint/idle event cycle.

    wxSize size = GetSize();
    unsigned int w = size.GetWidth();
    unsigned int h = size.GetHeight();
    sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
    if (lock.IsLocked()) {
        if (w > 0 && h > 0 && m_Video && m_Video->IsOk() && m_Bitmap->GetBuffer()) {
            // Video is created, active and available. Let's play our current bitmap.
            wxBitmap bmp(wxImage(w, h, m_Bitmap->GetBuffer(), true));
            lock.Unlock();
            wxBufferedPaintDC dc(this, bmp);
        } else {
            lock.Unlock();
            // Video has not yet been created. Let's paint a black bitmap.
            wxPaintDC pdc(this);
            wxBrush mybrush(*wxBLACK, wxSOLID);
            pdc.SetBackground(mybrush);
            pdc.Clear();
        }
    }
}

void wxVideoPanel::OnEraseBackground(wxEraseEvent &event) {
    // Do Nothing (this way we'll avoid flicker)
}

void wxVideoPanel::OnIdle(wxIdleEvent &event) {
    if (m_BufferChanged) {
        m_BufferChanged = false;
        Refresh();
    }
}

void wxVideoPanel::LoadData(const syBitmap* bitmap) {
    if (m_SizeChanging) {
        // Do not load the new data while resizing
        return;
    }
    if (m_Video && m_Video->IsOk()) {
        {
            sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
            if (lock.IsLocked()) {
                m_Bitmap->PasteFrom(bitmap);
            }
        }
        FlagForRepaint();
    }
}

void wxVideoPanel::OnSize(wxSizeEvent& event) {
    bool result = false;
    {
        syBoolSetter setter(m_SizeChanging, true);
        wxSize newsize = event.GetSize();
        if (m_Video && m_Video->IsOk()) {
            result = m_Video->ChangeSize(newsize.GetWidth(), newsize.GetHeight());
        }
        sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
        if (lock.IsLocked()) {
            m_Bitmap->Realloc(newsize.GetWidth(), newsize.GetHeight(), m_NativeFormat);
        }
    }
    if (result) {
        Refresh();
    }
}

void wxVideoPanel::FlagForRepaint() {
    m_BufferChanged = true;
    if(!syThread::IsMain()) {
        syApp::Get()->WakeUpIdle();
    }
    // This sends an idle event to the Event Loop running in the main thread.
    // We don't need to get hold of any GUI mutex, because all wxWakeupIdle() does is
    // wake up the main thread.
}

wxVideoOutputDevice* wxVideoPanel::GetVideo() {
    return m_Video;
}
