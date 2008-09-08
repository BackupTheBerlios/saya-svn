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
#include "sythread.h"

class syBitmap;
class VideoOutputDevice;

/**
 * VideoInputDevice is the base class for video frame providers, be it images or movies (flash animations
 * count as movies). All methods in this class (except the empty constructor) are virtual so you can
 * override them as much as you want.
 */
class VideoInputDevice : public syAborter {

    public:

        /** @brief Standard constructor.
         *
         *  Additionally, VideoInputDevice::VideoInputDevice() will allocate m_Bitmap.
         */
        VideoInputDevice();

        /** @brief Standard destructor.
         *
         *  Additionally, VideoInputDevice::~VideoInputDevice() will deallocate m_Bitmap.
         */
        virtual ~VideoInputDevice();

        /** @brief Initializer.
         *
         *  After setting the parameters in your derived class, you can call Init() to open the file /
         *  memory resource / whatever.
         *  After calling ConnectInput(), Init() will reallocate the bitmap with the current parameters.
         *  @warning This function must ONLY be called by the main thread!
         */
        bool Init();

        /** @brief Shutdown procedure.
         *
         *  Call this function after finishing reading all the data, to free memory and associated resources.
         *  ShutDown calls DisconnectInput() and then ReleaseBitmap().
         *  @warning This function must ONLY be called by the main thread!
         */
        void ShutDown();

        /** @brief Non-waiting Shutdown procedure.
         *  This function starts the object shutdown, but does not wait until it's finished.
         *  This function is safe for calling from any thread.
         *  @warning If you start shutdown using this procedure, you MUST call WaitForShutDown() and FinishShutDown()!
         *  @see VideoInputDevice::WaitForShutDown()
         */
        void StartShutDown();

        /** @brief Waits for the Shutdown procedure to complete.
         *  @see VideoInputDevice::ShutDown()
         */
        void WaitForShutDown();

        /** @brief Finishes the Shutdown procedure started by StartShutDown().
         *  @see VideoInputDevice::StartShutDown()
         */
        void FinishShutDown();

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in milliseconds, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        unsigned long Seek(unsigned long time,bool fromend = false);


        /** Gets the current position in time (milliseconds) */
        unsigned long GetPos();

        /** @brief Gets the length of the data being read.
         *  @return The length of the resource's data, in milliseconds. Minimum one.
         */
        unsigned long GetLength();

        /** Gets the Video Color format of the resource. */
        VideoColorFormat GetColorFormat();

        /** Gets the resource's width in pixels  */
        unsigned long GetWidth();

        /** Gets the resource's height in pixels. */
        unsigned long GetHeight();

        /** Gets the resource's pixel aspect ratio. */
        float GetPixelAspect();

        /** @brief Sends the current frame to device.
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
         *  @param time Instant in time (milliseconds) where we want to get the frame index.
         *  @return The frame index (zero-based) corresponding to the given time.
         */
        virtual unsigned long GetFrameIndex(unsigned long time);

        /** Returns the OK status. */
        bool IsOk();

        /** Returns the value of m_IsBusy.
         *
         *  @return true if the object is reading or sending a frame; false otherwise.
         */
        bool IsBusy();

        /** @brief If true, all tasks must abort IMMEDIATELY!
         *
         *  @see syAborter
         */
        virtual bool InnerMustAbort();

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
         *  @param time The time, in milliseconds, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note Called by Seek(); Calls SeekResource().
         */
        unsigned long InternalSeek(unsigned long time, bool fromend = false);

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param time The time, in milliseconds, to seek to.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This function is called by InternalSeek().
         *  @warning This function MUST NOT update m_CurrentTime. That is done by InternalSeek().
         */
        virtual unsigned long SeekResource(unsigned long time) { return time; }

        /** @brief Allocates memory for m_Bitmap. Called by Init(). */
        void AllocateBitmap();

        /** Releases the memory for m_Bitmap. Called by ShutDown(). */
        void ReleaseBitmap();

        /** @brief Connects to the input resource. Called by Init().
         *
         *  @return true on success; false otherwise.
         *  @note  This is where you will open the required files to decode.
         */
        virtual bool ConnectInput() { return true; }

        /** @brief Disconnects the input resource.
         *
         *  Called by ShutDown().
         *  @note In this function, you MUST release any sources allocated by ConnectInput().
         */
        virtual void DisconnectInput() {}

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
        unsigned long m_CurrentTime;

        /** Indicates the resource's total length in milliseconds. One-based. */
        unsigned long m_Length;

        /** The width of the current resource. */
        unsigned long m_Width;

        /** The height of the current resource. */
        unsigned long m_Height;

        /** The Video Color format of the current resource. */
        VideoColorFormat m_ColorFormat;

        /** The pixel aspect ratio for the current resource. */
        float m_PixelAspect;

        /** Safe mutex for the busy operations. */
        sySafeMutex* m_Busy;
private:
        /** The current OK status. */
        bool m_IsOk;

        /** The current Shutting-down status. */
        bool m_IsShuttingDown;
};

// TODO: Implement the following functions for all input/oputput device classes: StartShutDown(), WaitForShutDown() and FinishShutDown()

#endif
