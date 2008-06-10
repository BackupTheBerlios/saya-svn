/***************************************************************
 * Name:      audiooutputdevice.h
 * Purpose:   Implementation of the AudioOutputDevice class
AudioOutputDevice::* Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
AudioOutputDevice::* Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "audiooutputdevice.h"
/* #include "audiosourcedevie.h" */

AudioOutputDevice::AudioOutputDevice() :
m_ok(false),
m_playing(false),
m_bytespersample(2),
m_freq(44100),
m_numchannels(2),
m_numbuffers(0),
m_buflen(88200),
m_defaultbuflen(88200),
m_input(NULL),
m_stop(false)
{
    size_t i;
    for(i = 0; i < 256; i++) {
        m_buffers[i] = NULL;
    }

}

bool AudioOutputDevice::Init() {
    DeallocateBuffers();
    m_ok = InitializeOutput();
    if(m_ok) {
        m_numchannels = GetDeviceNumChannels();
        if(m_numchannels > 255) m_numchannels = 255;
        AllocateBuffers();
    }
    m_playing = false;
    return m_ok;
}

bool AudioOutputDevice::IsOk() {
    return m_ok;
}

bool AudioOutputDevice::InitializeOutput() {
    return true;
}

void AudioOutputDevice::PlayPeriod(unsigned int starttime, unsigned int stoptime) {
    if(!IsOk()) return;
    m_stop = false;
    if(m_input != NULL) {
//  TODO: Write the AudioSourceDevice class
//       m_input->PlayAudio(this, starttime, stoptime);
    }
}

void AudioOutputDevice::Stop() {
    m_stop = true;
}

void AudioOutputDevice::ShutDown() {
    Clear();
    DisconnectOutput();
    DeallocateBuffers();
}

unsigned int AudioOutputDevice::GetSampleFreq() {
    return m_freq;
}

unsigned int AudioOutputDevice::SetSampleFreq(unsigned int freq) {
    m_freq = SetDeviceSampleFreq(freq);
    return m_freq;
}

unsigned int AudioOutputDevice::GetBytesPerSample() {
    return m_bytespersample;
}

unsigned int AudioOutputDevice::SetBytesPerSample(unsigned int newsize) {
    newsize = SetDeviceBytesPerSample(newsize);
    if(newsize < 1) {
        newsize = 1;
    }
    if(newsize > 4) {
        newsize = 4;
    }
    m_bytespersample = newsize;
    return m_bytespersample;
}

unsigned int AudioOutputDevice::GetNumBuffers() {
    return m_numbuffers;
}

unsigned int AudioOutputDevice::GetNumChannels() {
    return m_numchannels;
}

unsigned int AudioOutputDevice::SetDeviceSampleFreq(unsigned int freq) {
    return 44100; // Default
}

unsigned int AudioOutputDevice::SetDeviceBytesPerSample(unsigned int newsize) {
    return 2; // Default
}

unsigned int AudioOutputDevice::GetDeviceNumChannels() {
    return 0; // Default
}

char* AudioOutputDevice::GetChannelBuffer(unsigned int idx) {
    if(idx > m_numbuffers)
        return NULL;
    return m_buffers[idx];
}

void AudioOutputDevice::AllocateBuffers() {
    size_t i;

    for(i = 0; i < 256 && i < m_numchannels; i++) {
        m_buffers[i] = new char[m_defaultbuflen * m_bytespersample];
    }
    m_numbuffers = m_numchannels;
    m_buflen = m_defaultbuflen;
}

void AudioOutputDevice::DeallocateBuffers() {
    size_t i;
    for(i = 0; i < 256 && i < m_numbuffers; i++) {
        if(m_buffers[i] != NULL) {
            delete[] m_buffers[i];
        }
    }
    for(i = 0; i < 256; i++) {
        m_buffers[i] = NULL;
    }
    m_numbuffers = 0;
}

unsigned int AudioOutputDevice::GetBufferLength() {
    return m_buflen;
}

unsigned int AudioOutputDevice::SetBufferDefaultLength(unsigned int newlen) {
    if(newlen < 44100) {
        newlen = 44100;
    }
    m_defaultbuflen = 44100;
    return m_defaultbuflen;
}

unsigned int AudioOutputDevice::GetBufferDefaultLength() {
    return m_defaultbuflen;
}

void AudioOutputDevice::Clear() {
    if(!IsOk()) return;
}

void AudioOutputDevice::DisconnectOutput() {
    if(!IsOk()) return;
}

void AudioOutputDevice::RenderData() {
    if(!IsOk()) return;
}

bool AudioOutputDevice::ConnectInput(AudioSourceDevice* inputdev) {
    if(m_input != NULL) {
        DisconnectInput();
    }
    m_input = inputdev;
    return (m_input != NULL);
}

void AudioOutputDevice::DisconnectInput() {
    m_input = NULL;
}

void AudioOutputDevice::LoadAudioData(unsigned int channel,unsigned int bytespersample,unsigned int freq,const char *buf,unsigned int buflen) {
}

AudioOutputDevice::~AudioOutputDevice() {
    DisconnectInput();
    ShutDown();
}
