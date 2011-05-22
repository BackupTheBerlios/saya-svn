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
#include "sybitmapsink.h"
#include "videocolorformat.h"
#include "imagefilters.h"

class syBitmap;
class sySafeMutex;
class syString;

/** Stretching strategy for pasting bitmaps */
enum syStretchMode {
    sy_stcopy, /** just copy the bitmap and center it.
              *   If the original is larger, it will be resized while keeping the aspect ratio
              */
    sy_stkeepaspectratio, /** Resize as possible without distorting */
    sy_ststretch /** Full stretching/resizing of the original */
};


class syBitmap : public syBitmapSink {
    public:

        /** Standard constructor */
        syBitmap();

        /** Standard destructor */
        virtual ~syBitmap();

        /** Constructor with incorporated bitmap creation */
        syBitmap(unsigned int width,unsigned int height,VideoColorFormat colorformat);

        /** Creates a buffer with the desired width, height and color format */
        void Create(unsigned int width,unsigned int height,VideoColorFormat colorformat);

        /** Makes a copy from a raw buffer. */
        void CopyFrom(const unsigned char* source, unsigned int width, unsigned int height, VideoColorFormat colorformat, unsigned long maxlength);

        /** Makes a copy of another syBitmap */
        void CopyFrom(const syBitmap* source);

        /** Resamples from another bitmap using the given resample algorithm. */
        void ResampleFrom(const syBitmap* source, syFilterType resamplemode = filter_lanczos4);

        /** Makes a copy of another syBitmap. */
        void LoadData(const syBitmap* bitmap);

        /** @brief Loads the bitmap from a file, using the registered codec plugins as necessary.
         *  @return true on success, false otherwise.
         */
        bool LoadFromFile(const char* filename);

        /** @brief Loads the bitmap from a file, using the registered codec plugins as necessary.
         *  @return true on success, false otherwise.
         */
        bool LoadFromFile(const syString& filename);

        /** @brief Loads the bitmap from a file in memory, using the registered codec plugins as necessary.
         *  @param data A syString object containing the data.
         *  @param imageformat The MIME type to read; Also accepts file extensions.
         *  @return true on success, false otherwise.
         */
        bool LoadFromString(const syString& data, const char* mimetype);

        /** @brief Loads the bitmap from a base64-encoded string, using the registered codec plugins as necessary.
         *  @param data A syString object containing the data in base64 format.
         *  @param imageformat The MIME type to read; Also accepts file extensions.
         *  @return true on success, false otherwise.
         */
        bool LoadFromBase64(const syString& data, const char* mimetype);

        /** @brief Creates a bitmap from a file, using the registered codec plugins as necessary.
         *  @return Pointer to a bitmap if successful; null otherwise.
         */
        static syBitmap* FromFile(const char* filename);

        /** @brief Creates a bitmap from a file, using the registered codec plugins as necessary.
         *  @return Pointer to a bitmap if successful; null otherwise.
         */
        static syBitmap* FromFile(const syString& filename);

        /** @brief Creates a bitmap from a file in memory, using the registered codec plugins as necessary.
         *  @param data A syString object containing the data.
         *  @param mimetype The MIME type to read; Also accepts file extensions.
         *  @return Pointer to a bitmap if successful; null otherwise.
         */
        static syBitmap* FromString(const syString& data, const char* mimetype);

        /** @brief Loads the bitmap from a base64-encoded string, using the registered codec plugins as necessary.
         *  @param data A syString object containing the data in base64 format.
         *  @param imageformat The MIME type to read; Also accepts file extensions.
         *  @return Pointer to a bitmap if successful; null otherwise.
         */
        static syBitmap* FromBase64(const syString& data, const char* mimetype);

        /** @brief Saves the bitmap to the destination filename using the given MIME type. */
        bool SaveToFile(const char* filename, const char* mimetype = "image/jpeg");

        /** @brief Saves the bitmap to the destination filename using the given MIME type. */
        bool SaveToFile(const syString& filename, const char* mimetype = "image/jpeg");

        /** @brief Saves the bitmap to the destination string (binary) using the given MIME type. */
        bool SaveToString(syString& dest, const char* mimetype = "image/jpeg");

        /** @brief Saves the bitmap to the destination string (base64) using the given MIME type. */
        bool SaveToBase64(syString& dest, const char* mimetype = "image/jpeg");

        /** Creates a bitmap icon from a given file. */
        static syBitmap* CreateIconFromFile(const syString& filename, unsigned int width = 64, unsigned int height = 64);

        /** @brief Creates an icon string from a given file, using the registered codec plugins as necessary.
         *  @param dest The string to hold the base64-encoded icon.
         *  @return true on success; false otherwise.
         */
        static bool CreateBase64IconFromFile(syString& dest, const syString& filename, const char* mimetype = "image/jpeg", unsigned int width = 64, unsigned int height = 64);

        /** Pastes from another bitmap, resizing if necessary */
        void PasteFrom(const syBitmap* source,syStretchMode stretchmode = sy_stkeepaspectratio);

        /** @brief Exchanges data with another bitmap.
         *  For the bitmaps to be exchanged successfully, both bitmaps' mutexes must be either unlocked or belonging to this thread.
         *  @param other The bitmap to exchange data with.
         *  @return true on success; false on failure.
         */
        bool ExchangeWith(syBitmap* other);

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

        /** Returns the size in bytes of each pixel */
        int GetBytesPerPixel() const;

        /** Returns the least common multiple, in bits ( 8 / 16 / 32), for the length of each scanline. */
        int GetRowPadding() const;

        /** Returns the least common multiple, in bits ( 8 / 16 / 32), for each pixel. */
        int GetPixelPadding() const;

        /** Returns the bit depth of the image (total of all RGB channels). */
        int GetDepth() const;

        /** Returns the length in bytes of each row. */
        int GetBytesPerLine() const;

        /** Returns the color mask for the Red component. */
        unsigned long GetRedMask() const;

        /** Returns the color mask for the Green component. */
        unsigned long GetGreenMask() const;

        /** Returns the color mask for the Blue component. */
        unsigned long GetBlueMask() const;

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

        /** @brief returns pixel at (0,x) given the row at rowaddr */
        unsigned long GetPixel(const unsigned char* rowaddr, int x) const;

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
