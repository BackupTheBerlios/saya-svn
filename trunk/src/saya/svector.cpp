/************************************************************
 * Name:      svector.cpp
 * Purpose:   Implementation of Serializable Vector classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#include "svector.h"
#include "serialized.h"
#include "core/cstr.h"

const char* SStringVector::GetTagName() const {
    static const char* result = "SStringVector";
    return result;
}

bool SStringVector::unserialize(const char* src) {
    // TODO: Implement SStringVector::unserialize
    return false;
}

void SStringVector::serialize(serialized& dest) const {
    unsigned int i;
    for(i = 0; i < data.size(); ++i) {
        serializestring(dest,"#",data[i].c_str());
    }
}
