/***************************************************************
 * Name:      sybitmapcopier.h
 * Purpose:   Declaration for the syBitmapCopier class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  syBitmapCopier is a lightweight class specialized
 *            in efficient data copying between syBitmaps.
 **************************************************************/

#ifndef sybitmapcopier_h
#define sybitmapcopier_h

#include "videocolorformat.h"
#include "imagefilters.h"
#include <cmath>

class syBitmap;

class syFloatPixel {
    public:
        double r;
        double g;
        double b;
        double a;
        syFloatPixel(double r0 = 0,double g0 = 0, double b0 = 0, double a0 = 0) : r(r0),g(g0),b(b0),a(a0) {}

        inline void fromRGBAValues(unsigned long r0 = 0,unsigned long g0 = 0, unsigned long b0 = 0, unsigned long a0 = 0) {
            r = pixeltofloat(r0);
            g = pixeltofloat(g0);
            b = pixeltofloat(b0);
            a = pixeltofloat(a0);
        }

        inline void fromRGBA(unsigned long pixel) {
            fromRGBAValues(pixel >> 24, pixel >> 16, pixel >> 8, pixel);
        }

        syFloatPixel(unsigned long r0,unsigned long g0, unsigned long b0, unsigned long a0) {
            fromRGBAValues(r0,g0,b0,a0);
        }
        syFloatPixel(unsigned long pixel) {
            fromRGBA(pixel);
        }

        static inline double truncate(double d) {
            if(d < 0) {
                d = 0;
            } else if(d > 1.0) {
                d = 1.0;
            }
            return d;
        };

        static inline unsigned long floattopixel(double d) {
            return (unsigned long)(truncate(d)*255.0);
        }

        static inline double pixeltofloat(unsigned long p) {
            return truncate((double)(p & 0xFF)/255.0d);
        }

        inline unsigned long toRGBA() {
            unsigned long result;
            r = truncate(r);
            g = truncate(g);
            b = truncate(b);
            a = truncate(a);
            if(r < 0) { r = 0; }
            result = (floattopixel(a) << 24) | (floattopixel(b) << 16) | (floattopixel(g) << 8) | (floattopixel(r));
            return result;
        }

};

class syPixelContrib {
    public:
        int srcx;
        int dstx;
        double weight;
};

class syPixelContribBuffer {
    public:
        syPixelContribBuffer(unsigned long sizeinpixels) : m_Capacity(sizeinpixels),m_Size(0),pixels(new syPixelContrib[sizeinpixels]) {}
        ~syPixelContribBuffer() { delete[] pixels;pixels = 0; }
        /** Returns false if ran out of space */
        bool AddWeight(int sourcex,int destx,double weight);
        void Clear();
        unsigned long m_Capacity;
        unsigned long m_Size;
        syPixelContrib* pixels;
};




/** @brief Optimized bitmap pixel copier.
  *
  * The objective of this class is that you need to init the parameters for copying bitmaps only once,
  * then you can keep copying without needing to recalculate the pointers, the bytes per pixel or
  * pass the color formats through the stack.
  * Feel free to add as many functions as you need.
  */
class syBitmapCopier {
    public:
        syBitmapCopier();
        /** Initializes the member variables to perform the batch copying */
        void Init(const syBitmap *sourcebmp, syBitmap *destbmp, syFilterType filtertype = filter_none);

        /** Resets m_Src and m_Dst to the bitmaps original addresses */
        void Reset();

        /** Copies pixel from m_Src to m_Dst, converting the color format if necessary. */
        void CopyPixel();

        /** Copies pixel from m_Src to m_Dst, converting the color format if necessary.
         *  m_Src is incremented.
         */
        void CopyPixelAndIncrementSrc();

        /** Copies pixel from m_Src to m_Dst, converting the color format if necessary.
         *  m_Dst is incremented.
         */
        void CopyPixelAndIncrementDst();

        /** Copies pixel from m_Src to m_Dst, converting the color format if necessary.
         *  m_Src and m_Dst are incremented.
         */
        void CopyPixelAndIncrementBoth();

        /** @brief Copies row from m_Src to m_Dst, converting the color format if necessary.
         *  m_Src and m_Dst are preserved.
         *
         *  @note If the m_SourceWidth is greater than m_DestWidth, only m_DestWidth pixels are copied.
         */
        void CopyRow();

