/***************************************************************
 * Name:      sythread.cpp
 * Purpose:   Reimplementation of the wxWidgets Thread classes
 *            with some extra functions for multimedia processing
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-06-12
 * Copyright:
 *            (c) Wolfram Gloger (1996, 1997),
 *            (c) Guilhem Lavaux (1998),
 *            (c) Vadim Zeitlin (1999-2002),
 *            (c) Robert Roebling (1999),
 *            (c) K. S. Sreeram (2002),
 *            (c) Ricardo Garcia (2008)
 *
 * License:   WxWindows License
 * Comments:  The syThread and related classes are a
 *            reimplementation and clean-up of the wxWidgets
 *            wxThread API into one header and one cpp file
 *            (unlike wxWidgets, which has a .cpp and .h file
 *            per operating system).
 *            The majority of functions were the
 *            same, while others were rewritten from scratch.
 *            So if there's a bug, don't blame the wxThread
 *            authors, blame me (Rick) instead.
 *
 * Original wxWidgets thread files copyright info:
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
 * /////////////////////////////////////////////////////////////////////////////
 * // Name:        src/msw/thread.cpp
 * // Purpose:     wxThread Implementation
 * // Author:      Original from Wolfram Gloger/Guilhem Lavaux
 * // Modified by: Vadim Zeitlin to make it work :-)
 * // Created:     04/22/98
 * // Copyright:   (c) Wolfram Gloger (1996, 1997), Guilhem Lavaux (1998);
 * //                  Vadim Zeitlin (1999-2002)
 * // Licence:     wxWindows licence
 * /////////////////////////////////////////////////////////////////////////////
 *
 **************************************************************/

#include "sythread.h"

#ifdef __WIN32__
    #include <process.h>
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <sys/time.h>
    #include <errno.h>
#endif
#include <limits.h>
#include <vector>

// -----------------------------------------
// Private functions (courtesy of wxWidgets)
// -----------------------------------------

static void syScheduleThreadForDeletion();
static void syDeleteThread(syThread *This);
static unsigned syThreadStart(syThread* thread);

// --------------------------------------------------------------------
// Global variables  (courtesy of wxWidgets)
// --------------------------------------------------------------------

#ifdef __WIN32__
typedef pthread_key_t DWORD;
#endif

// TLS index of the slot where we store the pointer to the current thread
static pthread_key_t gs_keySelf = 0xFFFFFFFF;
static size_t gs_nThreadsBeingDeleted = 0;
static syMutex *gs_mutexDeleteThread = (syMutex *)NULL;
static syCondition *gs_condAllDeleted = (syCondition *)NULL;
static std::vector<syThread*> gs_allThreads;

// -----------------------
// Timing functions (mine)
// -----------------------

unsigned long syGetTime();
unsigned long syMainThreadId = syThread::GetThreadId();
unsigned long sySecondsAtInit = syGetTime();

void syMilliSleep(unsigned long msec) {
    if(!msec) { msec = 1; }
    #ifdef __WIN32__
    ::Sleep(msec);
    #else
    // The following code was adapted from http://cc.byexamples.com/20070525/nanosleep-is-better-than-sleep-and-usleep/
    struct timespec req={0};
    time_t sec=(int)(msec/1000);
    msec=msec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=msec*1000000L;
    while(nanosleep(&req,&req)==-1) {
         continue;
    }
    #endif
}

void syMicroSleep(unsigned long usec) {
    if(!usec) { usec = 1; }
    #ifdef __WIN32__
    usec = (usec + 999) / 1000;
    ::Sleep(usec);
    #else
    // The following code was adapted from http://cc.byexamples.com/20070525/nanosleep-is-better-than-sleep-and-usleep/
    struct timespec req={0};
    time_t sec=(int)(usec/1000000L);
    usec=usec-(sec*1000000L);
    req.tv_sec=sec;
    req.tv_nsec=usec*1000L;
    while(nanosleep(&req,&req)==-1) {
         continue;
    }
    #endif
}

