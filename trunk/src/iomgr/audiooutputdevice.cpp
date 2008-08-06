/***************************************************************
 * Name:      audiooutputdevice.h
 * Purpose:   Implementation of the AudioOutputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "audiooutputdevice.h"
#include "mutex.h"
/* #include "audiosourcedevie.h" */

AudioOutputDevice::AudioOutputDevice(bool usedefaultbuffers) :
m_usedefaultbuffers(usedefaultbuffers),
m_playing(false),
m_changingparams(false),
m_shuttingdown(false),
m_ok(false),
m_mutex(NULL),
m_bytespersample(2),
m_freq(44100),
m_numchannels(2),
m_numbuffers(0),
m_buflen(88200),
m_defaultbuflen(88200)
{
    size_t i;
    m_mutex = new syMutex;
    for(i = 0; i < 256; i++) {
        m_buffers[i] = NULL;
    }


}

AudioOutputDevice::~AudioOutputDevice() {
    ShutDown();
    delete m_mutex;
    m_mutex = NULL; // Clear memory
}

bool AudioOutputDevice::Init() {
    if(!m_ok) {
        // Mutex not needed here because only Init() and ShutDown() change m_ok and they're only called by the main thread.
        m_changingparams = true;
        DeallocateBuffers();
        m_ok = InitializeOutput();

        if(m_ok) {
            m_numchannels = GetDeviceNumChannels();
            if(m_numchannels > 255) m_numchannels = 255;
            AllocateBuffers();
            m_playing = false;
        }
        m_changingparams = false;
    }
    return m_ok;
}

bool AudioOutputDevice::IsOk() {
    return m_ok;
}

bool AudioOutputDevice::InitializeOutput() {
    return true;
}

bool AudioOutputDevice::MustAbort() {
    return (!m_ok || m_shuttingdown || m_changingparams);
}

void AudioOutputDevice::ShutDown() {
    m_shuttingdown = true;
    while(m_playing) {
        syMilliSleep(10); // Sleep for 10 milliseconds
    }
    Clear();
    DisconnectOutput();
    DeallocateBuffers();
    m_ok = false;
    m_shuttingdown = false;
}

unsigned int AudioOutputDevice::GetSampleFreq() {
    return m_freq;
}

unsigned int AudioOutputDevice::SetSampleFreq(unsigned int freq) {
    bool result = true;
    {
        syMutexLocker mylocker(*m_mutex);
        if(m_shuttingdown || m_changingparams || m_playing) {
            result = false;
        } else {
            m_changingparams = true;
        }
    }

    if(result) {
        m_freq = SetDeviceSampleFreq(freq);
        m_changingparams = false;
    }
    return m_freq;
}

unsigned int AudioOutputDevice::GetBytesPerSample() {
    return m_bytespersample;
}

unsigned int AudioOutputDevice::SetBytesPerSample(unsigned int newsize) {
    bool result = true;
    {
        syMutexLocker mylocker(*m_mutex);
        if(m_shuttingdown || m_changingparams || m_playing) {
            result = false;
        } else {
            m_changingparams = true;
        }
    }

    if(result) {
        if(newsize < 1) {
            newsize = 1;
        }
        if(newsize > 4) {
            newsize = 4;
        }
        newsize = SetDeviceBytesPerSample(newsize);
        m_bytespersample = newsize;
        m_changingparams = false;
    }

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
    // This function must be called ONLY by Init()!
    size_t i;

    if(!m_usedefaultbuffers) {
        m_numbuffers = 0;
        return;
    }
    for(i = 0; i < 256 && i < m_numchannels; i++) {
        m_buffers[i] = new char[m_defaultbuflen * m_bytespersample];
    }
    m_numbuffers = m_numchannels;
    m_buflen = m_defaultbuflen;
}

void AudioOutputDevice::DeallocateBuffers() {
    // This function must be called ONLY by Init() or Shutdown()!
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

void AudioOutputDevice::LoadAudioData(unsigned int channel,unsigned int bytespersample,unsigned int freq,const char *buf,unsigned int buflen) {
    if(!IsOk()) return;
    bool result = true;
    {
        syMutexLocker mylocker(*m_mutex);
        if(m_playing || MustAbort()) {
            result = false;
        } else {
            m_playing = true;
        }
    }

    if(result) {
        LoadDeviceAudioData(channel, bytespersample, freq, buf, buflen);
        m_playing = false;
    }
}


void AudioOutputDevice::Clear() {
    if(!IsOk()) return;
}

void AudioOutputDevice::DisconnectOutput() {
    if(!IsOk()) return;
}

void AudioOutputDevice::LoadDeviceAudioData(unsigned int channel,unsigned int bytespersample,unsigned int freq,const char *buf,unsigned int buflen) {
    if(!IsOk()) return;
}

void AudioOutputDevice::RenderData() {
    if(!IsOk()) return;
}
