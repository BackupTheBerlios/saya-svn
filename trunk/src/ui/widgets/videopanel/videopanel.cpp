/**************************************************************************************
 * Name:      videopanel.cpp
 * Purpose:   implementation of class VideoPanel
 * Authors:   Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3 or later
 **************************************************************************************/

#include "../../../saya/core/debuglog.h"
#include "../../../saya/core/videooutputdevice.h"
#include "../../../saya/core/sythread.h"
#include "../../../saya/core/sybitmap.h"
#include "../../../saya/core/sentryfuncs.h"
#include "../../../saya/core/app.h"
#include "videopanel.h"

#include <QPainter>
#include <QPaintEvent>
#include <QBitmap>
#include <QCoreApplication>

#if defined(Q_WS_X11)
    #include <QX11Info>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
#elif defined(Q_WS_WIN)
    //
#endif

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

        #if defined(Q_WS_X11)
            /** @brief X11 Bitmap Renderer.
             *  @return true on success.
             */
            bool X11Render(const syBitmap* bitmap);
        #elif defined(Q_WS_WIN)
            /** @brief Win32 Bitmap Renderer.
             *  @return true on success.
             */
            bool Win32Render(const syBitmap* bitmap);
        #else
            /** @brief Do-Nothing Bitmap Renderer.
             *  @return false always (no native renderer could be implemented).
             */
            bool DoNothingRender(const syBitmap* bitmap) { return false; }
        #endif

        /** @brief tries to render the image using the Operating System's native rendering functions. */
        bool NativeRender(const syBitmap* bitmap);

        void QtRender(const syBitmap* bitmap, QPainter& painter);

        VideoPanel* m_Parent;
        VideoOutputDevice* m_Video;
        volatile bool m_IsPlaying;
        volatile bool m_SizeChanging;
        volatile bool m_BufferChanged;
        bool m_PaintingDemo;

        /** We need a bitmap besides the one in VideoOutputDevice to hold the video data temporarily.
         *  Otherwise we can get into a lot of awful situations that are nearly impossible to debug.
         *  To avoid hitting a mutex while updating the bitmap, we'll use TWO bitmaps.
         */
        syBitmap* m_Bitmap1;
        syBitmap* m_Bitmap2;
        volatile syBitmap* m_BitmapPointer;
        VideoColorFormat m_NativeFormat;
        bool m_UseNativeRenderer;
        #ifdef Q_WS_X11
        /** @brief Updates our XImage data with our current bitmap.
         *  @return true on success; false otherwise.
         */
        bool UpdateX11Image(const syBitmap* bitmap);
        #endif
        XImage* m_XImage;
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
m_NativeFormat(vcfRGB32), // by default until we can get the display's capabilities
m_UseNativeRenderer(false)
{
#if defined(Q_WS_X11)
    m_XImage = new XImage;
    const QX11Info &info = m_Parent->x11Info();
    int depth = info.depth();
    switch(depth) {
        case 8:
            m_NativeFormat = vcfRGB8;
            m_UseNativeRenderer = true;
            break;
        case 15:
            m_NativeFormat = vcfRGB15;
            m_UseNativeRenderer = true;
            break;
        case 16:
            m_NativeFormat = vcfRGB16;
            m_UseNativeRenderer = true;
            break;
        case 24:
            m_NativeFormat = vcfRGB24;
            m_UseNativeRenderer = true;
            break;
        case 32:
            m_NativeFormat = vcfRGB24;
            m_UseNativeRenderer = true;
            break;
        default:
            m_NativeFormat = vcfRGB32;
            m_UseNativeRenderer = false;
    }
    m_UseNativeRenderer = false; // Something's not working with our X11 rendering implementation.
#endif
    m_Bitmap1->Clear();
    m_Bitmap2->Clear();
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

    #ifdef Q_WS_X11
    delete m_XImage;
    m_XImage = 0;
    #endif

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

void VideoPanel::Data::QtRender(const syBitmap* bitmap, QPainter& painter) {
    unsigned int w = GetWidth();
    unsigned int h = GetHeight();
    QImage tmpbitmap(bitmap->GetReadOnlyBuffer(), w,h, QImage::Format_RGB32);
    painter.drawImage(0,0,tmpbitmap,0,0,-1,-1,Qt::ColorOnly);
}

bool VideoPanel::Data::UpdateX11Image(const syBitmap* bitmap) {
    m_XImage->width  = bitmap->GetWidth();
    m_XImage->height = bitmap->GetHeight();
    m_XImage->xoffset = 0;
    m_XImage->format = ZPixmap;
    m_XImage->data = (char*)(const_cast<syBitmap*>(bitmap)->GetBuffer());
    m_XImage->byte_order = LSBFirst;

    // Not sure if this should be bitmap->GetRowPadding(), too. (I hate Xlib's poor documentation!)
    m_XImage->bitmap_unit = bitmap->GetPixelPadding();
    m_XImage->bitmap_bit_order = LSBFirst;

    // From http://tronche.com/gui/x/xlib/utilities/XCreateImage.html :
    // bitmap_pad : Specifies the quantum of a scanline (8, 16, or 32). In other words, the start of one scanline is
    // separated in client memory from the start of the next scanline by an integer multiple of this many bits.
    m_XImage->bitmap_pad = bitmap->GetRowPadding();

    m_XImage->depth = bitmap->GetDepth();
    m_XImage->bytes_per_line = bitmap->GetBytesPerLine();
    m_XImage->bits_per_pixel = bitmap->GetBytesPerPixel()*8;
    m_XImage->red_mask = bitmap->GetRedMask();
    m_XImage->green_mask = bitmap->GetGreenMask();
    m_XImage->blue_mask = bitmap->GetBlueMask();
    return XInitImage(m_XImage) ? true : false;
};

#ifdef Q_WS_X11
bool VideoPanel::Data::X11Render(const syBitmap* bitmap) {
    bool result = false;
    const QX11Info &info = m_Parent->x11Info();
    Drawable x11handle = m_Parent->x11PictureHandle();
    if(!x11handle) {
        return false; // The X11 version in this computer is too old to support XRender.
    }
    unsigned int w = bitmap->GetWidth();
    unsigned int h = bitmap->GetHeight();
    if(UpdateX11Image(bitmap)) {
        XGCValues gcvalues;
        GC gc = XCreateGC(info.display(),x11handle, 0,&gcvalues);
        XPutImage(info.display(), x11handle, gc, m_XImage, 0,0,0,0,w,h);
        m_XImage->data = 0; // Tell X11 that our image data must not be destroyed (it's ours)
        XFreeGC(info.display(),gc);
        result = true;
    } else {
        result = false;
    }
    return result;
}
#endif

#ifdef Q_WS_WIN
bool VideoPanel::Data::Win32Render(const syBitmap* bitmap) {
    return false; // Not implemented
}
#endif

bool VideoPanel::Data::NativeRender(const syBitmap* bitmap) {
    if(!m_UseNativeRenderer) {
        return false;
    }
    #if defined(Q_WS_X11)
        return X11Render(bitmap);
    #elif defined(Q_WS_WIN)
        return Win32Render(bitmap);
    #else
        return false;
    #endif
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
    QPainter painter(this); // Required by Qt
    unsigned int w = width();
    unsigned int h = height();
    if (!m_Data || m_Data->m_SizeChanging) {
        // Do not try to repaint screen while resizing, or if m_Data is null.
        // Instead we'll fill the screen with black.
        painter.fillRect(0,0,w,h,QColor(0,0,0));
        return;
    }
    // m_Data->m_Video->FlushVideoData();
    // FlushVideoData must enabled only for testing; normally it must be called via AVController.
    // m_Data->m_BufferChanged = false;
    // If we call FlushVideoData, we must set m_BufferChanged to false, or we'll continually
    // consume the CPU with an endless paint/idle event cycle.

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
            if(!m_Data->NativeRender(currentbitmap)) {
                m_Data->QtRender(currentbitmap, painter);
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