unsigned long syGetTime() {
    unsigned long result;
    #ifdef __WIN32__
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        // To obtain the seconds, we divide by 10,000,000
        // (which is 1 second / 10 nanoseconds used by the WIN API)
        // But we need to use 64-bit math which C++ lacks!
        // Luckily, we have algebra on our side :)

        // 2^32 / 10,000,000 = 429.4967296
        // hi * 2^32 / 10,000,000 =
        //        = hi * 429.4967296
        //        = hi * (429 + 0.4967296)
        //        = hi * 429 + hi*0.496 + hi*.0007296
        //        = hi*0.0007296 + hi*0.496 + hi*429 // We reverse the order for greater precision
        //          57/78125 + 62/125 + 419 = 429.4967296. // We're lucky, it's 100% exact!
        //        = ((hi*57)/78125) + ((hi*62)/125) + (hi* 429)
        //
        //        Finally, ((hi << 32) + low) / 10,000,000 =
        //       (low / 10000000) + ((hi*57)/78125) + ((hi*62)/125) + (hi* 429)

        unsigned long low = ft.dwLowDateTime;

        // We spare the highest 16 bits - we don't want to overflow the calculation.
        unsigned long hi  = ft.dwHighDateTime & 0x0ffff;
        result = (low / 10000000) + ((hi*57)/78125) + ((hi*62)/125) + (hi* 429);
    #else
        // Here we call the posix function gettimeofday which returns the time in seconds and microseconds.
        // We're just using the seconds right now.
        struct timeval mytime;
        gettimeofday(&mytime, NULL);
        result = (unsigned long)(mytime.tv_sec);
    #endif
    return result;
}

unsigned long syGetTicks() {
    unsigned long result;
    #ifdef __WIN32__
        // Turns out that Windows already has a function to get the number of milliseconds since the
        // system was started. It's called GetTickCount.
        result = GetTickCount();
    #else
        // Here we call the posix function gettimeofday which returns the time in seconds and microseconds.
        // Since we're aiming for milliseconds, we have to multiply and divide by 1,000 the seconds
        // and microseconds, respectively.
        struct timeval mytime;
        gettimeofday(&mytime, NULL);
        result = (unsigned long)(mytime.tv_sec - sySecondsAtInit)*1000;
        result += (((unsigned long)(mytime.tv_usec)) / 1000);
    #endif
    return result;
}

// ---------------------------
// End timing functions (mine)
// ---------------------------

// -------------------
// Begin syMutex class
// -------------------

syMutex::syMutex() {
    #ifdef __WIN32__
        InitializeCriticalSection(&m_mutexobj);
    #else
        pthread_mutex_init(&m_mutexobj,NULL);
    #endif
}

syMutex::~syMutex() {
    #ifdef __WIN32__
        DeleteCriticalSection(&m_mutexobj);
    #else
        pthread_mutex_destroy(&m_mutexobj);
    #endif
}

void syMutex::Lock() {
    #ifdef __WIN32__
        EnterCriticalSection(&m_mutexobj);
    #else
        pthread_mutex_lock(&m_mutexobj);
    #endif
}

void syMutex::Unlock() {
    #ifdef __WIN32__
        LeaveCriticalSection(&m_mutexobj);
    #else
        pthread_mutex_unlock(&m_mutexobj);
    #endif
}

syMutexLocker::syMutexLocker(syMutex& mutex) :
m_locked(false)
{
    m_mutex = &mutex;
    Lock();
}

void syMutexLocker::Lock() {
    if(!m_locked) {
        m_mutex->Lock();
        m_locked = true;
    }
}
void syMutexLocker::Unlock() {
    if(m_locked) {
        m_locked = false;
        m_mutex->Unlock();
    }
}

syMutexLocker::~syMutexLocker() {
    Unlock();
}

// -----------------
// End syMutex class
// -----------------

// -------------------------------------------------
// Begin syCondition class
// This class was almost copied completely
// From the wxwidgets threadpsx.cpp and thrimpl.cpp
// ------------------------------------------------

syCondition::syCondition(syMutex& mutex) :
m_mutex(mutex)
{
    #ifdef __WIN32__
    m_numWaiters = 0;
    m_isOk = true;
    #else
    int err = pthread_cond_init(&m_cond, NULL);
    m_isOk = (err == 0);
    #endif
}

syCondition::~syCondition() {
    #ifndef __WIN32__
    if ( m_isOk ) {
        pthread_cond_destroy(&m_cond);
    }
    #endif
}

bool syCondition::IsOk() const {
    return m_isOk;
}

