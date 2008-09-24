/******************************************************************
 * Name:      audiobuffer.cpp
 * Purpose:   Implementation of the syAudioBuffer class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-07
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 * Comments:  A lock-free, thread-safe (for 1 reader, 1 writer)
 *            circular FIFO buffer for audio data.
 *****************************************************************/

// Programmer notes:

// In this implementation, the tail grows with each new write, while the head shrinks with each new read.
// To make this class thread-safe, we must ABSOLUTELY make sure that the writer does not write to m_Head,
// and that the reader does not write to m_Tail.

// Writing to the buffer will increase m_Tail, while reading from the buffer will increase m_Head
// (think of it as a snake moving backwards).

// Clearing will first set the m_Clear flag to true, and then set the tail equal to the head.
// if(m_Clear) is true, setting the m_Clear to false will be the last operation done by the writer.
// This is to avoid race conditions where the buffer is cleared by the writer but then the head is disminished by
// the reader, resulting in a garbage buffer of size m_Size.

#include "audiobuffer.h"

const unsigned int syAudioBuffer::maxbitspersample = 32;
const unsigned int syAudioBuffer::maxsamplefreq = 96000;
const unsigned int syAudioBuffer::maxaudiochannels = 256;
const unsigned int syAudioBuffer::minsamplesperbuffer = 256;
const unsigned int syAudioBuffer::maxsamplesperbuffer = 1048576;

class syAudioBufferData {
    public:

        /** The number of channels allocated for the frame */
        unsigned int m_NumChannels;

        /** The number of samples allocated for each channel buffer */
        unsigned int m_Size;

        /** The buffer's head. Points to the sample to be read by the output. */
        mutable volatile unsigned int m_Head;

        /** The buffer's tail. Points to the sample to be written by the input. */
        volatile unsigned int m_Tail;

        /** Sample precision */
        unsigned int m_Precision;

        /** Sample Frequency */
        unsigned int m_Freq;

        /** The buffer holding the data */
        int* m_Data;

        /** Indicates whether the buffer has been cleared. */
        mutable volatile bool m_Clear;

        /** Quotient for samplerate conversion */
        unsigned long m_Quotient;

        /** Remainder for samplerate conversion */
        unsigned long m_Remainder;

        /** Remainder Counter for samplerate conversion */
        unsigned long m_RCounter;

        /** The last input frequency that was given. */
        unsigned long m_LastInputFreq;

        /** Copies a sample between two buffers. Does the precision conversion on-the-fly. */
        static void CopySample(const int* src, int* dst,unsigned int numchannels, unsigned int srcprecision, unsigned int dstprecision);
};

inline void syAudioBufferData::CopySample(const int* src,int* dst, unsigned int numchannels, unsigned int srcprecision, unsigned int dstprecision) {
    if(srcprecision > syAudioBuffer::maxbitspersample) { srcprecision = syAudioBuffer::maxbitspersample; }
    if(dstprecision > syAudioBuffer::maxbitspersample) { dstprecision = syAudioBuffer::maxbitspersample; }
    if(srcprecision == dstprecision) {
        // Is it the same precision that we're handling?
        for(unsigned int i = numchannels; i; --i, ++dst, ++src) {
            *dst = *src;
        }
    } else if(srcprecision < dstprecision) {
        // Source has more precision than the destination; shift right.
        unsigned int shifts = dstprecision - srcprecision;
        for(unsigned int i = numchannels; i; --i, ++dst, ++src) {
            *dst = *src >> shifts;
        }
    } else {
        // Source has less precision than the destination; shift left.
        unsigned int shifts = srcprecision - dstprecision;
        for(unsigned int i = numchannels; i; --i, ++dst, ++src) {
            *dst = *src << shifts;
        }
    }
}

syAudioBuffer::syAudioBuffer(unsigned int numsamples, unsigned int numchannels, unsigned int precision, unsigned int freq) {

    m_Data = new syAudioBufferData;

    if(numchannels < 1) {
        numchannels = 1;
    }
    if(numchannels > maxaudiochannels) {
        numchannels = maxaudiochannels;
    }
    m_Data->m_NumChannels = numchannels;

    if(numsamples < minsamplesperbuffer) {
        numsamples = minsamplesperbuffer;
    }
    if(numsamples > maxsamplesperbuffer) {
        numsamples = maxsamplesperbuffer;
    }
    m_Data->m_Size = numsamples + 1;
    m_Data->m_Data = new int[m_Data->m_NumChannels*m_Data->m_Size];
    m_Data->m_Clear = true;
    m_Data->m_Head = 0;
    m_Data->m_Tail = 0;

    SetSamplePrecision(precision);
    SetSampleFrequency(freq);

    m_Data->m_Quotient = 0;
    m_Data->m_Remainder = 0;
    m_Data->m_RCounter = 0;
    m_Data->m_LastInputFreq = 0;
}

