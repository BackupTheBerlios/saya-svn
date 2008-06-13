/***************************************************************
 * Name:      mutex.h
 * Purpose:   Declaration of a Cross-platform Mutex class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 **************************************************************/

#ifndef iomgr_mutex_h
#define iomgr_mutex_h

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

#endif
