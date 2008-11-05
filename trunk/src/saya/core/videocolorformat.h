/***************************************************************
 * Name:      videocolorformat.h
 * Purpose:   Declaration of the VideoColorFormat enum
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef videocolorformat_h
#define videocolorformat_h

/** enumerates the Video Color formats that a VideoOutputDevice can render / encode
 *
 *  @note Apparently, this list is incomplete and some formats are now obsolete.
 *  Needs update from an authoritative source.
 */
enum VideoColorFormat {
    vcfRGB8,
    vcfRGB32,
    vcfRGB24,
    vcfRGB16,
    vcfRGB15,
    vcfBGR15,
    vcfBGR16,
    vcfBGR24,
    vcfBGR32,
    vcfYUY2,
    vcfYUV,
    vcfYV12,
    vcfYUV12,
    vcfYVYU,
    vcfUYVY,
    vcfYUY9,
    vcfY800
};

#endif
