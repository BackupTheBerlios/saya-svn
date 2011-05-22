/***************************************************************
 * Name:      avtrack.h
 * Purpose:   Declaration for Timeline track class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avtrack_h
#define avtrack_h

#include "serializable.h"
#include <saya/core/avcommon.h>

class AVClips;
class SMapUintUint;

class AVTrack:public serializable {
    public:

        /** Standard constructor. */
        AVTrack();

        /** Standard destructor. */
        virtual ~AVTrack();

        AVType m_AVType; /// Indicates which type of Track we're handling (video or audio).
        bool m_Readonly; /// Set it to lock the track and prevent any operation.
        bool m_Hidden;   /// Set it to hide/mute the track
        bool m_Collapsed; /// Set it to true to collapse the track

        /** @brief The set of clips in the timeline.
         *
         *  The Track keeps the clips in an STL map. The index is the clip's id, the data is the clip itself.
         */
        AVClips* m_Clips;

        /** @brief Stores the clips' positions in the timeline.
         *
         *  The index is the clip's ID; the value is the clip's position in time.
         *  This map is updated on every clip operation.
         */
        SMapUintUint* m_TimeIndex;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;
};

#endif
