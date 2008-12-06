/***************************************************************
 * Name:      syString.cpp
 * Purpose:   Implementation for the syString class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#include "systring.h"
#include <cstring>

class syStringHelper {
    public:
        static void assign(syString* dest, const char* str,bool useref = false);
        static void assign(syString* dest,const syString& copy);
        static void reset(syString* dest);
};

syString::syString() :
m_Size(0),
m_Capacity(0),
m_UseRef(true),
m_Str(NULL)
{
}

syString::syString(const char* str,bool useref) :
m_Size(0),
m_Capacity(0),
m_Str(NULL)
{
    syStringHelper::assign(this,str, useref);
}

syString::syString(const syString& copy) :
m_Size(0),
m_Capacity(0),
m_Str(NULL)
{
    syStringHelper::assign(this,copy.c_str(),false);
}

syString::~syString() {
    syStringHelper::reset(this);
}

syString& syString::operator=(const syString& copy) {
    if(&copy == this) {
        syString tmps(m_Str, false);
        syStringHelper::reset(this);
        m_UseRef = false;
        m_Str = tmps.m_Str;
        m_Size = tmps.m_Size;
        tmps.m_UseRef = true;
    } else {
        syStringHelper::reset(this);
        syStringHelper::assign(this,copy.c_str(), false);
    }
    return *this;
    return *this;
}

syString& syString::operator=(const char* str) {
    if(str == m_Str) {
        if(m_UseRef) {
            return *this;
        }
        return operator=(syString(*this));
    }
    syStringHelper::reset(this);
    syStringHelper::assign(this,str, false);
    return *this;
}

void syString::clear() {
    syStringHelper::reset(this);
    m_UseRef = true;
    m_Str = NULL;
    m_Size = 0;
}

bool syString::empty() const {
    return (m_Size == 0);
}

void syStringHelper::reset(syString* dest) {
    if(!dest->m_UseRef && dest->m_Str) {
        delete[] dest->m_Str;
    }
    dest->m_Str = 0;
}

void syStringHelper::assign(syString* dest, const syString& copy) {
    syStringHelper::assign(dest,copy.c_str(), false);
}

void syStringHelper::assign(syString* dest, const char* str,bool useref) {
    if(useref && str == dest->m_Str) return;
    dest->m_UseRef = useref;
    if(dest->m_UseRef) {
        dest->m_Str = const_cast<char*>(str);
        dest->m_Size = strlen(str);
    } else {
        dest->m_Size = 0;
        if(str) {
            dest->m_Size = strlen(str);
        }
        if(dest->m_Size) {
            char* tmp = new char[dest->m_Size + 1];
            if(str) {
                strncpy(tmp,str,dest->m_Size);
            }
            tmp[dest->m_Size] = 0;
            dest->m_Str = tmp;
        } else {
            dest->clear();
        }
    }
}

unsigned int syString::size() const {
    return m_Size;
}

unsigned int syString::capacity() const {
    if(m_Size > m_Capacity) {
        m_Capacity = m_Size + 1;
    }
    return m_Capacity;
}

const char* syString::c_str() const {
    return m_Str;
}

bool syString::operator==(const syString& s) const {
    if(m_Size != s.size()) {
        return false;
    }
    return strcmp(c_str(), s.c_str()) == 0;
}

bool syString::operator!=(const syString& s) const {
    return !(operator==(s));
}

bool syString::compare(const syString& s) const {
    return strcmp(c_str(), s.c_str()) < 0;
}

/** Concatenation unary operator */
syString& syString::operator+=(const syString& s) {
    if(!s.size()) return *this;
    unsigned paramlen = s.size();
    unsigned newsize = m_Size + paramlen;
    if(m_Capacity < m_Size) {
        m_Capacity = m_Size;
    }
    char* newbuf = m_Str;
    if(newsize + 1 > m_Capacity) {
        do {
            m_Capacity *=2;
        }while(newsize + 1 > m_Capacity);
        newbuf = new char[m_Capacity];
        strncpy(newbuf,m_Str,m_Size);
    }
    strncpy(newbuf + m_Size,s.c_str(), paramlen);
    newbuf[newsize] = 0;

    if(newbuf != m_Str) {
        syStringHelper::reset(this);
        m_Str = newbuf;
    }
    m_Size = newsize;
    return *this;
}

/** Concatenation binary operator */
const syString syString::operator+(const syString& s) const {
    unsigned paramlen = s.size();
    unsigned newsize = m_Size + paramlen;
    char* newbuf = new char[newsize + 1];
    strncpy(newbuf,m_Str,m_Size);
    strncpy(newbuf + m_Size,s.c_str(), paramlen);
    newbuf[newsize] = 0;
    syString result;
    result.m_Str = newbuf;
    result.m_Size = newsize;
    result.m_UseRef = false;
    return result;
}

char syString::operator[](unsigned int i) const {
    if(i > m_Size) {
        return 0;
    }
    return m_Str[i];
}

char& syString::operator[](unsigned int i) {
    if(m_UseRef) {
        operator=(*this); // Copy-on-modify
    }
    if(i >= m_Size) {
        return *((char*)NULL); // This is surely to generate a segfault!
    }
    return m_Str[i];
}

