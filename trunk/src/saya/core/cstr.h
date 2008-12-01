/***************************************************************
 * Name:      cstr.h
 * Purpose:   Declaration for the cstr class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#ifndef saya_cstr_h
#define saya_cstr_h

class cstr {
    /** @brief A simple wrapper for a const char*.
     *  cstr allows passing strings as return values on the stack without having to include
     *  the C++ std::string headers. It also allows to use of simple strings for use as keys in
     *  maps.
     */
    public:
        /** Standard constructor. */
        cstr();

        /** @brief const-char constructor.
         *  @param str the string to use as data for our "container".
         *  @param useref Tells the constructor whether to just copy the pointer (true), or copy the data (false).
         *         Default false.
         */
        cstr(const char* str,bool useref = false);

        /** Copy Constructor. */
        cstr(const cstr& copy);

        /** Standard destructor. */
        ~cstr();

        /** Returns the stored string */
        const char* c_str() const;

        /** Compares the string with another. Used for std::maps. */
        bool compare(const cstr& s) const;

        /** Assignment operator (object version). */
        cstr& operator=(const cstr& copy);

        /** Assignment operator (const-char* version). */
        cstr& operator=(const char* str);
    private:
        void assign(const char* str,bool useref = false);
        void assign(const cstr& copy);
        void reset();
        const char* m_Str;
        bool m_UseRef;
};

struct ltcstr
{
  bool operator()(const cstr& s1, const cstr& s2) const
  {
    return s1.compare(s2);
  }
};

#endif
