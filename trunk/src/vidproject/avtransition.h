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

class PropertyMap;

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
        std::string TransitionId; // Transition's ID

        /** parameters for the transition */
        PropertyMap* m_Parameters;

        /** Enables or disables the transition */
        bool m_Enabled;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
