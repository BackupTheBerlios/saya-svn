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
class syString;

static const char* svector_name = "vector";

template <class T> class SVector : public serializable {

    public:

        /** standard constructor */
        SVector() {}

        /** standard destructor */
        virtual ~SVector() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src) {
            // TODO: Implement SVector::unserialize
            return false;
        }

        virtual const char* GetTagName() const { return svector_name; }

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) {
            unsigned int i;
            for(i = 0; i < data.size(); ++i) {
                serialize_object(dest,"#",&(data[i]));
            }
        }

        T& operator[](unsigned int i) {
            return data[i];
        }

        unsigned int size() {
            return data.size();
        }

        void clear() {
            data.clear();
        };

        std::vector<T> data;
};

#endif
