/************************************************************************
 * Name:      serializable.h
 * Purpose:   Declaration for the "serializable" and "serialized" classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************************/

#ifndef serializable_h
#define serializable_h

class serialized;

class serializable {
    public:
        serializable() {}

        /** @brief Gets the tag name for (de)serialization.
         *  @note To implement this correctly, you must point to a static const char*, defined inside your cpp file.
         */
        virtual const char* GetTagName() const;

        /** @brief Loads serialized data into the object.
         *  @param data The data to unserialize into the object
         *  @return true if successful, false otherwise. Defaults to false.
         */
        virtual bool unserialize(const char* src);

        /** @brief Compresses the object data into a serialized string format.
         *  @param data The data to unserialize into the object
         *  @return The serialized data
         */
        virtual void serialize(serialized& dest) const;

        /** @brief Finds a named item in the current level of the given serialized data.
         *  @param name The name of the item to look for.
         *  @param src a pointer to the string to be searched.
         *  @return a pointer to the item's serialized data if found; null otherwise.
         */
        static const char* find_item(const char* name,const char* itemtype, const char* src);

        /** Serializes a processed string to the output */
        static void serializeprocessedstring(serialized& dest, const char* s);

        /** Preprocesses a string for serialization. */
        static void escape_string(serialized& dest, const char* s);

        /** Postprocesses a string after unserialization. */
        static void unescape_string(serialized& dest, const char* s);

        /** Serializes a string into an unserializable string format */
        static void serializestring(serialized& dest, const char* name, const char* src);

        /** Serializes an integer */
        static void serializeint(serialized& dest, const char* name, int src);

        /** Serializes an unsigned integer */
        static void serializeuint(serialized& dest, const char* name, unsigned int src);

        /** Serializes a 64-bit integer */
        static void serializeint64(serialized& dest,const char* name, long long src);

        /** Serializes an unsigned 64-bit integer */
        static void serializeuint64(serialized& dest,const char* name, unsigned long long src);

        /** Serializes a floating point number */
        static void serializefloat(serialized& dest, const char* name, double src);

        /** Serializes a boolean */
        static void serializeboolean(serialized& dest, const char* name, bool src);

        /** Serializes a serializable, with the given name. */
        static void serialize_object(serialized& dest, const char* name, const serializable* src);

        /** Serializes a string. */
        static void serialize_object(serialized& dest, const char* name, const char* src);

        /** Serializes an unsigned integer. */
        static void serialize_object(serialized& dest, const char* name, const unsigned int* src);

        /** Serializes a signed integer. */
        static void serialize_object(serialized& dest, const char* name, const int* src);

        /** Serializes a 64-bit integer */
        static void serialize_object(serialized& dest,const char* name, const long long *src);

        /** Serializes an unsigned 64-bit integer */
        static void serialize_object(serialized& dest,const char* name, const unsigned long long *src);

        /** Serializes a floating point number */
        static void serialize_object(serialized& dest, const char* name, const double *src);

        /** Serializes a boolean */
        static void serialize_object(serialized& dest, const char* name, const bool *src);

        /** Unserializes a string */
        static bool unserializestring(serialized& dest, const char* name, const char* src, const char* defaultvalue = "");

        /** Unserializes an integer */
        static bool unserializeint(int& dest, const char* name, const char* src,int defaultvalue = 0);

        /** Unserializes an unsigned integer */
        static bool unserializeuint(unsigned int& dest, const char* name, const char* src,unsigned int defaultvalue = 0);

        /** Unserializes a 64-bit integer */
        static bool unserializeint64(long long& dest, const char* name, const char* src,long long defaultvalue = 0);

        /** Unserializes an unsigned 64-bit integer */
        static bool unserializeuint64(unsigned long long& dest, const char* name, const char* src,unsigned long long defaultvalue = 0);

        /** Unserializes a floating point number */
        static bool unserializefloat(double& dest, const char* name, const char* src, double defaultvalue = 0.0);

        /** Unserializes a boolean */
        static bool unserializeboolean(bool& dest, const char* name, const char* src, bool defaultvalue = false);

        /** Unserializes a serializable. The destination object must already have been created. */
        static bool unserialize_object(serializable* obj, const char* name, const char* src);

        virtual ~serializable() {}
    private:

        /** @brief Opens a serialization tag for the current object.
         *  @param dest the destination object.
         *  @param datalen The final length of the serialized data.
         */
        void serialize_opentag(serialized& dest,unsigned int datalen) const;

        /** Closes a serialization tag. */
        void serialize_closetag(serialized& dest) const;

};

#endif
