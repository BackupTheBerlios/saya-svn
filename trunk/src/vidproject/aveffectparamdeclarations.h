/***************************************************************
 * Name:      aveffectparamdeclarations.h
 * Purpose:   Serializable Map of AVEffectParamDeclaration class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef aveffectparamdeclarations_h
#define aveffectparamdeclarations_h

#include "smap.h"

class AVEffect;

class AVEffectParamDeclarations : public SMapStr<AVEffectParamDeclaration> {

    public:

        /** standard constructor */
        AVEffectParamDeclarations() {}

        /** standard destructor */
        ~AVEffectParamDeclarations() {}
};

#endif