        /** @brief Copies row from m_Src to m_Dst, converting the color format if necessary.
         *  m_Src is incremented by one row.
         *
         *  @note If the m_SourceWidth is greater than m_DestWidth, only m_DestWidth pixels are copied.
         */
        void CopyRowAndIncrementSrc();

        /** @brief Copies row from m_Src to m_Dst, converting the color format if necessary.
         *  m_Dst is incremented by one row.
         *
         *  @note If the m_SourceWidth is greater than m_DestWidth, only m_DestWidth pixels are copied.
         */
        void CopyRowAndIncrementDst();

        /** @brief Copies row from m_Src to m_Dst, converting the color format if necessary.
         *  m_Src and m_Dst are incremented by one row.
         *
         *  @note If the m_SourceWidth is greater than m_DestWidth, only m_DestWidth pixels are copied.
         */
        void CopyRowAndIncrementBoth();

        /** @brief Fills row in m_Dst with the specified pixel color. m_Dst is not incremented. */
        void FillRow(unsigned long pixel);

        /** @brief Fills row in m_Dst with zeroes. m_Dst is not incremented. */
        void ClearRow();

        /** @brief Gets the pixel in m_SourceBitmap at the given offset. */
        unsigned long GetPixelAt(unsigned int offset);

        /** @brief Sets the pixel in m_DestBitmap at the given offset. */
        void SetPixelAt(unsigned int offset, unsigned long pixel);

        /** @brief Clears the pixel in m_DestBitmap at the given offset. */
        void ClearPixelAt(unsigned int offset);

        /** Source bitmap */
        const syBitmap* m_SourceBitmap;

        /** Destination bitmap */
        syBitmap* m_DestBitmap;

        /** Source color format obtained by Init(). Kept public to allow external modification. */
        VideoColorFormat m_SourceFmt;

        /** Destination color format obtained by Init(). Kept public to allow external modification. */
        VideoColorFormat m_DestFmt;

        /** Source bytes per pixel obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceBypp;

        /** Destination bytes per pixel obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestBypp;

        /** Source address obtained by Init(). Kept public to allow external modification. */
        const unsigned char* m_Src;

        /** Destination address obtained by Init(). Kept public to allow external modification. */
        unsigned char* m_Dst;

        /** Source Width obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceWidth;

        /** Destination Width obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestWidth;

        /** Source Height obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceHeight;

        /** Destination Height obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestHeight;

        /** Effective Destination Width that keeps the aspect ratio. */
        unsigned int m_EffectiveDestWidth;

        /** Effective Destination Height that keeps the aspect ratio. */
        unsigned int m_EffectiveDestHeight;

        /** Effective X Scale obtained by Init(). */
        double m_XScale;

        /** Effective Y Scale obtained by Init(). */
        double m_YScale;

        /** Source Row Length, in bytes, obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceRowLength;

        /** Destination Row Length, in bytes, obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestRowLength;

        /** Source buffer length (not size) in bytes */
        unsigned int m_SourceBufferLength;

        /** Destination buffer length (not size) in bytes */
        unsigned int m_DestBufferLength;

        /** Destination pixel's x coordinates corresponding to source pixel (0,0) (for resampling). */
        unsigned int m_DestX0;

        /** Destination pixel's y coordinates corresponding to source pixel (0,0) (for resampling). */
        unsigned int m_DestY0;

        /** Filter for resampling */
        syImageFilter* m_Filter;

        /** Contribution Destination buffer length (not size) in bytes */
        syPixelContribBuffer *m_XContrib, *m_YContrib;

        void InitContribBuffers();

        /** @brief Converts a pixel between two color formats.
         *  @param pixel The original pixel
         *  @param sourcefmt The color format of the original pixel
         *  @param destfmt The desired color format of the result
         *  @return The pixel converted to the new color format
         */
        static unsigned long ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt);

        /** @brief Resamples a Row from a bitmap into a floating-point pixel row.
         *  @param src Pointer to a row in the source bitmap
         *  @param dst Pointer to a row in the destination buffer
         *  @param srcwidth The width, in pixels, of the source
         *  @param dstwidth The width, in pixels, of the destination
         *  @param filtertype The filter to apply when resampling.
         */
        void ResampleRow(const unsigned char* src, syFloatPixel* dst, syFilterType filtertype);

        /** @brief Resamples a Column from a floating-point pixel buffer into the destination bitmap. */
        void ResampleCol(syFloatPixel* src, unsigned char* dst, syFilterType filtertype);
};

