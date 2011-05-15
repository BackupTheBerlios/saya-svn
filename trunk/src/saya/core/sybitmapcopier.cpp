/***************************************************************
 * Name:      sybitmapcopier.h
 * Purpose:   Implementation of the syBitmapCopier class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  syBitmapCopier is a lightweight class specialized
 *            in efficient data copying between syBitmaps.
 **************************************************************/

#include "sybitmapcopier.h"
#include "sybitmap.h"

bool syPixelContribBuffer::AddWeight(int sourcex,int destx,double weight) {
    if(m_Size >= m_Capacity) {
        return false;
    }
    syPixelContrib* pixel = &pixels[m_Size];
    pixel->srcx = sourcex;
    pixel->dstx = destx;
    pixel->weight = weight;
    ++m_Size;
    return true;
}

void syPixelContribBuffer::Clear() {
    m_Size = 0;
}

syBitmapCopier::syBitmapCopier() :
m_SourceBitmap(0),
m_DestBitmap(0),
m_Src(0),
m_Dst(0),
m_XContrib(0),
m_YContrib(0),
m_YBuffer(0),
m_FullBuffer(0)
{}

void syBitmapCopier::Init(const syBitmap *sourcebmp, syBitmap *destbmp, syFilterType filtertype) {
    if(!sourcebmp || !destbmp) { return; }
    m_SourceBitmap = sourcebmp;
    m_DestBitmap = destbmp;
    m_SourceFmt = sourcebmp->GetColorFormat();
    m_DestFmt = destbmp->GetColorFormat();
    m_SourceBypp = sourcebmp->GetBytesPerPixel();
    m_DestBypp = destbmp->GetBytesPerPixel();
    m_Src = sourcebmp->GetReadOnlyBuffer();
    m_Dst = destbmp->GetBuffer();
    m_SourceWidth = sourcebmp->GetWidth();
    m_DestWidth = destbmp->GetWidth();
    m_SourceHeight = sourcebmp->GetHeight();
    m_DestHeight = destbmp->GetHeight();
    m_SourceRowLength = sourcebmp->GetBytesPerLine();
    m_DestRowLength = destbmp->GetBytesPerLine();
    m_SourceBufferLength = sourcebmp->GetBufferLength();
    m_DestBufferLength = destbmp->GetBufferLength();
    if(m_XContrib) {
        delete m_XContrib;
        m_XContrib = 0;
    }
    if(m_YContrib) {
        delete m_YContrib;
        m_YContrib = 0;
    }
    if(m_YBuffer) {
        delete[] m_YBuffer;
        m_YBuffer = 0;
    }
    if(m_FullBuffer) {
        delete[] m_FullBuffer;
        m_FullBuffer = 0;
    }
    m_YBuffer  = new syFloatPixel[m_DestHeight];
    m_FullBuffer  = new syFloatPixel[m_DestWidth*m_SourceHeight];

    if(m_DestWidth == m_SourceWidth && m_DestHeight == m_SourceHeight) {
        m_EffectiveDestHeight = m_DestHeight;
        m_EffectiveDestWidth = m_DestWidth;
        m_XScale = 1.0;
        m_YScale = 1.0;
        m_DestX0 = 0;
        m_DestY0 = 0;
    } else {
        // Scale and keep aspect ratio
        double xscale = ((double)m_DestWidth) / m_SourceWidth;
        double yscale = ((double)m_DestHeight) / m_SourceHeight;
        if(yscale < xscale) { // Pick the smallest scale so that everything fits inside
            xscale = yscale;
            m_EffectiveDestWidth = (unsigned int)(m_SourceWidth / xscale);
            if(m_EffectiveDestWidth > m_DestWidth) {
                m_EffectiveDestWidth = m_DestWidth;
            }
        } else {
            yscale = xscale;
            m_EffectiveDestHeight = (unsigned int)(m_SourceHeight / yscale);
            if(m_EffectiveDestHeight > m_DestHeight) {
                m_EffectiveDestHeight = m_DestHeight;
            }
        }
        m_XScale = xscale;
        m_YScale = yscale;
        m_DestX0 = (m_DestWidth - m_EffectiveDestWidth)/2;
        m_DestY0 = (m_DestHeight - m_EffectiveDestHeight)/2;
        m_XContrib = 0;
        m_YContrib = 0;

        if(filtertype == filter_none) {
            m_Filter = 0;
        } else {
            m_Filter = syImageFilter::Create(filtertype);
            InitContribBuffers();
        }
    }
}

