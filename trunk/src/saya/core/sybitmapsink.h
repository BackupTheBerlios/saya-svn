/***************************************************************
 * Name:      sybitmapsink.h
 * Purpose:   Declaration for the syBitmapSink class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-03
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#ifndef sybitmapsink_h
#define sybitmapsink_h

#include "videocolorformat.h"
class syBitmap;

/** @class syBitmapSink
 *  @brief This class is used by VideoInputDevice and VideoOutputDevice to render data.
 *  @warning The functions provided by subclasses MUST be thread-safe.
 */
class syBitmapSink {
    public:
        /** Loads data from the bitmap. */
        virtual void LoadData(const syBitmap* bitmap) {}

        /** Returns the currently-used color format. */
        virtual VideoColorFormat GetColorFormat() const;

        /** Returns the current width. */
        virtual unsigned int GetWidth() const;

        /** Returns the current height. */
        virtual unsigned int GetHeight() const;

        /** Returns the distance from the current screen's top to the bitmapsink's top.
         *  To be used by hardware accelerated video output devices.
         */
        virtual int GetTop() const;

        /** Returns the distance from the current screen's left to the bitmapsink's left.
         *  To be used by hardware accelerated video output devices.
         */
        virtual int GetLeft() const;

        virtual ~syBitmapSink() {}
};

#endif
