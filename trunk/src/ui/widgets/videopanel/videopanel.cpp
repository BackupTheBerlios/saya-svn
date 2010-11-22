/**************************************************************************************
 * Name:      videopanel.cpp
 * Purpose:   implementation of class VideoPanel
 * Authors:   Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3 or later
 **************************************************************************************/

#include "videopanel.h"

#include "../../../saya/core/videooutputdevice.h"
#include "../../../saya/core/sythread.h"
#include "../../../saya/core/sybitmap.h"
#include "../../../saya/core/sentryfuncs.h"
#include "../../../saya/core/app.h"

#include <QPainter>
#include <QPaintEvent>
#include <QBitmap>
#include <QCoreApplication>

// ----------------------
// Begin VideoPanel::Data
// ----------------------

class VideoPanel::Data : public syBitmapSink {
    public:
        Data(VideoPanel* parent);
        virtual ~Data();

        /** @brief Loads data from m_Video->RenderData().
         *
         *  @see VideoOutputDevice::RenderData
         */
        void LoadData(const syBitmap* bitmap);

        /**
         * Tells the Panel that the buffer has changed. This function is thread-safe.
         */
        void FlagForRepaint();

        /** @brief Returns the Width. */
        unsigned int GetWidth() const;

        /** @brief Returns the Height. */
        unsigned int GetHeight() const;

        /** @brief Returns the distance from the screen top. */
        int GetTop() const;

        /** @brief Returns the distance from the screen top. */
        int GetLeft() const;

        /** @brief returns the current color format. */
        VideoColorFormat GetColorFormat() const;

        VideoPanel* m_Parent;
        VideoOutputDevice* m_Video;
        bool m_IsPlaying;
        mutable bool m_SizeChanging;
        mutable bool m_BufferChanged;
        bool m_PaintingDemo;

        /** We need a bitmap besides the one in VideoOutputDevice to hold the video data temporarily.
         *  Otherwise we can get into a lot of awful situations that are nearly impossible to debug.
         *  To avoid hitting a mutex while updating a bitmap, we'll use TWO bitmaps.
         */
        syBitmap* m_Bitmap1;
        syBitmap* m_Bitmap2;
        mutable syBitmap* m_BitmapPointer;

        VideoColorFormat m_NativeFormat;
};

VideoPanel::Data::Data(VideoPanel* parent) :
m_Parent(parent),
m_Video(new VideoOutputDevice()),
m_IsPlaying(false),
m_SizeChanging(false),
m_BufferChanged(false),
m_PaintingDemo(false),
m_Bitmap1(new syBitmap()),
m_Bitmap2(new syBitmap()),
m_BitmapPointer(m_Bitmap1),
m_NativeFormat(vcfRGB32) // Due to Qt limitations, we're restricted to use this format.
{
    m_Video->SetBitmapSink(this);
    m_Video->Init();
}

VideoPanel::Data::~Data() {
    m_Video->ShutDown();
    m_Video->SetBitmapSink(0);
    m_Video = 0;
    // We'll let the static class AVDeviceRegistry take care of the deletion.
    // We do this because the InputMonitor is managed by PlaybackManager, and it still has access to
    // the devices. And since this object will be deleted first, we need to shut down m_Video but not delete it,
    // to avoid leaving PlaybackManager with a dangling pointer.
    m_BitmapPointer = 0;
    delete m_Bitmap2;
    m_Bitmap2 = 0;
    delete m_Bitmap1;
    m_Bitmap1 = 0;

    m_Parent->m_Data = 0;
    m_Parent = 0;
}

unsigned int VideoPanel::Data::GetWidth() const {
    return m_Parent->width();
}

unsigned int VideoPanel::Data::GetHeight() const  {
    return m_Parent->height();
}

VideoColorFormat VideoPanel::Data::GetColorFormat() const  {
    return m_NativeFormat;
}

int VideoPanel::Data::GetTop() const  {
    return 0; // We don't need the exact screen coordinates for our implementation.
}

int VideoPanel::Data::GetLeft() const  {
    return 0; // We don't need the exact screen coordinates for our implementation.
}

