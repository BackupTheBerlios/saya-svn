/***************************************************************
 * Name:      avclips.h
 * Purpose:   Serializable Vector of AVClip class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avclips_h
#define avclips_h

#include "smap.h"

class AVClip;

class AVClips : public SMapUint<AVClip> {

    public:

        /** standard constructor */
        AVClips() {}

        /** standard destructor */
        virtual ~AVClips() {}
};

#endif
