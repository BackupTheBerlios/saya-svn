/***************************************************************
 * Name:      svector.h
 * Purpose:   Declaration for Serializable Vector classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef svector_h
#define svector_h

#include "serializable.h"
#include <vector>

template <class T> class SVector : public serializable {

    public:

        /** standard constructor */
        SVector() {}

        /** standard destructor */
        virtual ~SVector() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src) {
            // TODO: Implement SVector::unserialize
            return false;
        }

        virtual std::string GetTagName() { return "vector"; }

        /** @see serializable::serialize */
        virtual std::string serialize() {
            std::string result = "<" + GetTagName() + ">";
            unsigned int i;
            for(i = 0; i < data.size(); ++i) {
                result += "<item id=\"" + serializeuint(i) + "\">";
                result += data[i].serialize() + "</item>";
            }
            result += "</" + GetTagName() + ">";
            return result;
        }

        T& operator[](unsigned int i) {
            return data[i];
        }

        void clear() {
            data.clear();
        };

        std::vector<T> data;
};

class SStringVector : public serializable {

    public:

        /** standard constructor */
        SStringVector() {}

        /** standard destructor */
        virtual ~SStringVector() {}

        virtual std::string GetTagName();

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        std::string& operator[](unsigned int i) {
            return data[i];
        }

        void clear() {
            data.clear();
        };

        std::vector<std::string> data;
};

#endif
