/***************************************************************
 * Name:      serializable.h
 * Purpose:   Declaration for the serializable class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef serializable_h
#define serializable_h

#include <string>

class serializable {
    public:
        serializable() {}

        /** Loads serialized data into the object.
         *  @param data The data to unserialize into the object
         *  @return the result. true if successful.
         */
        virtual bool unserialize(const std::string& src);

        /** Compresses the object data into a serialized string format.
         *  @param data The data to unserialize into the object
         *  @return The serialized data
         */
        virtual std::string serialize();

        /** Gets the tag name for (de)serialization */
        virtual std::string GetTagName() { return "null"; }

        /** Serializes a string into an xml-compatible string */
        static std::string serializestring(const std::string& s);

        /** Unserializes an xml-compatible string */
        static std::string unserializestring(const std::string& s);

        /** Serializes a string into an xml-compatible string attribute */
        static std::string serializeattribute(const std::string& s);

        /** Unserializes an xml-compatible string attribute */
        static std::string unserializeattribute(const std::string& s);

        /** Serializes an integer */
        static std::string serializeint(int src);

        /** Unserializes an integer */
        static int unserializeint(const std::string& s);

        /** Serializes an unsigned integer */
        static std::string serializeuint(unsigned int src);

        /** Unserializes an unsigned integer */
        static unsigned int unserializeuint(const std::string& s);

        /** Serializes a floating point number */
        static std::string serializefloat(double src);

        /** Unserializes a floating point number */
        static double unserializefloat(const std::string& s);

        /** Serializes a boolean */
        static std::string serializeboolean(bool src);

        /** Unserializes a boolean */
        static bool unserializeboolean(const std::string& s);

        virtual ~serializable() {}
};

#endif
