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

class syAudioBuffer {

    public:

        /** @brief Constructor.
         *  @param samplesperframe The number of samples to reserve for this buffer.
         *  @param numchannels The number of channels to reserve for this buffer.
         */
        syAudioBuffer(unsigned int numsamples, unsigned int numchannels);

        /** Standard destructor. */
        ~syAudioBuffer();

        /** Gets the number of samples in this buffer */
        unsigned int GetSize();

        /** Returns the actual length of the buffer, in samples. */
        unsigned int GetLength();

        /** Gets the number of channels the frame has */
        unsigned int GetNumChannels();

        /** @brief Reads a sample from the buffer and stores it in dst.
         *
         *  @param dst The destination minibuffer for the sample. Must have as many integers as channels in the data.
         *  @return true if the sample was read correctly; false if the buffer's empty.
         */
        bool Read(int* dst);

        /** @brief Writes a sample into the buffer from the minibuffer *src.
         *
         *  @param src The source minibuffer for the sample. Must have as many integers as channels in the data.
         *  @return true if the sample was written correctly; false if the buffer's full.
         */
        bool Write(const int* src);

        /**  @brief Reads one sample of data and writes it into a destination buffer.
         *   @param dst the destination buffer.
         *   @return true on success; false if either the source was empty or the destination was full.
         */
        bool ReadInto(syAudioBuffer* dst);

        /**  @brief Writes one sample of data from a source buffer.
         *   @param src The source buffer.
         *   @return true on success; false if either the source was empty or the destination was full.
         */
        bool WriteFrom(syAudioBuffer* src);

    private:

        /** The number of channels allocated for the frame */
        unsigned int m_NumChannels;

        /** The number of samples allocated for each channel buffer */
        unsigned int m_Size;

        /** The buffer's head. Points to the sample to be read by the output. */
        unsigned int m_Head;

        /** The buffer's tail. Points to the sample to be written by the input. */
        unsigned int m_Tail;

        /** The buffer holding the data */
        int* m_Data;

        /** Indicates whether the buffer has been cleared. */
        bool m_Clear;
};

#endif
