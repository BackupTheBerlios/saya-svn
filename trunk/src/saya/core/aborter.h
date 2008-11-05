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

#endif
