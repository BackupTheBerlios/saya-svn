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
        bool IsOk();

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

        /** Returns the Busy status.
         *
         *  @return true if the object is altering or sending its data; false otherwise.
         */
        bool IsBusy();

        /** Returns true if it's a Video Device. */
        bool IsVideo();

        /** Returns true if it's an Audio Device. */
        bool IsAudio();

        /** Returns true if it's an Input Device. */
        bool IsInput();

        /** Returns true if it's an Output Device. */
        bool IsOutput();


    protected:

        /** @brief If true, all tasks must abort IMMEDIATELY!
         *
         *  @see syAborter
         */
        virtual bool InnerMustAbort();

        /** Allocates the memory resources required by Init(). */
        virtual bool AllocateResources() { return true; }

        /** Deallocates the memory resources allocated by AllocateResources(). */
        virtual void FreeResources() {}

        /** Connects to the input/output device. */
        virtual bool Connect() { return true; }

        /** @brief Disconnects from the input/output device.
         *
         *  @note In this function you MUST deallocate any resources allocated by Connect()!
         */
        virtual void Disconnect() {}

        /** Mutex used for busy status */
        sySafeMutex* m_Busy;

        /** Flag for Video devices */
        bool m_IsVideo;

        /** Flag for Audio devices */
        bool m_IsAudio;

        /** Flag for Input devices */
        bool m_IsInput;

        /** Flag for Output devices */
        bool m_IsOutput;

    private:

        bool m_IsOk;
        bool m_IsShuttingDown;
};

#endif
