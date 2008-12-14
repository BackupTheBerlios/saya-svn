/***************************************************************
 * Name:      sayaconfig.cpp
 * Purpose:   Implementation of the syConfig class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "systring.h"
#include "config.h"

syString syConfig::Read(const syString& key, const char* defaultvalue) const {
    return Read(key.c_str(), defaultvalue);
}

unsigned int syConfig::ReadUint(const syString& key, unsigned int defaultvalue) const {
    return ReadUint(key.c_str(), defaultvalue);
}

int syConfig::ReadInt(const syString& key, int defaultvalue) const {
    return ReadInt(key.c_str(), defaultvalue);
}
bool syConfig::ReadBool(const syString& key, bool defaultvalue) const {
    return ReadBool(key.c_str(), defaultvalue);
}
double syConfig::ReadFloat(const syString& key, double defaultvalue) const {
    return ReadFloat(key.c_str(), defaultvalue);
}

bool syConfig::Exists(const syString& key) const {
    return Exists(key.c_str());
}

bool syConfig::Write(const syString& key, const char* value) {
    return Write(key.c_str(), value);
}

bool syConfig::Write(const syString& key, const syString& value) {
    return Write(key.c_str(), value.c_str());
}

bool syConfig::WriteUint(const syString& key, unsigned int value) {
    return WriteUint(key.c_str(), value);
}

bool syConfig::WriteInt(const syString& key, int value) {
    return WriteInt(key.c_str(), value);
}

bool syConfig::WriteBool(const syString& key, bool value) {
    return WriteBool(key.c_str(), value);
}

bool syConfig::WriteFloat(const syString& key, double value) {
    return WriteFloat(key.c_str(), value);
}
