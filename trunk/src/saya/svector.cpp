/************************************************************
 * Name:      svector.cpp
 * Purpose:   Implementation of Serializable Vector classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#include "svector.h"

std::string SStringVector::GetTagName() {
    return "SStringVector";
}

bool SStringVector::unserialize(const std::string& src) {
    // TODO: Implement SStringVector::unserialize
    return false;
}

std::string SStringVector::serialize() {
    std::string result = "<SStringVector>";
    unsigned int i;
    for(i = 0; i < data.size(); ++i) {
        result += "<item id=\"" + serializeuint(i) + "\" value=\"" + serializeattribute(data[i]) + "\" />";
    }
    result += "</SStringVector>";
    return result;
}