syAudioBuffer::~syAudioBuffer() {
    delete[] m_Data->m_Data;
    delete m_Data;
}

unsigned int syAudioBuffer::GetSize() const {
    return m_Data->m_Size;
}

unsigned int syAudioBuffer::GetLength() const {
    unsigned int curhead = m_Data->m_Head;
    unsigned int curtail = m_Data->m_Tail;
    // We must obtain the values first because the calculation result could be altered by the other thread.
    // This way we make sure the result is consistent, even if a bit late.
    return (curtail >= curhead) ? (curtail - curhead) : ((m_Data->m_Size + curtail) - curhead);
}

unsigned int syAudioBuffer::GetNumChannels() const {
    return m_Data->m_NumChannels;
}

unsigned int syAudioBuffer::GetSamplePrecision() const {
    return m_Data->m_Precision;
}

unsigned int syAudioBuffer::SetSamplePrecision(unsigned int precision) {
    if(precision < 1) { precision = 1; }
    if(precision > maxbitspersample) { precision = maxbitspersample; }
    m_Data->m_Precision = precision;
    return m_Data->m_Precision;
}

unsigned int syAudioBuffer::GetSampleFrequency() const {
    return m_Data->m_Freq;
}

unsigned int syAudioBuffer::SetSampleFrequency(unsigned int freq) {
    if(freq < 1) { freq = 1; }
    if(freq > maxsamplefreq) { freq = maxsamplefreq; }
    m_Data->m_Freq = freq;
    return m_Data->m_Freq;
}

bool syAudioBuffer::Read(int* dest,unsigned int precision) const {
    if(m_Data->m_Head == m_Data->m_Tail) { m_Data->m_Clear = true; return false; }
    if(m_Data->m_Clear) { return false; } // Buffer empty
    const int* ptr = &m_Data->m_Data[m_Data->m_NumChannels*m_Data->m_Head];
    if(!precision) { precision = m_Data->m_Precision; } // Default
    // Copy the sample
    syAudioBufferData::CopySample(ptr,dest, m_Data->m_NumChannels, m_Data->m_Precision, precision);

    // First calculate the new value of head, and then set the variable. Can't be set in 2 steps because
    // the writer may read m_Data->m_Head.
    m_Data->m_Head = (m_Data->m_Head + 1) % m_Data->m_Size;
    return true;
}

