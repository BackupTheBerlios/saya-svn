/***************************************************************
 * Name:      videoinputdevice.h
 * Purpose:   Declaration for the VideoInputDevice class
 *            and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef videoinputdevice_h
#define videoinputdevice_h

#include "videocolorformat.h"
#include "avdevice.h"
#include "avtypes.h"

class sySafeMutex;
class syBitmap;
class VideoOutputDevice;

/**
 * VideoInputDevice is the base class for video frame providers, be it images or movies (flash animations
 * count as movies). The functions you must override are GetFrameIndex(), LoadCurrentFrame() and SeekResource().
 * The functions AllocateResources and FreeResources MUST call the parent class' methods.
 */
class VideoInputDevice : public AVDevice {

    public:

        /** Standard constructor. */
        VideoInputDevice();

        /** Standard destructor. */
        virtual ~VideoInputDevice();


        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in milliseconds, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t Seek(avtime_t time,bool fromend = false);

        /** @brief Seeks to a determinate frame.
         *
         *  @param frame The frame to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t SeekFrame(unsigned long frame,bool fromend = false);

        /** Gets the current position in time */
        avtime_t GetPos() const;

        /** @brief Gets the length of the data being read.
         *  @return The length of the resource's data, in avtime_t units. Minimum one.
         */
        avtime_t GetLength() const;

        /** Gets the Video Color format of the resource. */
        VideoColorFormat GetColorFormat() const;

        /** Gets the resource's width in pixels  */
        unsigned long GetWidth() const;

        /** Gets the resource's height in pixels. */
        unsigned long GetHeight() const;

        /** Gets the resource's pixel aspect ratio. */
        float GetPixelAspect() const;

        /** @brief Sends the current frame to the specified VideoOutputDevice.
         *
         * This routine just calls LoadCurrentFrame() and then
         * calls device->LoadVideoData(this->m_Bitmap).
         */
        void SendCurrentFrame(VideoOutputDevice* device);

        /** @brief Sends the current frame to an external bitmap.
         *
         * This routine just calls LoadCurrentFrame() and then
         * copies m_Bitmap to the destination bitmap. This is done
         * by design to always keep a buffer of the current frame.
         * @warning SendCurrentFrame will lock the bitmap while sending the data.
         */
        void SendCurrentFrame(syBitmap* bitmap);

        /** @brief Gets the frame corresponding to the given time.
         *  @param time Instant in time where we want to get the frame index.
         *  @return The frame index (zero-based) corresponding to the given time.
         */
        virtual unsigned long GetFrameIndex(avtime_t time);

        /** @brief Gets the time corresponding to a given frame.
         *  @param The frame index (zero-based) where we want to get the time.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The instant in time from the video start corresponding to the given frame.
         */
        virtual avtime_t GetTimeFromFrameIndex(unsigned long  frame, bool fromend = false);

    protected:

        /** @brief Loads the current frame into m_Bitmap.
         *
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadCurrentFrame(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         */
        virtual void LoadCurrentFrame();

        /** @brief Internal seeking routine.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note Called by Seek(); Calls SeekResource().
         */
        avtime_t InternalSeek(avtime_t time, bool fromend = false);

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in avtime_t units, to seek to.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This function is called by InternalSeek().
         *  @warning This function MUST NOT update m_CurrentTime. That is done by InternalSeek().
         */
        virtual avtime_t SeekResource(avtime_t time) { return time; }

        /** @brief Allocates memory for m_Bitmap. Called by Init().
         *  @note If you override this function, remember to call it in your derived class' AllocateResources()
         */
        virtual bool AllocateResources();

        /** @brief Releases the memory for m_Bitmap. Called by ShutDown().
         *  @note If you override this function, remember to call it in your derived class' FreeResources().
         */
        virtual void FreeResources();

        /** @brief The bitmap for the current frame.
         *
         *  @note This member is protected because the only authorized way to send
         *  the info to an external object is through the SendCurrentFrame functions.
         *  This way you can implement a cache of most used frames or something.
         */
        syBitmap* m_Bitmap;

        /** @brief A pointer indicating the current time in the resource.
         *
         *  Modified by Seek().
         */
        avtime_t m_CurrentTime;

        /** Indicates the resource's total length in avtime_t units. Minimum one. */
        avtime_t m_VideoLength;

        /** The width of the current resource. */
        unsigned long m_Width;

        /** The height of the current resource. */
        unsigned long m_Height;

        /** The Video Color format of the current resource. */
        VideoColorFormat m_ColorFormat;

        /** The pixel aspect ratio for the current resource. */
        float m_PixelAspect;
};

// TODO: Make all the I/O Device classes derivatives of AVDevice

#endif
