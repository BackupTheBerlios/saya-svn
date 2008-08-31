/***************************************************************
 * Name:      sythread.h
 * Purpose:   Declaration of a Cross-platform Thread class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 * Comments:  The syThread class API was modelled after the
 *            wxWidgets thread API. Some functions and enums
 *            were the same, others were rewritten from scratch.
 *            So if there's a bug, don't blame the wxThread authors,
 *            blame me instead.
 *
 * Original wx/thread.h copyright info:
 *
 *  ///////////////////////////////////////////////////////
 *  // Name:        wx/thread.h
 *  // Purpose:     Thread API
 *  // Author:      Guilhem Lavaux
 *  // Modified by: Vadim Zeitlin (modifications partly
 *  //              inspiredby omnithreads package from
 *  //              Olivetti & Oracle Research Laboratory)
 *  // Created:     04/13/98
 *  // Copyright:   (c) Guilhem Lavaux
 *  // Licence:     wxWindows licence
 *  ///////////////////////////////////////////////////////
 *
 * Original threadpsx.cpp copyright info:
 *  /////////////////////////////////////////////////////////////////////////////
 *  // Name:        src/unix/threadpsx.cpp
 *  // Purpose:     wxThread (Posix) Implementation
 *  // Author:      Original from Wolfram Gloger/Guilhem Lavaux
 *  // Modified by: K. S. Sreeram (2002): POSIXified wxCondition, added wxSemaphore
 *  // Created:     04/22/98
 *  // Copyright:   (c) Wolfram Gloger (1996, 1997)
 *  //                  Guilhem Lavaux (1998)
 *  //                  Vadim Zeitlin (1999-2002)
 *  //                  Robert Roebling (1999)
 *  //                  K. S. Sreeram (2002)
 *  // Licence:     wxWindows licence
 *  /////////////////////////////////////////////////////////////////////////////
 *
 **************************************************************/

#ifndef sythread_h
#define sythread_h

#ifdef __WIN32__
    #include <windows.h>
#else
    #include <pthread.h>
#endif

class syCondition;

/** @brief Generic Critical Section (a.k.a. intraprocess Mutex)
  *
  * A critical section is a mutex that allows various threads in a single process access the same resource.
  * In posix, they're pthread_mutexes, while in WinNT they're CRITICAL_SECTIONs.
  * After researching a couple of mutex classes in Windows and Posix OS's, I realized implementing a cross-platform mutex
  * is much easier than I thought. So I'll wrap the generic mutex class in a single .h and .cpp file.
  */
class syMutex {
    #ifndef __WIN32__
    friend class syCondition;
    #endif
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
  * Additionally, I have added Lock() and Unlock() methods which manipulate the mutex
  * without needing to destroy the locker.
  */
class syMutexLocker {
    public:
        /** Constructor. Locks the mutex */
        syMutexLocker(syMutex& mutex);

        /** (re)Locks the mutex */
        void Lock();

        /** Unlocks the mutex */
        void Unlock();

        /** Destructor. Unlocks the mutex */
        ~syMutexLocker();

    private:
        syMutex* m_mutex;
        bool m_locked;
};

#ifndef __WIN32__

/** Errors for syCondition */
enum syCondError {
    syCOND_NO_ERROR = 0,    /** successful completion */
    syCOND_INVALID,         /** object hasn't been initialized successfully */
    syCOND_TIMEOUT,         /** WaitTimeout() has timed out */
    syCOND_MISC_ERROR       /** Another kind of error */
};

/** POSIX implementation of Condition variables */
class syCondition {
    public:
        /** Constructor. The mutex must be locked by the caller before calling Wait function. */
        syCondition(syMutex& mutex);

        /** @brief Destructor.
         *  @note The destructor is not virtual so you can't derive from this class.
         */
        ~syCondition();

        /** @brief Broadcasts to all waiting threads, waking all of them up.
         *  @note This method may be called whether the mutex associated with this condition is locked or not.
         */
        syCondError Broadcast();

        /** Returns true if the Condition was initialized successfully. */
        bool IsOk() const;

        /** @brief Signals the object waking up at most one thread.
         *  @note If more than one thread is waiting on the condition, which thread is woken up is undefined.
         */
        syCondError Signal();

