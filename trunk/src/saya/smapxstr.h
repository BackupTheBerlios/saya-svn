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

class SMapUintStrData;
class SMapIntStrData;
class SMapStrStrData;
class SMapUintUintData;


class SMapUintStr : public serializable {
    public:
        /** standard constructor */
        SMapUintStr();

        /** standard destructor */
        virtual ~SMapUintStr();

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        virtual const char* GetTagName() const;

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        syString& operator[](unsigned int i);

        void clear();

    private:
        SMapUintStrData* m_Data;
};

class SMapIntStr : public serializable {
    public:

        /** standard constructor */
        SMapIntStr();

        /** standard destructor */
        virtual ~SMapIntStr();

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        virtual const char* GetTagName() const;

        syString& operator[](int i);

        void clear();

    private:
        SMapIntStrData* m_Data;
};

class SMapStrStr : public serializable {
    public:

        /** standard constructor */
        SMapStrStr();

        /** standard destructor */
        virtual ~SMapStrStr();

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        virtual const char* GetTagName() const;

        syString& operator[](const char* s);

        /** @return a pointer to the string corresponding to the key s, if found; NULL otherwise. */
        const syString* find(const char* s);

        /** @return 1 if the string is found in the map, 0 otherwise */
        int count(const char* s);

        void clear();

    private:
        SMapStrStrData* m_Data;
};

class SMapUintUint : public serializable {
    public:

        /** standard constructor */
        SMapUintUint();

        /** standard destructor */
        virtual ~SMapUintUint();

        virtual const char* GetTagName() const;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        unsigned int& operator[](unsigned int i);

        void clear();

    private:
        SMapUintUintData* m_Data;
};

#endif
