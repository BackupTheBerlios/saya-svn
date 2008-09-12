/***************************************************************
 * Name:      aveffect.h
 * Purpose:   Definitions for Video/Audio effect classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef aveffect_h
#define aveffect_h

#include "avcommon.h"
#include "serializable.h"

class FXParameterList;

class AVEffect : public serializable {

    public:

        /** standard constructor */
        AVEffect();

        /** standard destructor */
        virtual ~AVEffect();

        /** Tells whether the effect is video or audio */
        AVType m_EffectType;

        /** @brief Effect's unique id.
          *
          * @see AVEffectDeclaration::EffectId
          */
        std::string EffectId; // Effect's ID

        /** @brief The effect's parameters.
          *
          * @see FXParameterList
          */
        FXParameterList* m_Parameters;

        /** The enabled flag for the effect */
        bool m_Enabled;

        /** Begining time (clip-time) for the effect */
        unsigned int m_StartTime;

        /** Ending time (clip-time) for the effect */
        unsigned int m_EndTime;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