void VideoPanel::Data::LoadData(const syBitmap* bitmap) {
    if (m_SizeChanging) {
        // Do not load the new data while resizing
        return;
    }
    if (m_Video && m_Video->IsOk()) {
        {
            // We will copy to the bitmap NOT currently assigned.
            // To avoid having to use a mutex, we use a comparison and switch between two predefined values.
            syBitmap* currentbitmap = m_Bitmap1;
            if(m_BitmapPointer == m_Bitmap1) { currentbitmap =  m_Bitmap2; }
            sySafeMutexLocker lock(*(currentbitmap->m_Mutex));
            if (lock.IsLocked()) {
                currentbitmap->PasteFrom(bitmap);
                m_BitmapPointer = currentbitmap;
                FlagForRepaint();
            }
        }
    }
}

void VideoPanel::Data::FlagForRepaint() {
    m_BufferChanged = true;
    QCoreApplication::postEvent(m_Parent, new QPaintEvent(m_Parent->rect()));
}

// --------------------
// End VideoPanel::Data
// --------------------

// ----------------
// Begin VideoPanel
// ----------------

//// *** Begin VideoPanel code ***
//
VideoPanel::VideoPanel(QWidget *parent) :
QWidget(parent),
m_Data(new Data(this))
{
    setAttribute(Qt::WA_OpaquePaintEvent, true); // To save CPU cycles, we don't erase the background on each repaint.
}

VideoPanel::~VideoPanel() {
    delete m_Data;
    m_Data = 0;
}

void VideoPanel::paintEvent ( QPaintEvent * pe ) {
    if (!m_Data || m_Data->m_SizeChanging) {
        // Do not try to repaint screen while resizing, or if m_Data is null
        return;
    }
    // m_Data->m_Video->FlushVideoData();
    // FlushVideoData must enabled only for testing; normally it must be called via AVController.
    // m_Data->m_BufferChanged = false;
    // If we call FlushVideoData, we must set m_BufferChanged to false, or we'll continually
    // consume the CPU with an endless paint/idle event cycle.

    unsigned int w = width();
    unsigned int h = height();

    QPainter painter(this); // Required by Qt
    // We will copy from the bitmap CURRENTLY assigned.
    // To avoid having to use a mutex, we use a comparison and switch between two predefined values.
    syBitmap* currentbitmap = m_Data->m_Bitmap1;
    if(m_Data->m_BitmapPointer != currentbitmap) { currentbitmap =  m_Data->m_Bitmap2; }

    sySafeMutexLocker lock(*(currentbitmap->m_Mutex));
    if (lock.IsLocked()) {
        if (m_Data->m_BufferChanged) {
            m_Data->m_BufferChanged = false;
        }
        if (w > 0 && h > 0 && m_Data && m_Data->m_Video && m_Data->m_Video->IsOk() && currentbitmap->GetBuffer()) {
            // Video is created, active and available. Let's play our current bitmap.
            // We enclose our temporary bitmap inside brackets to free our mutex as soon as painting ends.
            {
                QImage tmpbitmap(currentbitmap->GetBuffer(), w,h, QImage::Format_RGB32);
                painter.drawImage(0,0,tmpbitmap,0,0,-1,-1,Qt::ColorOnly);
            }
            lock.Unlock();
        } else {
            lock.Unlock();
            // Video has not yet been created. Let's paint a black bitmap.
            painter.fillRect(0,0,w,h,QColor(0,0,0));
        }
    }
}

void VideoPanel::resizeEvent(QResizeEvent *event) {
    bool result = false;
    {
        syBoolSetter setter(m_Data->m_SizeChanging, true);
        QSize newsize = event->size();
        if (m_Data && m_Data->m_Video && m_Data->m_Video->IsOk()) {
            result = m_Data->m_Video->ChangeSize(newsize.width(), newsize.height());
        }
        sySafeMutexLocker lock1(*(m_Data->m_Bitmap1->m_Mutex));
        if (lock1.IsLocked()) {
            m_Data->m_Bitmap1->Realloc(newsize.width(), newsize.height(), m_Data->m_NativeFormat);
        }
        sySafeMutexLocker lock2(*(m_Data->m_Bitmap2->m_Mutex));
        if (lock2.IsLocked()) {
            m_Data->m_Bitmap2->Realloc(newsize.width(), newsize.height(), m_Data->m_NativeFormat);
        }
    }
    if (result) {
        update();
    }
}

VideoOutputDevice* VideoPanel::GetVideo() const {
    return m_Data->m_Video;
}

syBitmapSink* VideoPanel::GetBitmapSink() const {
    return m_Data;
}

// --------------
// End VideoPanel
// --------------
