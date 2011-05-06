/***************************************************************
 * Name:      sybitmap.cpp
 * Purpose:   Implementation of the syBitmap class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-03
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  After some analysis, I realized we need an
 *            implementation of an image buffer with added
 *            height, width and color format so that we can
 *            convert between different size/format for
 *            display. This isn't intended to be a replacement
 *            for fully-featured bitmap libraries. Just for
 *            buffering between the VideoOutputDevice and
 *            the actual display.
 **************************************************************/

#include "aborter.h"
#include "sythread.h"
#include "sybitmap.h"
#include "sybitmapcopier.h"
#include "codecplugin.h"
#include "sentryfuncs.h"
#include "base64.h"
#include "resampler/resampler.h"
#include <math.h>
#include <cstddef>

class syBitmap::Data {

    public:
        Data();

        /** Width in pixels */
        unsigned int m_Width;

        /** Height in pixels */
        unsigned int m_Height;

        /** The Buffer's Color Format */
        VideoColorFormat m_ColorFormat;

        /** The actual memory buffer for the operations */
        unsigned char* m_Buffer;

        /** Length in bytes of the current buffer's workable area */
        unsigned long m_BufferLength;

        /** Length in bytes of the whole buffer */
        unsigned long m_BufferSize;

        /** Bytes per pixel of the current color format */
        unsigned int m_bypp;

        /** Bit depth for all color channels. */
        unsigned int m_Depth;

        /** Red color mask. */
        unsigned int m_RMask;
        /** Green color mask. */
        unsigned int m_GMask;
        /** Blue color mask. */
        unsigned int m_BMask;

        /** Length in bytes of each row */
        int m_RowSize;

        /** @brief Placeholder for an syAborter object.
          *
          * @see syBitmap::MustAbort
          */
        mutable syAborter* m_Aborter;

        /** @brief For use by PasteFrom when scaling bitmaps.
         *
         *  Contains the source bitmap's y offsets (in bytes) corresponding to our nth row.
         */
        mutable int* m_YOffsets;

        /** @brief For use by PasteFrom when scaling bitmaps.
         *
         *  Contains the source bitmap's x offsets (in bytes) corresponding to our nth column.
         */
        mutable int* m_XOffsets;

        /** Contains the current size used by m_YOffsets */
        mutable unsigned long m_YOffsetsSize;

        /** Contains the current size used by m_XOffsets */
        mutable unsigned long m_XOffsetsSize;
};

syBitmap::Data::Data() :
    m_Width(0),
    m_Height(0),
    m_ColorFormat(vcfBGR32),
    m_Buffer(NULL),
    m_BufferLength(0),
    m_BufferSize(0),
    m_bypp(4),
    m_Depth(0),
    m_RMask(0),
    m_GMask(0),
    m_BMask(0),
    m_RowSize(0),
    m_Aborter(NULL),
    m_YOffsets(NULL),
    m_XOffsets(NULL),
    m_YOffsetsSize(0),
    m_XOffsetsSize(0)
{}

syBitmap::syBitmap() :
m_Mutex(new sySafeMutex(true)),
m_Data(new Data)
{
}

syBitmap::syBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat) :
m_Mutex(new sySafeMutex(true)),
m_Data(new Data)
{
    Realloc(width, height, colorformat);
}

syBitmap::~syBitmap() {
    delete[] m_Data->m_XOffsets;
    delete[] m_Data->m_YOffsets;
    delete m_Mutex;
    delete m_Data;
}

void syBitmap::Create(unsigned int width,unsigned int height,VideoColorFormat colorformat) {
    Realloc(width, height, colorformat);
}

