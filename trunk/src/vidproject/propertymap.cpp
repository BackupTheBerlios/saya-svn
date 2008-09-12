/******************************************************************
 * Name:      propertymap.cpp
 * Purpose:   Implementation of the PropertyMap and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 *****************************************************************/

#include "propertymap.h"
using namespace std;

bool PropertyMap::unserialize(const string& src) {
    // TODO: Implement PropertyMap::unserialize
    return false;
}

string PropertyMap::serialize() {
    string result;
    map<string,string>::iterator iter;
    for(iter = data.begin();iter != data.end(); iter++) {
        result += string("<item name=\"") + serializeattribute(iter->first) +
                  "\">" + serializeattribute(iter->second) + "</item>";
    }
    return result;
}

SerializableUIntMap::SerializableUIntMap() {
}

SerializableUIntMap::~SerializableUIntMap() {
}

bool SerializableUIntMap::unserialize(const std::string& src) {
    //TODO: Implement SerializableUIntMap::unserialize
    return false;
}

std::string SerializableUIntMap::serialize() {
    //TODO: Implement SerializableUIntMap::unserialize
    return "";
}

void SerializableUIntMap::operator=(const SerializableUIntMap& src) {
}