bool syAudioBuffer::Write(const int* src, unsigned int freq, unsigned int precision) {
    if(GetLength() == m_Data->m_Size) {
        return false; // Buffer full
    }
    if(m_Data->m_Clear) {
        // Note that m_Clear could be set in the middle of this function call; there's
        // nothing we can do about it, but the times that we can catch it, let's reset m_LastInputFreq.
        m_Data->m_LastInputFreq = 0;
    }

    // Set default values in case of 0
    if(!freq) { freq = m_Data->m_Freq; }
    if(!precision) { precision = m_Data->m_Precision; }

    // Calculate Quotient, remainder and set the counters to 0 for resampling.
    if(freq != m_Data->m_LastInputFreq) {

        // Explanation for the resampling:
        //
        // We divide the current frequency by the input frequency.
        // If our sample freq. is 22050 Hz and the input freq. is 44100 Hz, we have a 0.5 coefficient;
        // So for every input sample (at 44100 samples/sec), we have 0.5 output samples (at 22050 samples/sec).
        // What we do is accumulate this coefficient with time (actually just the remainder), and when the
        // sum is greater than one, we add it to the quotient.
        // This quotient gives us the actual number of samples that we must copy into our destination buffer.
        // For a coefficient of 0.5, every 2 input samples we get a quotient of 1, so that's what we actually
        // put in the buffer.

        // Let's do the opposite case. If our sample frequency is 44100 and the input frequency is 22050,
        // Our quotient is 2. It means that we must copy the sample received 2 times.

        // Now, for a mixed integer / fractional result. If our sample frequency is 48000 and the input
        // frequency is 44100, we get a coefficient of 1.08843... which means a quotient of 1 and a
        // remainder of 3900.
        // So everytime we add 1 copy of the input sample to the buffer, and add 3900 to the counter.
        // After the 12th time we're given a sample, we've added 3900 times 12 to the counter, which
        // makes it equal to 46800. That is greater than 44100 (the input sample frequency), meaning that
        // the remainder has reached an overflow. So we add 1 to the quotient and get the modulus.
        // This makes the quotient = 1 (the original quotient) + 1 (the overflow) = 2,
        // and the remainder counter is left at 2700 for the next input.
        // Since the quotient is 2, we copy the input sample twice.

        // The next overflow will come the 11th time, when the counter = 2700 + 3900*11 = 45600 = 44100 + 1500.
        // So we copy the sample twice and leave the counter at 1500.
        // And so on.

        // A final example: Input frequency = 32000 Hz. Our frequency = 48000.
        // coefficient = 48000 / 32000 = 1.5. Quotient = 1, remainder = 16000.
        // The second time we'll have an overflow at 16000 + 16000 = 32000, leaving the counter at 0.

        // Note that our sampling is rudimentary and we don't do anything to avoid aliasing artifacts.
        // That belongs to the signal processing part of the application. The resampling provided here is
        // implemented to simplify the data transfer routines.

        m_Data->m_Quotient = m_Data->m_Freq / freq;
        m_Data->m_Remainder = m_Data->m_Freq % freq;
        m_Data->m_RCounter = 0;
        m_Data->m_LastInputFreq = freq;
    }

    if(freq == m_Data->m_Freq) {

        // Input sample frequency == buffer's sample frequency. Copy.
        int* ptr = &m_Data->m_Data[m_Data->m_NumChannels*m_Data->m_Tail];
        syAudioBufferData::CopySample(src, ptr, m_Data->m_NumChannels, precision, m_Data->m_Precision);
        m_Data->m_Tail = (m_Data->m_Tail + 1) % m_Data->m_Size;
    } else {

        // Input sample frequency != buffer's sample frequency. We have to resample.

        // Calculate the amount in samples that we need to advance.
        // See the above explanation for the logic behind the resampling.
        unsigned int quotient = m_Data->m_Quotient;
        unsigned int newr = m_Data->m_Remainder + m_Data->m_RCounter;
        if(newr >= m_Data->m_LastInputFreq) {
            quotient += newr / m_Data->m_LastInputFreq;
            newr %= m_Data->m_LastInputFreq;
        }

        if(quotient + GetLength() >= GetSize()) {
            return false; // Buffer full
        }

        // All clear. Let's add the samples.
        // NOTE: This operation assumes that we're the only thread writing to the buffer.
        // Otherwise we'd have to check for alterations in Head and Tail.
        for(unsigned int i = quotient; i; --i) {
            int* ptr = &m_Data->m_Data[m_Data->m_NumChannels*m_Data->m_Tail];
            syAudioBufferData::CopySample(src, ptr, m_Data->m_NumChannels, precision, m_Data->m_Precision);
            // We have to update tail everytime because we could wrap at the end of the buffer.
            m_Data->m_Tail = (m_Data->m_Tail + 1) % m_Data->m_Size;
        }
        // Now, update the remainder counter.
        m_Data->m_RCounter = newr;
    }
    if(m_Data->m_Clear) { m_Data->m_LastInputFreq = 0; m_Data->m_Clear = false; }
    return true;
}

bool syAudioBuffer::ReadInto(syAudioBuffer* dest) const {
    if(m_Data->m_Head == m_Data->m_Tail) { m_Data->m_Clear = true; return false; }
    if(m_Data->m_Clear) { return false; } // Buffer empty
    int* ptr = &m_Data->m_Data[m_Data->m_NumChannels*m_Data->m_Head];
    if(dest->Write(ptr)) {
        m_Data->m_Head = (m_Data->m_Head + 1) % m_Data->m_Size;
        return true;
    }
    return false;
}

bool syAudioBuffer::WriteFrom(const syAudioBuffer* src) {
    return src->ReadInto(this);
}

void syAudioBuffer::Clear() const {
    m_Data->m_Clear = true;
}
