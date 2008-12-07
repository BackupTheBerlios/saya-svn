/************************************************************
 * Name:      serialized.h
 * Purpose:   Declaration for the "serialized" class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#ifndef serialized_h
#define serialized_h

class serialized;
class serializable;
class serialized_innerdata;

/** @brief the simplest of all serializable classes.
 *  @note You must not derive from this class; derive from "serializable" instead.
 *  @see serializable
 */
class serializable_basic {
    public:
        /** standard constructor. */
        serializable_basic() {}

        /** standard destructor. */
        virtual ~serializable_basic() {}

        /** @brief serializes itself to a serialized object.
         *  @note This is a pure virtual method.
         */
        virtual void serialize(serialized& dest) const = 0;
};

/** Class serialized was originally a wrapper for ostringstream. Its only purpose was to avoid the
 *  inclusion of the std header files. Now ostringstream has been replaced with syStrng, and there
 *  are plans to include JSON deserialization in this class.
 */
class serialized {
    friend class serialized_innerdata;
    friend class serializable;
    public:
        /** Basic constructor */
        serialized();

        /** Constructor initialized with a string. */
        serialized(const char* input);
        /** Constructor initialized with an integer. */
        serialized(int input);
        /** Constructor initialized with an unsigned integer. */
        serialized(unsigned int input);
        /** Constructor initialized with a 64-bit integer. */
        serialized(long long input);
        /** Constructor initialized with an unsigned 64-bit integer.*/
        serialized(unsigned long long input);
        /** Constructor initialized with a float. */
        serialized(double input);
        /** Constructor initialized with a boolean. */
        serialized(bool input);

        /** Basic destructor */
        ~serialized();

        /** Operator << will take care of string concatenation. Notice that the input is a pointer. */
        serialized& operator<<(const serializable_basic* input);

        /** Concatenates a "serialized" object. */
        serialized& operator<<(const serialized& input);

        /** Concatenates a string. */
        serialized& operator<<(const char* input);

        /** Concatenates a signed integer input. */
        serialized& operator<<(int input);

        /** Concatenates an unsigned integer input. */
        serialized& operator<<(unsigned int input);

        /** Concatenates a 64-bit integer */
        serialized& operator<<(long long input);

        /** Concatenates an unsigned 64-bit integer */
        serialized& operator<<(unsigned long long input);

        /** Concatenates a floating point input. */
        serialized& operator<<(double input);

        /** Concatenates a boolean. */
        serialized& operator<<(bool input);

        /** Returns the current serialized data. */
        const char* c_str() const;

        /** Returns the length of the current serialized data. */
        unsigned int size() const;

        /** Clears the current data. */
        void Clear();

    private:
        serialized_innerdata* m_Data;
};

#endif
