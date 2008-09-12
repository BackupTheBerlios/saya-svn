/****************************************************************
 * Name:      propertymap.h
 * Purpose:   Declaration for the PropertyMap and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#ifndef propertymap_h
#define propertymap_h

#include "serializable.h"

#include <string>
#include <vector>
#include <map>

class PropertyMap : public serializable {
    public:
        /** Standard constructor. */
        PropertyMap() {}

        /** Standard destructor. */
        virtual ~PropertyMap() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        /** Copies its data from another property map */
        void operator=(const PropertyMap& src);

        std::map<std::string,std::string> data;
};

class SerializableVector : public serializable {

    public:
        /** Standard constructor. */
        SerializableVector() {}

        /** Standard destructor. */
        virtual ~SerializableVector() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        void operator=(const SerializableVector& src);

        std::vector<std::string> data;
};

class SerializableIntMap : public serializable {

    public:
        /** Standard constructor. */
        SerializableIntMap() {}

        /** Standard destructor. */
        virtual ~SerializableIntMap() {}

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        void operator=(const SerializableIntMap& src);

        std::map<int,std::string> data;
};

class SerializableUIntMap : public serializable {

    public:

        /** Standard constructor. */
        SerializableUIntMap();

        /** Standard destructor. */
        virtual ~SerializableUIntMap();

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        void operator=(const SerializableUIntMap& src);

        std::map<unsigned int,std::string> data;
};

#endif
