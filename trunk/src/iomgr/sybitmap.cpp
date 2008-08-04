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

#include "sybitmap.h"
#include <math.h>

syBitmap::syBitmap() :
m_Width(0),
m_Height(0),
m_ColorFormat(vcfRGB32),
m_Buffer(NULL),
m_BufferLength(0),
m_BufferSize(0),
m_bypp(4)
{

}

syBitmap::syBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat) :
m_Width(0),
m_Height(0),
m_ColorFormat(vcfRGB32),
m_Buffer(NULL),
m_BufferLength(0),
m_BufferSize(0),
m_bypp(4)

{
    Realloc(width, height, colorformat);
}

syBitmap::~syBitmap() {
    if(m_Buffer != NULL) {
        delete[] m_Buffer;
    }
    m_Buffer = NULL;
    m_BufferLength = 0;
    m_BufferSize = 0;
    m_Width = 0;
    m_Height = 0;
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

    if(newsize > m_BufferSize) {

        if(m_Buffer != NULL) {
            delete[] m_Buffer;
            m_Buffer = NULL;
        }
        m_Buffer = new unsigned char[newsize];
        m_BufferSize = newsize;
    }
    m_BufferLength = newsize;
    m_ColorFormat = newformat;
    m_bypp = CalculateBytesperPixel(newformat);
    m_Width = newwidth;
    m_Height = newheight;
}


unsigned char* syBitmap::GetBuffer() {
    return m_Buffer;
}

unsigned int syBitmap::GetWidth() {
    return m_Width;
}

unsigned int syBitmap::GetHeight() {
    return m_Height;
}

VideoColorFormat syBitmap::GetColorFormat() {
    return m_ColorFormat;
}

unsigned long syBitmap::GetBufferLength() {
    return m_BufferLength;
}

