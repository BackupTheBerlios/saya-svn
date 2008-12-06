/****************************************************************
 * Name:      smapxstr.cpp
 * Purpose:   Implementation of std::map-based syString serializables
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#include "smapxstr.h"
#include <map>
using namespace std;

// -------------------------------------------------
// begin hidden classes and constructors/destructors
// -------------------------------------------------

class SMapUintStrData {
    public:
        std::map<unsigned int,syString> m_Map;
};

class SMapIntStrData {
    public:
        std::map<int,syString> m_Map;
};

class SMapStrStrData {
    public:
        std::map<syString,syString,ltsystr> m_Map;
};

class SMapUintUintData {
    public:
        std::map<unsigned int,unsigned int> m_Map;
};

SMapUintStr::SMapUintStr() {
    m_Data = new SMapUintStrData;
}

SMapUintStr::~SMapUintStr() {
    delete m_Data;
}

SMapIntStr::SMapIntStr() {
    m_Data = new SMapIntStrData;
}

SMapIntStr::~SMapIntStr() {
    delete m_Data;
}

SMapStrStr::SMapStrStr() {
    m_Data = new SMapStrStrData;
}

SMapStrStr::~SMapStrStr() {
    delete m_Data;
}

SMapUintUint::SMapUintUint() {
    m_Data = new SMapUintUintData;
}

SMapUintUint::~SMapUintUint() {
    delete m_Data;
}

// -----------------------------------------------
// end hidden classes and constructors/destructors
// -----------------------------------------------



bool SMapUintStr::unserialize(const char* src) {
    // TODO: Implement SMapUintStr::unserialize
    return false;
}

const char* SMapUintStr::GetTagName() const {
    static const char* result = "SMapUintStr";
    return result;
}

void SMapUintStr::serialize(serialized& dest) const {
    map<unsigned int,syString>::const_iterator i;
    for(i = m_Data->m_Map.begin(); i != m_Data->m_Map.end(); ++i) {
        serialized tmp(i->first);
        serializestring(dest, tmp.c_str(), i->second.c_str());
    }
}

syString& SMapUintStr::operator[](unsigned int i) {
    return m_Data->m_Map[i];
}

void SMapUintStr::clear() {
    m_Data->m_Map.clear();
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
    map<int,syString>::const_iterator i;
    for(i = m_Data->m_Map.begin(); i != m_Data->m_Map.end(); ++i) {
        serialized tmp(i->first);
        serializestring(dest, tmp.c_str(), i->second.c_str());
    }
}

syString& SMapIntStr::operator[](int i) {
    return m_Data->m_Map[i];
}

void SMapIntStr::clear() {
    m_Data->m_Map.clear();
}


bool SMapStrStr::unserialize(const char* src) {
    // TODO: Implement SMapIntStr::unserialize
    return false;
}

void SMapStrStr::serialize(serialized& dest) const {
    map<syString,syString,ltsystr>::const_iterator i;
    for(i = m_Data->m_Map.begin(); i != m_Data->m_Map.end(); ++i) {
        serializestring(dest, i->first.c_str(), i->second.c_str());
    }
}

syString& SMapStrStr::operator[](const char* s) {
    return m_Data->m_Map[syString(s,true)];
}

void SMapStrStr::clear() {
    m_Data->m_Map.clear();
}

const char* SMapStrStr::GetTagName() const {
    static const char* result = "SMapStrStr";
    return result;
}

const char* tag_smapuint = "SMapUint";

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
    for(i = m_Data->m_Map.begin(); i != m_Data->m_Map.end(); ++i) {
        serialized idx(i->first);
        serializeuint(dest,idx.c_str(),i->second);
    }
}

unsigned int& SMapUintUint::operator[](unsigned int i) {
    return m_Data->m_Map[i];
}

void SMapUintUint::clear() {
    m_Data->m_Map.clear();
}

/** @return a pointer to the string corresponding to the key s, if found; NULL otherwise. */
const syString* SMapStrStr::find(const char* s) {
    map<syString,syString,ltsystr>::const_iterator i = m_Data->m_Map.find(syString(s, true));
    if(i != m_Data->m_Map.end()) {
        return &(i->second);
    }
    return NULL;
}
