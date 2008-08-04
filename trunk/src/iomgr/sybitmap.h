/***************************************************************
 * Name:      sybitmap.h
 * Purpose:   Declaration for the syBitmap class
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

#ifndef sybitmap_h
#define sybitmap_h

#include <cstdlib>

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

/** Stretching strategy for pasting bitmaps */
enum syStretchMode {
    sy_stcopy, /** just copy the bitmap and center it.
              *   If the original is larger, it will be resized while keeping the aspect ratio
              */
    sy_stkeepaspectratio, /** Resize as possible without distorting */
    sy_ststretch /** Full stretching/resizing of the original */
};

class syBitmap {
    public:

        /** Standard constructor */
        syBitmap();

        /** Standard destructor */
        virtual ~syBitmap();

        /** Constructor with incorporated bitmap creation */
        syBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat);

        /** Creates a buffer with the desired width, height and color format */
        void Create(unsigned int width,unsigned int height,VideoColorFormat colorformat);

        /** Makes a copy of another syBitmap */
        void CopyFrom(syBitmap* source);

        /** Pastes from another bitmap, resizing if necessary */
        void PasteFrom(syBitmap* source,syStretchMode stretchmode = sy_stkeepaspectratio);

        /** Returns a pointer to the Buffer being used */
        unsigned char* GetBuffer();

        /** Returns the bitmap's current width */
        unsigned int GetWidth();

        /** Returns the bitmap's current height */
        unsigned int GetHeight();

        /** Returns the length of the buffer's workable area, in bytes */
        unsigned long GetBufferLength();

        /** Returns the size in bytes of the whole buffer */
        unsigned long GetBufferSize();

        /** Returns the bitmap's color format */
        VideoColorFormat GetColorFormat();

        /** @brief Gets a pixel in the current color format at the specified coordinates
         *
         *  @param x The x coordinate (zero-based)
         *  @param y The y coordinate (zero-based)
         *  @return The contents of the specified pixel, in the current color format
         *  @note If the coordinates exceed the bitmap's dimensions, we get a 0.
         */
        unsigned long GetPixel(unsigned int x, unsigned int y);

        /** @brief Sets a pixel in the current color format at the specified coordinates
         *
         *  @param x The x coordinate (zero-based)
         *  @param y The y coordinate (zero-based)
         *  @param pixel A long integer containing the pixel in the current color format
         *  @note If the coordinates exceed the bitmap's dimensions, no action is taken.
         */
        void SetPixel(unsigned int x, unsigned int y, unsigned long pixel);

        /** @brief Returns a pointer to the start of the specified row
         *
         *  @param y The row to be accessed (zero-based)
         *  @return A pointer to the current row if the value of "y" is valid; NULL otherwise.
         */
        unsigned char* GetRow(unsigned int y);

        /** @brief Converts a pixel between two color formats
         *  @param pixel The original pixel
         *  @param sourcefmt The color format of the original pixel
         *  @param destfmt The desired color format of the result
         *  @return The pixel converted to the new color format
         */
        static unsigned long ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt);

        static unsigned int CalculateBytesperPixel(VideoColorFormat format);

        /** @brief For use with VideoOutputDevice. If the result is true, the operation must be aborted ASAP.
         *  @note  You must override this method accordingly to use it in multithreaded apps.
         */
        virtual bool MustAbort();

        /** Reallocates buffer to hold a bitmap with different size/color format
         *
         *  @note The actual buffer contents are lost
         */
        void Realloc(unsigned int newwidth,unsigned int newheight,const VideoColorFormat newformat);

        /** Clears the buffer, filling it with zeroes. */
        void Clear();

    protected:

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

};

#endif
