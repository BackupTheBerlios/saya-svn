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

syBitmap::syBitmap() :
m_Width(0),
m_Height(0),
m_ColorFormat(vcfRGB32),
m_Buffer(NULL),
m_BufferLength(0),
m_BufferSize(0)
{

}

syBitmap::syBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat) :
m_Width(0),
m_Height(0),
m_ColorFormat(vcfRGB32),
m_Buffer(NULL),
m_BufferLength(0),
m_BufferSize(0)
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
    unsigned int bypp = syBitmap::CalculateBytesperPixel(newformat);
    unsigned int rowlen = bypp * newwidth;
    while(rowlen & 3) { ++rowlen; } // Round to 4
    unsigned int newsize = rowlen * newwidth;

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
        m_Buffer[i] = original_buffer[i];
    }
}

void syBitmap::PasteFrom(syBitmap* source,syStretchMode stretchmode) {
    if(!source || !source->GetBuffer()) {
        return;
    }

    do {
        // Trivial case: Original has exactly the same dimensions and color format
        if(source->GetWidth() == m_Width && source->GetHeight() == m_Height
           && source->GetColorFormat() == m_ColorFormat) {
            CopyFrom(source);
            break;
        }
        if(source->GetWidth() == m_Width && source->GetHeight() == m_Height) {
            // Same dimensions, different color format
            break;
            // TODO: Impelment bitmap pasting (same dimensions, different color format)
        }
        // Nontrivial case: Resize and possibly stretch from the original
        // TODO: Impelment bitmap pasting (different dimensions and color format)

    }while(false);
}

unsigned long syBitmap::GetPixel(unsigned int x, unsigned int y) {
    return 0;
    // TODO: Implement syBitmap::GetPixel
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

void syBitmap::SetPixel(unsigned int x, unsigned int y, unsigned long pixel) {
    // TODO: Implement syBitmap::SetPixel
}

unsigned char* syBitmap::GetRow(unsigned int y) {
    return NULL;
    // TODO: Implement syBitmap::GetRow
}

unsigned long syBitmap::ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {
    return 0;
    // TODO: Implement syBitmap::ConvertPixel

}