inline void syBitmapCopier::CopyPixel() {
    unsigned int i;
    if(m_SourceFmt == m_DestFmt) {
        for(i = 0; i < m_SourceBypp; ++i) {
            m_Dst[i] = m_Src[i];
        }
    } else {
        unsigned long pixel = 0;
        // FIXME: Make sure we don't invert the R and B components in this code
        for(i = 0; i < m_SourceBypp; ++i) {
            pixel = (pixel << 8) | (m_Src[i] & 255);
        }
        pixel = syBitmapCopier::ConvertPixel(pixel, m_SourceFmt, m_DestFmt);
        for(i = 0; i < m_DestBypp; ++i) {
            m_Dst[i] = pixel & 255;
            pixel >>= 8;
        }
    }
}

inline void syBitmapCopier::CopyPixelAndIncrementSrc() {
    CopyPixel();
    m_Src += m_SourceBypp;
}

inline void syBitmapCopier::CopyPixelAndIncrementDst() {
    CopyPixel();
    m_Dst += m_DestBypp;
}

inline void syBitmapCopier::CopyPixelAndIncrementBoth() {
    CopyPixel();
    m_Src += m_SourceBypp;
    m_Dst += m_DestBypp;
}

inline void syBitmapCopier::CopyRow() {
    if(m_SourceFmt == m_DestFmt) {
        unsigned int maxlength = m_SourceRowLength;
        if(maxlength > m_DestRowLength) {
            maxlength = m_DestRowLength;
        }
        for(unsigned int j = 0; j < maxlength; ++j) {
            m_Dst[j] = m_Src[j];
        }
    } else {
        unsigned int maxw = m_SourceWidth;
        if(maxw > m_DestWidth) {
            maxw = m_DestWidth;
        }
        const unsigned char* sourceptr = m_Src;
        unsigned char* destptr = m_Dst;
        for(unsigned int j = 0; j < maxw; ++j, sourceptr += m_SourceBypp, destptr += m_DestBypp) {
            unsigned long pixel = 0;
            unsigned int i;
            // FIXME: Make sure we don't invert the R and B components in this code
            for(i = 0; i < m_SourceBypp; ++i) {
                pixel = (pixel << 8) | (sourceptr[i] & 255);
            }
            pixel = syBitmapCopier::ConvertPixel(pixel, m_SourceFmt, m_DestFmt);
            for(i = 0; i < m_DestBypp; ++i) {
                destptr[i] = pixel & 255;
                pixel >>= 8;
            }
        }
    }
}

inline void syBitmapCopier::FillRow(unsigned long pixel) {
    unsigned char* destptr = m_Dst;
    for(unsigned int j = 0; j < m_DestWidth; ++j, destptr += m_DestBypp) {
        unsigned int i;
        for(i = 0; i < m_DestBypp; ++i) {
            destptr[i] = (pixel >> (i << 3)) & 255;
        }
    }
}

inline void syBitmapCopier::ClearRow() {
    unsigned char* destptr = m_Dst;
    for(unsigned int j = 0; j < m_DestRowLength;  ++j, ++destptr) {
        *destptr = 0;
    }
}

inline void syBitmapCopier::SetPixelAt(unsigned int offset, unsigned long pixel) {
    for(unsigned int i = 0; i < m_DestBypp; ++i) {
        m_Dst[offset + i] = pixel & 255;
        pixel >>= 8;
    }
}

inline unsigned long syBitmapCopier::GetPixelAt(unsigned int offset) {
    unsigned long pixel;
    for(unsigned int i = 0; i < m_SourceBypp; ++i) {
        pixel = (pixel << 8) | (m_Src[offset+i] & 255);
    }
    return pixel;
}


inline void syBitmapCopier::ClearPixelAt(unsigned int offset) {
    for(unsigned int i = 0; i < m_DestBypp; ++i) {
        m_Dst[offset + i] = 0;
    }
}

inline void syBitmapCopier::CopyRowAndIncrementSrc() {
    CopyRow();
    m_Src += m_SourceRowLength;

}

inline void syBitmapCopier::CopyRowAndIncrementDst() {
    CopyRow();
    m_Dst += m_DestRowLength;
}

inline void syBitmapCopier::CopyRowAndIncrementBoth() {
    CopyRow();
    m_Src += m_SourceRowLength;
    m_Dst += m_DestRowLength;
}

#endif
