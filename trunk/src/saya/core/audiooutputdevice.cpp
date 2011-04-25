/***************************************************************
 * Name:      audiooutputdevice.h
 * Purpose:   Implementation of the AudioOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <cstddef>
#include "sythread.h"
#include "sentryfuncs.h"
#include "audiobuffer.h"
#include "audiooutputdevice.h"

const unsigned int DefaultAODFrequency = 44100;
const unsigned int DefaultAODNumChannels = 2;
const unsigned int DefaultAODBufferSize = 88200;
const unsigned int DefaultAODPrecision = 16;

class syAudioOutputDeviceData {
    public:
        syAudioOutputDeviceData() : m_Condition(m_Mutex) {}

        /** Buffer to hold the audio data. */
        syAudioBuffer* m_Buffer;

        /** Mutex for thread safety. */
        syMutex m_Mutex;

        /** Condition for waking up if the buffer's full. */
        syCondition m_Condition;

        /** Frequency of the device. */
        unsigned int m_Freq;

        /** Number of audio channels in the device. */
        unsigned int m_NumChannels;

        /** Buffer Size, in samples. */
        unsigned int m_BufSize;

        /** Bits per sample. */
        unsigned int m_Precision;

        /** Default Device Frequency. */
        unsigned int m_DefaultFreq;

        /** Default number of channels. */
        unsigned int m_DefaultNumChannels;

        /** Default buffer size. */
        unsigned int m_DefaultBufSize;

        /** Default precision. */
        unsigned int m_DefaultPrecision;
        };

AudioOutputDevice::AudioOutputDevice() {
    m_IsAudio = true;
    m_IsOutput = true;

    m_Data = new syAudioOutputDeviceData;

    // We must keep a copy of the parameters because we haven't allocated the buffer yet.
    m_Data->m_Freq        = m_Data->m_DefaultFreq        = DefaultAODFrequency;
    m_Data->m_NumChannels = m_Data->m_DefaultNumChannels = DefaultAODNumChannels;
    m_Data->m_BufSize     = m_Data->m_DefaultBufSize     = DefaultAODBufferSize;
    m_Data->m_Precision   = m_Data->m_DefaultPrecision   = DefaultAODPrecision;
}

AudioOutputDevice::~AudioOutputDevice() {
    ShutDown();
    delete m_Data;
}

bool AudioOutputDevice::Connect() {
    return ConnectAudioOutput(m_Data->m_DefaultFreq, m_Data->m_DefaultNumChannels, m_Data->m_DefaultPrecision);
}

bool AudioOutputDevice::ConnectAudioOutput(unsigned int& freq, unsigned int& numchannels, unsigned int& precision) {
    return true;
}

void AudioOutputDevice::Disconnect() {
}

bool AudioOutputDevice::AllocateResources() {
    // This function must be called ONLY by Init()!

    // First, let's allocate the buffer.
    m_Data->m_Buffer = new syAudioBuffer(m_Data->m_DefaultBufSize,
                                         m_Data->m_DefaultNumChannels,
                                         m_Data->m_DefaultPrecision,
                                         m_Data->m_DefaultFreq);
    // Then, get the parameters.
    m_Data->m_Freq = m_Data->m_Buffer->GetSampleFrequency();
    m_Data->m_NumChannels = m_Data->m_Buffer->GetNumChannels();
    m_Data->m_BufSize = m_Data->m_Buffer->GetSize();
    m_Data->m_Precision = m_Data->m_Buffer->GetSamplePrecision();

    return AllocateAudioData();
}

void AudioOutputDevice::FreeResources() {
    FreeAudioData();
    delete m_Data->m_Buffer;
    m_Data->m_Buffer = NULL;
}

bool AudioOutputDevice::AllocateAudioData() {
    return true;
}

void AudioOutputDevice::FreeAudioData() {
}


unsigned int AudioOutputDevice::GetSampleFreq() {
    return m_Data->m_Freq;
}

