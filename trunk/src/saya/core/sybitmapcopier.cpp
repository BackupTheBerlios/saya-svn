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

void syBitmapCopier::Init(const syBitmap *sourcebmp, syBitmap *destbmp) {
    if(!sourcebmp || !destbmp) { return; }
    m_SourceBitmap = sourcebmp;
    m_DestBitmap = destbmp;
    m_SourceFmt = sourcebmp->GetColorFormat();
    m_DestFmt = destbmp->GetColorFormat();
    m_SourceBypp = syBitmap::CalculateBytesperPixel(m_SourceFmt);
    m_DestBypp = syBitmap::CalculateBytesperPixel(m_DestFmt);
    m_Src = sourcebmp->GetReadOnlyBuffer();
    m_Dst = destbmp->GetBuffer();
    m_SourceWidth = sourcebmp->GetWidth();
    m_DestWidth = destbmp->GetWidth();
    m_SourceHeight = sourcebmp->GetHeight();
    m_DestHeight = destbmp->GetHeight();
    m_SourceRowLength = m_SourceWidth * m_SourceBypp;
    m_DestRowLength = m_DestWidth * m_DestBypp;
    m_SourceBufferLength = sourcebmp->GetBufferLength();
    m_DestBufferLength = destbmp->GetBufferLength();
}

void syBitmapCopier::Reset() {
    m_Src = m_SourceBitmap->GetReadOnlyBuffer();
    m_Dst = m_DestBitmap->GetBuffer();
}

unsigned long syBitmapCopier::ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt) {
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
