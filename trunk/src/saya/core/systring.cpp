/***************************************************************
 * Name:      syString.cpp
 * Purpose:   Implementation for the syString class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#include "systring.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <wchar.h>

class syStringHelper {
    public:
        static void assign(syString* dest, const char* str,bool useref = false);
        static void assign(syString* dest,const syString& copy);
        static void reset(syString* dest);
        static unsigned int GetMinLength(const syString& s1, const syString& s2);
};

inline unsigned int syStringHelper::GetMinLength(const syString& s1, const syString& s2) {
    unsigned int result = s1.m_Size;
    if(result > s2.m_Size) result = s2.m_Size;
    return result;
}


const int syString::npos = -1;

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
    syStringHelper::assign(this,copy.m_Str,false);
}

syString::syString(const wchar_t* str) :
m_Size(0),
m_Capacity(0),
m_Str(NULL)
{
    unsigned int size = (wcslen(str) * MB_CUR_MAX + 1);
    char buf[size]; // str has 4 bytes per character. Doubling that should be enough for an UTF-8 string.
    wcstombs(buf, str, size);
    buf[size - 1] = 0;
    syStringHelper::assign(this,(const char*) buf, false);
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
        syStringHelper::assign(this,copy.m_Str, false);
    }
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

syString& syString::operator=(const char c) {
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    return operator=((const char*)buf);
}

syString& syString::operator=(const wchar_t c) {
    wchar_t buf[2];
    buf[0] = c;
    buf[1] = 0;
    return operator=((const wchar_t*)buf);
}

syString& syString::operator=(const wchar_t* str) {
    syStringHelper::reset(this);
    unsigned int size = (wcslen(str) * MB_CUR_MAX + 1);
    char buf[size]; // str has 4 bytes per character. Doubling that should be enough for an UTF-8 string.
    wcstombs(buf, str, size);
    buf[size - 1] = 0;
    syStringHelper::assign(this,(const char*) buf, false);
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
    syStringHelper::assign(dest,copy.m_Str, false);
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

unsigned int syString::size() const {
    return m_Size;
}

unsigned int syString::length() const {
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

const syString syString::operator+(const syString& s) const {
    syString result(*this);
    result += s;
    return result;
}

const syString syString::operator+(const char* str) const {
    syString result(*this);
    result += str;
    return result;
}

const syString syString::operator+(const char c) const {
    syString result(*this);
    result += c;
    return result;
}

const syString syString::operator+(const wchar_t c) const {
    syString result(*this);
    result += c;
    return result;
}

const syString syString::operator+(const wchar_t* str) const {
    syString result(*this);
    result += str;
    return result;
}

syString& syString::operator+=(const syString& s) {
    if(!s.size()) return *this;
    unsigned paramlen = s.size();
    unsigned newsize = m_Size + paramlen;
    if(m_Capacity < m_Size) {
        m_Capacity = m_Size;
    }
    char* newbuf = m_Str;
    if(newsize + 1 > m_Capacity) {
        unsigned int tmpcapacity = 1;
        while(tmpcapacity < newsize + 1) {
            if(tmpcapacity >= 1 << 16) {
                tmpcapacity += 1 << 16;
            } else {
                tmpcapacity <<=1;
            }
        }
        m_Capacity = tmpcapacity;
        newbuf = new char[m_Capacity];
        strncpy(newbuf,m_Str,m_Size);
    }
    strncpy(newbuf + m_Size,s.m_Str, paramlen);
    newbuf[newsize] = 0;

    if(newbuf != m_Str) {
        syStringHelper::reset(this);
        m_Str = newbuf;
    }
    m_Size = newsize;
    return *this;
}

syString& syString::operator+=(const char* str) {
    return operator+=(syString(str));
}

syString& syString::operator+=(const wchar_t* str) {
    return operator+=(syString(str));
}

syString& syString::operator+=(const char c) {
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    return operator+=(syString(buf, true));
}

syString& syString::operator+=(const wchar_t c) {
    wchar_t buf[2];
    buf[0] = c;
    buf[1] = 0;
    return operator+=(syString(buf));
}

syString& syString::append(const syString& s) {
    return operator+=(s);
}

syString& syString::append(const char* str) {
    return operator+=(syString(str));
}

syString& syString::append(const wchar_t* str) {
    return operator+=(syString(str));
}

syString& syString::append(const char c) {
    return operator+=(c);
}

syString& syString::append(const wchar_t c) {
    return operator+=(c);
}

syString& syString::operator<<(const syString& s) {
    return operator+=(s);
}

syString& syString::operator<<(const char* str) {
    return operator+=(syString(str));
}

syString& syString::operator<<(const wchar_t* str) {
    return operator+=(syString(str));
}

syString& syString::operator<<(const char c) {
    return operator+=(c);
}

syString& syString::operator<<(const wchar_t c) {
    return operator+=(c);
}

syString& syString::operator<<(const int input) {
    char buf[64];
    snprintf(buf, 63, "%d", input);
    return operator+=(syString(buf));
}

syString& syString::operator<<(const unsigned int input) {
    char buf[64];
    snprintf(buf, 63, "%u", input);
    return operator+=(syString(buf));
}

syString& syString::operator<<(const long long input) {
    char buf[64];
    snprintf(buf, 63, "%lld", input);
    return operator+=(syString(buf));
}

syString& syString::operator<<(const unsigned long long input) {
    char buf[64];
    snprintf(buf, 63, "%llu", input);
    return operator+=(syString(buf));
}

syString& syString::operator<<(const double input) {
    char buf[256];
    snprintf(buf, 255, "%f", input);
    return operator+=(syString(buf));
}

syString& syString::operator<<(const bool input) {
    if(input) {
        operator+=("1");
    } else {
        operator+=("0");
    }
    return *this;
}

int syString::find(const syString& needle, unsigned int pos) const {
    if(pos >= m_Size) return npos;
    const char* result = strstr(m_Str + pos, needle.m_Str);
    if(!result) {
        return npos;
    }
    return (result + pos - m_Str);
}

int syString::find(char needle, unsigned int pos) const {
    if(pos >= m_Size) return npos;
    const char* result = static_cast<char*>(memchr(m_Str + pos, needle, m_Size - pos));
    if(!result) {
        return npos;
    }
    return (result + pos - m_Str);
}

int syString::rfind(const syString& needle, int pos) const {
    if(needle.empty()) return npos;
    if(needle.size() > m_Size) return npos;
    char c = needle[0];
    unsigned int ns = needle.size();
    if(pos == npos) pos = m_Size - ns;
    while(pos >= 0) {
        pos = rfind(c, pos);
        if(pos < 0) { break; }
        if(strncmp(needle.m_Str,m_Str + pos,ns)== 0) { break; }
    }
    return pos;
}

int syString::rfind(char needle, int pos) const {
    if(pos == npos) pos = m_Size - 1;
    char* ptr = &m_Str[pos];
    while(pos >= 0 && *ptr!=needle) { --pos;--ptr; }
    return pos;
}

const syString syString::substr(unsigned int pos, unsigned int n) const {
    if(pos + n > m_Size) {
        n = m_Size - pos;
    }
    if(!n) {
        return "";
    }
    char* tmpbuf = new char[n + 1];
    strncpy(tmpbuf,m_Str + pos, n);
    tmpbuf[n] = 0;
    syString result(tmpbuf, true);
    result.m_UseRef = false;
    return result;
}

bool syString::operator==(const syString& s) const {
    if(m_Size != s.m_Size) return false;
    if(!m_Size && !s.m_Size) return true;
    unsigned int maxsize = m_Size;
    if(maxsize < s.m_Size) maxsize = s.m_Size;
    return strncmp(c_str(), s.m_Str,syStringHelper::GetMinLength(*this, s)) == 0;
}

bool syString::operator!=(const syString& s) const {
    return !(operator==(s));
}

bool syString::operator<(const syString& s) const {
    if(!m_Size && s.m_Size) return true; // The empty string loses
    if(m_Size && !s.m_Size) return false;
    if(!m_Size && !s.m_Size) return false; // Both are empty, then they're equal.
    return strncmp(m_Str, s.m_Str, syStringHelper::GetMinLength(*this, s)) < 0;
}

bool syString::operator>(const syString& s) const {
    if(!m_Size && s.m_Size) return false;
    if(m_Size && !s.m_Size) return true;
    if(!m_Size && !s.m_Size) return false;
    return strncmp(m_Str, s.m_Str, syStringHelper::GetMinLength(*this, s)) > 0;
}

bool syString::operator<=(const syString& s) const {
    if(!m_Size && s.m_Size) return true;
    if(m_Size && !s.m_Size) return false;
    if(!m_Size && !s.m_Size) return true;
    return strncmp(m_Str, s.m_Str, syStringHelper::GetMinLength(*this, s)) <= 0;
}

bool syString::operator>=(const syString& s) const {
    if(!m_Size && s.m_Size) return false;
    if(m_Size && !s.m_Size) return true;
    if(!m_Size && !s.m_Size) return true;
    return strncmp(m_Str, s.m_Str, syStringHelper::GetMinLength(*this, s)) >= 0;
}

bool syString::operator!() const {
    return !m_Size;
}

const syString& syString::Printf(const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    unsigned long bufsize = 2048; // We have to set a limit. 2K should be enough for most strings
    char* buffer;
    buffer = new char[bufsize + 1];

    // vsnprintf is a version of sprintf that takes a variable number of arguments. Additionally,
    // it allows you to set a limit on the buffer size used for storing the resulting syString.
    // See http://linux.about.com/library/cmd/blcmdl3_vsnprintf.htm

    va_start(arguments, format);
    numchars = vsnprintf(buffer, bufsize, format, arguments);
    va_end(arguments);

    buffer[bufsize] = 0;

    if(numchars < bufsize) {
        buffer[numchars] = 0;
    }
    operator=(buffer);
    delete[] buffer;
    return *this;
}

const syString syString::Format(const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    unsigned long bufsize = 2048; // We have to set a limit. 2K should be enough for most strings
    char* buffer;
    buffer = new char[bufsize + 1];

    // vsnprintf is a version of sprintf that takes a variable number of arguments. Additionally,
    // it allows you to set a limit on the buffer size used for storing the resulting syString.
    // See http://linux.about.com/library/cmd/blcmdl3_vsnprintf.htm

    va_start(arguments, format);
    numchars = vsnprintf(buffer, bufsize, format, arguments);
    va_end(arguments);

    buffer[bufsize] = 0;

    if(numchars < bufsize) {
        buffer[numchars] = 0;
    }
    s = buffer;
    delete[] buffer;
    return s;
}

const syString& syString::PrintfBig(unsigned long bufsize, const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    if(bufsize <= 1) {
        operator=("");
        return *this;
    } else {
        char* buffer = new char[bufsize]; // For big strings
        va_start(arguments, format);
        numchars = vsnprintf(buffer, bufsize - 1, format, arguments);
        va_end(arguments);
        if(numchars < bufsize - 1) {
            buffer[numchars] = 0;
        } else {
            buffer[bufsize - 1] = 0;
        }
        operator=(buffer);
        delete[] buffer;
    }
    return *this;
}


const syString syString::FormatBig(unsigned long bufsize, const char* format, ... ) {
    syString s;
    va_list arguments;
    unsigned int numchars;
    if(bufsize <= 1) {
        return syString("");
    } else {
        char* buffer = new char[bufsize]; // For big strings
        va_start(arguments, format);
        numchars = vsnprintf(buffer, bufsize - 1, format, arguments);
        va_end(arguments);
        if(numchars < bufsize - 1) {
            buffer[numchars] = 0;
        } else {
            buffer[bufsize - 1] = 0;
        }
        s = syString(buffer);
        delete[] buffer;
    }
    return s;
}

// The following syString functions were taken from a personal syString library of mine - Rick.
syString rtrim(const syString& str,const syString& chars) {
  if( chars.empty() || str.empty() ) return str;
  int i;

  for(i=str.length();i>0;i--) {
    if(chars.find(str[i -1],0)==syString::npos) break;
  }
  return str.substr(0,i);
}

syString ltrim(const syString& str,const syString& chars) {
  if( chars.empty() || str.empty() ) return str;
  unsigned int i;

  for(i=0;i<str.length();i++) {
    if(chars.find(str[i],0)==syString::npos) break;
  }
  return str.substr(i,str.length());
}

syString trim(const syString& str,const syString& chars)
{
  return ltrim(rtrim(str,chars),chars);
}
