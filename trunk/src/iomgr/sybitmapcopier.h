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

#include "sybitmap.h"

/** @brief Optimized bitmap pixel copier.
  *
  * The objective of this class is that you need to init the parameters for copying bitmaps only once,
  * then you can keep copying without needing to recalculate the pointers, the bytes per pixel or
  * pass the color formats through the stack.
  * Feel free to add as many functions as you need.
  */
class syBitmapCopier {
    public:
        /** Initializes the member variables to perform the batch copying */
        void Init(syBitmap *sourcebmp, syBitmap *destbmp);

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

        /** Source color format obtained by Init(). Kept public to allow external modification. */
        VideoColorFormat m_SourceFmt;

        /** Destination color format obtained by Init(). Kept public to allow external modification. */
        VideoColorFormat m_DestFmt;

        /** Source bytes per pixel obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceBypp;

        /** Destination bytes per pixel obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestBypp;

        /** Source address obtained by Init(). Kept public to allow external modification. */
        unsigned char* m_Src;

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

        /** Source Row Length, in bytes, obtained by Init(). Kept public to allow external modification. */
        unsigned int m_SourceRowLength;

        /** Destination Row Length, in bytes, obtained by Init(). Kept public to allow external modification. */
        unsigned int m_DestRowLength;
};

#endif