syBitmapCopier::~syBitmapCopier() {
    if(m_XContrib) {
        delete m_XContrib;
        m_XContrib = 0;
    }
    if(m_YContrib) {
        delete m_YContrib;
        m_YContrib = 0;
    }
    if(m_YBuffer) {
        delete[] m_YBuffer;
        m_YBuffer = 0;
    }
    if(m_FullBuffer) {
        delete[] m_FullBuffer;
        m_FullBuffer = 0;
    }
}

void syBitmapCopier::InitContribBuffers() {
    if(!m_Filter) {
        return;
    }
    if(m_XContrib) {
        delete m_XContrib;
        m_XContrib = 0;
    }
    if(m_YContrib) {
        delete m_YContrib;
        m_YContrib = 0;
    }

    int fullfilterwidth = (int)(ceil(m_Filter->GetWidth())*2+1);
    m_XContrib = new syPixelContribBuffer(fullfilterwidth*m_EffectiveDestWidth);
    m_YContrib = new syPixelContribBuffer(fullfilterwidth*m_EffectiveDestHeight);

    // Calculating X contributions

    double xdiff = 1/m_XScale;
    double filter_scale = 1.0; // Used for calculating the weights.
    if(m_XScale < 1.0) { // Downsampling
            filter_scale = m_XScale;
    }

    double center = m_DestX0/m_XScale;
    for(unsigned int x = m_DestX0; x < m_DestX0 + m_EffectiveDestWidth; ++x,center+=xdiff) {
        int left = (int)floor(center - m_Filter->GetWidth()); // It's actually the half-width; This is, the filter's radius
        if(left < 0) { left = 0; }
        int right = (int)floor(center + m_Filter->GetWidth());
        if(right >= (int)m_SourceWidth) { right = m_SourceWidth - 1; }

        // Check if we're taking samples outside the sliding window
        if(right - left >= fullfilterwidth) {
            // Either we increment left, or decrease right.
            // First, check if incrementing the left limit would pass the center of the source image
            if(left + 1 <= (int)m_SourceWidth / 2) {
                ++left;
            } else {
                --right;
            }
        }
        double weight_sum = 0; // For normalization
        int weight_count = 0;
        int srcx;
        for(srcx = left; srcx <= right; ++srcx) {
            double deltax = center - (double)srcx;
            double curweight = filter_scale * m_Filter->Filter(filter_scale * deltax);
            weight_sum += curweight;
            if(m_XContrib->AddWeight(srcx,x,curweight)) {
                ++weight_count;
            }
        }
        // Normalize the weights
        if(weight_sum > 0.000000001) { // We use a minimum to differentiate from zero
            for(int i = 1; i <= weight_count; ++i) {
                m_XContrib->pixels[m_XContrib->size() - i].weight /= weight_sum;
            }
        }
    }

    // Calculating Y contributions

    double ydiff = 1/m_XScale;
    filter_scale = 1.0; // Used for calculating the weights.
    if(m_YScale < 1.0) { // Downsampling
            filter_scale = m_YScale;
    }

    center = m_DestY0/m_YScale;
    for(unsigned int y = m_DestY0; y < m_DestY0 + m_EffectiveDestHeight; ++y,center+=ydiff) {
        int top = (int)floor(center - m_Filter->GetWidth()); // It's actually the half-width; This is, the filter's radius
        if(top < 0) { top = 0; }
        int bottom = (int)floor(center + m_Filter->GetWidth());
        if(bottom >= (int)m_SourceHeight) { bottom = m_SourceHeight - 1; }

        // Check if we're taking samples outside the sliding window
        if(bottom - top >= fullfilterwidth) {
            // Either we increment top, or decrease bottom.
            // First, check if incrementing the top limit would pass the center of the source image
            if(top + 1 <= (int)m_SourceHeight / 2) {
                ++top;
            } else {
                --bottom;
            }
        }
        double weight_sum = 0; // For normalization
        int weight_count = 0;
        int srcy;
        for(srcy = top; srcy <= bottom; ++srcy) {
            double deltay = center - (double)srcy;
            double curweight = filter_scale * m_Filter->Filter(filter_scale * deltay);
            weight_sum += curweight;
            if(m_YContrib->AddWeight(srcy,y,curweight)) {
                ++weight_count;
            }
        }
        // Normalize the weights
        if(weight_sum > 0.000000001) { // We use a minimum to differentiate from zero
            for(int i = 1; i <= weight_count; ++i) {
                m_YContrib->pixels[m_YContrib->size() - i].weight /= weight_sum;
            }
        }
    }
}