void syBitmap::Realloc(unsigned int newwidth,unsigned int newheight,VideoColorFormat newformat) {
    if(!newwidth || !newheight) {
        return;
    }
    unsigned int bypp = syBitmap::CalculateBytesperPixel(newformat);
    unsigned int rowlen = bypp * newwidth;

    if(rowlen & 1 || rowlen < 2) {
        rowlen = (rowlen + 2) & ~1; // Round up the row size to 2 bytes.
    }

    if(newformat == vcfBGR24_Line32 || newformat == vcfRGB24_Line32) {
        if(rowlen & 3 || rowlen < 4) {
            rowlen = (rowlen + 4) & ~3; // Round up the row size to 4 bytes.
        }
    }
    unsigned int newsize = rowlen * newheight;
    if(newsize & 3 || newsize < 4) {
        newsize = (newsize + 4) & ~3; // Round up the buffer size to 4 bytes.
    }

    if(newsize > m_Data->m_BufferSize) {

        if(m_Data->m_Buffer != NULL) {
            delete[] m_Data->m_Buffer;
            m_Data->m_Buffer = NULL;
        }
        m_Data->m_Buffer = new unsigned char[newsize];
        m_Data->m_BufferSize = newsize;
    }

    if(newheight > m_Data->m_YOffsetsSize) {
        if(m_Data->m_YOffsets != NULL) {
            delete[] m_Data->m_YOffsets;
            m_Data->m_YOffsets = NULL;
        }
        m_Data->m_YOffsets = new int[newheight];
        m_Data->m_YOffsetsSize = newheight;
    }

    if(newwidth > m_Data->m_XOffsetsSize) {
        if(m_Data->m_XOffsets != NULL) {
            delete[] m_Data->m_XOffsets;
            m_Data->m_XOffsets = NULL;
        }
        m_Data->m_XOffsets = new int[newwidth];
        m_Data->m_XOffsetsSize = newwidth;
    }

    m_Data->m_BufferLength = newsize;
    m_Data->m_ColorFormat = newformat;
    m_Data->m_bypp = CalculateBytesperPixel(newformat);
    m_Data->m_Width = newwidth;
    m_Data->m_Height = newheight;
    m_Data->m_RowSize = newwidth*m_Data->m_bypp;
    int depth;
    switch(newformat) {
        case vcfRGB8:
            depth = 8;
            m_Data->m_RMask = 0x07;
            m_Data->m_GMask = 0x38;
            m_Data->m_BMask = 0xc0;
            break;
        case vcfRGB32:
            depth = 24;
            m_Data->m_RMask = 0x000000ff;
            m_Data->m_GMask = 0x0000ff00;
            m_Data->m_BMask = 0x00ff0000;
            break;
        case vcfBGR32:
            depth = 24;
            m_Data->m_RMask = 0x00ff0000;
            m_Data->m_GMask = 0x0000ff00;
            m_Data->m_BMask = 0x000000ff;
            break;
        case vcfRGB24:
        case vcfRGB24_Line32:
            depth = 24;
            m_Data->m_RMask = 0x000000ff;
            m_Data->m_GMask = 0x0000ff00;
            m_Data->m_BMask = 0x00ff0000;
            break;
        case vcfBGR24:
        case vcfBGR24_Line32:
            depth = 24;
            m_Data->m_RMask = 0x00ff0000;
            m_Data->m_GMask = 0x0000ff00;
            m_Data->m_BMask = 0x000000ff;
            break;
        case vcfRGB16:
            m_Data->m_RMask = 0x0000001f;
            m_Data->m_GMask = 0x000007e0;
            m_Data->m_BMask = 0x0000f800;
            depth = 16;
            break;
        case vcfBGR16:
            m_Data->m_RMask = 0x0000f800;
            m_Data->m_GMask = 0x000007e0;
            m_Data->m_BMask = 0x0000001f;
            depth = 16;
            break;
        case vcfRGB15:
            depth = 15;
            m_Data->m_RMask = 0x0000001f;
            m_Data->m_GMask = 0x000003e0;
            m_Data->m_BMask = 0x00007c00;
            break;
        case vcfBGR15:
            depth = 15;
            m_Data->m_RMask = 0x00007c00;
            m_Data->m_GMask = 0x000003e0;
            m_Data->m_BMask = 0x0000001f;
            break;
        case vcfYUY2:
            depth = 8;
            break;
        case vcfYV12:
            depth = 12;
            m_Data->m_RMask = 0;
            m_Data->m_GMask = 0;
            m_Data->m_BMask = 0;
        case vcfYUV12:
            depth = 12;
            m_Data->m_RMask = 0;
            m_Data->m_GMask = 0;
            m_Data->m_BMask = 0;
            break;
        case vcfYUV:
        case vcfYVYU: // Not sure about these two, but since we're only using Depth() for X11
        case vcfUYVY: // and these formats aren't supported, it doesn't matter.
            depth = 16;
            m_Data->m_RMask = 0;
            m_Data->m_GMask = 0;
            m_Data->m_BMask = 0;
            break;
        case vcfYUY9:
            depth = 9;
            m_Data->m_RMask = 0;
            m_Data->m_GMask = 0;
            m_Data->m_BMask = 0;
            break;
        case vcfY800:
            m_Data->m_RMask = 0x01;
            m_Data->m_GMask = 0x01;
            m_Data->m_BMask = 0x01;
            depth = 1;
            break;
        default:
            ;
    }
    m_Data->m_Depth = depth;
    Clear();
}

