/***************************************************************
 * Name:      smap_uint.h
 * Purpose:   Unsigned-indexed Map of Serializable classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef svector_h
#define svector_h

#include "serializable.h"
#include <map>

template <class T> class SMapUint : public serializable {
    public:

        /** standard constructor */
        SMapUint() {}

        /** standard destructor */
        virtual ~SMapUint() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src) {
            // TODO: Implement SVector::unserialize
            return false;
        }

        virtual std::string GetTagName() { return "vector"; }

        /** @see serializable::serialize */
        virtual std::string serialize() {
            std::string result = "<map_uint>";
            std::map::iterator i;
            unsigned int i;
            for(i = data.begin(); i != data.end(); ++i) {
                result += "<item id=\"" + serializeuint(i->first) + "\">";
                result += i->second.serialize() + "</item>";
            }
            result += "</" + GetTagName() + ">";
            return result;
        }

        std::map<unsigned int,T> data;
};

class SerializableUIntMap : public serializable {
    public:

        /** standard constructor */
        SMapUint() {}

        /** standard destructor */
        virtual ~SMapUint() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src) {
            // TODO: Implement SVector::unserialize
            return false;
        }

        virtual std::string GetTagName() { return "vector"; }

        /** @see serializable::serialize */
        virtual std::string serialize() {
            std::string result = "<map_uint>";
            std::map::iterator i;
            unsigned int i;
            for(i = data.begin(); i != data.end(); ++i) {
                result += "<item id=\"" + serializeuint(i->first) + "\">";
                result += i->second.serialize() + "</item>";
            }
            result += "</" + GetTagName() + ">";
            return result;
        }

        std::map<unsigned int,T> data;
};

#endif
