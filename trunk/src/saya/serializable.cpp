/***************************************************************
 * Name:      serializable.cpp
 * Purpose:   Implementation of the serializable class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "serializable.h"
using namespace std;

string serializable::serialize() {
    return "<null />";
}

bool serializable::unserialize(const string& src) {
    return false;
}

std::string serializable::serializestring(const std::string& s) {
    // TODO: Implement serializable::serializestring
    return "";
}

std::string serializable::unserializestring(const std::string& s) {
    // TODO: Implement serializable::unserializestring
    return "";
}

std::string serializable::serializeattribute(const std::string& s) {
    // TODO: Implement serializable::serializeattribute
    return "";
}

std::string serializable::unserializeattribute(const std::string& s) {
    // TODO: Implement serializable::unserializeattribute
    return "";
}

std::string serializable::serializeint(int src) {
    // TODO: Implement serializable::serializeint
    return "";
}

int serializable::unserializeint(const std::string& s) {
    // TODO: Implement serializable::unserializeint
    return 0;
}

std::string serializable::serializeuint(unsigned int src) {
    // TODO: Implement serializable::serializeuint
    return "";
}

unsigned int serializable::unserializeuint(const std::string& s) {
    // TODO: Implement serializable::unserializeuint
    return 0;
}

std::string serializable::serializefloat(double src) {
    // TODO: Implement serializable::serializefloat
    return "";
}

double serializable::unserializefloat(const std::string& s) {
    // TODO: Implement serializable::unserializefloat
    return 0;
}

std::string serializable::serializeboolean(bool src) {
    // TODO: Implement serializable::serializeboolean
    return "";
}

bool serializable::unserializeboolean(const std::string& s) {
    // TODO: Implement serializable::unserializeboolean
    return false;
}