bool syBitmap::ReleaseBuffer(bool force) {
     sySafeMutexLocker lock(*m_Mutex);
     bool isLockedNow = lock.IsLocked();
     bool result = isLockedNow;
     if(isLockedNow || force) {
         delete[] m_Data->m_Buffer;
         m_Data->m_Buffer = NULL;
         m_Data->m_BufferLength = 0;
         m_Data->m_BufferSize = 0;
         result = true;
    }
    return result;
}

unsigned char* syBitmap::GetBuffer() {
    return m_Data->m_Buffer;
}

const unsigned char* syBitmap::GetReadOnlyBuffer() const {
    return m_Data->m_Buffer;
}

unsigned int syBitmap::GetWidth() const {
    return m_Data->m_Width;
}

unsigned int syBitmap::GetHeight() const {
    return m_Data->m_Height;
}

VideoColorFormat syBitmap::GetColorFormat() const {
    return m_Data->m_ColorFormat;
}

unsigned long syBitmap::GetBufferLength() const {
    return m_Data->m_BufferLength;
}

unsigned long syBitmap::GetBufferSize() const {
    return m_Data->m_BufferSize;
}

int syBitmap::GetBytesPerPixel() const {
    return m_Data->m_bypp;
}

int syBitmap::GetRowPadding() const {
    int result = 8;
    int rowlen = m_Data->m_bypp * m_Data->m_Width;
    if(rowlen != m_Data->m_RowSize) {
        if(!m_Data->m_RowSize & 3) {
            result = 32;
        }
        if(!m_Data->m_RowSize & 1) {
            result = 16;
        }
    }
    return result;
}

int syBitmap::GetPixelPadding() const {
    int result = m_Data->m_bypp*8;
    if(result != 16 && result != 32) {
        result = 8;
    }
    return result;
}

int syBitmap::GetDepth() const {
    return m_Data->m_Depth;
}

int syBitmap::GetBytesPerLine() const {
    return m_Data->m_RowSize;
}

unsigned long syBitmap::GetRedMask() const {
    return m_Data->m_RMask;
}

unsigned long syBitmap::GetGreenMask() const {
    return m_Data->m_GMask;
}

unsigned long syBitmap::GetBlueMask() const {
    return m_Data->m_BMask;
}

