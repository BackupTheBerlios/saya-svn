/****************************************************************
 * Name:      audiobuffer.h
 * Purpose:   Declaration for the syAudioBuffer class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-07
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 * Comments:  A lock-free, thread-safe (for 1 reader, 1 writer)
 *            circular FIFO buffer for audio data.
 ***************************************************************/

#ifndef syaudiobuffer_h
#define syaudiobuffer_h

class syAudioBufferData;

class syAudioBuffer {

    public:

        static const unsigned int maxbitspersample;
        static const unsigned int maxsamplefreq;
        static const unsigned int maxaudiochannels;
        static const unsigned int minsamplesperbuffer;
        static const unsigned int maxsamplesperbuffer;

        /** @brief Constructor.
         *  @param numsamples The number of samples to reserve for this buffer.
         *  @param numchannels The number of channels to reserve for this buffer.
         *  @param precision The precision, in bits per sample, to assign to this buffer.
         *  @param freq The frequency, in Hz, to assign to this buffer.
         */
        syAudioBuffer(unsigned int numsamples, unsigned int numchannels, unsigned int precision, unsigned int freq);

        /** Standard destructor. */
        ~syAudioBuffer();

        /** Gets the number of samples in this buffer */
        unsigned int GetSize() const;

        /** Returns the actual length of the buffer, in samples. */
        unsigned int GetLength() const;

        /** Gets the buffer's number of channels */
        unsigned int GetNumChannels() const;

        /** Gets the number of bits per sample. From 1 to 32. */
        unsigned int GetSamplePrecision() const;

        /** @brief Sets the sample precision, in bits per sample. Changes take effect immediately.
         *
         *  @param precision The desired precision. From 1 to 32.
         *  @return the actual number of bits per sample assigned (the value is constraint-checked).
         */
        unsigned int SetSamplePrecision(unsigned int precision);

        /** Gets the Buffer Sample Frequency, in Hz. */
        unsigned int GetSampleFrequency() const;

        /** @brief Sets the sample frequency, in Hz.  Changes take effect immediately.
         *
         *  @param precision The desired sample frequency, in Hz (from 1 to 96000).
         *  @return the actual frequency assigned (the value is constraint-checked).
         */
        unsigned int SetSampleFrequency(unsigned int freq);

        /** @brief Reads a sample from the buffer and stores it in dst.
         *
         *  @param dst The destination minibuffer for the sample. Must have as many integers as channels in the data.
         *  @param precision The destination precision, in bits per sample. 0 to use the same precision as the buffer
         *  @return true if the sample was read correctly; false if the buffer's empty.
         *
         *  @note We declare this method as 'const' so we can pass the buffer as 'const' when
         *        we want only to read the buffer. But since reading actually alters two hidden variables
         *        (namely m_Head and m_Clear), we declare them as 'mutable', which allows them to be modified
         *        even if the object was accessed as 'const'.
         */
        bool Read(int* dst, unsigned int precision = 0) const;

        /** @brief Writes a sample into the buffer from the minibuffer *src.
         *
         *  @param src The source minibuffer for the sample. Must have as many integers as channels in the data.
         *  @param freq The input sample frequency, in Hz.
         *         The input will be resampled (i.e. ignored and/or duplicated) to match output frequency.
         *         Set freq to 0 if you want to match the buffer's sampling frequency.
         *  @param precision The source precision, in bits per sample. Use 0 to match the buffer's precision.
         *  @return true if the sample was written correctly; false if the buffer's full.
         */
        bool Write(const int* src, unsigned int freq = 0, unsigned int precision = 0);

        /**  @brief Reads one sample of data and writes it into a destination buffer.
         *   @param dst the destination buffer.
         *   @return true on success; false if either the source was empty or the destination was full.
         */
        bool ReadInto(syAudioBuffer* dst) const;

        /**  @brief Writes one sample of data from a source buffer.
         *   @param src The source buffer.
         *   @return true on success; false if either the source was empty or the destination was full.
         */
        bool WriteFrom(const syAudioBuffer* src);

        /**  @brief Signals the buffer to be cleared for the next write.
         *   @note Reads done before the next write, will return false.
         *   We set this function to const because technically, a reader can clear the buffer if it
         *   reads all the input, so setting a flag will just suffice.
         */
        void Clear() const;

    private:
        syAudioBufferData* m_Data;
};

#endif
