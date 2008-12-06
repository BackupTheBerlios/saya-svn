/***************************************************************
 * Name:      systring.h
 * Purpose:   Declaration for the syString class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#ifndef systring_h
#define systring_h
class syStringHelper;
class syString {
    /** @brief A not-so simple wrapper for a const char* (it's been growing).
     *  syString allows passing strings as return values on the stack without having to include
     *  the C++ syString headers. It also allows to use of simple strings for use as keys in
     *  maps.
     */

    friend class syStringHelper;
    public:
        /** Standard constructor. */
        syString();

        /** @brief const-char constructor.
         *  @param str the string to use as data for our "container".
         *  @param useref Tells the constructor whether to just copy the pointer (true), or copy the data (false).
         *         Default false.
         */
        syString(const char* str,bool useref = false);

        /** Copy Constructor. */
        syString(const syString& copy);

        /** Standard destructor. */
        ~syString();

        /** Sets the value to an empty string. */
        void clear();

        /** Returns true if the string is empty. */
        bool empty() const;

        /** Returns the stored string */
        const char* c_str() const;

        /** Compares the string with another. Used for std::maps. */
        bool compare(const syString& s) const;

        /** Comparison operator. */
        bool operator==(const syString& s) const;

        /** Comparison operator. */
        bool operator!=(const syString& s) const;

        /** Assignment operator (object version). */
        syString& operator=(const syString& copy);

        /** Assignment operator (const-char* version). */
        syString& operator=(const char* str);

        /** Concatenation unary operator */
        syString& operator+=(const syString& s);

        /** Concatenation binary operator */
        const syString operator+(const syString& s) const;

        /** Returns the character from a given index. */
        char operator[](unsigned int i) const;

        /** Returns the character from a given index. */
        char& operator[](unsigned int i);

        /** Returns the currently stored string's size. */
        unsigned int size() const;

        unsigned int capacity() const;
    private:
        unsigned int m_Size;
        mutable unsigned int m_Capacity;
        bool m_UseRef;
        char* m_Str;
};

struct ltsystr
{
  bool operator()(const syString& s1, const syString& s2) const
  {
    return s1.compare(s2);
  }
};

#endif