unsigned int syBitmap::CalculateBytesperPixel(VideoColorFormat format) {
    unsigned int result = 4; // Default
    switch(format) {
        case vcfRGB8:
            result = 1;
            break;
        case vcfRGB32:
            result = 4;
            break;
        case vcfRGB24:
        case vcfRGB24_Line32:
            result = 3;
            break;
        case vcfRGB16:
        case vcfRGB15:
        case vcfBGR15:
        case vcfBGR16:
            result = 2;
            break;
        case vcfBGR24:
        case vcfBGR24_Line32:
            result = 3;
            break;
        case vcfBGR32:
            result = 4;
            break;
        case vcfYUY2:
            result = 2;
            break;
        case vcfYUV:
        case vcfYV12:
        case vcfYUV12:
        case vcfYVYU:
        case vcfUYVY:
        case vcfYUY9:
            result = 2;
            break;
        case vcfY800:
            result = 1;
    }
    return result;
}

void syBitmap::CopyFrom(const unsigned char* source, unsigned int width, unsigned int height, VideoColorFormat colorformat, unsigned long maxlength) {
    if(!source) {
        return;
    }
    Realloc(width,height,colorformat);

    const unsigned long* src = (const unsigned long*)source;
    unsigned long* dst = (unsigned long*)(m_Data->m_Buffer);

    // Copy the data in 4-byte chunks
    for(unsigned int i = maxlength >> 2; i; --i, ++src, ++dst) {
        if((i & 8192) == 0 && MustAbort()) { break; } // Check for abort every 32K
        *dst = *src;
    }

    // Copy the remaining 3 bytes
    unsigned int i = maxlength;
    i = (i > 3) ? (i - 3) : 0;
    for(; i < maxlength; ++i) {
        m_Data->m_Buffer[i] = source[i];
    }
}

void syBitmap::CopyFrom(const syBitmap* source) {
    if(!source) {
        return;
    }
    Realloc(source->GetWidth(),source->GetHeight(),source->GetColorFormat());
    const unsigned char* original_buffer = source->GetReadOnlyBuffer();
    if(!original_buffer) {
        return;
    }
    CopyFrom(original_buffer, source->GetWidth(),source->GetHeight(),source->GetColorFormat(),source->GetBufferLength());
}

void syBitmap::ResampleFrom(const syBitmap* source, syResampleMode resamplemode) {
    const char* filtername = Resampler::get_filter_name(resamplemode);
    if(!filtername) {
        return PasteFrom(source, sy_stkeepaspectratio); // Unknown filter. Use nearest-neighbor.
    }

   if (m_Data->m_Width > RESAMPLER_MAX_DIMENSION || m_Data->m_Height > RESAMPLER_MAX_DIMENSION) {
       return PasteFrom(source, sy_stkeepaspectratio); // Image too large. Use nearest-neighbor.

   }

   // Allocate memory for the operation.



}

