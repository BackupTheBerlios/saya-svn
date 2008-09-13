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
#include <map>

template <class T> class SMapUint : public serializable {
    public:

        typedef typename std::map<unsigned int,T>::iterator SMapUintIt;
        /** standard constructor */
        SMapUint() {}

        /** standard destructor */
        virtual ~SMapUint() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src) {
            // TODO: Implement SMapUint::unserialize
            return false;
        }

        virtual std::string GetTagName() { return "SMapUint"; }

        /** @see serializable::serialize */
        virtual std::string serialize() {
            std::string result = "<" + GetTagName() + ">";
            SMapUintIt i;
            for(i = data.begin(); i != data.end(); ++i) {
                result += "<item id=\"" + serializeuint(i->first) + "\">";
                result += i->second.serialize() + "</item>";
            }
            result += "</" + GetTagName() + ">";
            return result;
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

        typedef typename std::map<std::string,T>::iterator SMapStrIt;
        /** standard constructor */
        SMapStr() {}

        /** standard destructor */
        virtual ~SMapStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src) {
            // TODO: Implement SMapStr::unserialize
            return false;
        }

        virtual std::string GetTagName() { return "SMapStr"; }

        /** @see serializable::serialize */
        virtual std::string serialize() {
            std::string result = "<" + GetTagName() + ">";
            SMapStrIt i;
            for(i = data.begin(); i != data.end(); ++i) {
                result += "<item id=\"" + serializeattribute(i->first) + "\">";
                result += i->second.serialize() + "</item>";
            }
            result += "</" + GetTagName() + ">";
            return result;
        }

        T& operator[](const std::string& s) {
            return data[s];
        }

        void clear() {
            data.clear();
        }

        std::map<std::string,T> data;
};

class SMapUintUint : public serializable {
    public:

        /** standard constructor */
        SMapUintUint() {}

        /** standard destructor */
        virtual ~SMapUintUint() {}

        virtual std::string GetTagName() { return "SMapUintUint"; }

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        unsigned int& operator[](unsigned int i);

        void clear();

        std::map<unsigned int,unsigned int> data;
};

class SMapUintStr : public serializable {
    public:

        /** standard constructor */
        SMapUintStr() {}

        /** standard destructor */
        virtual ~SMapUintStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        virtual std::string GetTagName() { return "SMapUintStr"; }

        /** @see serializable::serialize */
        virtual std::string serialize();

        std::string& operator[](unsigned int i);

        void clear();

        std::map<unsigned int,std::string> data;
};

class SMapIntStr : public serializable {
    public:

        /** standard constructor */
        SMapIntStr() {}

        /** standard destructor */
        virtual ~SMapIntStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        virtual std::string GetTagName() { return "SMapIntStr"; }

        std::string& operator[](int i);

        void clear();

        std::map<int,std::string> data;
};

class SMapStrStr : public serializable {
    public:

        /** standard constructor */
        SMapStrStr() {}

        /** standard destructor */
        virtual ~SMapStrStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        virtual std::string GetTagName() { return "SMapStrStr"; }

        std::string& operator[](const std::string& s);

        void clear();

        std::map<std::string,std::string> data;
};

#endif