void syBitmapCopier::Reset() {
    m_Src = m_SourceBitmap->GetReadOnlyBuffer();
    m_Dst = m_DestBitmap->GetBuffer();
}

unsigned long syBitmapCopier::ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {

    // Let's get the "real" color format
    if(sourcefmt == vcfRGB24_Line32) {
        sourcefmt = vcfRGB24;
    }
    if(sourcefmt == vcfBGR24_Line32) {
        sourcefmt = vcfBGR24;
    }
    if(destfmt == vcfRGB24_Line32) {
        destfmt = vcfRGB24;
    }
    if(destfmt == vcfBGR24_Line32) {
        destfmt = vcfBGR24;
    }

    if(sourcefmt == destfmt) { // Trivial case: Formats are the same
        return pixel;
    }

    // Near trivial case: Swap B and R components in the pixel.
    if(
       (sourcefmt == vcfRGB24 && destfmt == vcfBGR24) ||
       (sourcefmt == vcfBGR24 && destfmt == vcfRGB24) ||
       (sourcefmt == vcfRGB32 && destfmt == vcfBGR32) ||
       (sourcefmt == vcfBGR32 && destfmt == vcfRGB32)
      ) {
        // Swap first and third bytes
        unsigned char c0, c1, c2, c3;
        c0 = pixel & 0xFF;
        c1 = (pixel >> 8) & 0xFF;
        c2 = (pixel >> 16) & 0xFF;
        c3 = (pixel >> 24) & 0xFF;
        pixel = (c3 << 24) | (c0 << 16) | (c1 << 8) | c2;
    }

    // TODO: Implement non-trivial cases in syBitmap::ConvertPixel
    return pixel;
}

void syBitmapCopier::ResampleRow(unsigned int y) {
    unsigned int i;
    const unsigned char* src = &m_Src[y*m_SourceWidth];
    syFloatPixel* dst = &m_FullBuffer[y*m_DestWidth];

    if(m_SourceWidth == m_DestWidth) {
        const unsigned long* srcrgba = (const unsigned long*)src;
        for(i = 0; i < m_SourceWidth; ++i) {
            dst[i].fromRGBA(ConvertPixel(srcrgba[i], m_SourceFmt,vcfRGB32));
        }
    } else if(m_XContrib) {
        // Clear destination buffer
        for(i = 0; i < m_DestWidth; ++i) {
            dst[i].clear();
        }

        // First we iterate over the source row and convert all the pixels to float.
        syFloatPixel floatpixels[m_SourceWidth];
        for(i = 0; i < m_SourceWidth; ++i) {
            floatpixels[i].fromRGBA(syBitmap::ConvertPixel(m_SourceBitmap->GetPixel(src,i),m_SourceFmt,vcfRGB32));
        }

        // Now we iterate over the contrib buffer and resample each pixel.
        for(i = 0; i < m_XContrib->size();++i) {
            syPixelContrib* contrib = &m_XContrib->pixels[i];
            if(contrib->srcx >= 0 && contrib->srcx < (int)m_SourceWidth) {
                dst[contrib->dstx].MultiplyAndAdd(floatpixels[contrib->srcx],contrib->weight);
            }
        }
    }
}

void syBitmapCopier::ResampleCol(unsigned int x) {
    syFloatPixel* src = m_FullBuffer;

    if(m_YContrib) {
        // 1. Clear the temporary Y buffer.
        unsigned int i;
        for(i = 0; i < m_DestHeight; ++i) {
            m_YBuffer[i].clear();
        }

        // 2. Iterate over the contrib buffer and resample each pixel.
        for(i = 0; i < m_YContrib->size();++i) {
            syPixelContrib* contrib = &m_YContrib->pixels[i];
            // Here, contrib->srcx and contrib->dstx refer to Y coordinates,
            // because we're operating on a x-to-y translated vector.
            int srcy = contrib->srcx;
            int dsty = contrib->dstx;
            if(srcy >= 0 && srcy < (int)m_SourceHeight && dsty >=0 && dsty < (int)m_DestHeight) {
                m_YBuffer[dsty].MultiplyAndAdd(src[srcy*m_DestWidth+x],contrib->weight);
            }
        }

        // TODO: 3. Convert the the float pixels into bitmap pixels and store them in dst.
        unsigned int xofs = x*m_DestBypp;
        for(i = 0; i < m_DestHeight; ++i) {
            SetPixelAt(xofs,ConvertPixel(m_YBuffer[i].toRGBA(),vcfRGB32,m_DestFmt));
            xofs += m_DestRowLength;
        }
    }
}