void syBitmap::PasteFrom(const syBitmap* source,syStretchMode stretchmode) {
    if(!source || !source->GetReadOnlyBuffer()) {
        return;
    }

    do {
        unsigned int srcw = source->GetWidth();
        unsigned int srch = source->GetHeight();
        if(!srcw || !srch || !m_Data->m_Width || !m_Data->m_Height) {
            break; // Invalid width/height
        }
        int x,y;
        VideoColorFormat srcfmt = source->GetColorFormat();
        // Trivial case: Original has exactly the same dimensions and color format
        if(srcw == m_Data->m_Width && srch == m_Data->m_Height && srcfmt == m_Data->m_ColorFormat) {
            CopyFrom(source);
            break; // Get out of the do-while-false construct
        }


        syBitmapCopier copier;
        copier.Init(source, this);

        // Near trivial case: Same dimensions, different color format
        if(srcw == m_Data->m_Width && srch == m_Data->m_Height) {
            for(y = 0; y < (int)m_Data->m_Height; ++y) {
                if((y & 32) == 0 && MustAbort()) { break; } // Check for abort every 32 rows
                copier.CopyRowAndIncrementBoth();
            }
            break; // Get out of the do-while-false construct
        }


        // Nontrivial case: Resize and possibly stretch from the original
        int srcx, srcy;
        double xscale, yscale, xoffset, yoffset, tmpx, tmpy;

        if(stretchmode == sy_stcopy && (srcw > m_Data->m_Width || srch > m_Data->m_Height)) {
            stretchmode = sy_stkeepaspectratio;
        }

        // xscale, yscale, xoffset and yoffset are calculated according to the following formula:
        // srcx = (x * xscale) + xoffset // This formula is applied in the loop at the bottom
        // Ignoring the offset, we have:
        //      srcx = x * xscale;
        // Therefore,
        //      xscale = srcx / x;
        // If x = m_Data->m_Width, then:
        //      xscale = srcw / m_Data->m_Width.
        //
        // Now, if we want to make the centers fit together, then we have the following condition:
        //      srcx = srcw/2, and x = m_Data->m_Width/2.
        // Applying it to srcx = (x * xscale) + xoffset, we have:
        //      srcw/2 = (m_Data->m_Width/2 * xscale) + xoffset
        //      xoffset = srcw/2 - (m_Data->m_Width/2 * xscale).
        // By Getting the 1/2 out, we have:
        //      xoffset = (srcw - m_Data->m_Width*xscale)/2
        // And so, we end up with the following formulas:
        //
        // -----------------------------------------
        // xscale = srcw / m_Data->m_Width;
        // yscale = srch / m_Data->m_Height;
        // xoffset = (srcw - (m_Data->m_Width*xscale)) / 2;
        // yoffset = (srch - (m_Data->m_Height*yscale)) / 2;
        // -----------------------------------------

        switch(stretchmode) {
            case sy_stcopy: // Just center the bitmap
            xscale = 1.0;
            yscale = 1.0;
            xoffset = (double)(srcw - m_Data->m_Width) / 2;
            yoffset = (double)(srch - m_Data->m_Height) / 2;
            break;

            case sy_stkeepaspectratio: // Scale without distorting
            default: // Resize to fit
            xscale = ((double)srcw) / m_Data->m_Width;
            yscale = ((double)srch) / m_Data->m_Height;

            if(stretchmode == sy_stkeepaspectratio) {
                if(yscale >= xscale) { // Pick the greatest scale so that everything fits inside
                    xscale = yscale;
                } else {
                    yscale = xscale;
                }
            }
            xoffset = ((double)srcw - (double)(m_Data->m_Width*xscale)) / 2;
            yoffset = ((double)srch - (double)(m_Data->m_Height*yscale)) / 2;
        }

        // Now, we implement the formula srcx = (x * xscale) + xoffset into a for loop
        // But instead of just calculating x and y, we calculate the byte offsets, too!
        for(y = 0, tmpy = yoffset; y < (int)m_Data->m_Height; ++y, tmpy += yscale) {
            m_Data->m_YOffsets[y] = copier.m_SourceRowLength*int(floor(tmpy));
        }
        for(x = 0, tmpx = xoffset; x < (int)m_Data->m_Width; ++x, tmpx += xscale) {
            m_Data->m_XOffsets[x] = copier.m_SourceBypp*int(floor(tmpx));
        }

        copier.Reset();
        for(y = 0; y < (int)m_Data->m_Height; ++y, copier.m_Dst += copier.m_DestRowLength) {
            if((y & 32) == 0 && MustAbort()) { break; } // Check for abort every 32 rows
            srcy = m_Data->m_YOffsets[y];
            if((srcy < 0) | ((unsigned int)srcy >= copier.m_SourceBufferLength)) {
                copier.ClearRow();
            } else {
                unsigned int xoffset = 0;
                for(x = 0; x < (int)m_Data->m_Width; ++x, xoffset += copier.m_DestBypp) {
                    srcx = m_Data->m_XOffsets[x];
                    if((srcx < 0) || ((unsigned int)srcx >= copier.m_SourceRowLength)) {
                        copier.ClearPixelAt(xoffset);
                    } else {
                        copier.SetPixelAt(xoffset,syBitmapCopier::ConvertPixel(copier.GetPixelAt(srcy+srcx), srcfmt, m_Data->m_ColorFormat));
                    }
                }
            }
        }
    }while(false);
}

