/************************************************************
 * Name:      serialized.cpp
 * Purpose:   Implementation of the "serialized" class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-29
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#include "serialized.h"
#include "../core/systring.h"

// --------------------------
// begin serialized_innerdata
// --------------------------

class serialized_innerdata {
    public:
        serialized_innerdata(serialized* parent);
        ~serialized_innerdata();
        syString m_String;

    private:
        serialized* m_Parent;
};

serialized_innerdata::serialized_innerdata(serialized* parent) : m_Parent(parent) {}
serialized_innerdata::~serialized_innerdata() {}

// ------------------------
// end serialized_innerdata
// ------------------------

// ----------------
// begin serialized
// ----------------
serialized::serialized() {
    m_Data = new serialized_innerdata(this);
}

serialized::serialized(const char* input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(int input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(unsigned int input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(long long input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(unsigned long long input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(double input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized(bool input) {
    m_Data = new serialized_innerdata(this);
    *this << input;
}

serialized::serialized::~serialized() {
    delete m_Data;
}

serialized& serialized::operator<<(const serializable_basic* input) {
    input->serialize(*this);
    return *this;
}

/** Concatenates a "serialized" object. */
serialized& serialized::operator<<(const serialized& input) {
    m_Data->m_String << input.m_Data->m_String.c_str();
    return *this;
}

serialized& serialized::operator<<(const char* input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(int input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(unsigned int input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(long long input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(unsigned long long input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(double input) {
    m_Data->m_String << input;
    return *this;
}

serialized& serialized::operator<<(bool input) {
    m_Data->m_String << (input ? "1" : "0");
    return *this;
}

unsigned int serialized::size() const {
    return m_Data->m_String.size();
}

const char* serialized::c_str() const {
    return m_Data->m_String.c_str();
}

void serialized::Clear() {
    m_Data->m_String.clear();
}

// --------------
// end serialized
// --------------