syCondError syCondition::Wait() {
    syCondError result = syCOND_MISC_ERROR;
    #ifdef __WIN32__
    // increment the number of waiters
    {
        syMutexLocker lock(m_csWaiters);
        ++m_numWaiters;
    }
    m_mutex.Unlock();

    // Explanation courtesy of wxWidgets:
    // a potential race condition can occur here
    //
    // after a thread increments m_numWaiters, and unlocks the mutex and before
    // the semaphore.Wait() is called, if another thread can cause a signal to
    // be generated
    //
    // this race condition is handled by using a semaphore and incrementing the
    // semaphore only if m_numWaiters is greater that zero since the semaphore,
    // can 'remember' signals the race condition will not occur

    // wait ( if necessary ) and decrement semaphore
    sySemaError err = m_semaphore.Wait();
    m_mutex.Lock();

    if ( err == sySEMA_NO_ERROR )
        result = syCOND_NO_ERROR;
    else if ( err == sySEMA_TIMEOUT )
        result = syCOND_TIMEOUT;
    else
        result = syCOND_MISC_ERROR;
    #else
    int err = pthread_cond_wait(&m_cond, GetPMutex());
    result = (err != 0) ? syCOND_MISC_ERROR : syCOND_NO_ERROR;
    #endif
    return result;
}

syCondError syCondition::WaitTimeout(unsigned long msec) {
    syCondError result = syCOND_MISC_ERROR;
    #ifdef __WIN32__
    {
        syMutexLocker lock(m_csWaiters);
        m_numWaiters++;
    }
    m_mutex.Unlock();

    // a race condition can occur at this point in the code
    // please see the comments in Wait(), for details

    sySemaError err = m_semaphore.WaitTimeout(milliseconds);

    if ( err == sySEMA_TIMEOUT ) {
        // another potential race condition exists here it is caused when a
        // 'waiting' thread times out, and returns from WaitForSingleObject,
        // but has not yet decremented m_numWaiters
        //
        // at this point if another thread calls signal() then the semaphore
        // will be incremented, but the waiting thread will miss it.
        //
        // to handle this particular case, the waiting thread calls
        // WaitForSingleObject again with a timeout of 0, after locking
        // m_csWaiters. This call does not block because of the zero
        // timeout, but will allow the waiting thread to catch the missed
        // signals.
        syMutexLocker lock(m_csWaiters);
        sySemaError err2 = m_semaphore.WaitTimeout(0);

        if ( err2 != wxSEMA_NO_ERROR ) {
            m_numWaiters--;
        }
    }
    m_mutex.Lock();
    result = (err == wxSEMA_NO_ERROR) ? wxCOND_NO_ERROR :
             ((err == wxSEMA_TIMEOUT) ? wxCOND_TIMEOUT : wxCOND_MISC_ERROR);
    #else
    unsigned long curtime = syGetTicks() + (sySecondsAtInit*1000);
    curtime += msec;
    unsigned long temp = curtime / 1000;
    int sec = temp;
    temp *= 1000;
    temp = curtime - temp;
    int millis = temp;

    timespec tspec;

    tspec.tv_sec = sec;
    tspec.tv_nsec = millis * 1000L * 1000L;

    int err = pthread_cond_timedwait( &m_cond, GetPMutex(), &tspec );
    switch (err) {
        case ETIMEDOUT:
            result = syCOND_TIMEOUT;
            break;
        case 0:
            result = syCOND_NO_ERROR;
            break;
        default:
            ;
    }
    return result;
    #endif
}

syCondError syCondition::Signal() {
    #ifdef __WIN32__
    syMutexLocker lock(m_csWaiters);
    if (m_numWaiters > 0) {
        // increment the semaphore by 1
        if ( m_semaphore.Post() != sySEMA_NO_ERROR )
            return syCOND_MISC_ERROR;
        m_numWaiters--;
    }
    #else
    int err = pthread_cond_signal(&m_cond);
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    #endif
    return syCOND_NO_ERROR;
}

syCondError syCondition::Broadcast() {
    #ifdef __WIN32__
    syMutexLocker lock(m_csWaiters);
    while (m_numWaiters > 0) {
        if (m_semaphore.Post() != wxSEMA_NO_ERROR)
            return wxCOND_MISC_ERROR;
        m_numWaiters--;
    }
    #else
    int err = pthread_cond_broadcast(&m_cond);
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    #endif
    return syCOND_NO_ERROR;
}

