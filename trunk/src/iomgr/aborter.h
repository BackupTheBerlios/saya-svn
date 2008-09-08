/***************************************************************
 * Name:      aborter.h
 * Purpose:   Declaration of the syAborter class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-07
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 ***************************************************************/

#ifndef syaborter_h
#define syaborter_h

/** @brief Base class for classes with long operations that can be aborted.
  *
  * When working in multi-threaded environments, it's common that you must check for a signal flagging to
  * abort a long operation. Here we provide a class that you can derive from.
  * As an example, the syBitmap class has an syAborter* member pointer. If you set that pointer to another
  * class, syBitmap will know when to abort a bitmap copying.
  */
class syAborter {
    public:
        /** Standard constructor */
        syAborter() {}

        /** Checks if either an operation must be aborted or if the current thread is going to be closed. */
        bool MustAbort();

        /** Standard destructor */
        virtual ~syAborter() {}

    protected:

        /** Checks if an operation must be aborted. To be overriden by your subclass. */
        virtual bool InternalMustAbort() { return false; }

};

/** @brief Class that sets a flag to a specific value during its lifetime. */
class syBoolSetter {
    public:
        /** Constructor */
        syBoolSetter(bool& flag,bool newvalue);

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

        /** Destructor. Sets the flag to its original value. */
        ~syUcharSetter();
    private:
        unsigned char &m_Var;
        unsigned char m_Old;
};

#endif
