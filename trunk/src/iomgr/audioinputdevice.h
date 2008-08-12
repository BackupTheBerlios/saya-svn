/***************************************************************
 * Name:      audioinputdevice.h
 * Purpose:   Declaration of the AudioInputDevice class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-10
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef audioinputdevice_h
#define audioinputdevice_h

class AudioOutputDevice;

class AudioInputDevice {
    //(TODO: Design and implement the Audio Input Device. (This is a partial stub)

    public:
        unsigned long GetLength();
        unsigned long GetPos();
        unsigned long Seek(unsigned long pos, bool fromend = false);
        void ShutDown();
        void SendAudioData(AudioOutputDevice* device);
};

#endif
