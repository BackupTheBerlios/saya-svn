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

#include "aborter.h"
#include "videocolorformat.h"

class sySafeMutex;

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
        void CopyFrom(const syBitmap* source);

        /** Pastes from another bitmap, resizing if necessary */
        void PasteFrom(const syBitmap* source,syStretchMode stretchmode = sy_stkeepaspectratio);

        /** Returns a pointer to the Buffer being used */
        unsigned char* GetBuffer();

        /** Returns a const pointer to the bitmap's buffer */
        const unsigned char* GetReadOnlyBuffer() const;

        /** Returns the bitmap's current width */
        unsigned int GetWidth() const;

        /** Returns the bitmap's current height */
        unsigned int GetHeight() const;

        /** Returns the length of the buffer's workable area, in bytes */
        unsigned long GetBufferLength() const;

        /** Returns the size in bytes of the whole buffer */
        unsigned long GetBufferSize() const;

        /** Returns the bitmap's color format */
        VideoColorFormat GetColorFormat() const;

        /** @brief Returns a pointer to the start of the specified row
         *
         *  @param y The row to be accessed (zero-based)
         *  @return A pointer to the current row if the value of "y" is valid; NULL otherwise.
         */
        unsigned char* GetRow(int y);

        /** @brief Returns a const pointer to the start of the specified row
         *
         *  @param y The row to be accessed (zero-based)
         *  @return A pointer to the current row if the value of "y" is valid; NULL otherwise.
         */
        const unsigned char* GetReadOnlyRow(int y) const;


        /** @brief Returns a pointer to the pixel corresponding to the specified coordinates
         *  @param x The column (zero-based)
         *  @param y The row (zero-based)
         *  @return The pointer to the pixel in question, or NULL in case of an invalid y
         *  @note y is invalid if it exceeds the bitmap's height or is less than 0.
         */
        unsigned char* GetPixelAddr(int x, int y);

        /** @brief Returns a const pointer to the pixel corresponding to the specified coordinates
         *  @param x The column (zero-based)
         *  @param y The row (zero-based)
         *  @return The pointer to the pixel in question, or NULL in case of an invalid y
         *  @note y is invalid if it exceeds the bitmap's height or is less than 0.
         */
        const unsigned char* GetReadOnlyPixelAddr(int x, int y) const;

        /** @brief Gets a pixel in the current color format at the specified coordinates
         *
         *  @param x The x coordinate (zero-based)
         *  @param y The y coordinate (zero-based)
         *  @return The contents of the specified pixel, in the current color format
         *  @note If the coordinates exceed the bitmap's dimensions or are less than 0, we get a 0.
         */
        unsigned long GetPixel(int x, int y) const;

        /** @brief Sets a pixel in the current color format at the specified coordinates
         *
         *  @param x The x coordinate (zero-based)
         *  @param y The y coordinate (zero-based)
         *  @param pixel A long integer containing the pixel in the current color format
         *  @note If the coordinates exceed the bitmap's dimensions or are less than 0, no action is taken.
         */
        void SetPixel(int x, int y, unsigned long pixel);

        /** @brief Gets a pixel in the current color format at the specified address
         *
         *  @param addr The address of said pixel
         *  @return The contents of the specified pixel, in the current color format
         *  @warning No bounds checking is done in this function!
         */
        unsigned long GetPixel(const unsigned char* addr) const;

        /** @brief Sets a pixel in the current color format at the specified address
         *
         *  @param addr The address of said pixel
         *  @param pixel The value to set the specified pixel, in the current color format
         *  @warning No bounds checking is done in this function!
         */
        void SetPixel(unsigned char* addr, unsigned long pixel);

        /** @brief Converts a pixel between two color formats. It's a wrapper for syBitmapCopier::ConvertPixel()
         *  @see syBitmapCopier::ConvertPixel
         *  @param pixel The original pixel
         *  @param sourcefmt The color format of the original pixel
         *  @param destfmt The desired color format of the result
         *  @return The pixel converted to the new color format
         */
        static unsigned long ConvertPixel(unsigned long pixel,VideoColorFormat sourcefmt,VideoColorFormat destfmt);

        /** @brief Copies a pixel from a memory location to another, converting the color format if necessary.
         *
         *  @warning absolutely no memory checking is done. Use with care.
         *  @param src The address of the source pixel to copy
         *  @param dst The address of the destination pixel to copy into
         *  @param sourcefmt The color format of the source
         *  @param destfmt The color format of the destination
         */
        static void CopyPixel(unsigned char* src,unsigned char* dst,VideoColorFormat sourcefmt,VideoColorFormat destfmt);

        static unsigned int CalculateBytesperPixel(VideoColorFormat format);

        /** @brief Sets the Aborter object so we can interrupt a copy operation on request.
         *  @see syAborter
         */
        void SetAborter(syAborter* aborter);

        /** @brief For use with VideoOutputDevice. If the result is true, the operation must be aborted ASAP.
         *  @see syAborter
         */
        bool MustAbort();

        /** Reallocates buffer to hold a bitmap with different size/color format
         *
         *  @note The actual buffer contents are lost
         */
        void Realloc(unsigned int newwidth,unsigned int newheight,const VideoColorFormat newformat);

        /** Releases the buffer from memory. */
        bool ReleaseBuffer(bool force = false);

        /** Clears the buffer, filling it with zeroes. */
        void Clear();

        /** A mutex to protect the buffer for multithreaded apps */
        mutable sySafeMutex* m_Mutex;

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
