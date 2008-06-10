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

class AudioSourceDevice;

/** @brief Generic class for Audio Output
  *
  * The AudioOutputDevice class is a generic output class that will connect an audio output device driver (or file encoder)
  * with a data source.
  * It can be an MP3 decoder, a sound generator, or in our case, the audio output for our project.
  * This class was designed so that you won't have to worry about sample rate, timing or audio/video synchronization.
  * The syncrhonization and optional frame/skipping will be handled by the MasterOutputDevice class.
  * The class has the following virtual methods (not including the destructor) that will be implemented by derived classes:
  *
  * InitializeOutput()          - Initializes the output device.
  * Clear()                     - Mutes the audio output device.
  * DisconnectOutput()          - Disconnects the output device from the program.
  * RenderData()                - Sends a data stream from the buffer to the output device.
  * SetDeviceSampleFreq()       - Tries to Set the output device's sample frequency.
  * SetDeviceBytesPerSample()   - Tries to set the output device's bytes-per-sample size.
  * GetDeviceNumChannels()      - Gets the output device's number of channels.
  *
  * All of these methods are protected. Additionally, there's a non-virtual, protected method:
  *
  * GetChannelBuffer()          - Gets the output buffer for a given audio channel.
  *
  * These protected methods are used by the derived classes to implement low-level audio playback / encoding.
  *
  * Loading of data is done through the function LoadAudioData(), which must be invoked only by the AudioSourceDevice class.
  *
  * @see AudioSourceDevice
  *
  */
class AudioOutputDevice {
    public:

        AudioOutputDevice();        /** Constructor */

        /** @brief Initializes the output device and sets the m_ok flag.
         *
         *  @note This is a wrapper for the InitializeOutput() virtual method.
         *  @return True on success; false otherwise.
         */
        bool Init();

         /** Returns the OK status for the output device. */
        bool IsOk();

        /** @brief Starts playback / encoding
         *
         *  The PlayPeriod method reproduces the input audio corresponding to a short period of time.
         *  It's designed to be called by the MasterOutputDevice class.
         *  @param starttime The beginning time (in milliseconds) to be played
         *  @param stoptime  The ending time (in milliseconds; inclusive) to be played
         */
        void PlayPeriod(unsigned int starttime, unsigned int stoptime);

        /** Stops playback / encoding. */
        void Stop();

        /** Clears and disconnects the output device. Sets m_ok to false. */
        void ShutDown();

        /** Returns the output sample frequency in Hz. */
        unsigned int GetSampleFreq();

        /** Returns the output buffer length, in samples. */
        unsigned int GetBufferLength();

        /** Gets the default buffer length for the next Init(). */
        unsigned int GetBufferDefaultLength();

        /** Sets the default buffer length for the next Init(). */
        unsigned int SetBufferDefaultLength(unsigned int newlen);

        /** @brief Sets the output sample frequency in Hz. This is a wrapper for SetDeviceSampleFreq()
          *
          * @param freq The desired sample frequency to be used.
          * @return The actual sample frequency used.
          */
        unsigned int SetSampleFreq(unsigned int freq);


        /** Returns the output sample size in bytes. */
        unsigned int GetBytesPerSample();

        /** @brief Sets the bytes per sample. This is a wrapper for SetDeviceBytesperSample().
          *
          * @param newsize the new sample size, in bytes (from 1 to 4).
          * @return The actual sample size, in bytes, used by the output.
          */
        unsigned int SetBytesPerSample(unsigned int newsize);

        /** Gets the number of audio-channel buffers currently allocated. */
        unsigned int GetNumBuffers();

        /** Returns the number of audio-channels in the output (from 0 to 255).  */
        unsigned int GetNumChannels();

        /** Connects the input to an AudioSourceDevice object */
        bool ConnectInput(AudioSourceDevice* inputdev);

        /** @brief Disconnects the audio source.
          * @note The source is NOT deallocated - just disconnected. Allocation and deallocation must be done elsewhere.
          */
        void DisconnectInput();

        /** @brief Gets the Audio Source connected to this output device, or NULL if none. */
        AudioSourceDevice* GetInput();

        /** @brief Loads audio data from an external buffer - to be used by AudioSourceDevice
         *
         *  @param channel Number of Channel being processed.
         *  @param bytespersample Number of Bytes per audio sample. From 1 to 4.
         *  @param freq Frequency in Hz.
         *  @param buf Buffer containing the data to be processed.
         *  @param buflen The length of the buffer to be processed, in bytes.
         */
        void LoadAudioData(unsigned int channel,unsigned int bytespersample,unsigned int freq,const char *buf,unsigned int buflen);
        // TODO: Implement a FIFO circular buffer algorithm and add the required variables as necessary.

        /** Standard destructor. */
        virtual ~AudioOutputDevice();

    protected:

        /** @brief Initializes the output device.
         *
         *  @note The m_ok flag MUST be set by this method.
         *  @return True on success; false otherwise.
         */
        virtual bool InitializeOutput();

        /** Mutes the audio output device. Called by Stop() and ShutDown(). */
        virtual void Clear();

        /** Frees the audio output device. Called by ShutDown(). */
        virtual void DisconnectOutput();

        /** Plays / encodes the data received. */
        virtual void RenderData();
        // TODO: Implement a FIFO circular buffer algorithm and add the required variables as necessary.

        /** @brief Sets the output sample frequency in Hz.
          *
          * @param freq The desired sample frequency to be used.
          * @return The actual sample frequency used.
          */
        virtual unsigned int SetDeviceSampleFreq(unsigned int freq);

        /** @brief Sets the bytes per sample.
          *
          * @param newsize the new sample size, in bytes (from 1 to 4).
          * @return The actual sample size, in bytes, used by the output.
          */
        virtual unsigned int SetDeviceBytesPerSample(unsigned int newsize);

        /** Gets the number of audio-channels in the output. Called by Init(). */
        virtual unsigned int GetDeviceNumChannels();

        /** @brief Returns the address for the audio buffer corresponding to a given channel
          *
          * @param idx The channel from which we want to retrieve the output audio buffer.
          * @return a pointer to the audio buffer. NULL if the channel index is invalid.
          */
        char* GetChannelBuffer(unsigned int idx);

    private:

        /** Allocates the Audio output buffers. Called by Init. */
        void AllocateBuffers();

        /** Deallocates the Audio output buffers. Called by ShutDown. */
        void DeallocateBuffers();

        bool m_ok;                      /** Tells whether the output device was initialized correctly. */
        bool m_playing;                 /** Set to true if playing is active. */
        unsigned int m_bytespersample;  /** Current setting of bytes per sample */
        unsigned int m_freq;            /** Current sample frequency */
        unsigned int m_numchannels;     /** Number of audio channels for this device */
        unsigned int m_numbuffers;      /** Number of audio buffers currently allocated */
        unsigned int m_buflen;          /** Current Output buffer's length, in samples */
        unsigned int m_defaultbuflen;   /** Output buffer length for the next Init(). */
        AudioSourceDevice* m_input;     /** The audio source device connected to this object */
        char *m_buffers[256];           /** Array of 256 pointers to char buffers */

    protected:

        /** The m_stop flag indicates whether playback should be stopped. Set to true by Stop(). */
        bool m_stop;
};

#endif
