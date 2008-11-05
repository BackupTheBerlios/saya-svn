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

bool SMapUintUint::unserialize(const string& src) {
    // TODO: Implement SMapUintUint::unserialize
    return false;
}

string SMapUintUint::serialize() {
    string result = "<" + GetTagName() + ">\n";
    map<unsigned int,unsigned int>::iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        result += "<item id=\"" + serializeuint(i->first) + "\" value=\"" + serializeuint(i->second) + "\" />\n";
    }
    result += "</" + GetTagName() + ">\n";
    return result;
}


unsigned int& SMapUintUint::operator[](unsigned int i) {
    return data[i];
}

void SMapUintUint::clear() {
    data.clear();
}

bool SMapUintStr::unserialize(const string& src) {
    // TODO: Implement SMapUintStr::unserialize
    return false;
}

string SMapUintStr::serialize() {
    string result = "<" + GetTagName() + ">\n";
    map<unsigned int,string>::iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        result += "<item id=\"" + serializeuint(i->first) + "\" value=\"" + serializeattribute(i->second) + "\" />\n";
    }
    result += "</" + GetTagName() + ">\n";
    return result;
}

std::string& SMapUintStr::operator[](unsigned int i) {
    return data[i];
}

void SMapUintStr::clear() {
    data.clear();
}

bool SMapIntStr::unserialize(const string& src) {
    // TODO: Implement SMapIntStr::unserialize
    return false;
}

string SMapIntStr::serialize() {
    string result = "<" + GetTagName() + ">\n";
    map<int,string>::iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        result += "<item id=\"" + serializeuint(i->first) + "\" value=\"" + serializeattribute(i->second) + "\" />\n";
    }
    result += "</" + GetTagName() + ">\n";
    return result;
}

std::string& SMapIntStr::operator[](int i) {
    return data[i];
}

void SMapIntStr::clear() {
    data.clear();
}


bool SMapStrStr::unserialize(const string& src) {
    // TODO: Implement SMapIntStr::unserialize
    return false;
}

string SMapStrStr::serialize() {
    string result = "<" + GetTagName() + ">\n";
    map<string,string>::iterator i;
    for(i = data.begin(); i != data.end(); ++i) {
        result += "<item id=\"" + serializeattribute(i->first) + "\" value=\"" + serializeattribute(i->second) + "\" />\n";
    }
    result += "</" + GetTagName() + ">\n";
    return result;
}

std::string& SMapStrStr::operator[](const std::string& s) {
    return data[s];
}

void SMapStrStr::clear() {
    data.clear();
}