unsigned int AudioOutputDevice::SetSampleFreq(unsigned int freq) {
    // To avoid complex synchronization issues, we'll just change the Default Sample Frequency
    // and set the new frequency the next Init().

    if(freq < 1) { freq = 1; }
    if(freq > syAudioBuffer::maxsamplefreq) { freq = syAudioBuffer::maxsamplefreq; }
    m_Data->m_DefaultFreq = freq;
    return m_Data->m_Freq;
}

unsigned int AudioOutputDevice::GetSamplePrecision() {
    return m_Data->m_Precision;
}

unsigned int AudioOutputDevice::SetSamplePrecision(unsigned int precision) {
    if(precision < 1) { precision = 1; }
    if(precision > syAudioBuffer::maxbitspersample) { precision = syAudioBuffer::maxbitspersample; }
    m_Data->m_DefaultPrecision = precision;
    return m_Data->m_Precision;
}

unsigned int AudioOutputDevice::GetNumChannels() {
    return m_Data->m_NumChannels;
}

unsigned int AudioOutputDevice::SetNumChannels(unsigned int numchannels) {
    if(numchannels < 1) { numchannels = 1; }
    if(numchannels > syAudioBuffer::maxaudiochannels) { numchannels = syAudioBuffer::maxaudiochannels; }
    m_Data->m_DefaultNumChannels = numchannels;
    return m_Data->m_NumChannels;
}

unsigned int AudioOutputDevice::GetBufferSize() {
    return m_Data->m_BufSize;
}

unsigned int AudioOutputDevice::SetBufferSize(unsigned int newsize) {
    if(newsize < syAudioBuffer::minsamplesperbuffer) { newsize = syAudioBuffer::minsamplesperbuffer; }
    if(newsize > syAudioBuffer::maxsamplesperbuffer) { newsize = syAudioBuffer::maxsamplesperbuffer; }
    m_Data->m_DefaultBufSize = newsize;
    return newsize;
}

unsigned int AudioOutputDevice::SetDeviceSampleFreq(unsigned int freq) {
    return DefaultAODFrequency; // Default
}

unsigned int AudioOutputDevice::SetDeviceSamplePrecision(unsigned int precision) {
    return DefaultAODPrecision; // Default
}

unsigned int AudioOutputDevice::SetDeviceNumChannels(unsigned int numchannels) {
    return DefaultAODNumChannels; // Default
}

unsigned int AudioOutputDevice::GetDeviceSampleFreq() {
    return DefaultAODFrequency; // Default
}

unsigned int AudioOutputDevice::GetDeviceSamplePrecision() {
    return DefaultAODPrecision; // Default
}

unsigned int AudioOutputDevice::GetDeviceNumChannels() {
    return DefaultAODNumChannels; // Default
}

unsigned long AudioOutputDevice::LoadAudioData(const syAudioBuffer* buf, unsigned long numsamples) {
    if(!IsOk()) return 0;
    if(MustAbort()) return 0;
    unsigned long samplesread = 0;
    sySafeMutexLocker lock(*m_InputAudioMutex, this);
    if(lock.IsLocked()) {
        // Read the source buffer until our buffer is filled or the other buffer is empty.

        while(!MustStop() && !(numsamples && samplesread >= numsamples)) {
            if(!buf->ReadInto(m_Data->m_Buffer)) {
                break;
            }
            ++samplesread;
        }
    }
    return samplesread;
}

void AudioOutputDevice::Clear() {
    if(!IsOk()) return;
    m_Data->m_Buffer->Clear();
}


void AudioOutputDevice::FlushAudioData() {
    if(!IsOk()) return;
    if(MustAbort()) return;
    sySafeMutexLocker lock(*m_OutputAudioMutex, this);
    if(lock.IsLocked()) {
        RenderAudioData(m_Data->m_Buffer);
    }
}

void AudioOutputDevice::RenderAudioData(const syAudioBuffer* buf) {
    if(!IsOk()) return;
}
