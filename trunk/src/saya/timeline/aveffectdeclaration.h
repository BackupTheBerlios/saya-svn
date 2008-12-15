/***************************************************************
 * Name:      aveffectdeclaration.h
 * Purpose:   Declaration for the AVEffectDeclaration class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef aveffectdeclaration_h
#define aveffectdeclaration_h

#include "avcommon.h"
#include "serializable.h"
#include "../core/systring.h"

class AVEffectParamDeclarations;

class AVEffectDeclaration : public serializable {
    public:

        /** Standard constructor. */
        AVEffectDeclaration();

        /** Standard destructor. */
        virtual ~AVEffectDeclaration();

        /** Specifies whether the effect is audio or video. */
        AVType EffectType;

        /** @brief Effect's unique id.
          *
          * Must be in the form: namespace:effectname, where namespace and effectname
          * must be of alphanumeric / underscore. First character must be alphabetic.
          * @note the namespaces "saya" and "openvip" are reserved.
          */
        syString EffectId;
        /** Displayable effect name. */
        syString DisplayName; // Displayable effect name
        /** Short description for the effect (like in a tooltip). */
        syString ShortDescription; // for tooltip
        /** Long description for the effect for documentation / help. */
        syString LongDescription; // For documentation and help
        /** @brief The declaration for the parameters.
          *
          * @see AVEffectParamDeclaration
          */
        AVEffectParamDeclarations* ParametersInfo;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;
};

#endif