        /** @brief Waits until the condition is signalled.
         *  @note Coditions and mutexes work together. When a Condition is signaled or waited for,
         *  the associated mutex is relesed for a bit while the signal is transmitted
         *  (otherwise, the signal would be lost as the receiving thread would still be waiting for the mutex).
         *  Right after that, the mutex is locked again before returning from the function call.
         *  For this reason, before calling Wait() or Signal(), be sure to lock the associated mutex.
         */
        syCondError Wait();

        /** Same as Wait, but returns syCOND_TIMEOUT after the timeout has ellapsed. */
        syCondError WaitTimeout(unsigned long msec);
    private:
        syMutex& m_mutex;
        // get the POSIX mutex associated with us
        pthread_mutex_t *GetPMutex() const { return &(m_mutex.m_mutexobj); }
        pthread_cond_t m_cond;
        bool m_isOk;
};

#endif

/** Semaphore errors */
enum sySemaError {
    sySEMA_NO_ERROR,    /** There was no error. */
    sySEMA_INVALID,     /** Semaphore hasn't been initialized successfully. */
    sySEMA_OVERFLOW,    /** Post() would increase counter past the max. */
    sySEMA_BUSY,        /** Returned by TryWait() if Wait() would block, i.e. the count is zero. */
    sySEMA_TIMEOUT,     /** Timeout occurred without receiving semaphore. */
    sySEMA_MISC_ERROR   /** Miscellaneous error. */
};

/** @brief Generic cross-platform implementation of a Semaphore.
 *
 *  A semaphore lets a thread wait until a signal is sent to it, as if it was a car on the street
 *  waiting for the green light.
 *  This class was copied nearly verbatim from the wxWidgets wxInternalSemaphore class.
 */
class sySemaphore {
    public:

        /** @brief Constructor.
         *  @param maxcount If it's 0, the semaphore behaves as if there was no upper limit.
         *         If it's 1, it behaves exactly as a mutex.
         *  @param initialcount The semaphore's initial value, from 0 to maxcount (unless maxcount = 0)
         */
        sySemaphore(int initialcount = 0, int maxcount = 0);

        /** Destructor */
        ~sySemaphore();

        /** @brief Increments the semaphore count and signals one of the waiting threads.
         *  @note wxSEMA_OVERFLOW is returned if the count would increase the counter past the maximum,
         */
        sySemaError Post();

        /** Same as Wait(), but returns immediately. */
        sySemaError TryWait();

        /** Waits until the semaphore count becomes strictly positive; then decrements it and returns. */
        sySemaError Wait();

        /** Same as Wait(), but with a timeout limit. */
        sySemaError  WaitTimeout(unsigned long timeout_msec);
    private:
        #ifdef __WIN32__
        HANDLE m_semaphore;
        #else
        syMutex m_mutex;
        syCondition m_cond;
        int m_Count;
        int m_MaxCount;
        bool m_isOk;
        #endif
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

/**
 * Sleeps for the determinate number of microseconds.
 * @note Due to platform limitations, on Windows this function has a resolution of milliseconds or less.
 */
void syMicroSleep(unsigned long usec);

/** @brief Obtains the number of milliseconds that have happened since the program has started.
 *  Useful for microtime measurements.
 */
unsigned long syGetTicks();

/** @brief Enum for thread errors. Some copied from the wxWidgets documentation.
 *
 *  @warning Values might actually differ from wxWidgets'.
 */
enum syThreadError {
    syTHREAD_NO_ERROR = 0,          /** No error. */
    syTHREAD_NO_RESOURCE,           /** Not enough resources. */
    syTHREAD_RUNNING,               /** Thread already running. */
    syTHREAD_NOT_RUNNING,           /** Thread is not running. */
    syTHREAD_KILLED,                /** The Thread we waited for had to be killed */
    syTHREAD_MISC_ERROR,            /** Some other error */
    syTHREAD_WRONG_THREAD_CONTEXT   /** Wrong thread context */
};

/** @brief Enum for thread status. */
enum syThreadStatus {
    syTHREADSTATUS_NOT_CREATED = 0,
    syTHREADSTATUS_CREATED,
    syTHREADSTATUS_PAUSED,
    syTHREADSTATUS_RUNNING,
    syTHREADSTATUS_TERMINATING,
    syTHREADSTATUS_EXITED,
    syTHREADSTATUS_KILLED
};

/** @brief Enum for thread priority. */
enum
{
    SYTHREAD_MIN_PRIORITY      = 0u,
    SYTHREAD_DEFAULT_PRIORITY  = 50u,
    SYTHREAD_MAX_PRIORITY      = 100u
};


/** @brief Enum for thread kinds. Blatantly copied from the wxWidgets documentation.
 *
 *  @warning Values might actually differ from wxWidgets'.
 */
enum syThreadKind {
    /** @brief Detached threads are independent from the main thread and can't be polled.
     *
     *  Think of a detached thread as a Lemming: It does it work, and then jumps right onto a cliff
     *  without you having to worry about it. Unfortunately, you can't control it. Good luck.
     */
    syTHREAD_DETACHED = 0,
    /** @brief Joinable threads must be destroyed by the main thread (Recommended).
     *
     *  A joinable thread doesn't destroy itself after it's done its job. The main thread must delete the object.
     *  Joinable threads, of course, can be polled as you have much more control over them.
     */
    syTHREAD_JOINABLE
};

/** The syThread class is a reimplementation of the wxWidgets wxThread class
 *  using unix pthreads (we avoid STL 3rd party implementations on purpose).
 *  For Win32 systems, we'll link to the pthreads-win32 DLL
 *  @see http://sourceware.org/pthreads-win32/
 *
 *  @warning syThreads MUST NOT be stored in application's global memory memory.
 *  Instead, create them dynamically, or for joinable threads, you can create them inside
 *  a function's stack.
 */
class syThread {

