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

#include "avsource.h"
/**
 * NullVID is a derivate of AVSource. It only displays a blank screen.
 */
class NullVID : public AVSource {
    public:

        /** Standard constructor. */
        NullVID();

        /** Standard destructor. */
        virtual ~NullVID();

    protected:
        void LoadCurrentFrame();
};

#endif
