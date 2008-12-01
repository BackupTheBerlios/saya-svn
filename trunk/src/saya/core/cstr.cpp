/***************************************************************
 * Name:      cstr.cpp
 * Purpose:   Implementation for the cstr class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#include "cstr.h"
#include <cstring>

cstr::cstr() :
m_Str(NULL) {}

cstr::cstr(const char* str,bool useref) :
m_Str(NULL)
{
    assign(str, useref);
}

cstr::cstr(const cstr& copy) :
m_Str(NULL)
{
    assign(copy.c_str(),false);
}

cstr::~cstr() {
    reset();
}

cstr& cstr::operator=(const cstr& copy) {
    reset();
    assign(copy.c_str(), false);
    return *this;
}

cstr& cstr::operator=(const char* str) {
    reset();
    assign(str, false);
    return *this;
}

void cstr::reset() {
    if(!m_UseRef && m_Str) {
        delete[] m_Str;
    }
    m_Str = 0;
}

void cstr::assign(const cstr& copy) {
    assign(copy.c_str(), false);
}
void cstr::assign(const char* str,bool useref) {
    m_UseRef = useref;
    if(m_UseRef) {
        m_Str = str;
    } else {
        unsigned long slen = 0;
        if(str) {
            slen = strlen(str);
        }
        char* tmp = new char[slen + 1];
        if(str) {
            strncpy(tmp,str,slen);
        }
        tmp[slen] = 0;
        m_Str = tmp;
    }
}

const char* cstr::c_str() const {
    return m_Str;
}

bool cstr::compare(const cstr& s) const {
    return strcmp(c_str(), s.c_str()) < 0;
}