    public:

        /** @brief Gets a platform-dependent id for the currently running thread.
         *  @deprecated Use syThread::GetCurrentId() instead.
         */
        static unsigned long GetThreadId();

        /** @brief Gets a platform-dependent id for the main thread. */
        static unsigned long GetMainThreadId();

        /** @brief Is the current thread the main thread?
         *  @deprecated Use syThread::IsMain() instead.
         */
        static bool IsMainThread();

        /** Constructor */
        syThread(syThreadKind kind = syTHREAD_DETACHED);

        /** @brief Destructor.
         *
         *  @warning You MUST NOT delete a detached thread, as they delete themselves.
         *           For this reason they cannot be allocated on the stack.
         *
         *  @note The Delete and Kill functions will not delete the C++ thread object.
         *        It is also safe to allocate them on stack.
         */
        virtual ~syThread();

        /** @brief Creates a new thread in a suspended state. Start it with run.
         *
         *  @see syThread::Run
         *
         *  @note if stackSize is 0, the default quantity of memory is reserved for the stack.
         *  The stack size is a compromise: Too little, and your app may crash. Too much, and
         *  you may run out of memory and start trashing the swap space.
         *
         *  @return One of the following: syTHREAD_NO_ERROR, syTHREAD_NO_RESOURCE, syTHREAD_RUNNING.
         *  @see syThreadError
         */
        syThreadError Create(unsigned int stackSize = 0);

        /** @brief Terminates gracefully a thread. The thread MUST call TestDestroy() regularly.
         *  @param rc A pointer to store the thread's return code into.
         *  @return syTHREAD_NO_ERROR if everything went OK;
         *  syTHREAD_WRONG_THREAD_CONTEXT if we were in the same thread.
         *  @note For detached threads, this is the only authorized way to delete a thread, so
         *  do not try to use the C++ delete operator to delete a detached thread.
         */
        syThreadError Delete(int* rc = NULL);

        /** @brief Pure virtual function which belongs to your thread class.
         *
         *  This function is the thread's "main" routine, and is called by Run().
         *  Its return value is the one returned by Wait().
         *
         *  @warning NEVER, EVER call this function directly!
         */
        virtual int Entry() = 0;

        /** Gets the number of CPUs in the system; -1 if the number can't be determined. */
        static int GetCPUCount();

        /** @brief Gets a platform-dependent id for the currently running thread.
         *  @note same as syThread::GetThreadId(). Implemented for compatibility purposes.
         */
        static unsigned long GetCurrentId();

        /** @brief Gets a platform-dependent id for the object's thread.
         *  @return 0 for non-created (or destroyed) threads; non-zero otherwise.
         */
        unsigned long GetId() const;

        /** Gets the thread's priority, from 0 to 100 (inclusive). */
        int GetPriority() const;

        /** @return True if the thread is started and not terminating. */
        bool IsAlive() const;

        /** @return True for detached thrads; false for joinable threads. */
        bool IsDetached() const;

        /** @brief Is the current thread the main thread?
         *
         *  @note Sames as IsMainThread(); written for compatibility.
         */
        static bool IsMain();

