/***************************************************************
 * Name:      saya_events.h
 * Purpose:   Definition for the Project Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef saya_events_h
#define saya_events_h

#include "core/events.h"

class syProjectStatusEvent : public syEvent {
    public:
        virtual syEvent* clone() { return new syProjectStatusEvent(*this); }
};


#endif
