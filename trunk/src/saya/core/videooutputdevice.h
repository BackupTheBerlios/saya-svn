/***************************************************************
 * Name:      videooutputdevice.h
 * Purpose:   Declaration of the VideoOutputDevice class
 *            and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef videooutputdevice_h
#define videooutputdevice_h

#include "videocolorformat.h"
#include "avdevice.h"

class syBitmap;
class syMutex;
class VideoOutputDevice;

/**
 * @brief Generic wrapper for a Video Output device.
 */
class VideoOutputDevice : public AVDevice {
    public:

        /** Maximum width for the rendered window. Currently the value is 10240 pixels. */
        static const unsigned int MaxWidth;

        /** Maximum height for the rendered window. Currently the value is 10240 pixels. */
        static const unsigned int MaxHeight;

        /** Standard constructor. */
        VideoOutputDevice();

        /** Standard destructor. */
        virtual ~VideoOutputDevice();

        /** Returns the output color format. */
        VideoColorFormat GetColorFormat() const;

        /** Returns the output width, in pixels. */
        unsigned int GetWidth() const;

        /** Returns the output height, in pixels. */
        unsigned int GetHeight() const;

        /** @brief Called whenever the output screen size is changed (i.e. by resizing the playback window)
         *
         *  @param newwidth the new width to be set for playback
         *  @param newheight the new height to be set for playback
         *  @return true if size was successfully changed, false otherwise.
         *  @note This is a wrapper for the protected method ChangeDeviceSize(). After change takes place, variables
         *  m_Width and m_Height are updated.
         *  @note This method can be called ONLY by the main thread!
         */
        bool ChangeSize(unsigned int newwidth,unsigned int newheight);

        /** @brief Loads video data from an external buffer.
         *
         *  @param bitmap the bitmap (buffer) containing the image to be sent.
         *  @note  This method is a wrapper for LoadDeviceVideoData.
         *  @note  This method should be called by the worker thread
         */
        void LoadVideoData(const syBitmap* bitmap);

        /** @brief Writes the video data into the device.
         *  @note This method is a wrapper for RenderVideoData.
         *  @note This method updates the output bitmap. If you don't call it, you'll be seeing a blank bitmap
         *  all the time.
         */
        void FlushVideoData();

        /** @brief Flag indicating that playback must be aborted immediately.
          * @return true if playback/encoding thread must be aborted; false otherwise.
          * @note This method MUST be called by LoadDeviceVideoData.
          * @see syAborter::MustAbort
          */
        virtual bool InnerMustAbort() const;

        /** @brief Flag indicating whether it's a playback or an encoding device.
         *  @return true for encoding; false for playback.
         */
        virtual bool IsEncoder() const;

    protected:

        /** @brief Initializes the output device.
         *
         *  @note The m_ok flag is set to this method's return value.
         *  @return True on success; false otherwise.
         *  @note This method MUST set the m_Width, m_Height and m_ColorFormat variables.
         */
        virtual bool Connect();

        /** @brief Clears the video output device (i.e. sets it all to black). Called by ShutDown().
         *
         *  @warning If you derive this method , you MUST call VideoOutputDevice::Clear().
         */
        virtual void Clear();

        /** @brief Allocates the Bitmaps' memory.
         *
         *  @warning If you derive this method , you MUST call VideoOutputDevice::AllocateResources().
         */
        virtual bool AllocateResources();

        /** @brief Disconnects the video output device or memory resource. Called by ShutDown().
         *
         *  @warning If you derive this method , you MUST call VideoOutputDevice::FreeResources().
         */
        virtual void FreeResources();

        /** @brief Called by ChangeSize whenever the output screen size is changed.
          *
          * This method must deal with size changes so that it resizes the buffers used for playback.
          * @param newwidth the new width to be set for playback
          * @param newheight the new height to be set for playback
          * @return true if size was changed, false otherwise.
          */
        virtual bool ChangeDeviceSize(unsigned int newwidth,unsigned int newheight);

        /** Plays the received frames.
          * @note This method MUST check MustAbort() regularly and abort rendering when the result is true.
          * @note This method MUST do nothing if either m_Width or m_Height are set to 0.
          */
        virtual void RenderVideoData(const syBitmap* bitmap);

        /** Output color format */
        VideoColorFormat m_ColorFormat;

        /** Output width */
        unsigned int m_Width;

        /**  Output height */
        unsigned int m_Height;

    private:
        /** flag that forbids playback when changing the device size */
        bool m_ChangingSize;

        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
