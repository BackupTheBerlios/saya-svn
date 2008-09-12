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

AVEffect::AVEffect() {
    m_Parameters = new FXParameterList;
}

AVEffect::~AVEffect() {
    delete m_Parameters;
}

bool AVEffect::unserialize(const std::string& data) {
// TODO: Implement AVEffect::unserialize
    return false;
}

std::string AVEffect::serialize() {
    // TODO: Implement AVEffect::serialize
    return "";
}

AVEffects::AVEffects() {
}

AVEffects::~AVEffects() {
}

bool AVEffects::unserialize(const std::string& data) {
// TODO: Implement AVEffects::unserialize
    return false;
}

std::string AVEffects::serialize() {
    // TODO: Implement AVEffect::serialize
    return "";
}
