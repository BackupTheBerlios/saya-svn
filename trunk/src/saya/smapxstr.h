/****************************************************************
 * Name:      smapxstr.h
 * Purpose:   Declaration for std::map-based string serializables
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#ifndef smapxstr_h
#define smapxstr_h

#include "serializable.h"
#include "serialized.h"
#include "core/systring.h"
#include <map>

class SMapUintStr : public serializable {
    public:
        /** standard constructor */
        SMapUintStr() {}

        /** standard destructor */
        virtual ~SMapUintStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        virtual const char* GetTagName() const;

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        syString& operator[](unsigned int i);

        void clear();

        std::map<unsigned int,syString> data;
};

class SMapIntStr : public serializable {
    public:

        /** standard constructor */
        SMapIntStr() {}

        /** standard destructor */
        virtual ~SMapIntStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        virtual const char* GetTagName() const;

        syString& operator[](int i);

        void clear();

        std::map<int,syString> data;
};

class SMapStrStr : public serializable {
    public:

        /** standard constructor */
        SMapStrStr() {}

        /** standard destructor */
        virtual ~SMapStrStr() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        virtual const char* GetTagName() const;

        syString& operator[](const char* s);

        void clear();

        std::map<syString,syString,ltsystr> data;
};

#endif
