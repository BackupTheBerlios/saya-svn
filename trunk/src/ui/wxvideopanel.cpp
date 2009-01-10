///***************************************************************************
// * Name:      wxvideopanel.cpp
// * Purpose:   Implementation of class wxVideoPanel
// * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
// * Created:   2008-07-30
// * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
// * License:   GPL version 3 or later
// ***************************************************************************/
//
//#include <wx/dcbuffer.h>
//#include <wx/image.h>
//#include <wx/bitmap.h>
//
//#include "wxvideopanel.h"
//
//#include "../saya/core/sythread.h"
//#include "../saya/core/sybitmap.h"
//#include "../saya/core/sentryfuncs.h"
//#include "../saya/core/app.h"
//
//IMPLEMENT_CLASS(wxVideoPanel, wxPanel)
//
//BEGIN_EVENT_TABLE(wxVideoPanel, wxPanel)
//    EVT_PAINT(wxVideoPanel::OnPaint)
//    EVT_ERASE_BACKGROUND(wxVideoPanel::OnEraseBackground)
//    EVT_SIZE(wxVideoPanel::OnSize)
//    EVT_IDLE(wxVideoPanel::OnIdle)
//END_EVENT_TABLE()
//
//// *** Begin wxVideoPanel code ***
//
//wxVideoPanel::wxVideoPanel(wxWindow *parent) : wxPanel(parent),
//        m_Video(NULL),
//        m_IsPlaying(false),
//        m_SizeChanging(false),
//        m_BufferChanged(false),
//        m_PaintingDemo(false),
//        m_Bitmap(NULL),
//        m_NativeFormat(
//#ifdef __WIN32__
//        vcfRGB24
//#else
//        vcfBGR24
//#endif
//        )
//{
//    m_Bitmap = new syBitmap();
//    m_Video = new VideoOutputDevice();
//    m_Video->SetBitmapSink(this);
//    m_Video->Init();
//}
//
//wxVideoPanel::~wxVideoPanel() {
//    m_Video->ShutDown();
//    m_Video->SetBitmapSink(0);
//    m_Video = 0;
//    // We'll let the static class AVDeviceRegistry take care of the deletion.
//    // We do this because the InputMonitor is managed by PlaybackManager, and it still has access to
//    // the devices. And since this object will be deleted first, we need to shut down m_Video but not delete it,
//    // to avoid leaving PlaybackManager with a dangling pointer.
//    delete m_Bitmap;
//}
//
//void wxVideoPanel::OnPaint(wxPaintEvent &event) {
//    if (m_SizeChanging) {
//        // Do not try to repaint screen while resizing
//        return;
//    }
//    // m_Video->FlushVideoData();
//    // FlushVideoData must enabled only for testing; normally it must be called via AVController.
//    // m_BufferChanged = false;
//    // If we call FlushVideoData, we must set m_BufferChanged to false, or we'll continually
//    // consume the CPU with an endless paint/idle event cycle.
//
//    wxSize size = GetSize();
//    unsigned int w = size.GetWidth();
//    unsigned int h = size.GetHeight();
//    sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
//    if (lock.IsLocked()) {
//        wxPaintDC dc(this); // Required by wxWindow::OnPaint()
//        wxClientDC cdc(this); // This is what we'll really use for the painting.
//        if (w > 0 && h > 0 && m_Video && m_Video->IsOk() && m_Bitmap->GetBuffer()) {
//            // Video is created, active and available. Let's play our current bitmap.
//            wxMemoryDC mdc;
//            wxBitmap bmp(wxImage(w, h, m_Bitmap->GetBuffer(), true));
//            lock.Unlock();
//            mdc.SelectObject(bmp);
//            cdc.Blit(0, 0, w, h, &mdc, 0, 0);
//        } else {
//            lock.Unlock();
//            // Video has not yet been created. Let's paint a black bitmap.
//            wxBrush mybrush(*wxBLACK, wxSOLID);
//            cdc.SetBackground(mybrush);
//            cdc.Clear();
//        }
//    }
//}
//
//void wxVideoPanel::OnEraseBackground(wxEraseEvent &event) {
//    // Do Nothing (this way we'll avoid flicker)
//}
//
//void wxVideoPanel::OnIdle(wxIdleEvent &event) {
//    if (m_BufferChanged) {
//        m_BufferChanged = false;
//        Refresh();
//        Update();
//    }
//}
//
//void wxVideoPanel::LoadData(const syBitmap* bitmap) {
//    if (m_SizeChanging) {
//        // Do not load the new data while resizing
//        return;
//    }
//    if (m_Video && m_Video->IsOk()) {
//        {
//            sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
//            if (lock.IsLocked()) {
//                m_Bitmap->PasteFrom(bitmap);
//            }
//        }
//        FlagForRepaint();
//    }
//}
//
//void wxVideoPanel::OnSize(wxSizeEvent& event) {
//    bool result = false;
//    {
//        syBoolSetter setter(m_SizeChanging, true);
//        wxSize newsize = event.GetSize();
//        if (m_Video && m_Video->IsOk()) {
//            result = m_Video->ChangeSize(newsize.GetWidth(), newsize.GetHeight());
//        }
//        sySafeMutexLocker lock(*(m_Bitmap->m_Mutex));
//        if (lock.IsLocked()) {
//            m_Bitmap->Realloc(newsize.GetWidth(), newsize.GetHeight(), m_NativeFormat);
//        }
//    }
//    if (result) {
//        Refresh();
//        Update();
//    }
//}
//
//void wxVideoPanel::FlagForRepaint() {
//    m_BufferChanged = true;
//    if(!syThread::IsMain()) {
//        syApp::Get()->WakeUpIdle();
//    }
//    // This sends an idle event to the Event Loop running in the main thread.
//    // We don't need to get hold of any GUI mutex, because all wxWakeupIdle() does is
//    // wake up the main thread.
//}
//
//VideoOutputDevice* wxVideoPanel::GetVideo() {
//    return m_Video;
//}
//
//unsigned int wxVideoPanel::GetWidth() const {
//    int w,h;
//    GetSize(&w,&h);
//    if(w < 0) return 0;
//    return w;
//}
//
//unsigned int wxVideoPanel::GetHeight() const  {
//    int w,h;
//    GetSize(&w,&h);
//    if(h < 0) return 0;
//    return h;
//}
//
//VideoColorFormat wxVideoPanel::GetColorFormat() const  {
//    return m_NativeFormat;
//}
//
//int wxVideoPanel::GetTop() const  {
//    return 0;
//}
//
//int wxVideoPanel::GetLeft() const  {
//    return 0;
//}
