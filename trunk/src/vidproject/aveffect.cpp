/***************************************************************
 * Name:      aveffect.h
 * Purpose:   Classes for Video/Audio effect classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "aveffect.h"

AVEffect::AVEffect() {
}

AVEffect::~AVEffect() {
}

AVTransition::AVTransition() {
}

AVTransition::~AVTransition() {
}

bool AVTransition::unserialize(const std::string& data) {
// FIXME (rick#1#): Implement me!
    return false;
}

std::string AVTransition::serialize() {
    return "";
}

bool AVEffect::unserialize(const std::string& data) {
// FIXME (rick#1#): Implement me!
    return false;
}

std::string AVEffect::serialize() {
    return "";
}


FXParamKeyFrame::FXParamKeyFrame() {
}

FXParamKeyFrame::~FXParamKeyFrame() {
}