// ---------------------
// End syCondition class
// ---------------------

// -----------------------------------------------
// Begin sySemaphore class (courtesy of wxWidgets)
// -----------------------------------------------

#ifdef __WIN32__
sySemaphore::sySemaphore(int initialcount, int maxcount) {
    if ( maxcount == 0 ) {
        // make it practically infinite
        maxcount = INT_MAX;
    }
    m_semaphore = ::CreateSemaphore(NULL, initialcount, maxcount, NULL);
}
#else
sySemaphore::sySemaphore(int initialcount, int maxcount)
: m_cond(m_mutex)
{
    if ( maxcount == 0 ) {
        // make it practically infinite
        maxcount = INT_MAX;
    }
    if ( (initialcount < 0 || maxcount < 0) ||
            ((maxcount > 0) && (initialcount > maxcount)) ) {
        m_isOk = false; // Invalid initial counts
    } else {
        m_MaxCount = (size_t)maxcount;
        m_Count = (size_t)initialcount;
    }

    m_isOk = m_cond.IsOk();
}
#endif


sySemaphore::~sySemaphore() {
    #ifdef __WIN32__
    if (m_semaphore) { ::CloseHandle(m_semaphore); }
    #endif
}

sySemaError sySemaphore::Post() {
    sySemaError result = sySEMA_NO_ERROR;
    #ifdef __WIN32__
    if (!::ReleaseSemaphore(m_semaphore, 1, NULL)) {
        if (::GetLastError() == ERROR_TOO_MANY_POSTS) {
            result = sySEMA_OVERFLOW;
        } else {
            result = sySEMA_MISC_ERROR;
        }
    }
    #else
    syMutexLocker lock(m_mutex);

    if ( m_MaxCount > 0 && m_Count == m_MaxCount ) {
        result = sySEMA_OVERFLOW;
    } else {
        ++m_Count;
        result = (m_cond.Signal() == syCOND_NO_ERROR) ? sySEMA_NO_ERROR : sySEMA_MISC_ERROR;
    }
    #endif
    return result;
}

sySemaError sySemaphore::TryWait() {
    #ifdef __WIN32__
    sySemaError rc = WaitTimeout(0);
    if (rc == sySEMA_TIMEOUT) { rc = sySEMA_BUSY; }
    return rc;
    #else
    syMutexLocker locker(m_mutex);
    if (m_Count == 0) { return sySEMA_BUSY; }
    m_Count--;
    return sySEMA_NO_ERROR;
    #endif
}

sySemaError sySemaphore::Wait() {
    #ifdef __WIN32__
    return WaitTimeout(INFINITE);
    #else
    syMutexLocker locker(m_mutex);

    while ( m_Count == 0 ) {
        if ( m_cond.Wait() != syCOND_NO_ERROR )
            return sySEMA_MISC_ERROR;
    }
    m_Count--;
    return sySEMA_NO_ERROR;
    #endif
}

sySemaError  sySemaphore::WaitTimeout(unsigned long timeout_msec) {
    sySemaError result;
    #ifdef __WIN32__
    DWORD rc;
    rc = ::WaitForSingleObject(m_semaphore, timeout_msec);
    switch (rc) {
        case WAIT_OBJECT_0:
           result = sySEMA_NO_ERROR;
           break;
        case WAIT_TIMEOUT:
           result = sySEMA_TIMEOUT;
           break;
        default:
            result = sySEMA_MISC_ERROR;
    }
    #else
    syMutexLocker locker(m_mutex);
    unsigned long startTime = syGetTicks();
    result = sySEMA_NO_ERROR;
    while(m_Count == 0) {
        unsigned long elapsed = syGetTicks() - startTime;
        long remainingTime = (long)timeout_msec - (long)elapsed;
        if (remainingTime <= 0) {
            return sySEMA_TIMEOUT;
        }
        switch (m_cond.WaitTimeout(remainingTime)) {
            case syCOND_TIMEOUT:
                result = sySEMA_TIMEOUT;
                break;
            default:
                result = sySEMA_MISC_ERROR;
            case syCOND_NO_ERROR:
                ;
        }
    }
    m_Count--;
    #endif
    return result;
}

