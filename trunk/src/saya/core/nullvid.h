/***************************************************************
 * Name:      nullvid.h
 * Purpose:   Declaration for the NullVID class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef nullvid_h
#define nullvid_h

#include "videoinputdevice.h"
/**
 * NullVID is a derivate of VideoInputDevice. It only displays a blank screen.
 */
class NullVID : public VideoInputDevice {
    public:

        /** Standard constructor. */
        NullVID();

        /** Standard destructor. */
        virtual ~NullVID();

    protected:
        void LoadCurrentFrame();
};

#endif