bool syBitmap::ExchangeWith(syBitmap* other) {
    if(!this || !other) {
        return false;
    }
    bool result = false;
    syAborter* aborter1 = m_Data->m_Aborter;
    syAborter* aborter2 = other->m_Data->m_Aborter;
    sySafeMutexLocker lock1(*m_Mutex, aborter1);
    sySafeMutexLocker lock2(*(other->m_Mutex), aborter1);
    if(lock1.IsLocked() && lock2.IsLocked()) {
        Data* tmpdata = other->m_Data;
        other->m_Data = m_Data;
        m_Data = tmpdata;
        other->m_Data->m_Aborter = aborter2;
        m_Data->m_Aborter = aborter1;
        result = true;
    }
    return result;
}

bool syBitmap::MustAbort() {
    if(m_Data->m_Aborter) {
        return m_Data->m_Aborter->MustAbort();
    }
    return syThread::MustAbort();
}

void syBitmap::SetAborter(syAborter* aborter) {
    m_Data->m_Aborter = aborter;
}

void syBitmap::Clear() {
    if(!m_Data->m_Buffer || m_Data->m_Width == 0 || m_Data->m_Height == 0) {
        return;
    }
    // Now we'll clear all the buffer and not just the visible part

    // By using pointer to ints, we can cut the clearing time by 4x.
    // In Realloc() we already made sure that our buffer fits in 4-byte chunks, so this is 100% safe.
    unsigned int* ptr = (unsigned int*)m_Data->m_Buffer;
    for(unsigned long i = m_Data->m_BufferSize >> 2;i; --i, ++ptr) {
        *ptr = 0;
    }
}

unsigned char* syBitmap::GetRow(int y) {
    if(m_Data->m_Width == 0 || m_Data->m_Height == 0 || y < 0 || y >= (int)m_Data->m_Height) {
        return NULL;
    }
    unsigned long w = m_Data->m_Width*m_Data->m_bypp;
    return (m_Data->m_Buffer + (w*y));
}

unsigned char* syBitmap::GetPixelAddr(int x, int y) {
    unsigned char* base = GetRow(y);
    if(x < 0 || x >= (int)m_Data->m_Width) {
        base = NULL;
    }
    if(base != NULL) {
        base += (x*m_Data->m_bypp);
    }
    return base;
}

const unsigned char* syBitmap::GetReadOnlyRow(int y) const {
    if(m_Data->m_Width == 0 || m_Data->m_Height == 0 || y < 0 || y >= (int)m_Data->m_Height) {
        return NULL;
    }
    unsigned long w = m_Data->m_Width*m_Data->m_bypp;
    return (m_Data->m_Buffer + (w*y));
}

const unsigned char* syBitmap::GetReadOnlyPixelAddr(int x, int y) const {
    const unsigned char* base = GetReadOnlyRow(y);
    if(x < 0 || x >= (int)m_Data->m_Width) {
        base = NULL;
    }
    if(base != NULL) {
        base += (x*m_Data->m_bypp);
    }
    return base;
}

unsigned long syBitmap::GetPixel(const unsigned char* addr) const {
    unsigned int i;
    unsigned long result = 0;
    if(addr != NULL) {
        for(i = 0; i < m_Data->m_bypp; ++i) {
            result |= ((*addr & 255) << (8*i)) ;
            ++addr;
        }
    }
    return result;
}

void syBitmap::SetPixel(unsigned char* addr, unsigned long pixel) {
    unsigned int i;
    if(addr != NULL) {
        for(i = 0; i < m_Data->m_bypp; ++i) {
            *addr = pixel & 255;
            pixel >>= 8;
            ++addr;
        }
    }
}


unsigned long syBitmap::GetPixel(int x, int y) const {
    unsigned int i;
    unsigned long result = 0;
    const unsigned char* base = GetReadOnlyPixelAddr(x,y);
    if(base != NULL) {
        for(i = 0; i < m_Data->m_bypp; ++i) {
            result |= ((*base & 255) << (8*i)) ;
            ++base;
        }
    }
    return result;
}

