/**************************************************************************
 * Name:      serializable.cpp
 * Purpose:   Implementation of the "serializable" and "serialized" classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************************/

#include "serializable.h"
#include "serialized.h"
#include <cstddef>

void serializable::serialize(serialized& dest) const {
    dest << "null"; // Defaults to a null value
}

const char* serializable::GetTagName() const {
    static const char* serializable_null = "null";
    return serializable_null;
}

bool serializable::unserialize(const char* src) {
    return false; // Defaults to error
}

// ---------------------------
// begin serializing functions
// ---------------------------

void serializable::escape_string(serialized& dest, const char* s) {
//TODO: Implement serializable::escape_string
}

void serializable::unescape_string(serialized& dest, const char* s) {
//TODO: Implement serializable::unescape_string
}

void serializable::serialize_opentag(serialized& dest,unsigned int datalen) const {
    serialized opentag;
    escape_string(opentag,GetTagName());
    dest << opentag << '[' << datalen << "]{";
}

void serializable::serialize_closetag(serialized& dest) const {
    dest << '}';
}

void serializable::serialize_object(serialized& dest, const char* name, const serializable* obj) {
    serialized key;
    escape_string(key,name);
    dest << key << ":o"; // o stands for "object". The true key will be "o" + the object's tagname.
    if(!obj) {
        dest << "null";
    } else {
        serialized tmp;
        obj->serialize(tmp);
        obj->serialize_opentag(dest,tmp.size());
        dest << tmp;
        obj->serialize_closetag(dest);
    }
    dest << "\n";
}

void serializable::serializeprocessedstring(serialized& dest, const char* s) {
    dest << s;
}

void serializable::serializestring(serialized& dest, const char* name, const char* src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "s{";
    tmp.Clear();
    escape_string(tmp, src);
    dest << tmp << "}\n";
}

void serializable::serializeint(serialized& dest, const char* name, int src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "i{" << src << "}\n";
}

void serializable::serializeuint(serialized& dest, const char* name, unsigned int src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "u{" << src << "}\n";
}

void serializable::serializeint64(serialized& dest,const char* name, long long src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "i8{" << src << "}\n";
}

void serializable::serializeuint64(serialized& dest,const char* name, unsigned long long src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "u8{" << src << "}\n";
}

void serializable::serializefloat(serialized& dest, const char* name, double src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "f{" << src << "}\n";
}

void serializable::serializeboolean(serialized& dest, const char* name, bool src) {
    serialized tmp;
    escape_string(tmp,name);
    dest << tmp << ':' << "b{" << (src ? '1' : '0') << "}\n";
}

// -------------------------
// end serializing functions
// -------------------------


// -----------------------------
// begin unserializing functions
// -----------------------------

const char* serializable::find_item(const char* name,const char* itemtype, const char* src) {
    return NULL;
}

bool serializable::unserializestring(serialized& dest, const char* name, const char* src, const char* defaultvalue) {
    dest.Clear();
    dest << defaultvalue;
    const char* item = find_item(name,"s",src);
    if(!item) {
        return false;
    }
    dest.Clear();
    // TODO: Implement serializable::unserializestring
    return true;
}

bool serializable::unserializeint(int& dest, const char* name, const char* src,int defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"i",src);
    if(!item) {
        return false;
    }
    // TODO: Implement serializable::unserializeint
    return true;
}

bool serializable::unserializeuint(unsigned int& dest, const char* name, const char* src,unsigned int defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"u",src);
    if(!item) {
        return false;
    }
    // TODO: Implement serializable::unserializeuint
    return true;
}

bool serializable::unserializeint64(long long& dest, const char* name, const char* src,long long defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"i8",src);
    if(!item) {
        return false;
    }
    // TODO: Implement serializable::unserializeint64
    return true;
}

bool serializable::unserializeuint64(unsigned long long& dest, const char* name, const char* src,unsigned long long defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"u8",src);
    if(!item) {
        return false;
    }
    // TODO: Implement serializable::unserializeuint64
    return true;
}

bool serializable::unserializefloat(double& dest, const char* name, const char* src, double defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"f",src);
    if(!item) {
        return false;
    }
    // TODO: Implement serializable::unserializefloat
    return true;
}

bool serializable::unserializeboolean(bool& dest, const char* name, const char* src, bool defaultvalue) {
    dest = defaultvalue;
    const char* item = find_item(name,"b",src);
    if(!item) {
        return false;
    }
    dest = (*item != '0');
    return true;
}

bool serializable::unserialize_object(serializable* obj, const char* name, const char* src) {
    serialized itemtype;
    itemtype << 'o' << obj->GetTagName();
    const char* item = find_item(name,itemtype.c_str(),src);
    if(!item) {
        return false;
    }
    return obj->unserialize(item);
}
// ---------------------------
// end unserializing functions
// ---------------------------
