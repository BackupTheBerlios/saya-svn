/***************************************************************
 * Name:      audioinputdevice.h
 * Purpose:   Declaration of the AudioInputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-10
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef audioinputdevice_h
#define audioinputdevice_h

#include "avtypes.h"
#include "avdevice.h"

class syAudioBuffer;
class AudioInputDevice;
class AudioOutputDevice;

/** @brief Generic class for Audio Output
 *
 */
class AudioInputDevice : public AVDevice {
    //TODO: Design and implement the Audio Input Device. (This is a partial stub)

    public:

        /** Standard constructor. */
        AudioInputDevice();

        /** Standard destructor. */
        virtual ~AudioInputDevice();

        /** @brief Seeks to a determinate instant in time.
         *
         *  @param The time, in avtime_t units, to seek to.
         *  @param fromend Boolean telling the device to seek from the end rather than the beginning.
         *  @return The current instant in time where the current frame will be read.
         *  @note  This is a thread-safe wrapper for the protected functions InternalSeek() and SeekResource().
         */
        avtime_t Seek(avtime_t time,bool fromend = false);

        /** Gets the current position in time (milliseconds) */
        avtime_t GetPos() const;

        /** @brief Gets the length of the data being read.
         *  @return The length of the resource's data, in milliseconds. Minimum one.
         */
        avtime_t GetLength() const;

        /** @brief Gets the audio sample number corresponding to the given time.
         *  @param time Instant in time (milliseconds) where we want to get the sample index.
         *  @return The sample index (zero-based) corresponding to the given time.
         */
        virtual unsigned long GetSampleIndex(avtime_t time);

        /** @brief Sends the buffer contents to the specified AudioOutputDevice.
         *
         * This method just calls LoadAudioBuffer() and then
         * calls device->LoadAudioData(this->m_Buffer, numsamples).
         * @param device the AudioOutputDevice object to send the data to.
         * @param numsamples The number of samples to send. 0 = unlimited.
         * @note When sending data, the device will not exit until the specified number
         * of samples has been sent, we have reached EOF, or a stop/abort signal has been received.
         */
        void SendAudioData(AudioOutputDevice* device,unsigned long numsamples = 0);

        /** @brief Sends the buffer contents to the specified AudioOutputDevice.
         *
         *  This method calls LoadAudioBuffer() and then device->LoadAudioData.
         *  If duration != 0, the number of samples is obtained via GetSampleIndex;
         *  then SendAudioData is called using the obtained number of samples.
         */
        void SendAudioData(AudioOutputDevice* device,avtime_t duration = 0);

    protected:


        /** @brief Loads a chunk of audio into m_Buffer.
         *
         *  @param numsamples The maximum number of samples to be read. 0 = let the derived class choose.
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadAudioBuffer(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         *  @note This method MUST NOT exit until the specified number of samples has been sent,  we
         *  have reached EOF, OR a stop/abort signal has been received.
         *
         */
        virtual void LoadAudioBuffer(unsigned long numsamples = 0);

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
         *  @return The current instant in time where the audio will be read.
         *  @note  This function is called by InternalSeek().
         *  @warning This function MUST NOT update m_CurrentTime. That is done by InternalSeek().
         */
        virtual avtime_t SeekResource(avtime_t time) { return time; }

        /** @brief Allocates memory for m_Buffer. Called by Init().
         *  @note If you override this function, remember to call it in your derived class' AllocateResources()
         */
        virtual bool AllocateResources();

        /** @brief Releases the memory for m_Buffer. Called by ShutDown().
         *  @note If you override this function, remember to call it in your derived class' FreeResources().
         */
        virtual void FreeResources();

        /** @brief A pointer indicating the current time in the resource.
         *
         *  Modified by Seek().
         */
        avtime_t m_CurrentTime;

        /** Indicates the resource's total length in avtime_t units. One-based. */
        avtime_t m_AudioLength;

        /** Circular Buffer to hold the audio data */
        syAudioBuffer* m_Buffer;


};

#endif
