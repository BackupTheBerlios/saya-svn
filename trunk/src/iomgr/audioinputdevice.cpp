/***************************************************************
 * Name:      audioinputdevice.h
 * Purpose:   Implementation for the AudioInputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-10
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "audioinputdevice.h"


unsigned long AudioInputDevice::GetLength() {
    return 0; // This is a stub
}

unsigned long AudioInputDevice::GetPos() {
    return 0; // This is a stub
}

void AudioInputDevice::ShutDown() {
    return;
}

unsigned long AudioInputDevice::Seek(unsigned long pos, bool fromend) {
    return 0;
}

void AudioInputDevice::SendAudioData(AudioOutputDevice* device) {
}
