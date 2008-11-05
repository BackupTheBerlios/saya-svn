/***************************************************************
 * Name:      audioinputdevice.h
 * Purpose:   Implementation for the AudioInputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-10
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "audioinputdevice.h"

avtime_t AudioInputDevice::InternalSeek(avtime_t time, bool fromend) {
    if(fromend) {
        if(time >= m_AudioLength) {
            time = 0;
        } else {
            time = (m_AudioLength) - time;
        }
    } else {
        if(time >= m_AudioLength) {
            if(m_AudioLength > 0) {
                time = m_AudioLength;
            } else {
                time = 0;
            }
        }
    }
    avtime_t result;
    if(!MustAbort()) {
        m_CurrentTime = result = SeekResource(time);
    } else {
        result = m_CurrentTime;
    }
    return result;
}

avtime_t AudioInputDevice::GetLength() const {
    return 0; // This is a stub
}

avtime_t AudioInputDevice::GetPos() const {
    return 0; // This is a stub
}

avtime_t AudioInputDevice::Seek(avtime_t time, bool fromend) {
    return 0;
}

void AudioInputDevice::SendAudioData(AudioOutputDevice* device, unsigned long numsamples) {
    // TODO: Implement AudioInputDevice::SendAudioData
    // Here we have to implement a loop to make sure the desired quantity of numsamples
    // has been sent.
}

void AudioInputDevice::SendAudioData(AudioOutputDevice* device, avtime_t duration) {
    unsigned long numsamples = 0;
    if(duration) {
        unsigned long begin = GetSampleIndex(m_CurrentTime);
        unsigned long end = GetSampleIndex(m_CurrentTime + duration);
        if(end <= begin) {
            return;
        } else {
            numsamples = end + 1- begin;
        }
    }
    SendAudioData(device, numsamples);
}