void syBitmap::SetPixel(int x, int y, unsigned long pixel) {
    unsigned int i;
    unsigned char* base = GetPixelAddr(x,y);
    if(base != NULL) {
        for(i = 0; i < m_Data->m_bypp; ++i) {
            *base = pixel & 255;
            pixel >>= 8;
            ++base;
        }
    }
}

void syBitmap::CopyPixel(unsigned char* src,unsigned char* dst,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {
    unsigned int srcbypp = syBitmap::CalculateBytesperPixel(sourcefmt);
    unsigned int i;
    if(sourcefmt == destfmt) {
        for(i = 0; i < srcbypp; ++i, ++src, ++dst) {
            *dst = *src;
        }
    } else {
        unsigned int dstbypp = syBitmap::CalculateBytesperPixel(destfmt);
        unsigned long pixel = 0;
        for(i = 0; i < srcbypp; ++i, ++src) {
            pixel = (pixel << 8) | (*src & 255);
        }
        pixel = syBitmapCopier::ConvertPixel(pixel, sourcefmt, destfmt);
        for(i = 0; i < dstbypp; ++i, ++dst) {
            *dst = pixel & 255;
            pixel >>= 8;
        }
    }
}

unsigned long syBitmap::ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {
    return syBitmapCopier::ConvertPixel(pixel,sourcefmt,destfmt);
}

/** Makes a copy of another syBitmap. */
void syBitmap::LoadData(const syBitmap* bitmap) {
    if(!bitmap || !this) return;
    CopyFrom(bitmap);
}

// For reading a bitmap from a file

bool syBitmap::LoadFromFile(const char* filename) {
    return LoadFromFile(syString(filename, true));
}

bool syBitmap::LoadFromFile(const syString& filename) {
    bool result = false;
    CodecPlugin* plugin = CodecPlugin::FindReadPlugin(filename);
    if(plugin) {
        CodecInstance* codec = plugin->OpenFile(filename);
        if(codec) {
            AutoDeleter<CodecInstance> deleter(codec); // We must dispose the codec instance after use
            codec->LoadCurrentFrame(this);
            result = true;
        }
    }
    return result;
}

syBitmap* syBitmap::FromFile(const char* filename) {
    return syBitmap::FromFile(syString(filename, true));
}

syBitmap* syBitmap::FromFile(const syString& filename) {
    syBitmap* bitmap = new syBitmap;
    if(!bitmap->LoadFromFile(filename)) {
        delete bitmap;
        bitmap = 0;
    }
    return bitmap;
}

bool syBitmap::LoadFromString(const syString& data, const char* mimetype) {
    bool result = false;
    CodecPlugin* plugin = CodecPlugin::FindReadPluginByMimeType(mimetype);
    if(plugin) {
        CodecInstance* codec = plugin->OpenString(data, mimetype);
        if(codec) {
            AutoDeleter<CodecInstance> deleter(codec); // We must dispose the codec instance after use
            codec->LoadCurrentFrame(this);
            result = true;
        }
    }
    return result;
}

bool syBitmap::LoadFromBase64(const syString& data, const char* mimetype) {
    syString rawdata = base64_decode(data); // Rawdata now contains the file as it would exist on disk.
    return LoadFromString(rawdata, mimetype);
}


syBitmap* syBitmap::FromString(const syString& data, const char* mimetype) {
    syBitmap* bitmap = new syBitmap;
    if(!bitmap->LoadFromString(data, mimetype)) {
        delete bitmap;
        bitmap = 0;
    }
    return bitmap;
}

syBitmap* syBitmap::FromBase64(const syString& data, const char* mimetype) {
    syBitmap* bitmap = new syBitmap;
    if(!bitmap->LoadFromBase64(data, mimetype)) {
        delete bitmap;
        bitmap = 0;
    }
    return bitmap;
}