// ---------------------
// End sySemaphore class
// ---------------------

// ---------------------
// Begin syAborter class
// ---------------------

bool syAborter::MustAbort() {
    return (syThread::MustAbort() || InternalMustAbort());
}

// ---------------------
// End syAborter class
// ---------------------


// ----------------------------------------------------------
// Begin syThread auxiliary functions (courtesy of wxWidgets)
// ----------------------------------------------------------

/** syThreadExiter is a class of mine, an adaptation from wxMutexLocker, but for threads instead.
 *  It calls OnExit when destroyed.
 */
class syThreadExiter {
    public:
        syThreadExiter(syThread* thread) : m_Thread(thread) {}
        ~syThreadExiter() {
            if(m_Thread) { m_Thread->OnExit(); }
        }
    private:
        syThread* m_Thread;
};

/** @brief Deletes a thread. */
static void syDeleteThread(syThread *This) {
    // gs_mutexDeleteThread should be unlocked before signalling the condition
    // or syThread::OnExit() would deadlock
    syMutexLocker lock(*gs_mutexDeleteThread);
    delete This;
    if ( !--gs_nThreadsBeingDeleted ) {
        gs_condAllDeleted->Signal(); // no more threads left, signal it
    }
}

// The platform-specific thread starting functions.
#ifdef __WIN32__
unsigned __stdcall syWinThreadStart(void *thread) {
    return syThreadStart((syThread*)thread);
}
#else
void *syPthreadStart(void *thread) {
    return (void*)syThreadStart((syThread*)thread);
}
#endif

/** @brief This static function is the cross-platform entry point for a thread. */
static unsigned syThreadStart(syThread* thread) {

    // Whether an exception occurs or not while running the thread,
    // OnExit is still called via the exiter's destructor.
    // This was adapted from wxWidget's thread.cpp, with the difference that they used macros and C functions.
    syThreadExiter exiter(thread);

    int rc = -1;
    // Set up the indexing so that syThread::This() will work.
    #ifdef __WIN32__
    rc = (::TlsSetValue(gs_keySelf, thread)) ? 0 : -1;
    #else
    rc = pthread_setspecific(gs_keySelf, thread);
    if(rc != 0) { rc = -1; }
    #endif
    if(rc == 0) {
        rc = thread->InternalEntry();
    }
    return rc;
}

static void syScheduleThreadForDeletion() {
    syMutexLocker lock(*gs_mutexDeleteThread);
    gs_nThreadsBeingDeleted++;
}

// ---------------------------------------------------
// Begin syThread class (mostly wxWidgets, a bit mine)
// ---------------------------------------------------

syThread::syThread(syThreadKind kind) :
m_ThreadId(0),
m_ThreadKind(kind),
m_ThreadStatus(syTHREADSTATUS_NOT_CREATED),
m_PauseRequested(false),
m_StopRequested(false),
m_ExitCode(0)
{
}

syThread::~syThread() {
    // TODO: Implement syThread::~syThread
}

unsigned long syThread::GetThreadId() {
    #ifdef __WIN32__
        return (unsigned long)GetCurrentThreadId();
    #else
        return (unsigned long)syscall(__NR_gettid);
    #endif
}

unsigned long syThread::GetCurrentId() {
    #ifdef __WIN32__
        return (unsigned long)GetCurrentThreadId();
    #else
        return (unsigned long)syscall(__NR_gettid);
    #endif
}

int syThread::GetCPUCount() {
    // TODO: Implement syThread::GetCPUCount
    return -1;
}

unsigned long syThread::GetMainThreadId() {
    return syMainThreadId;
}

bool syThread::IsMainThread() {
    return (syThread::GetCurrentId() == syMainThreadId);
}

bool syThread::IsMain() {
    return (syThread::GetCurrentId() == syMainThreadId);
}

unsigned long syThread::GetId() const {
    return m_ThreadId;
}

