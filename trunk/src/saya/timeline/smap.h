/***************************************************************
 * Name:      smap.h
 * Purpose:   Declaration for std::map-based serializables
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef smap_h
#define smap_h

#include "serializable.h"
#include "serialized.h"
#include "../core/systring.h"
#include <map>

extern const char* tag_smapuint;
extern const char* tag_smapstr;

template <class T> class SMapUint : public serializable {
    public:

        typedef typename std::map<unsigned int,T>::const_iterator SMapUintIt;
        /** standard constructor */
        SMapUint() {}

        /** standard destructor */
        virtual ~SMapUint() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src) {
            // TODO: Implement SMapUint::unserialize
            return false;
        }

        virtual const char* GetTagName() const { return tag_smapuint; }

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const {
            SMapUintIt i;
            for(i = data.begin(); i != data.end(); ++i) {
                serialized idx(i->first); // Convert the integer to a string
                serialize_object(dest, idx.c_str(), &(i->second));
            }
        }

        T& operator[](unsigned int i) {
            return data[i];
        }

        void clear() {
            data.clear();
        }

        std::map<unsigned int,T> data;
};

template <class T> class SMapStr : public serializable {
    public:

        typedef typename std::map<syString,T, ltsystr>::const_iterator SMapStrIt;
        /** standard constructor */
        SMapStr() {}

        /** standard destructor */
        virtual ~SMapStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src) {
            // TODO: Implement SMapStr::unserialize
            return false;
        }

        virtual const char* GetTagName() const { return tag_smapstr; }

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const {
            SMapStrIt i;
            for(i = data.begin(); i != data.end(); ++i) {
                serialized idx;
                serialize_object(dest, i->first.c_str(), &(i->second));
            }
        }

        T& operator[](const char* s) {
            return data[syString(s,true)];
        }

        void clear() {
            data.clear();
        }

        std::map<syString,T,ltsystr> data;
};


#endif
