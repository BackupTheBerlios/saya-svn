/***************************************************************
 * Name:      smap.cpp
 * Purpose:   Implementation of map-based serializables
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "smap.h"
using namespace std;

const char* tag_smapuint = "SMapUint";
const char* tag_smapstr = "SMapStr";

const char* SMapUintUint::GetTagName() const {
    static const char* result = "SMapUintUint";
    return result;
}

bool SMapUintUint::unserialize(const char* src) {
    // TODO: Implement SMapUintUint::unserialize
    return false;
}

void SMapUintUint::serialize(serialized& dest) const {
    map<unsigned int,unsigned int>::const_iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        serialized idx(i->first);
        serializeuint(dest,idx.c_str(),i->second);
    }
}

unsigned int& SMapUintUint::operator[](unsigned int i) {
    return data[i];
}

void SMapUintUint::clear() {
    data.clear();
}
