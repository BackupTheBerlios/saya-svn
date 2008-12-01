/***************************************************************
 * Name:      avtransition.cpp
 * Purpose:   Implementation of the AVTransition class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avtransition.h"
#include "smapxstr.h"

AVTransition::AVTransition() {
    m_Parameters = new SMapStrStr;
}

AVTransition::~AVTransition() {
    delete m_Parameters;
}

bool AVTransition::unserialize(const char* data) {
    // TODO: Implement AVTransition::unserialize
    return false;
}

void AVTransition::serialize(serialized& dest) const {
    // TODO: Implement AVTransition::serialize
}
