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

class syString;
#ifdef SY_WXSTRING_COMPATIBILITY
class wxString;
#endif

/** @brief A not-so simple wrapper for a const char* (it's been growing).
 *  syString allows passing strings as return values on the stack without having to include
 *  the C++ syString headers. It also allows to use of simple strings for use as keys in
 *  maps.
 */
class syString {
    public:
        /** Standard constructor. */
        syString();

        /** const-char constructor and implicit converter. */
        syString(const char* str);

        /** @brief const-char constructor with optional pass-by-reference flag.
         *  @param str the string to use as data for our "container".
         *  @param useref Tells the constructor whether to just copy the pointer (true), or copy the data (false).
         *         Default false.
         */
        syString(const char* str,bool useref);

        /** Constructs an UTF-8 string based on a wide character string. */
        syString(const wchar_t* str);

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

        /** Returns the character from a given index. */
        const char operator[](unsigned int i) const;

        /** Returns the character from a given index. */
        char& operator[](unsigned int i);

        /** Returns the currently stored string's size. */
        unsigned int size() const;

        /** Returns the currently stored string's size. */
        unsigned int length() const;

        /** Returns the current buffer's capacity. */
        unsigned int capacity() const;

        /** Reallocates the string with a larger capacity. */
        void reserve(unsigned int n);

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

        /** Comparison operator. */
        bool operator==(const syString& s) const;

        /** Comparison operator. */
        bool operator!=(const syString& s) const;

        /** Comparison operator. */
        bool operator<(const syString& s) const;

        /** Comparison operator. */
        bool operator>(const syString& s) const;

        /** Comparison operator. */
        bool operator<=(const syString& s) const;

        /** Comparison operator. */
        bool operator>=(const syString& s) const;

        /** Returns true if the string is empty. */
        bool operator!() const;

        /** Assignment operator. */
        syString& operator=(const syString& copy);

        /** Assignment operator. */
        syString& operator=(const char* str);

        /** Assignment operator. */
        syString& operator=(const wchar_t* str);

        /** Assignment operator. */
        syString& operator=(const char c);

        /** Assignment operator. */
        syString& operator=(const wchar_t c);

        /** Concatenation binary operator. */
        const syString operator+(const syString& s) const;

        /** Concatenation binary operator. */
        const syString operator+(const char* str) const;

        /** Concatenation binary operator (wide-character version). */
        const syString operator+(const wchar_t* str) const;

        /** Concatenation binary operator. */
        const syString operator+(const char c) const;

        /** Concatenation binary operator. */
        const syString operator+(const wchar_t c) const;

        /** Concatenation unary operator. */
        syString& operator+=(const syString& s);

        /** Concatenation unary operator. */
        syString& operator+=(const char* str);

        /** Concatenation unary operator (wide-character version). */
        syString& operator+=(const wchar_t* str);

        /** Concatenation unary operator. */
        syString& operator+=(const char c);

        /** Concatenation unary operator (wide-character version). */
        syString& operator+=(const wchar_t c);

        /** Concatenation. */
        syString& append(const syString& s);

        /** Concatenation. */
        syString& append(const char* str);

        /** Concatenation. */
        syString& append(const char c);

        /** Concatenation. */
        syString& append(const wchar_t c);

        /** Concatenation. */
        syString& append(const wchar_t* str);

        /** Stream-like concatenation. */
        syString& operator<<(const syString& s);

        /** Stream-like concatenation. */
        syString& operator<<(const char* str);

        /** Stream-like concatenation. */
        syString& operator<<(const wchar_t* str);

        /** Stream-like concatenation. */
        syString& operator<<(const char c);

        /** Stream-like concatenation. */
        syString& operator<<(const wchar_t c);

        /** Stream-like concatenation. */
        syString& operator<<(const int input);

        /** Stream-like concatenation. */
        syString& operator<<(const unsigned int input);

        /** Stream-like concatenation. */
        syString& operator<<(const long long input);

        /** Stream-like concatenation. */
        syString& operator<<(const unsigned long long input);

        /** Stream-like concatenation. */
        syString& operator<<(const double input);

        /** Stream-like concatenation. */
        syString& operator<<(const bool input);

        /** Formats a string using sprintf syntax. 2Kbytes max. */
        const syString& Printf(const char* format, ... );

        /** Formats a string using sprintf syntax. 2Kbytes max. */
        static const syString Format(const char* format, ... );

        /** Formats a string with maximum length of bufsize - 1, using sprintf syntax. */
        const syString& PrintfBig(unsigned long bufsize, const char* format, ... );

        /** Formats a string with maximum length of bufsize - 1, using sprintf syntax. */
        static const syString FormatBig(unsigned long bufsize, const char* format, ... );

        static const int npos;

        #ifdef SY_WXSTRING_COMPATIBILITY
        operator wxString() const;
        explicit syString(const wxString& s);
        syString& operator=(const wxString& s);
        #endif

    private:
        class Helper;
        friend class Helper;
        unsigned int m_Size;
        mutable unsigned int m_Capacity;
        bool m_UseRef;
        char* m_Str;
};

const syString operator+(const char* s1, const syString& s2);
const syString operator+(const wchar_t* s1, const syString& s2);

struct ltsystr
{
  bool operator()(const syString& s1, const syString& s2) const
  {
    return s1 < s2;
  }
};

syString rtrim(const syString& str, const syString& chars = " \r\n\t");
syString ltrim(const syString& str, const syString& chars = " \r\n\t");
syString trim(const syString& str, const syString& chars = " \r\n\t");




#endif
