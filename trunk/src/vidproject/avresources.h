/***************************************************************
 * Name:      avresources.h
 * Purpose:   Serializable Vector of AVResource class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avresources_h
#define avresources_h

#include "svector.h"
#include "avresource.h"

class AVResources : public SVector<AVResource> {

    public:

        /** standard constructor */
        AVResources() {}

        /** standard destructor */
        ~AVResources() {}
};

#endif