syThreadError syThread::Create(unsigned int stackSize) {
    syMutexLocker lock(m_Mutex);
    if(m_ThreadStatus == syTHREADSTATUS_TERMINATING || m_ThreadStatus == syTHREADSTATUS_EXITED) {
        lock.Unlock();
        syMicroSleep(1); // Sleep for 1 uSeconds or more just to let the thread finish its cleanup
    }
    lock.Lock();
    if(m_ThreadStatus == syTHREADSTATUS_EXITED) {
        m_ThreadStatus = syTHREADSTATUS_NOT_CREATED;
    }
    if(m_ThreadStatus != syTHREADSTATUS_NOT_CREATED) {
        return syTHREAD_RUNNING;
    }
    bool success = false;
    #ifdef __WIN32__
        // Watcom is reported to not like 0 stack size (which means "use default"
        // for the other compilers and is also the default value for stackSize)
        #ifdef __WATCOMC__
            if ( !stackSize )
                stackSize = 10240;
        #endif // __WATCOMC__

        m_hThread = (HANDLE)_beginthreadex(
                          NULL,             // default security
                          stackSize,
                          syWinThreadStart, // entry point
                          thread,
                          0,                // Create Running (our implementation differs from wx's in
                                            // that we follow more the posix implementation, which IMHO
                                            // is much cleaner than win32's. so we let the thread run
                                            // and then it'll freeze automatically with our semaphore.
                          (unsigned int *)&m_ThreadId
                        );

        success = ( m_hThread != NULL );
    #else
        // For posix, we initialize a pthread attribute variable, and then we create the thread with that.
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        if (stackSize) { pthread_attr_setstacksize(&attr, stackSize); }
        // Try to set the threads really concurrent.
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        if (pthread_attr_getschedpolicy(&attr, &m_Policy) != 0) {
            m_Policy = -1; // Couldn't get the OS' scheduling policy!
        }

        m_ShouldBeJoined = (m_ThreadKind != syTHREAD_DETACHED);
        if (!m_ShouldBeJoined) {
            // Try to create the thread detached
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        }
        int rc = pthread_create(&m_ThreadId, &attr, syPthreadStart, (void *)this);

        pthread_attr_destroy(&attr); // Destroy the thread attribute or we'd leak resources.
        success = (rc == 0);
    #endif
    if(!success) {
        m_ThreadStatus = syTHREADSTATUS_NOT_CREATED;
        return syTHREAD_NO_RESOURCE; // Error creating the thread!
    }

    m_ThreadStatus = syTHREADSTATUS_CREATED;
    lock.Unlock();
    if (m_Priority != SYTHREAD_DEFAULT_PRIORITY) {
        SetPriority(m_Priority);
    }
    m_StackSize = stackSize;
    return syTHREAD_NO_ERROR;
}

// TODO: incorporate thread deletion scheduling (for both posix and Windows)

int syThread::InternalEntry() {
    bool dontRunAtAll;

    m_StartSemaphore.Wait();
    syMutexLocker lock(m_Mutex);
    dontRunAtAll = m_ThreadStatus == syTHREADSTATUS_CREATED && m_StopRequested;
    lock.Unlock();
    int result = -1;
    if (dontRunAtAll) {
        return -1;
    }
    if(!dontRunAtAll) {
        // call the main entry
        int exitcode = Entry();
        lock.Lock();
        m_ThreadStatus = syTHREADSTATUS_TERMINATING;
        lock.Unlock();
        Exit(exitcode); // Note: Exit should terminate the thread and not return at all.
        #ifdef __WIN32__
        result = exitcode;
        #else
        result = 0; // If the thread didn't exit, return 0. syThreadStart will know what to do with it.
        #endif
    }
    // If everything went fine, OnExit() should already be called and we shouldn't return in the first place.
    return result;

}

void syThread::Exit(int exitcode) {
    if(!IsThisThread()) { return; } // Exit() can only be called by the same thread context
    m_ExitCode = exitcode;
    if(IsDetached()) {
        // Comment from the wxWidgets source code:
        // from the moment we call OnExit(), the main program may terminate at
        // any moment, so mark this thread as being already in process of being
        // deleted or wxThreadModule::OnExit() will try to delete it again
        syScheduleThreadForDeletion();
    }
    OnExit();
    if(IsDetached()) {
        syDeleteThread(this);
        #ifndef __WIN32__
        pthread_setspecific(gs_keySelf, 0);
        #endif
    } else {
        syMutexLocker lock(m_Mutex);
        m_ThreadStatus = syTHREADSTATUS_EXITED;
    }
    // terminate the thread (pthread_exit() never returns)
    #ifdef __WIN32__
    _endthreadex((unsigned)exitcode);
    #else
    pthread_exit((void*)exitcode);
    #endif
}


