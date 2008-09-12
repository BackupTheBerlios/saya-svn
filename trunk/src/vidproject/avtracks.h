/***************************************************************
 * Name:      avtracks.h
 * Purpose:   Serializable Vector of AVTrack class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avtracks_h
#define avtracks_h

#include "svector.h"

class AVTrack;

class AVTracks : public SVector<AVTrack> {

    public:

        /** standard constructor */
        AVTracks() {}

        /** standard destructor */
        ~AVTracks() {}
};

#endif
