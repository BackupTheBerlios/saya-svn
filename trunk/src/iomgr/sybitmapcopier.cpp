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

void syBitmapCopier::Init(syBitmap *sourcebmp, syBitmap *destbmp) {
    if(!sourcebmp || !destbmp) { return; }
    m_SourceBitmap = sourcebmp;
    m_DestBitmap = destbmp;
    m_SourceFmt = sourcebmp->GetColorFormat();
    m_DestFmt = destbmp->GetColorFormat();
    m_SourceBypp = syBitmap::CalculateBytesperPixel(m_SourceFmt);
    m_DestBypp = syBitmap::CalculateBytesperPixel(m_DestFmt);
    m_Src = sourcebmp->GetBuffer();
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
    m_Src = m_SourceBitmap->GetBuffer();
    m_Dst = m_DestBitmap->GetBuffer();
}
