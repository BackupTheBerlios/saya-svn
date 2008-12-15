/*****************************************************************
 * Name:      sstrvector.h
 * Purpose:   Declaration for the Serializable String Vector class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-12-06
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ****************************************************************/

#ifndef sstrvector_h
#define sstrvector_h

#include "serializable.h"
class syString;

class SStringVectorData;

class SStringVector : public serializable {

    public:

        /** standard constructor */
        SStringVector();

        /** standard destructor */
        virtual ~SStringVector();

        virtual const char* GetTagName() const;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;

        syString& operator[](unsigned int i);

        const syString& operator[](unsigned int i) const;

        void push_back(const syString& s);

        unsigned int size();

        void clear();

    private:
        SStringVectorData* m_Data;
};

#endif
