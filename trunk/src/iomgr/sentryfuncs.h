/***************************************************************
 * Name:      sentryfuncs.h
 * Purpose:   Declaration of various Sentry functions
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-11
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 ***************************************************************/

#ifndef sentryfuncs_h
#define sentryfuncs_h

/** @brief Class that sets a flag to a specific value during its lifetime. */
class syBoolSetter {
    public:
        /** Constructor */
        syBoolSetter(bool& flag,bool newvalue);
        syBoolSetter(volatile bool& flag,bool newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syBoolSetter();
    private:
        bool& m_Flag;
        bool m_Old;

};

/** @brief Class that sets an int to a specific value during its lifetime. */
class syIntSetter {
    public:
        /** Constructor */
        syIntSetter(int& dest,int newvalue);
        syIntSetter(volatile int& dest,int newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syIntSetter();
    private:
        int &m_Var;
        int m_Old;
};

/** @brief Class that sets an unsigned int to a specific value during its lifetime. */
class syUintSetter {
    public:
        /** Constructor */
        syUintSetter(unsigned int& dest,unsigned int newvalue);
        syUintSetter(volatile unsigned int& dest,unsigned int newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syUintSetter();
    private:
        unsigned int &m_Var;
        unsigned int m_Old;
};

/** @brief Class that sets a long to a specific value during its lifetime. */
class syLongSetter {
    public:
        /** Constructor */
        syLongSetter(long& dest,long newvalue);
        syLongSetter(volatile long& dest,long newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syLongSetter();
    private:
        long &m_Var;
        long m_Old;
};

/** @brief Class that sets an unsigned long to a specific value during its lifetime. */
class syULongSetter {
    public:
        /** Constructor */
        syULongSetter(unsigned long& dest,unsigned long newvalue);
        syULongSetter(volatile unsigned long& dest,unsigned long newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syULongSetter();
    private:
        unsigned long &m_Var;
        unsigned long m_Old;
};

class syCharSetter {
    public:
        /** Constructor */
        syCharSetter(char& dest,char newvalue);
        syCharSetter(volatile char& dest,char newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syCharSetter();
    private:
        char &m_Var;
        char m_Old;
};

class syUcharSetter {
    public:
        /** Constructor */
        syUcharSetter(unsigned char& dest,unsigned char newvalue);
        syUcharSetter(volatile unsigned char& dest,unsigned char newvalue);

        /** Destructor. Sets the flag to its original value. */
        ~syUcharSetter();
    private:
        unsigned char &m_Var;
        unsigned char m_Old;
};

#endif
