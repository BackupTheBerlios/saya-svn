/***************************************************************
 * Name:      avtransition.cpp
 * Purpose:   Implementation of the AVTransition class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avtransition.h"
#include "smap.h"

AVTransition::AVTransition() {
    m_Parameters = new SMapStrStr;
}

AVTransition::~AVTransition() {
    delete m_Parameters;
}

bool AVTransition::unserialize(const std::string& data) {
    // TODO: Implement AVTransition::unserialize
    return false;
}

std::string AVTransition::serialize() {
    // TODO: Implement AVTransition::serialize
    return "";
}
