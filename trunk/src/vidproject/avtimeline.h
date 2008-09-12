/***************************************************************
 * Name:      avtimeline.h
 * Purpose:   Serializable Vector of AVSequence class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avtimeline_h
#define avtimeline_h

#include "svector.h"

class AVSequence;

class AVTimeline : public SVector<AVSequence> {

    public:

        /** standard constructor */
        AVTimeline() {}

        /** standard destructor */
        ~AVTimeline() {}
};

#endif
