/***************************************************************
 * Name:      avdevice.h
 * Purpose:   Declaration of the AVDevice class
 *            and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  AVDevice is a generic template for Audio/Video
 *            input/output buffering classes.
 **************************************************************/

#ifndef avdevice_h
#define avdevice_h

class sySafeMutex;
#include "aborter.h"

class AVDevice : public syAborter {

    public:
        /** @brief Standard constructor.
         *
         *  Additionally, AVDevice::AVDevice() calls AllocateResources().
         */
        AVDevice();

        /** @brief Standard destructor.
         *
         *  Additionally, AVDevice::~AVDevice() will call FreeResources().
         */
        virtual ~AVDevice();

        /** @brief Initializer.
         *
         *  After setting the parameters in your derived class, you can call Init() to open the file /
         *  memory resource / whatever.
         *  After calling Connect(), Init() will reallocate the resources with the current parameters.
         *  @warning This function must ONLY be called by the main thread!
         */
        bool Init();

        /** Returns the OK status. */
        bool IsOk() const;

        /** @brief Shutdown procedure.
         *
         *  Call this function after finishing reading all the data, to free memory and associated resources.
         *  ShutDown calls Disconnect() and then ReleaseResources().
         *  @warning This function must ONLY be called by the main thread!
         */
        void ShutDown();

        /** @brief Non-waiting Shutdown procedure.
         *  This function starts the object shutdown, but does not wait until it's finished.
         *  This function is safe for calling from any thread.
         *  @warning If you start shutdown using this procedure, you MUST call WaitForShutDown() and FinishShutDown()!
         *  @see AVDevice::WaitForShutDown()
         */
        void StartShutDown();

        /** @brief Waits for the Shutdown procedure to complete.
         *  @see AVDevice::ShutDown()
         */
        void WaitForShutDown();

        /** @brief Finishes the Shutdown procedure started by StartShutDown().
         *  @see AVDevice::StartShutDown()
         */
        void FinishShutDown();

        /** Returns true if it's a Video Device. */
        bool IsVideo() const;

        /** Returns true if it's an Audio Device. */
        bool IsAudio() const;

        /** Returns true if it's an Input Device. */
        bool IsInput() const;

        /** Returns true if it's an Output Device. */
        bool IsOutput() const;

        /** Returns true if a thread is sending or receiving data from the object. */
        bool IsPlaying() const;

        /** Returns true if a thread is sending data to the object. */
        bool IsReading() const;

        /** Returns true if a thread is receiving data from the object. */
        bool IsWriting() const;

        /** Aborts all operations immediately. Written data must be discarded as if it were corrupted.
         *
         *  @note This method can only be called from the main thread.
         */
        void Abort() const;

        /** Aborts all operations gracefully. Integrity of the written data is guaranteed.
         *
         *  @note This method can only be called from the main thread.
         */
        void Stop() const;

    protected:

        /** @brief If true, all tasks must abort IMMEDIATELY!
         *
         *  @see syAborter
         */
        virtual bool InnerMustAbort() const;

        /** @brief Indicates that the current operation must be aborted, either gracefully or immediately.
         *  @note To find out if the operation must be aborted gracefuly, compare with MustAbort().
         */
        bool MustStop() const;

        /** returns the m_IsShuttingDown status. */
        bool IsShuttingDown() const;

        /** Allocates the memory resources required by Init(). */
        virtual bool AllocateResources() { return true; }

        /** Deallocates the memory resources allocated by AllocateResources().
         *  @warning This function is called by Init()! You MUST make sure you don't free your buffers twice.
         */
        virtual void FreeResources() {}

        /** Connects to the input/output device. */
        virtual bool Connect() { return true; }

        /** @brief Disconnects from the input/output device.
         *
         *  @note In this function you MUST deallocate any resources allocated by Connect()!
         */
        virtual void Disconnect() {}

        /** Mutex used when the object is receiving data */
        mutable sySafeMutex* m_InputMutex;

        /** Mutex used when the object is sending data */
        mutable sySafeMutex* m_OutputMutex;

        /** Flag for Video devices */
        bool m_IsVideo;

        /** Flag for Audio devices */
        bool m_IsAudio;

        /** Flag for Input devices */
        bool m_IsInput;

        /** Flag for Output devices */
        bool m_IsOutput;

        /** @brief Flag to stop playback immediately.
         *  @note Pending transactions MUST be aborted, even if the resulting data is corrupted.
         */
        mutable volatile bool m_Stop;

        /** @brief Flag to stop playback, but not urgent.
         *  Pending transactions MUST either be completed or rolled back so that the data is not corrupted.
         */
        mutable volatile bool m_SoftStop;

    private:

        volatile bool m_IsOk;

        /** Using volatile guarantees that the variable won't be cached in any registers */
        mutable volatile bool m_IsShuttingDown;
};

#endif
