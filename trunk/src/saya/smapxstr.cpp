/****************************************************************
 * Name:      smapxstr.cpp
 * Purpose:   Implementation of std::map-based cstr serializables
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#include "smapxstr.h"
using namespace std;

bool SMapUintStr::unserialize(const char* src) {
    // TODO: Implement SMapUintStr::unserialize
    return false;
}

const char* SMapUintStr::GetTagName() const {
    static const char* result = "SMapUintStr";
    return result;
}

void SMapUintStr::serialize(serialized& dest) const {
    map<unsigned int,cstr>::const_iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        serialized tmp(i->first);
        serializestring(dest, tmp.c_str(), i->second.c_str());
    }
}

cstr& SMapUintStr::operator[](unsigned int i) {
    return data[i];
}

void SMapUintStr::clear() {
    data.clear();
}

const char* SMapIntStr::GetTagName() const {
    static const char* result = "SMapIntStr";
    return result;
}

bool SMapIntStr::unserialize(const char* src) {
    // TODO: Implement SMapIntStr::unserialize
    return false;
}

void SMapIntStr::serialize(serialized& dest) const {
    map<int,cstr>::const_iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        serialized tmp(i->first);
        serializestring(dest, tmp.c_str(), i->second.c_str());
    }
}

cstr& SMapIntStr::operator[](int i) {
    return data[i];
}

void SMapIntStr::clear() {
    data.clear();
}


bool SMapStrStr::unserialize(const char* src) {
    // TODO: Implement SMapIntStr::unserialize
    return false;
}

void SMapStrStr::serialize(serialized& dest) const {
    map<cstr,cstr>::const_iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        serializestring(dest, i->first.c_str(), i->second.c_str());
    }
}

cstr& SMapStrStr::operator[](const char* s) {
    return data[cstr(s,true)];
}

void SMapStrStr::clear() {
    data.clear();
}

const char* SMapStrStr::GetTagName() const {
    static const char* result = "SMapStrStr";
    return result;
}