unsigned long syBitmap::GetBufferSize() {
    return m_BufferSize;
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

void syBitmap::CopyFrom(syBitmap* source) {
    if(!source) {
        return;
    }
    Realloc(source->GetWidth(),source->GetHeight(),source->GetColorFormat());
    unsigned char* original_buffer = source->GetBuffer();
    if(!original_buffer) {
        return;
    }
    unsigned long i;
    unsigned long imax = source->GetBufferLength();
    for(i = 0; i < imax; ++i) {
        if((i & 32767) == 0 && MustAbort()) { break; } // Check for abort every 32K
        m_Buffer[i] = original_buffer[i];
    }
}

void syBitmap::PasteFrom(syBitmap* source,syStretchMode stretchmode) {
    if(!source || !source->GetBuffer()) {
        return;
    }

    do {
        unsigned int srcw = source->GetWidth();
        unsigned int srch = source->GetHeight();
        if(!srcw || !srch || !m_Width || !m_Height) {
            break; // Invalid width/height
        }
        int x,y;
        VideoColorFormat srcfmt = source->GetColorFormat();
        // Trivial case: Original has exactly the same dimensions and color format
        if(srcw == m_Width && srch == m_Height && srcfmt == m_ColorFormat) {
            CopyFrom(source);
            break; // Get out of the do-while-false construct
        }

        // Near trivial case: Same dimensions, different color format
        if(srcw == m_Width && srch == m_Height) {
            for(y = 0; y < (int)m_Height; ++y) {
                if((y & 32) == 0 && MustAbort()) { break; } // Check for abort every 32 rows
                for(x = 0; x < (int)m_Width; ++x) {
                    SetPixel(x,y,ConvertPixel(source->GetPixel(x,y), srcfmt, m_ColorFormat));
                }
            }
            break; // Get out of the do-while-false construct
        }

        // Nontrivial case: Resize and possibly stretch from the original
        int srcx, srcy;
        double xscale, yscale, xoffset, yoffset, tmpx, tmpy;

        if(stretchmode == sy_stcopy && (srcw > m_Width || srch > m_Height)) {
            stretchmode = sy_stkeepaspectratio;
        }

        // xscale, yscale, xoffset and yoffset are calculated according to the following formula:
        // srcx = (x * xscale) + xoffset + 0.5 // This formula is applied in the loop at the bottom
        // Ignoring the offset and the 0.5 for rounding, we have:
        //      srcx = x * xscale;
        // Therefore,
        //      xscale = srcx / x;
        // If x = m_Width, then:
        //      xscale = srcw / m_Width.
        //
        // Now, if we want to make the centers fit together, then we have the following condition:
        //      srcx = srcw/2, and x = m_Width/2.
        // Applying it to srcx = (x * xscale) + xoffset, we have:
        //      srcw/2 = (m_Width/2 * xscale) + xoffset
        //      xoffset = srcw/2 - (m_Width/2 * xscale).
        // By Getting the 1/2 out, we have:
        //      xoffset = (srcw - m_Width*xscale)/2
        // And so, we end up with the following formulas:
        //
        // -----------------------------------------
        // xscale = srcw / m_Width;
        // yscale = srch / m_Height;
        // xoffset = (srcw - (m_Width*xscale)) / 2;
        // yoffset = (srch - (m_Height*yscale)) / 2;
        // -----------------------------------------

        switch(stretchmode) {
            case sy_stcopy: // Just center the bitmap
            xscale = 1.0;
            yscale = 1.0;
            xoffset = (double)(srcw - m_Width) / 2;
            yoffset = (double)(srch - m_Height) / 2;
            break;

            case sy_stkeepaspectratio: // Scale without distorting
            default: // Resize to fit
            xscale = ((double)srcw) / m_Width;
            yscale = ((double)srch) / m_Height;

            if(stretchmode == sy_stkeepaspectratio) {
                if(yscale >= xscale) { // Pick the greatest scale so that everything fits inside
                    xscale = yscale;
                } else {
                    yscale = xscale;
                }
            }
            xoffset = ((double)srcw - (double)(m_Width*xscale)) / 2;
            yoffset = ((double)srch - (double)(m_Height*yscale)) / 2;
        }

        // And now we implement the formula srcx = (x * xscale) + xoffset + 0.5 into a for loop

        for(y = 0, tmpy = yoffset + 0.5; y < (int)m_Height; ++y, tmpy += yscale) {
            if((y & 32) == 0 && MustAbort()) { break; } // Check for abort every 32 rows
            srcy = int(floor(tmpy));
            for(x = 0, tmpx = xoffset + 0.5; x < (int)m_Width; ++x, tmpx += xscale) {
                // Simmetric rounding is done by adding 0.5 to tmpx beforehand.
                srcx = int(floor(tmpx));
                SetPixel(x,y,ConvertPixel(source->GetPixel(srcx,srcy), srcfmt, m_ColorFormat));
            }
        }
    }while(false);
}

bool syBitmap::MustAbort() {
    return false;
}

void syBitmap::Clear() {
    if(!m_Buffer || m_Width == 0 || m_Height == 0) {
        return;
    }

    unsigned long i;
    for(i = 0; i < m_BufferSize; ++i) {
        // We clear all the buffer and not just the visible part
        m_Buffer[i] = 0;
    }
}

unsigned char* syBitmap::GetRow(int y) {
    if(m_Width == 0 || m_Height == 0 || y < 0 || y >= (int)m_Height) {
        return NULL;
    }
    unsigned long w = m_Width*m_bypp;
    return (m_Buffer + (w*y));
}

unsigned char* syBitmap::GetPixelAddr(int x, int y) {
    unsigned char* base = GetRow(y);
    if(x < 0 || x >= (int)m_Width) {
        base = NULL;
    }
    if(base != NULL) {
        base += (x*m_bypp);
    }
    return base;
}

unsigned long syBitmap::GetPixel(int x, int y) {
    unsigned int i;
    unsigned long result = 0;
    unsigned char* base = GetPixelAddr(x,y);
    if(base != NULL) {
        for(i = 0; i < m_bypp; ++i) {
            result = (result << 8) | (*base & 255);
            base++;
        }
    }
    return result;
}

void syBitmap::SetPixel(int x, int y, unsigned long pixel) {
    unsigned int i;
    unsigned char* base = GetPixelAddr(x,y);
    if(base != NULL) {
        for(i = 0; i < m_bypp; ++i) {
            *base = pixel & 255;
            pixel >>= 8;
            base++;
        }
    }
}

unsigned long syBitmap::ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {
    if(sourcefmt == destfmt) { // Trivial case: Formats are the same
        return pixel;
    }
    // TODO: Implement non-trivial cases in syBitmap::ConvertPixel
    return pixel;
}