        /** @return true if the thread is paused. */
        bool IsPaused() const;

        /** @return true if the thread is running (not paused).
         *  @warning Use only for joinable threads.
         */
        bool IsRunning() const;

        /** @brief Kills the thread without performing cleanup.
         *  @warning EXTREMELY DANGEROUS. Use at your own risk.
         *  Instead, you should use Wait() for joinable threads, and Delete() for detached threads.
         *  @note This function cannot be used non-running or paused threads, in which case
         *  the return value will be syTHREAD_NOT_RUNNING. If the thread was paused, it will be
         *  resumed.
         */
        syThreadError Kill();

        /** @brief Called when the thread finishes execution.
         *  @note This function is called within the context of the respective thread, NOT the main thread,
         *  and won't be called if the thread was killed.
         *  @warning Do not try to call this function directly.
         */
        void OnExit();

        /** @brief Pauses the thread.
         *
         *  @note  In some implementations, the thread can't be suspended until it calls TestDestroy().
         *  @note  This function can only be called from another thread context.
         */
        syThreadError Pause();


        /** @brief Starts the thread execution. Needs Create() to be called first.
         *
         *  @note This function can only be called from another thread context.
         */
        syThreadError Run();

        /** @brief Sets the thread priority, from 0 (min) to 100 (max).
         *
         *  @note The thread's priority can only be set after calling Create() but before calling Run().
         */
        void SetPriority(int priority);

        /** @brief Pauses the thread execution for the given amount of time.
         *
         *  @note  This is the same as the function sySleep. Provided for wxWidgets compatibility.
         */
        static void Sleep(unsigned long milliseconds);

        /** @brief Resumes a paused thread.
         *  @see syThread::Pause
         *  @note This function can only be called from another thread context.
         */
        syThreadError Resume();

        /** @brief Sets the maximum number of threads that can be executed at the same time.
         *  @deprecated This function is only provided for wxWidgets API compatibility, and is not
         *  implemented. It will always return false.
         */
        static bool SetConcurrency(size_t level);

        /** @brief Suspends the thread if necessary and returns the "must abort" flag.
         *  @return true if the thread must abort immediately, false otherwise.
         *  @warning This function MUST be called periodically by the running thread!
         *  @warning Derived methods MUST call syThread::TestDestroy(), and then do their own checking.
         */
        virtual bool TestDestroy();

        /** @brief Gets the syThread object corresponding to the current thread.
         *  @return An syThread object, or NULL if either it's the main thread or if the thread
         *  wasn't created using syThread::Create.
         *  @warning This function must not be called to test if the thread is the main thread.
         *   Use syThread::IsMain() instead.
         */
        static syThread* This();

        /** Gives the rest of the thread time slice to the system allowing the other threads to run. */
        void Yield();

        /** @brief Waits for a joinable thread to terminate and gets the exit code obtained from Entry().
         *
         *  @see syThread::Entry()
         *  @note This function can only be called from another thread context.
         *  @note Unlike Delete, the thread is not cancelled, so the function can be called as many times
         *        as necessary.
         *  @warning You can only Wait() for joinable (not detached) threads.
         */
        int Wait() const;

    protected:
        /**  This function can only be called from a derived class, and only by the running thread.
         *   It will terminate the thread and, for detached threads, delete the syThread object.
         *   Just before exiting, syThread::OnExit will be called.
         */
        void Exit(int exitcode = 0);

        /**  Returns true if the current thread is the same as the object's thread. */
        bool IsThisThread() const;

    private:

        /** The OS-dependent thread ID */
        unsigned long m_ThreadId;

        /** The thread kind; syTHREAD_DETACHED or syTHREAD_JOINABLE */
        syThreadKind m_ThreadKind;

        /** The thread's current status */
        syThreadStatus m_ThreadStatus;

        /** Indicates if another thread requested a pause */
        bool m_PauseRequested;

        /** Indicates if another thread requested a resume */
        bool m_ResumeRequested;

        /** Indicates if another thread requested a stop */
        bool m_StopRequested;

        /** Mutex to prevent race conditions on status changing */
        syMutex* m_StatusMutex;

        /** The thread's exit code */
        int m_ExitCode;

        /** The thread's priority */
        unsigned int m_Priority;

        sySemaphore m_StartSemaphore;
        sySemaphore m_ResumeSemaphore;
};

#endif