bool syThread::IsAlive() {
    syMutexLocker lock(m_Mutex);
    return (m_ThreadStatus == syTHREADSTATUS_CREATED) ||
    (m_ThreadStatus == syTHREADSTATUS_PAUSED) ||
    (m_ThreadStatus == syTHREADSTATUS_RUNNING);
}



bool syThread::IsDetached() const {
    return (m_ThreadKind == syTHREAD_DETACHED);
}

bool syThread::IsPaused() {
    syMutexLocker lock(m_Mutex);
    return (m_ThreadStatus == syTHREADSTATUS_PAUSED);
}

bool syThread::IsRunning() {
    syMutexLocker lock(m_Mutex);
    return (m_ThreadStatus == syTHREADSTATUS_RUNNING);
}

syThreadError syThread::Kill() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    // TODO: Implement syThread::Kill
    return syTHREAD_MISC_ERROR;
}

syThreadError syThread::Pause() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    if(!IsRunning()) { return syTHREAD_NOT_RUNNING; }

    m_PauseRequested = true;
    while(m_PauseRequested && m_ThreadStatus == syTHREADSTATUS_RUNNING) {
        syMicroSleep(50);
    }
    if(!m_PauseRequested) {
        return syTHREAD_MISC_ERROR; // Could not pause the thread!
    }
    m_PauseRequested = false;
    if(m_ThreadStatus == syTHREADSTATUS_KILLED) {
        return syTHREAD_KILLED;
    }
    return syTHREAD_NO_ERROR;
}

syThreadError syThread::Run() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }

    syThreadError result;
    syMutexLocker lock(m_Mutex);
    if(m_ThreadKind == syTHREAD_JOINABLE && m_ThreadStatus == syTHREADSTATUS_EXITED) {
        lock.Unlock();
        if(Create(m_StackSize) != syTHREAD_NO_ERROR) {
            return syTHREAD_NO_RESOURCE;
        }
        lock.Lock();
        m_ThreadStatus = syTHREADSTATUS_CREATED;
        m_PauseRequested = false;
        m_StopRequested = false;
    }
    if(m_ThreadStatus != syTHREADSTATUS_CREATED) {
        result = syTHREAD_RUNNING;
    } else {
        m_ThreadStatus = syTHREADSTATUS_RUNNING;
        result = syTHREAD_NO_ERROR;
        lock.Unlock();
        m_StartSemaphore.Post();
    }
    return result;
}

void syThread::SetPriority(int priority) {
    syMutexLocker lock(m_Mutex);
    switch(m_ThreadStatus) {
        case syTHREADSTATUS_CREATED:
        case syTHREADSTATUS_PAUSED:
        case syTHREADSTATUS_RUNNING:
        break;
        default:
            return; // Cannot set priority in other cases
    }
    if(priority < 0) { priority = 0; }
    if(priority > 100) { priority = 100; }
    m_Priority = priority;
    int os_priority;
    #ifdef __WIN32__
        if (m_Priority <= 20)
            os_priority = THREAD_PRIORITY_LOWEST;
        else if (m_Priority <= 40)
            os_priority = THREAD_PRIORITY_BELOW_NORMAL;
        else if (m_Priority <= 60)
            os_priority = THREAD_PRIORITY_NORMAL;
        else if (m_Priority <= 80)
            os_priority = THREAD_PRIORITY_ABOVE_NORMAL;
        else
            os_priority = THREAD_PRIORITY_HIGHEST;
    #else
        int min_prio = -1;
        int max_prio = -1;
        if(m_Policy >= 0) {
            min_prio = sched_get_priority_min(m_Policy);
            max_prio = sched_get_priority_max(m_Policy);
        }
        if ( min_prio == -1 || max_prio == -1 || max_prio == min_prio) {
            m_Priority = 50; // Priority setting is being ignored by the OS; Set it to default and return
            return;
        }
        os_priority = min_prio + (m_Priority*(max_prio - min_prio))/100;
    #endif

    // Now that we have calculated the os-based priority, let's apply it.

    #ifdef __WIN32__
        ::SetThreadPriority(m_hThread, os_priority);
    #else
        pthread_setschedprio(m_ThreadId, os_priority);
    #endif
}

