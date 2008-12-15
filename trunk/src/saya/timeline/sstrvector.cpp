/************************************************************
 * Name:      sstrvector.cpp
 * Purpose:   Implementation of Serializable Vector classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-12-06
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#include "sstrvector.h"
#include "serialized.h"
#include "../core/systring.h"
#include <vector>

class SStringVectorData {
    public:
        std::vector<syString> m_Vector;
};

SStringVector::SStringVector() {
    m_Data = new SStringVectorData;
}

SStringVector::~SStringVector() {
    delete m_Data;
}

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
    for(i = 0; i < m_Data->m_Vector.size(); ++i) {
        serializestring(dest,"#",m_Data->m_Vector[i].c_str());
    }
}

syString& SStringVector::operator[](unsigned int i) {
    return m_Data->m_Vector[i];
}

const syString& SStringVector::operator[](unsigned int i) const {
    return m_Data->m_Vector[i];
}

unsigned int SStringVector::size() {
    return m_Data->m_Vector.size();
}

void SStringVector::clear() {
    m_Data->m_Vector.clear();
};

void SStringVector::push_back(const syString& s) {
    m_Data->m_Vector.push_back(s);
}
