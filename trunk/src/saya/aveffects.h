/***************************************************************
 * Name:      aveffects.h
 * Purpose:   Serializable Vector of AVEffect class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef aveffects_h
#define aveffects_h

#include "svector.h"
#include "aveffect.h"

class AVEffects : public SVector<AVEffect> {

    public:

        /** standard constructor */
        AVEffects() {}

        /** standard destructor */
        ~AVEffects() {}
};

#endif