int syThread::GetPriority() {
    syMutexLocker lock(m_Mutex);
    return m_Priority;
}

void syThread::Sleep(unsigned long msec) {
    if(!msec) { msec = 1; }
    #ifdef __WIN32__
    ::Sleep(msec);
    #else
    // The following code was adapted from http://cc.byexamples.com/20070525/nanosleep-is-better-than-sleep-and-usleep/
    struct timespec req={0};
    time_t sec=(int)(msec/1000);
    msec=msec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=msec*1000000L;
    while(nanosleep(&req,&req)==-1) {
         continue;
    }
    #endif
}

syThreadError syThread::Resume() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    syMutexLocker lock(m_Mutex);
    syThreadError result;
    switch(m_ThreadStatus) {
        case syTHREADSTATUS_PAUSED:
            m_PauseRequested = false;
            lock.Unlock();
            m_ResumeSemaphore.Post();
            result = syTHREAD_NO_ERROR;
            break;
        case syTHREADSTATUS_RUNNING:
            result = syTHREAD_RUNNING;
            break;
        case syTHREADSTATUS_KILLED:
            result = syTHREAD_KILLED;
            break;
        case syTHREADSTATUS_CREATED:
            result = syTHREAD_NOT_RUNNING;
            break;
        case syTHREADSTATUS_TERMINATING:
        case syTHREADSTATUS_EXITED:
        default:
            result = syTHREAD_MISC_ERROR;
    }
    return result;
}

bool syThread::SetConcurrency(size_t level) {
    return false; // This function is kept only for wxWidgets API compatibility
}

bool syThread::MustAbort() {
    syThread* thread = syThread::This();
    if(!thread) { return false; }
    return thread->TestDestroy();
}

bool syThread::TestDestroy() {
    if(!IsThisThread()) return false; // Not our thread
    if(m_StopRequested) return true; // No need to lock mutex, must stop.
    syMutexLocker lock(m_Mutex);
    if(m_PauseRequested && (m_ThreadStatus == syTHREADSTATUS_RUNNING)) {
        if(m_ThreadStatus!= syTHREADSTATUS_RUNNING) return false;
        m_ThreadStatus = syTHREADSTATUS_PAUSED;
        lock.Unlock();
        m_ResumeSemaphore.Wait();
        lock.Lock();
        m_ThreadStatus = syTHREADSTATUS_RUNNING;
        m_PauseRequested = false;
    }
    return m_StopRequested;
}

syThread* syThread::This() {
    syThread* thread;
    #ifdef __WIN32__
    thread = (syThread *)::TlsGetValue(gs_keySelf);
    #else
    thread = (syThread *)pthread_getspecific(gs_keySelf);
    #endif
    return thread;
}

void syThread::Yield() {
    if(!IsThisThread() || IsMain()) { return; }
    #ifdef __WIN32__
    ::SwitchToThread();
    #else
    ::sched_yield();
    #endif
}

int syThread::Wait() {
    if(IsThisThread()) { return -1; }
    while(IsRunning() || m_ThreadStatus == syTHREADSTATUS_TERMINATING) {
        syMicroSleep(50);
    }
    if(m_ThreadStatus == syTHREADSTATUS_KILLED) { return -1; }
    return m_ExitCode;
}

syThreadError syThread::Delete(int* rc) {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    bool isdetached = false;
    syThreadStatus status;
    // The following logic was adapted from wxWidgets' threadpsx.cpp
    syMutexLocker lock(m_Mutex);

    status = m_ThreadStatus;
    m_PauseRequested = false;
    m_StopRequested = true;
    isdetached = (m_ThreadKind == syTHREAD_DETACHED);
    if(status == syTHREADSTATUS_CREATED) { lock.Unlock();Run(); }
    // Run the thread (we can't stop it if it keeps waiting)
    if(status == syTHREADSTATUS_PAUSED) { lock.Unlock();Resume(); }
    // Resume the thread for the same reason.
    if (!isdetached) {
        // For joinable threads, wait until the thread stops;
        // We can't wait for detached threads.
        if(status != syTHREADSTATUS_EXITED) { lock.Unlock();Wait(); }
        if(rc) { *rc = m_ExitCode; } // return the thread's exit code
    }
    return syTHREAD_NO_ERROR;
}

bool syThread::IsThisThread() const {
    return (this && this == This());
}
