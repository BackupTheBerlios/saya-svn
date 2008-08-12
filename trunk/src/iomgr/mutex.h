/***************************************************************
 * Name:      mutex.h
 * Purpose:   Declaration of a Cross-platform Mutex class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 **************************************************************/

#ifndef saya_mutex_h
#define saya_mutex_h

#ifdef __WIN32__
    #include <windows.h>
#else
    #include <pthread.h>
#endif

/** @brief Generic Critical Section (a.k.a. intraprocess Mutex)
  *
  * A critical section is a mutex that allows various threads in a single process access the same resource.
  * In posix, they're pthread_mutexes, while in WinNT they're CRITICAL_SECTIONs.
  * After researching a couple of mutex classes in Windows and Posix OS's, I realized implementing a cross-platform mutex
  * is much easier than I thought. So I'll wrap the generic mutex class in a single .h and .cpp file.
  */
class syMutex {
    public:
        /** Initializes the Critical Section */
        syMutex();
        /** Deletes the Critical Section */
        ~syMutex();
        /** Enters the Critical Section */
        void Lock();
        /** Leaves the Critical Section */
        void Unlock();

        /** Gets a platform-dependent id for the currently running thread. */
        static unsigned long GetThreadId();

        /** Gets a platform-dependent id for the main thread. */
        static unsigned long GetMainThreadId();

        /** Is the current thread the main thread? */
        static bool IsMainThread();
    private:
        #ifdef __WIN32__
            CRITICAL_SECTION m_mutexobj;
        #else
            pthread_mutex_t m_mutexobj;
        #endif
};

/** @brief Implements a cross-platform Mutex Locker.
  *
  * Our mutex class can be locked and unlocked, but if there's a C++ exception, the unlock won't take place.
  * For this reason, we create a C++ object whose constructor locks the mutex and whose destructor unlocks it.
  * This way, in case of an exception, the destructor will be called, unlocking the mutex.
  */
class syMutexLocker {
    public:
        /** Constructor. Locks the mutex */
        syMutexLocker(syMutex& mutex);
        /** Destructor. Unlocks the mutex */
        ~syMutexLocker();
    private:
        syMutex* m_mutex;
};

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

        /** Checks if an operation must be aborted. To be overriden by your subclass. */
        virtual bool MustAbort() { return false; }

        /** Standard destructor */
        virtual ~syAborter() {}
};

/** Sleeps for the determinate number of milliseconds */
void syMilliSleep(unsigned long msec);

/** @brief Obtains the number of milliseconds that have happened since the program has started.
 *  Useful for microtime measurements.
 */
unsigned long syGetTicks();

#endif
