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
            result = 3;
            break;
        case vcfRGB16:
        case vcfRGB15:
        case vcfBGR15:
        case vcfBGR16:
            result = 2;
            break;
        case vcfBGR24:
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

void syBitmap::CopyFrom(const syBitmap* source) {
    if(!source) {
        return;
    }
    Realloc(source->GetWidth(),source->GetHeight(),source->GetColorFormat());
    const unsigned char* original_buffer = source->GetReadOnlyBuffer();
    if(!original_buffer) {
        return;
    }
    unsigned long imax = source->GetBufferLength();

    const unsigned long* src = (const unsigned long*)original_buffer;
    unsigned long* dst = (unsigned long*)(m_Data->m_Buffer);

    // Copy the data in 4-byte chunks
    for(unsigned int i = imax >> 2; i; --i, ++src, ++dst) {
        if((i & 8192) == 0 && MustAbort()) { break; } // Check for abort every 32K
        *dst = *src;
    }

    // Copy the remaining 3 bytes
    unsigned int i = imax;
    i = (i > 3) ? (i - 3) : 0;
    for(; i < imax; ++i) {
        m_Data->m_Buffer[i] = original_buffer[i];
    }
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

    // By using pointer to longs, we can cut the clearing time by 4x.
    // In Realloc() we already made sure that our buffer fits in 4-byte chunks, so this is 100% safe.
    unsigned long* ptr = (unsigned long*)m_Data->m_Buffer;
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
