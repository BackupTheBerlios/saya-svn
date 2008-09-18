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

const unsigned int maxaudiochannels = 256;
const unsigned int minsamplesperbuffer = 256;
const unsigned int maxsamplesperbuffer = 1048576;

syAudioBuffer::syAudioBuffer(unsigned int numsamples, unsigned int numchannels) {
     if(numchannels < 1) {
         numchannels = 1;
     }
     if(numchannels > maxaudiochannels) {
         numchannels = maxaudiochannels;
     }
     m_NumChannels = numchannels;

     if(numsamples < minsamplesperbuffer) {
         numsamples = minsamplesperbuffer;
     }
     if(numsamples > maxsamplesperbuffer) {
         numsamples = maxsamplesperbuffer;
     }
     m_Size = numsamples + 1;
     m_Data = new int[m_NumChannels*m_Size];
     m_Clear = true;
     m_Head = 0;
     m_Tail = 0;
}

syAudioBuffer::~syAudioBuffer() {
    delete[] m_Data;
}

unsigned int syAudioBuffer::GetSize() {
    return m_Size;
}

unsigned int syAudioBuffer::GetLength() {
    unsigned int curhead = m_Head;
    unsigned int curtail = m_Tail;
    // We must obtain the values first because the calculation result could be altered by the other thread.
    // This way we make sure the result is consistent.
    return (curtail >= curhead) ? (curtail - curhead) : ((m_Size + curtail) - curhead);
}

unsigned int syAudioBuffer::GetNumChannels() {
    return m_NumChannels;
}

bool syAudioBuffer::Read(int* dest) {
    if(m_Head == m_Tail) { m_Clear = true; }
    if(m_Clear) { return false; } // Buffer empty
    int* ptr = &m_Data[m_NumChannels*m_Head];
    for(unsigned int i = 0; i < m_NumChannels; ++i) {
        dest[i] = ptr[i];
    }

    // First calculate the new value of head, and then set the variable. Can't be set in 2 steps because
    // the writer may read m_Head.
    m_Head = (m_Head + 1) % m_Size;
    return true;
}

bool syAudioBuffer::Write(const int* src) {
    if(GetLength() == m_Size) {
        return false; // Buffer full
    }
    int* ptr = &m_Data[m_NumChannels*m_Tail];
    for(unsigned int i = 0; i < m_NumChannels; ++i) {
        ptr[i] = src[i];
    }
    m_Tail = (m_Tail + 1) % m_Size;
    if(m_Clear) { m_Clear = false; }
    return true;
}

bool syAudioBuffer::ReadInto(syAudioBuffer* dest) {
    if(m_Head == m_Tail) { m_Clear = true; }
    if(m_Clear) { return false; } // Buffer empty
    int* ptr = &m_Data[m_NumChannels*m_Head];
    if(dest->Write(ptr)) {
        m_Head = (m_Head + 1) % m_Size;
        return true;
    }
    return false;
}

bool syAudioBuffer::WriteFrom(syAudioBuffer* src) {
    return src->ReadInto(this);
}
