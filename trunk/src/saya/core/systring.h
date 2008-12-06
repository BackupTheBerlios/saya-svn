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

        /** Concatenation a-la stream */
        syString& operator<<(const syString& s);

        /** Concatenation a-la stream */
        syString& operator<<(const char* str);

        /** Concatenation a-la stream */
        syString& append(const syString& s);

        /** Concatenation a-la stream */
        syString& append(const char* str);

        /** Returns the character from a given index. */
        char operator[](unsigned int i) const;

        /** Returns the character from a given index. */
        char& operator[](unsigned int i);

        /** Returns the currently stored string's size. */
        unsigned int size() const;

        /** Returns the currently stored string's size. */
        unsigned int length() const;

        /** Returns the current buffer's capacity. */
        unsigned int capacity() const;

        /** @brief Returns the position of string <i>needle</i>  in the string.
         *  Returns syString::npos (-1) if <i>needle</i> was not found.
         *  @param needle The string to find.
         *  @param pos The initial position to search (zero-based).
         */
        int find(const syString& needle,unsigned int pos = 0) const;

        /** @brief Returns the position of string <i>needle</i>  in the string.
         *  Returns syString::npos (-1) if <i>needle</i> was not found.
         *  @param needle The string to find.
         *  @param pos The initial position to search (zero-based).
         */
        int find(char needle,unsigned int pos  = 0) const;

        /** @brief Returns the position of string <i>needle</i>  in the string.
         *  Returns syString::npos (-1) if <i>needle</i> was not found.
         *  @param needle The string to find.
         *  @param pos The initial position to search backwards (zero-based).
         *         npos means the entire string is searched.
         */
        int rfind(const syString& needle,int pos = npos) const;

        /** @brief Returns the position of string <i>needle</i>  in the string.
         *  Returns syString::npos (-1) if <i>needle</i> was not found.
         *  @param needle The string to find.
         *  @param pos The initial position to search backwards (zero-based).
         *         npos means the entire string is searched.
         */
        int rfind(char needle, int pos  = npos) const;

        /** @brief Obtains the substring located at pos, with length at most n. */
        const syString substr(unsigned int pos = 0, unsigned int n = npos) const;

        static const int npos;
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

syString rtrim(const syString& str, const syString& chars = " \r\n\t");
syString ltrim(const syString& str, const syString& chars = " \r\n\t");
syString trim(const syString& str, const syString& chars = " \r\n\t");



#endif
