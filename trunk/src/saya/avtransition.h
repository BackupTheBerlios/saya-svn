/***************************************************************
 * Name:      avtransition.h
 * Purpose:   Declaration for the AVTransition class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef avtransition_h
#define avtransition_h

#include "avcommon.h"
#include "serializable.h"
#include "core/cstr.h"
class SMapStrStr;

class AVTransition : public serializable {
    public:

        /** standard constructor */
        AVTransition();

        /** standard destructor */
        virtual ~AVTransition();


        /** @brief Transition type.
         * @see AVType
         */
        AVType m_TransitionType;

        /** @brief Transition's id
          *
          * Tells which transition effect we're applying
          */
        cstr TransitionId; // Transition's ID

        /** parameters for the transition */
        SMapStrStr* m_Parameters;

        /** Enables or disables the transition */
        bool m_Enabled;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;
};

#endif
