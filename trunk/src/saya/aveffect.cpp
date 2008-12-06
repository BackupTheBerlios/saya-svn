/***************************************************************
 * Name:      aveffect.h
 * Purpose:   Classes for Video/Audio effect classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "aveffect.h"
#include "fxparameterlist.h"
#include "serialized.h"

AVEffect::AVEffect() {
    m_Parameters = new FXParameterList;
}

AVEffect::~AVEffect() {
    delete m_Parameters;
}

bool AVEffect::unserialize(const char* data) {
// TODO: Implement AVEffect::unserialize
    return false;
}

void AVEffect::serialize(serialized& dest) const {
    // TODO: Implement AVEffect::serialize
}
