/***************************************************************
 * Name:      audiooutputdevice.h
 * Purpose:   Declaration for the AudioOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef audiooutputdevice_h
#define audiooutputdevice_h

#include "avdevice.h"

class syMutex;
class sySemaphore;
class syAudioBuffer;
class syAudioOutputDeviceData;

/** @brief Generic class for Audio Output
  *
  * The AudioOutputDevice class is a generic output class that will connect an audio output device driver (or file encoder)
  * with a data source. This class is only a wrapper;
  * It can be an MP3 decoder, a sound generator, or in our case, the audio output for our project.
  * The A/V syncrhonization and optional frame skipping needs to be handled by a streaming class.
  * This class has the following virtual methods (not including the destructor) that must be implemented by derived classes:
  *
  * AllocateAudioData()
  * FreeAudioData()
  * ConnectAudioOutput()
  * Disconnect()  (inherited from AVDevice)
  * Clear()                     - Mutes the audio output device.
  * LoadAudioData()             - Loads audio data for a single channel
  * RenderAudioData()           - Sends the multichannel data stream from the buffer to the output device.
  * SetDeviceSampleFreq()       - Tries to Set the output device's sample frequency.
  * SetDeviceBytesPerSample()   - Tries to set the output device's bytes-per-sample size.
  * GetDeviceNumChannels()      - Gets the output device's number of channels.
  *
  * These protected methods are used by the derived classes to implement low-level audio playback / encoding.
  */

// TODO: Update AudioOutputDevice to use the AVDevice class and the syAudioBuffer class.
class AudioOutputDevice : public AVDevice {

    public:

        /** @brief Constructor
          */
        AudioOutputDevice();

        /** Returns the output sample frequency in Hz. */
        unsigned int GetSampleFreq();

        /** Returns the output buffer length, in samples. */
        unsigned int GetBufferSize();

        /** Sets the default buffer length for the next Init(). */
        unsigned int SetBufferSize(unsigned int newsize);

        /** @brief Sets the output sample frequency in Hz. This is a wrapper for SetDeviceSampleFreq()
          *
          * @param freq The desired sample frequency to be used.
          * @return The actual sample frequency used.
          */
        unsigned int SetSampleFreq(unsigned int freq);


        /** Returns the output sample size in bits. */
        unsigned int GetSamplePrecision();

        /** @brief Sets the bytes per sample. This is a wrapper for SetDeviceSamplePrecision().
          *
          * @param newsize the new sample size, in bytes (from 1 to 4).
          * @return The actual sample size, in bytes, used by the output.
          */
        unsigned int SetSamplePrecision(unsigned int precision);

        /** Returns the number of audio-channels in the output (from 1 to 255).  */
        unsigned int GetNumChannels();

        /** @brief Sets the number of audio-channels in the output.
         *  @param The desired number of audio channels (from 1 to 255).
         *  @return The number of audio channels available.
         */
        unsigned int SetNumChannels(unsigned int numchannels);

        /** @brief Loads audio data from an external buffer.
         *
         *  @param buf Buffer containing the data to be processed.
         */
        void LoadAudioData(const syAudioBuffer* buf);

        /** @brief Flushes the output buffer and writes the data.
         *  This is a wrapper for the protected virtual method RenderData().
         */
        void FlushAudioData();

        /** Standard destructor. */
        virtual ~AudioOutputDevice();

        /** Flag indicating whether the Output Device is for playback or for encoding.
         *
         *  @return true if it's for encoding; false if it's for playback.
         */
        virtual bool IsEncoder() { return false; }

        /** Mutes the audio output device. */
        virtual void Clear();

    protected:

        /** @brief Initializes the audio output and sets the frequency, number of channels and sample precision.
         *
         *  @param freq The desired frequency. Must be set to the correct value if and ONLY if the connection was successful.
         *  @param numchannels The desired number of channels. Must be set to the correct value if and ONLY if the connection was successful.
         *  @param precision The number of bits per sample. Must be set to the correct value if and ONLY if the connection was successful.
         *  @return true on success; false otherwise.
         */
        virtual bool ConnectAudioOutput(unsigned int& freq, unsigned int& numchannels, unsigned int& precision);

        /** Frees the audio output device. Called by ShutDown(). */
        virtual void Disconnect();

        /** @brief Allocates the memory resources required by Init().
         *
         *  This method is called by AllocateResources().
         */
        virtual bool AllocateAudioData();

        /** @brief Deallocates the memory resources allocated by AllocateAudioData().
         *
         *  This method is called by FreeResources().
         */
        virtual void FreeAudioData();

        /** Plays/encodes the data received.
          * @param buf This is actually OUR Audio Buffer containing the data received by LoadAudioData.
          * @note This method MUST check MustAbort() regularly and abort rendering when the result is true.
          */
        virtual void RenderAudioData(const syAudioBuffer* buf);

        /** Returns the device's sample frequency in Hz. */
        virtual unsigned int GetDeviceSampleFreq();

        /** @brief Gets the device's sample precision. */
        virtual unsigned int GetDeviceSamplePrecision();

        /** @brief Gets the number of audio channels. */
        virtual unsigned int GetDeviceNumChannels();

        /** @brief Sets the output sample frequency in Hz.
          *
          * @param freq The desired sample frequency to be used.
          * @return The actual sample frequency used.
          */
        virtual unsigned int SetDeviceSampleFreq(unsigned int freq);

        /** @brief Sets the sample precision.
          *
          * @param precision the new sample precision, in bits per sample (from 1 to 32).
          * @return The actual sample precision used by the output.
          */
        virtual unsigned int SetDeviceSamplePrecision(unsigned int precision);

        /** @brief Sets the number of audio channels.
          *
          * @param The desired number of output channels (from 1 to 255).
          * @return The available number of channels in the output.
          */
        virtual unsigned int SetDeviceNumChannels(unsigned int numchannels);

    private:

        /** @brief Initializes the output device. This is a wrapper for ConnectAudioOutput().
         *  @return True on success; false otherwise.
         *  @note  Since this function is not to be overriden, we make it private.
         */
        virtual bool Connect();

        /** @brief Allocates the memory resources required by Init().
         *  @note This function allocates the circular buffer and then proceeds to call AllocateAudioData().
         */
        virtual bool AllocateResources();

        /** @brief Deallocates the memory resources allocated by AllocateResources().
         *
         *  This function calls FreeAudioData, and then proceeds to free the circular buffer.
         */
        virtual void FreeResources();

        syAudioOutputDeviceData* m_Data;
};

#endif
