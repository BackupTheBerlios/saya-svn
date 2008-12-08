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
#include "aborter.h"
#include "atomic.h"
#include "sentryfuncs.h"

#ifdef __WIN32__
    #include <windows.h>
    #include <process.h>
    #undef Yield
    // Now I wonder who on this planet could define a macro instead of defining a global function!
#else
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <sys/time.h>
    #include <errno.h>
    #include <pthread.h>
    #include <semaphore.h>
#endif
#include <limits.h>
#include <set>

// ----------------------------------
// Begin Private classes declarations
// ----------------------------------

/** syThreadExiter is a RAII-class designed for threads.
 *  It calls OnExit when destroyed.
 */
class syThreadExiter {
    public:
        /** Constructor. */
        syThreadExiter(syThread* thread) : m_Thread(thread) {}

        /** Calls the thread's OnExit() method. */
        void OnExit();

        /** Destructor. */
        ~syThreadExiter() {
            OnExit();
        }

    private:
        syThread* m_Thread;
};

class syMutexData {
    public:
        #ifdef __WIN32__
            CRITICAL_SECTION m_mutexobj;
        #else
            pthread_mutex_t m_mutexobj;
        #endif
};

class syCondData {
    public:
        syCondData(syMutex& mutex) : m_mutex(mutex) {}

        syMutex& m_mutex;
        #ifdef __WIN32__
        /** the number of threads currently waiting for this condition */
        LONG m_numWaiters;

        /** the critical section protecting m_numWaiters */
        syMutex m_csWaiters;
        /** The condition's semaphore */
        sySemaphore m_semaphore;
        #else
        /** get the POSIX mutex associated with us */
        pthread_mutex_t* GetPMutex() const { return &(m_mutex.m_Data->m_mutexobj); }

        /** The pthreads condition variable */
        pthread_cond_t m_cond;
        #endif
        bool m_isOk;
};

class sySemData {
    public:
        #ifdef __WIN32__
        HANDLE m_semaphore;
        #else
        syMutex m_mutex;
        syCondition* m_cond;
        int m_Count;
        int m_MaxCount;
        bool m_isOk;
        #endif
};

class sySafeMutexData {
    public:

        /** Is the mutex recursive? */
        bool m_Recursive;

        /** Count for recursive mutex */
        unsigned int m_LockCount;

        /** The thread owning the mutex. */
        unsigned long m_Owner;

        /** A condition for the waits */
        syCondition m_Condition;

        /** A mutex for the condition */
        syMutex m_Mutex;

        sySafeMutexData(bool recursive) :
        m_Recursive(recursive),
        m_LockCount(0),
        m_Owner(0xFFFFFFFF), // 0xFFFFFFFF means the mutex is unlocked.
        m_Condition(m_Mutex)
        {}

        void Wait(unsigned long ms) {
            syMutexLocker lock(m_Mutex);
            m_Condition.WaitTimeout(ms);
        }
        void Signal() {
            syMutexLocker lock(m_Mutex);
            m_Condition.Signal();
        }
};

class syThreadData {
    public:

        syThreadData(syThreadKind kind) :
            m_ThreadId(0),
            m_ThreadKind(kind),
            m_ThreadStatus(syTHREADSTATUS_NOT_CREATED),
            m_PauseRequested(false),
            m_StopRequested(false),
            m_ExitCode(0),
            m_PausedCondition(m_Mutex),
            m_ResumeCondition(m_Mutex),
            m_Exiter(0)
            {}

        static void InternalEntry(syThread* thread, int& rc);

        /** The OS-dependent thread ID */
        unsigned long m_ThreadId;

        /** The thread kind; syTHREAD_DETACHED or syTHREAD_JOINABLE */
        syThreadKind m_ThreadKind;

        /** The thread's current status */
        syThreadStatus m_ThreadStatus;

        /** Indicates if another thread requested a pause */
        volatile bool m_PauseRequested;

        /** Indicates if another thread requested a stop */
        volatile bool m_StopRequested;

        /** Mutex to prevent race conditions on status changing */
        syMutex m_Mutex;

        /** The thread's exit code */
        int m_ExitCode;

        /** The thread's handle */
        #ifdef __WIN32__
        HANDLE m_hThread;
        #else
        /** Flag for joinable threads */
        bool m_ShouldBeJoined;

        /** Mutex for setting m_ShouldBeJoined */
        syMutex m_csJoinFlag;

        /** Flag for thread policy */
        int m_Policy;
        #endif

        /** The thread's priority */
        unsigned int m_Priority;

        /** The thread's stack size when it was last created */
        unsigned int m_StackSize;

        /** Condition that signals that this thread has been paused. */
        syCondition m_PausedCondition;

        /** Condition that signals that this thread needs to be started or resumed. */
        syCondition m_ResumeCondition;

        syThreadExiter* m_Exiter;
};

// The OnExit process is a little complicated, because we might have an syThreadExiter object in the stack.
// by making syThreadData::OnExit a proxy for all the internal calls to syThread::OnExit,
// we can handle all the difficult logic in here.

// --------------------------------
// End Private classes declarations
// --------------------------------

// -----------------------------------------
// Private functions (courtesy of wxWidgets)
// -----------------------------------------

static bool syScheduleThreadForDeletion(syThread*, bool);
static void syDeleteThread(syThread *This);
static unsigned syThreadStart(syThread* thread);

// --------------------------------------------------------------------
// Global variables  (courtesy of wxWidgets)
// --------------------------------------------------------------------

#ifdef __WIN32__
typedef DWORD pthread_key_t;
#endif

// TLS index of the slot where we store the pointer to the current thread
static unsigned long syMainThreadId = syThread::GetThreadId();
static pthread_key_t gs_keySelf = 0xFFFFFFFF;
static size_t gs_nThreadsBeingDeleted = 0;
static syMutex *gs_mutexAllThreads = (syMutex *)NULL;
static syMutex *gs_mutexDeleteThread = (syMutex *)NULL;
static syCondition *gs_condAllDeleted = (syCondition *)NULL;
static bool gs_ThreadsOk = false;

struct ltthread
{
  bool operator()(const syThread* s1, const syThread* s2) const
  {
    return (unsigned long)s1 < (unsigned long)s2;
  }
};

// We store the threads being created on a global set.
// When the program finishes, it deletes all the threads.
// Why a set?
// Because on deletion, the thread deletes itself from the set.
// Using a vector (like wxWidgets does) would require logarithmic
// time instead of linear time, which makes deleting all the threads
// terribly inefficient. On top of that, this is a locking operation,
// which makes things even slower.

static std::set<syThread*,ltthread> gs_JoinableThreads;
static std::set<syThread*,ltthread> gs_DetachedThreads;


// -----------------------
// Timing functions (mine)
// -----------------------

unsigned long syGetTime();
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

// -----------------------
// Begin sySafeMutex class
// -----------------------

sySafeMutex::sySafeMutex(bool recursive)
{
    m_Data = new sySafeMutexData(recursive);
}

sySafeMutex::~sySafeMutex() {
    delete m_Data;
}

bool sySafeMutex::TryLock(syAborter* aborter) {
    unsigned long id = syThread::GetCurrentId();

    if(m_Data->m_Owner == id) {
        if(m_Data->m_Recursive) {
            ++(m_Data->m_LockCount);
        }
        return true;
    }

    if(aborter && aborter->MustAbort()) {
        return false;
    }

    // Try to acquire the lock.
    if(!syAtomic::bool_CAS(&m_Data->m_Owner, 0xFFFFFFFF, id)) {
        return false;
    }
    m_Data->m_LockCount = 1;
    return true;
}

bool sySafeMutex::Wait(syAborter* aborter) {
    unsigned long id = syThread::GetCurrentId();
    while(m_Data->m_Owner != id && m_Data->m_Owner != 0xFFFFFFFF) {
        if(aborter && aborter->MustAbort()) {
            return false;
        }
        m_Data->Wait(1);
    }
    return true;
}

bool sySafeMutex::Lock(syAborter* aborter) {
    for(;;) {
        if(TryLock(aborter)) break;
        if(aborter && aborter->MustAbort()) { return false; }
        // wait for the lock to be unlocked, or 1 millisecond. Whatever comes first.
        m_Data->Wait(1);
    }
    return true;
}

bool sySafeMutex::SafeLock() {
    for(;;) {
        if(TryLock(NULL)) break;
        if(syThread::MustAbort()) { return false; }
        // wait for the lock to be unlocked, or 1 millisecond. Whatever comes first.
        m_Data->Wait(1);
    }
    return true;
}

void sySafeMutex::Unlock() {
    unsigned int id = syThread::GetCurrentId();
    if(m_Data->m_Owner == id) {
        if(m_Data->m_Recursive && m_Data->m_LockCount > 1) {
            --(m_Data->m_LockCount);
        } else {
            // Set m_LockCount to 0.
            m_Data->m_LockCount = 0;
            m_Data->m_Owner = 0xFFFFFFFF;
            m_Data->Signal();
        }
    }
}

bool sySafeMutex::IsUnlocked() {
    return (m_Data->m_Owner == 0xFFFFFFFF);
}

unsigned long sySafeMutex::GetOwner() {
    return m_Data->m_Owner;
}

// ---------------------
// End sySafeMutex class
// ---------------------

// -----------------------------
// Begin sySafeMutexLocker class
// -----------------------------

/** Constructor */
sySafeMutexLocker::sySafeMutexLocker(sySafeMutex& mutex, syAborter* aborter) :
m_Mutex(mutex),
m_Aborter(aborter)
{
    Lock();
}

sySafeMutexLocker::~sySafeMutexLocker() {
    Unlock();
}

bool sySafeMutexLocker::Lock() {
    if(IsLocked()) {
        return true;
    }
    return m_Mutex.Lock(m_Aborter);
}

bool sySafeMutexLocker::SafeLock() {
    if(IsLocked()) { // Returns true if the mutex is locked by us
        return true;
    }
    return m_Mutex.SafeLock();
}

void sySafeMutexLocker::Unlock() {
    if(IsLocked()) m_Mutex.Unlock();
}

bool sySafeMutexLocker::IsLocked() {
    return (m_Mutex.m_Data->m_Owner == syThread::GetCurrentId());
}

// -------------------
// Begin syMutex class
// -------------------

syMutex::syMutex() {
    m_Data = new syMutexData;
    #ifdef __WIN32__
        InitializeCriticalSection(&m_Data->m_mutexobj);
    #else
        pthread_mutex_init(&m_Data->m_mutexobj,NULL);
    #endif
}

syMutex::~syMutex() {
    #ifdef __WIN32__
        DeleteCriticalSection(&m_Data->m_mutexobj);
    #else
        pthread_mutex_destroy(&m_Data->m_mutexobj);
    #endif
    delete m_Data;
}

void syMutex::Lock() {
    #ifdef __WIN32__
        EnterCriticalSection(&m_Data->m_mutexobj);
    #else
        pthread_mutex_lock(&m_Data->m_mutexobj);
    #endif
}

void syMutex::Unlock() {
    #ifdef __WIN32__
        LeaveCriticalSection(&m_Data->m_mutexobj);
    #else
        pthread_mutex_unlock(&m_Data->m_mutexobj);
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
        // Since this is *NOT* an atomic operation, first set the flag in case something happens.
        // This way, the destructor might try to unlock an already unlocked mutex,
        // where nothing bad happens, unlike not unlocking a locked mutex.
        m_locked = true;
        m_mutex->Lock();
    }
}
void syMutexLocker::Unlock() {
    if(m_locked) {
        // To make this non-atomic operation safe, first unlock the mutex and then set the flag to false.
        m_mutex->Unlock();
        m_locked = false;
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

syCondition::syCondition(syMutex& mutex)
{
    m_Data = new syCondData(mutex);
    #ifdef __WIN32__
    m_Data->m_numWaiters = 0;
    m_Data->m_isOk = true;
    #else
    int err = pthread_cond_init(&m_Data->m_cond, NULL);
    m_Data->m_isOk = (err == 0);
    #endif
}

syCondition::~syCondition() {
    #ifndef __WIN32__
    if ( m_Data->m_isOk ) {
        pthread_cond_destroy(&m_Data->m_cond);
    }
    #endif
    delete m_Data;
}

bool syCondition::IsOk() const {
    return m_Data->m_isOk;
}

syCondError syCondition::Wait() {
    syCondError result = syCOND_MISC_ERROR;
    #ifdef __WIN32__
    // increment the number of waiters
    {
        syMutexLocker lock(m_Data->m_csWaiters);
        ++(m_Data->m_numWaiters);
    }
    m_Data->m_mutex.Unlock();

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
    sySemaError err = m_Data->m_semaphore.Wait();
    m_Data->m_mutex.Lock();

    if ( err == sySEMA_NO_ERROR )
        result = syCOND_NO_ERROR;
    else if ( err == sySEMA_TIMEOUT )
        result = syCOND_TIMEOUT;
    else
        result = syCOND_MISC_ERROR;
    #else
    int err = pthread_cond_wait(&m_Data->m_cond, m_Data->GetPMutex());
    result = (err != 0) ? syCOND_MISC_ERROR : syCOND_NO_ERROR;
    #endif
    return result;
}

syCondError syCondition::WaitTimeout(unsigned long msec) {
    syCondError result = syCOND_MISC_ERROR;
    #ifdef __WIN32__
    {
        syMutexLocker lock(m_Data->m_csWaiters);
        m_Data->m_numWaiters++;
    }
    m_Data->m_mutex.Unlock();

    // a race condition can occur at this point in the code
    // please see the comments in Wait(), for details

    sySemaError err = m_Data->m_semaphore.WaitTimeout(msec);

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
        syMutexLocker lock(m_Data->m_csWaiters);
        sySemaError err2 = m_Data->m_semaphore.WaitTimeout(0);

        if ( err2 != sySEMA_NO_ERROR ) {
            m_Data->m_numWaiters--;
        }
    }
    m_Data->m_mutex.Lock();
    result = (err == sySEMA_NO_ERROR) ? syCOND_NO_ERROR :
             ((err == sySEMA_TIMEOUT) ? syCOND_TIMEOUT : syCOND_MISC_ERROR);
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

    int err = pthread_cond_timedwait( &m_Data->m_cond, m_Data->GetPMutex(), &tspec );
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
    #endif
    return result;
}

syCondError syCondition::Signal() {
    #ifdef __WIN32__
    syMutexLocker lock(m_Data->m_csWaiters);
    if (m_Data->m_numWaiters > 0) {
        // increment the semaphore by 1
        if ( m_Data->m_semaphore.Post() != sySEMA_NO_ERROR )
            return syCOND_MISC_ERROR;
        m_Data->m_numWaiters--;
    }
    #else
    int err = pthread_cond_signal(&m_Data->m_cond);
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    #endif
    return syCOND_NO_ERROR;
}

syCondError syCondition::Broadcast() {
    #ifdef __WIN32__
    syMutexLocker lock(m_Data->m_csWaiters);
    while (m_Data->m_numWaiters > 0) {
        if (m_Data->m_semaphore.Post() != sySEMA_NO_ERROR)
            return syCOND_MISC_ERROR;
        m_Data->m_numWaiters--;
    }
    #else
    int err = pthread_cond_broadcast(&m_Data->m_cond);
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

sySemaphore::sySemaphore(int initialcount, int maxcount) {
    m_Data = new sySemData;
    #ifdef __WIN32__
        if ( maxcount == 0 ) {
            // make it practically infinite
            maxcount = INT_MAX;
        }
        m_Data->m_semaphore = ::CreateSemaphore(NULL, initialcount, maxcount, NULL);
    #else
        m_Data->m_cond = new syCondition(m_Data->m_mutex);
        if ( maxcount == 0 ) {
            // make it practically infinite
            maxcount = INT_MAX;
        }
        if ( (initialcount < 0 || maxcount < 0) ||
                ((maxcount > 0) && (initialcount > maxcount)) ) {
            m_Data->m_isOk = false; // Invalid initial counts
        } else {
            m_Data->m_MaxCount = (size_t)maxcount;
            m_Data->m_Count = (size_t)initialcount;
        }

        m_Data->m_isOk = m_Data->m_cond->IsOk();
    #endif
}

sySemaphore::~sySemaphore() {
    #ifdef __WIN32__
        if (m_Data->m_semaphore) { ::CloseHandle(m_Data->m_semaphore); }
    #else
    #endif
    delete m_Data;
}

sySemaError sySemaphore::Post() {
    sySemaError result = sySEMA_NO_ERROR;
    #ifdef __WIN32__
    if (!::ReleaseSemaphore(m_Data->m_semaphore, 1, NULL)) {
        if (::GetLastError() == ERROR_TOO_MANY_POSTS) {
            result = sySEMA_OVERFLOW;
        } else {
            result = sySEMA_MISC_ERROR;
        }
    }
    #else
    syMutexLocker lock(m_Data->m_mutex);

    if ( m_Data->m_MaxCount > 0 && m_Data->m_Count == m_Data->m_MaxCount ) {
        result = sySEMA_OVERFLOW;
    } else {
        ++(m_Data->m_Count);
        result = (m_Data->m_cond->Signal() == syCOND_NO_ERROR) ? sySEMA_NO_ERROR : sySEMA_MISC_ERROR;
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
    syMutexLocker locker(m_Data->m_mutex);
    if (m_Data->m_Count == 0) { return sySEMA_BUSY; }
    m_Data->m_Count--;
    return sySEMA_NO_ERROR;
    #endif
}

sySemaError sySemaphore::Wait() {
    #ifdef __WIN32__
    return WaitTimeout(INFINITE);
    #else
    syMutexLocker locker(m_Data->m_mutex);

    while ( m_Data->m_Count == 0 ) {
        if ( m_Data->m_cond->Wait() != syCOND_NO_ERROR )
            return sySEMA_MISC_ERROR;
    }
    m_Data->m_Count--;
    return sySEMA_NO_ERROR;
    #endif
}

sySemaError  sySemaphore::WaitTimeout(unsigned long timeout_msec) {
    sySemaError result;
    #ifdef __WIN32__
    DWORD rc;
    rc = ::WaitForSingleObject(m_Data->m_semaphore, timeout_msec);
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
    syMutexLocker locker(m_Data->m_mutex);
    unsigned long startTime = syGetTicks();
    result = sySEMA_NO_ERROR;
    while(m_Data->m_Count == 0) {
        unsigned long elapsed = syGetTicks() - startTime;
        long remainingTime = (long)timeout_msec - (long)elapsed;
        if (remainingTime <= 0) {
            return sySEMA_TIMEOUT;
        }
        switch (m_Data->m_cond->WaitTimeout(remainingTime)) {
            case syCOND_TIMEOUT:
                result = sySEMA_TIMEOUT;
                break;
            default:
                result = sySEMA_MISC_ERROR;
            case syCOND_NO_ERROR:
                ;
        }
    }
    m_Data->m_Count--;
    #endif
    return result;
}

// ---------------------
// End sySemaphore class
// ---------------------

// ----------------------------------------------------------
// Begin syThread auxiliary functions (courtesy of wxWidgets)
// ----------------------------------------------------------

void syThreadExiter::OnExit() {
    if(m_Thread) {
        syThread* thread = m_Thread;
        m_Thread = NULL;
        thread->OnExit();
    }
}

/** @brief Deletes a thread. */
static void syDeleteThread(syThread *This) {
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

    int rc = -1;
    if(!thread) { return rc; }

    // Set up the indexing so that syThread::This() will work.
    #ifdef __WIN32__
    rc = (::TlsSetValue(gs_keySelf, thread)) ? 0 : -1;
    #else
    rc = pthread_setspecific(gs_keySelf, thread);
    if(rc != 0) { rc = -1; }
    #endif
    if(rc == 0) {
        rc = -1;
        syThreadData::InternalEntry(thread, rc);
    }
    return rc;
}

static bool syScheduleThreadForDeletion(syThread* thread = NULL, bool dontlock = false) {
    // This method is called from both syThread::Delete() and syThread::Exit(),
    // Therefore, we need to use a mutex so that whoever comes later does no harm.

    if(!thread) { thread = syThread::This(); }

    bool result;
    if(!dontlock) {
        // This part is called by syThread::Exit and syThread:Kill
        // We must lock gs_mutexDeleteThread here, as it hasn't been locked by those methods.
        syMutexLocker lock(*gs_mutexDeleteThread); // First, use a global mutex
        {
            // Now we check if the thread was present in the detached threads list.
            syMutexLocker lock(*gs_mutexAllThreads);
            result = (gs_DetachedThreads.erase(thread) == 1);
        }
    } else {
        // This part is called by syThread::Delete, which has already locked gs_mutexDeleteThread.
        syMutexLocker lock(*gs_mutexAllThreads);
        result = (gs_DetachedThreads.erase(thread) == 1);
    }
    if(result) {
        gs_nThreadsBeingDeleted++;
    }
    return result;
}

bool IsThreadDetached(syThread* thread) {
    syMutexLocker lock(*gs_mutexAllThreads);
    std::set<syThread*>::iterator i = gs_JoinableThreads.find(thread);

    // it's not in the joinable threads list, assume it's a detached thread.
    bool result = (i == gs_JoinableThreads.end());
    return result;
}

// ---------------------------------------------------
// Begin syThread class (mostly wxWidgets, a bit mine)
// ---------------------------------------------------

syThread::syThread(syThreadKind kind) {
    m_Data = new syThreadData(kind);
    syMutexLocker lock(*gs_mutexAllThreads);
    if(kind == syTHREAD_JOINABLE) {
        gs_JoinableThreads.insert(this);
    }
    // We don't add detached threads here because they do their own deletion. Hence,
    // the addition must be done AFTER the thread has been created.
}

syThread::~syThread() {
    syMutexLocker lock(*gs_mutexAllThreads);
    if(m_Data->m_ThreadKind == syTHREAD_JOINABLE) {
        gs_JoinableThreads.erase(this);
    } else {
        gs_DetachedThreads.erase(this);
    }
    delete m_Data;
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
    // NOTE: syThread::GetCPUCount needs to be implemented
    return -1;
}

unsigned long syThread::GetMainThreadId() {
    return syMainThreadId;
}

bool syThread::IsMain() {
    return (syThread::GetCurrentId() == syMainThreadId);
}

unsigned long syThread::GetId() const {
    return m_Data->m_ThreadId;
}

syThreadError syThread::Create(unsigned int stackSize) {
    if(!gs_ThreadsOk) {
        return syTHREAD_NO_RESOURCE; // Cannot create threads: Error initializing global variables!
    }
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_EXITED) {
        m_Data->m_ThreadStatus = syTHREADSTATUS_NOT_CREATED;
    }
    if(m_Data->m_ThreadStatus != syTHREADSTATUS_NOT_CREATED) {
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

        m_Data->m_hThread = (HANDLE)_beginthreadex(
                          NULL,             // default security
                          stackSize,
                          syWinThreadStart, // entry point
                          (void *)this,     // Parameter (this) for syWinThreadStart
                          0,                // Create as Running. Our implementation differs from wx's in
                                            // that we follow more the posix implementation, which IMHO
                                            // is much cleaner than wxmsw. So we let the thread run
                                            // and then it'll freeze automatically with our semaphore.
                          (unsigned int *)&m_Data->m_ThreadId
                        );

        success = ( m_Data->m_hThread != NULL );
    #else
        // For posix, we initialize a pthread attribute variable, and then we create the thread with that.
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        if (stackSize) { pthread_attr_setstacksize(&attr, stackSize); }
        // Try to set the threads really concurrent.
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        if (pthread_attr_getschedpolicy(&attr, &m_Data->m_Policy) != 0) {
            m_Data->m_Policy = -1; // Couldn't get the OS' scheduling policy!
        }
        {
            syMutexLocker lock(m_Data->m_csJoinFlag);
            m_Data->m_ShouldBeJoined = (m_Data->m_ThreadKind != syTHREAD_DETACHED);
        }

        if (!m_Data->m_ShouldBeJoined) {
            // Try to create the thread detached
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        }
        int rc = pthread_create(&m_Data->m_ThreadId, &attr, syPthreadStart, (void *)this);

        pthread_attr_destroy(&attr); // Destroy the thread attribute or we'd leak resources.
        success = (rc == 0);
    #endif
    if(!success) {
        m_Data->m_ThreadStatus = syTHREADSTATUS_NOT_CREATED;
        return syTHREAD_NO_RESOURCE; // Error creating the thread!
    }

    syMutexLocker lock(m_Data->m_Mutex);
    m_Data->m_ThreadStatus = syTHREADSTATUS_CREATED;
    lock.Unlock();
    if (m_Data->m_Priority != SYTHREAD_DEFAULT_PRIORITY) {
        SetPriority(m_Data->m_Priority);
    }
    m_Data->m_StackSize = stackSize;
    return syTHREAD_NO_ERROR;
}

void syThreadData::InternalEntry(syThread* thread, int& rc) {
    bool dontRunAtAll;

    if(thread->IsDetached()) {
        // Detached threads follow their own deletion logic. Deleting a detached thread
        // schedules it for deletion, but if the thread was added to the list before being actually
        // created, the deletion will never take place, making syThreadModule wait for a signal
        // that will NEVER come.
        syMutexLocker lock(*gs_mutexAllThreads);
        gs_DetachedThreads.insert(thread);
    }

    {
        syMutexLocker lock(thread->m_Data->m_Mutex);

        // When created, the thread must wait before running.
        if(thread->m_Data->m_ThreadStatus != syTHREADSTATUS_RUNNING) {
            thread->m_Data->m_ResumeCondition.Wait();
        }

        // After the thread has started, we need to check whether it's safe to run or not.
        // We add a check for gs_ThreadsOk (the thread will stop if the application is being shut down)
        dontRunAtAll = (!gs_ThreadsOk || thread->m_Data->m_StopRequested);
    }
    rc = -1; // Default exit value in case dontRunAtAll is true
    if(!dontRunAtAll) {
        syThreadExiter exiter(thread);
        // Set the OnExit handler
        thread->m_Data->m_Exiter = &exiter;

        // call the main entry
        int exitcode = thread->Entry();
        {
            syMutexLocker lock(thread->m_Data->m_Mutex);
            thread->m_Data->m_ThreadStatus = syTHREADSTATUS_TERMINATING;
            thread->m_Data->m_PausedCondition.Signal(); // Just in case someone's waiting for us
        }
        thread->Exit(exitcode); // Note: Exit should terminate the thread and not return at all.
        #ifdef __WIN32__
        rc = exitcode;
        #else
        rc = 0; // If the thread didn't exit, return 0. syThreadStart will know what to do with it.
        #endif
    } else if(thread->IsDetached()) {
        // Cleanup
        syScheduleThreadForDeletion(thread);
        syDeleteThread(thread);
        #ifdef __WIN32__
        ::TlsSetValue(gs_keySelf, (LPVOID)0);
        #else
        pthread_setspecific(gs_keySelf, 0);
        #endif
    }
}

void syThread::Exit(int exitcode) {
    if(!IsThisThread()) { return; } // Exit() can only be called by the same thread context
    m_Data->m_ExitCode = exitcode;
    bool isdetached = IsDetached();
    if(isdetached) {
        // From the moment we call OnExit(), the main program may terminate at
        // any moment, and syThreadModule::~syThreadModule() will try to delete the thread.

        // Addition by Rick: The original wxWidgets code just set up a counter and a Condition Var,
        // but here we actually remove the thread from the detached threads' list,
        // making syThread::Delete() effectively harmless.
        syScheduleThreadForDeletion();
    }

    if(m_Data->m_Exiter) {
        m_Data->m_Exiter->OnExit();
    } else {
        OnExit();
    }

    if(isdetached) {
        syDeleteThread(this); // From now on, accessing "this" is forbidden!
        #ifdef __WIN32__
        ::TlsSetValue(gs_keySelf, (LPVOID)0);
        #else
        pthread_setspecific(gs_keySelf, 0);
        #endif
    } else {
        m_Data->m_ThreadStatus = syTHREADSTATUS_EXITED;
    }

    // terminate the thread (pthread_exit() never returns)
    // We already set m_ExitCode, so let's just ignore the value.
    #ifdef __WIN32__
    _endthreadex((unsigned)0);
    #else
    pthread_exit(NULL);
    #endif
}


bool syThread::IsAlive() {
    syMutexLocker lock(m_Data->m_Mutex);
    return (m_Data->m_ThreadStatus == syTHREADSTATUS_CREATED) ||
    (m_Data->m_ThreadStatus == syTHREADSTATUS_PAUSED) ||
    (m_Data->m_ThreadStatus == syTHREADSTATUS_RUNNING);
}



bool syThread::IsDetached() const {
    return (m_Data->m_ThreadKind == syTHREAD_DETACHED);
}

bool syThread::IsPaused() {
    syMutexLocker lock(m_Data->m_Mutex);
    return (m_Data->m_ThreadStatus == syTHREADSTATUS_PAUSED);
}

bool syThread::IsRunning() {
    syMutexLocker lock(m_Data->m_Mutex);
    return (m_Data->m_ThreadStatus == syTHREADSTATUS_RUNNING);
}

syThreadError syThread::Kill() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }

    switch (m_Data->m_ThreadStatus) {
        case syTHREADSTATUS_NOT_CREATED:
        case syTHREADSTATUS_EXITED:
        case syTHREADSTATUS_KILLED:
            return syTHREAD_NOT_RUNNING;

        case syTHREADSTATUS_PAUSED:
            // resume the thread first
            Resume();

        default:
            // Kill thread
            #ifdef __WIN32__
            if(!::TerminateThread(m_Data->m_hThread, 0xFFFFFFFF)) {
                return syTHREAD_MISC_ERROR;
            }
            #else
            if (pthread_cancel(m_Data->m_ThreadId)!= 0) {
                return syTHREAD_MISC_ERROR;
            }
            #endif
            m_Data->m_ThreadStatus = syTHREADSTATUS_KILLED;

            if (m_Data->m_ThreadKind == syTHREAD_DETACHED) {
                syScheduleThreadForDeletion();
                syDeleteThread(this); // Delete the thread
            } else {
                m_Data->m_ExitCode = -1;
            }
            return syTHREAD_NO_ERROR;
    }
    return syTHREAD_MISC_ERROR;
}

syThreadError syThread::Pause() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    if(!IsRunning()) { return syTHREAD_NOT_RUNNING; }

    syMutexLocker lock(m_Data->m_Mutex);
    syBoolSetter setter(m_Data->m_PauseRequested,true);
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_RUNNING) {
        // Wait unlocks our mutex before waiting, and locks it on exit.
        // This saves us a lot of trouble and allows us to simplify our code.
        m_Data->m_PausedCondition.Wait();
    }
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_KILLED) return syTHREAD_KILLED;
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_PAUSED) return syTHREAD_NO_ERROR;
    return syTHREAD_MISC_ERROR; // Could not pause the thread!
}

syThreadError syThread::Run() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }

    syThreadError result;
    syMutexLocker lock(m_Data->m_Mutex);
    if(m_Data->m_ThreadKind == syTHREAD_JOINABLE && m_Data->m_ThreadStatus == syTHREADSTATUS_EXITED) {
        lock.Unlock();
        if(Create(m_Data->m_StackSize) != syTHREAD_NO_ERROR) {
            return syTHREAD_NO_RESOURCE;
        }
        lock.Lock();
        m_Data->m_ThreadStatus = syTHREADSTATUS_CREATED;
        m_Data->m_PauseRequested = false;
        m_Data->m_StopRequested = false;
    }
    if(m_Data->m_ThreadStatus != syTHREADSTATUS_CREATED) {
        result = syTHREAD_RUNNING;
    } else {
        m_Data->m_ThreadStatus = syTHREADSTATUS_RUNNING;
        result = syTHREAD_NO_ERROR;
        m_Data->m_ResumeCondition.Signal();
    }
    return result;
}

void syThread::SetPriority(int priority) {
    syMutexLocker lock(m_Data->m_Mutex);
    switch(m_Data->m_ThreadStatus) {
        case syTHREADSTATUS_CREATED:
        case syTHREADSTATUS_PAUSED:
        case syTHREADSTATUS_RUNNING:
        break;
        default:
            return; // Cannot set priority in other cases
    }
    if(priority < 0) { priority = 0; }
    if(priority > 100) { priority = 100; }
    m_Data->m_Priority = priority;
    int os_priority;
    #ifdef __WIN32__
        if (m_Data->m_Priority <= 20)
            os_priority = THREAD_PRIORITY_LOWEST;
        else if (m_Data->m_Priority <= 40)
            os_priority = THREAD_PRIORITY_BELOW_NORMAL;
        else if (m_Data->m_Priority <= 60)
            os_priority = THREAD_PRIORITY_NORMAL;
        else if (m_Data->m_Priority <= 80)
            os_priority = THREAD_PRIORITY_ABOVE_NORMAL;
        else
            os_priority = THREAD_PRIORITY_HIGHEST;
    #else
        int min_prio = -1;
        int max_prio = -1;
        if(m_Data->m_Policy >= 0) {
            min_prio = sched_get_priority_min(m_Data->m_Policy);
            max_prio = sched_get_priority_max(m_Data->m_Policy);
        }
        if ( min_prio == -1 || max_prio == -1 || max_prio == min_prio) {
            // Priority setting is being ignored by the OS; Set it to default and return
            m_Data->m_Priority = 50;
            return;
        }
        os_priority = min_prio + (m_Data->m_Priority*(max_prio - min_prio))/100;
    #endif

    // Now that we have calculated the os-based priority, let's apply it.

    #ifdef __WIN32__
        ::SetThreadPriority(m_Data->m_hThread, os_priority);
    #else
        pthread_setschedprio(m_Data->m_ThreadId, os_priority);
    #endif
}

int syThread::GetPriority() {
    syMutexLocker lock(m_Data->m_Mutex);
    return m_Data->m_Priority;
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
    syMutexLocker lock(m_Data->m_Mutex);
    syThreadError result;
    switch(m_Data->m_ThreadStatus) {
        case syTHREADSTATUS_PAUSED:
            m_Data->m_PauseRequested = false;
            m_Data->m_ResumeCondition.Signal();
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

bool syThread::SetConcurrency(unsigned int level) {
    return false; // This function is kept only for wxWidgets API compatibility
}

bool syThread::MustAbort() {
    syThread* thread = syThread::This();
    if(!thread) { return false; }
    return thread->TestDestroy();
}

bool syThread::SelfPause() {
    if(!IsThisThread()) return false; // Not our thread
    if(!gs_ThreadsOk || m_Data->m_StopRequested) {
        return true; // No need to lock mutex, must stop.
    }
    syMutexLocker lock(m_Data->m_Mutex);
    m_Data->m_PauseRequested = true;
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_RUNNING) {
        if(m_Data->m_ThreadStatus!= syTHREADSTATUS_RUNNING) return false;
        m_Data->m_ThreadStatus = syTHREADSTATUS_PAUSED;
        m_Data->m_PausedCondition.Signal();
        m_Data->m_ResumeCondition.Wait();
        m_Data->m_ThreadStatus = syTHREADSTATUS_RUNNING;
    }
    return (!gs_ThreadsOk || m_Data->m_StopRequested);
}

bool syThread::TestDestroy() {
    if(!IsThisThread()) return false; // Not our thread
    if(!gs_ThreadsOk || m_Data->m_StopRequested) {
        return true; // No need to lock mutex, must stop.
    }
    syMutexLocker lock(m_Data->m_Mutex);
    if(m_Data->m_PauseRequested && (m_Data->m_ThreadStatus == syTHREADSTATUS_RUNNING)) {
        if(m_Data->m_ThreadStatus!= syTHREADSTATUS_RUNNING) return false;
        m_Data->m_ThreadStatus = syTHREADSTATUS_PAUSED;
        m_Data->m_PausedCondition.Signal();
        m_Data->m_ResumeCondition.Wait();
        m_Data->m_ThreadStatus = syTHREADSTATUS_RUNNING;
    }
    return (!gs_ThreadsOk || m_Data->m_StopRequested);
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
    #ifdef __WIN32__
    ::SwitchToThread();
    #else
    ::sched_yield();
    #endif
}

int syThread::Wait() {
    // Are we opening a rift in the timespace continuum?
    if(IsThisThread()) { return -1; }

    // Waiting for a detached thread is a very stupid thing to do. Even checking the thread kind
    // might give you a segfault if the thread was already deleted, and we DON'T want to access
    // the global threads list, causing a major slowdown due to the use of global mutexes.
    if(IsThreadDetached(this)) {
        return -1;
    }

    // First, make sure the thread is running and not paused.
    {
        syMutexLocker lock(m_Data->m_Mutex);

        syThreadStatus status = m_Data->m_ThreadStatus;
        m_Data->m_PauseRequested = false;
        // NO need to wait for a non created thread.
        if(status == syTHREADSTATUS_NOT_CREATED) { return m_Data->m_ExitCode; }

        // Run or unpause the thread (we can't stop it if it keeps waiting)
        if(status == syTHREADSTATUS_CREATED || status == syTHREADSTATUS_PAUSED) {
            m_Data->m_ResumeCondition.Signal();
        }
    }
    long int rc = 0;
    // Now, let's join the threads
    #ifdef __WIN32__
        rc = ::WaitForSingleObject(m_Data->m_hThread, INFINITE);
        if((unsigned long)rc == 0xFFFFFFFF) {
            // An error occurred and we must kill the thread.
            Kill();
            m_Data->m_ThreadStatus = syTHREADSTATUS_KILLED;
        } else {
            // Windows has this annoying behavior: It doesn't always do what you ask.
            // Even if you asked to WAIT for the thread to terminate, sometimes it doesn't!
            // So you have to keep waiting until the thread terminates on its own. Sheesh.
            for(;;) {
                if (!::GetExitCodeThread(m_Data->m_hThread, (LPDWORD)&rc)) {
                    m_Data->m_ExitCode = -1;
                    break;
                }
                // From the WinAPI documentation:
                // ---
                // Important
                // The GetExitCodeThread function returns a valid error code defined by the application
                // only after the thread terminates. Therefore, an application should not use
                // STILL_ACTIVE (259) as an error code.
                // ---
                // Talk about good API design. Pfft.
                // Fortunately, we already set m_ExitCode so we can ignore this bad api design.
                //
                if ((DWORD)rc != STILL_ACTIVE) {
                    break;
                }
                syMilliSleep(1);
            }
        }
    #else
        {
            syMutexLocker lock(m_Data->m_csJoinFlag);
            if(m_Data->m_ShouldBeJoined) {
                pthread_join(GetId(), (void**)&rc); // Wait for the thread to end.
                // We don't need to get the return value to m_ExitCode because the thread already set it.
                m_Data->m_ShouldBeJoined = false;
            }
        }
    #endif
    if(m_Data->m_ThreadStatus == syTHREADSTATUS_KILLED) { return -1; }
    return m_Data->m_ExitCode;
}

syThreadError syThread::Delete(int* rc) {
    if(IsThisThread()) {
        return syTHREAD_WRONG_THREAD_CONTEXT; // A thread can't delete itself!
    }

    // Under wxWidgets, checking if a thread was joinable could segfault if the thread
    // had already been deleted (we would be accessing non-allocated memory).
    // IsThreadDetached simply looks for the thread in a global list of joinable threads,
    // so it's 100% safe, even if the thread had already been deleted.
    bool isdetached = IsThreadDetached(this);
    syThreadStatus status = syTHREADSTATUS_EXITED;

    {
        // To prevent a race condition between deleting a thread here and the thread deleting itself,
        // we lock gs_mutexDeleteThread and then call syScheduleThreadForDeletion.
        // The function returns true if the thread hadn't been scheduled first.
        syMutexLocker lock(*gs_mutexDeleteThread);
        bool coulddelete = false;
        if(isdetached) {
            // The second parameter tells syScheduleThreadForDeletion that we've already locked gs_mutexDeleteThread.
            coulddelete = syScheduleThreadForDeletion(this,true);
        }

        if(!isdetached || coulddelete) {
            // The thread hasn't been deleted yet! It means we can safely access
            // its mutex and member variables.

            syMutexLocker lock(m_Data->m_Mutex); // Now we lock m_Mutex to check the thread's status
            status = m_Data->m_ThreadStatus;
            m_Data->m_PauseRequested = false;
            m_Data->m_StopRequested = true;

            // With the thread status available, we can know if we need to Run or Resume the thread.
            // However, we MUST NOT call Run() or Resume() because that would alter the
            // m_PauseRequested and m_StopRequested flags we just changed.

            if(status == syTHREADSTATUS_CREATED || status == syTHREADSTATUS_PAUSED) {
                m_Data->m_ResumeCondition.Signal(); // m_Mutex is already locked, so we don't need to lock it here.
            }
        }

        // If the thread is detached, now it will try to delete itself, not before
        // attempting a lock on gs_mutexDeleteThread which we have locked right now.
        // So, we free the lock from the stack and the detached thread will be deleted any moment now.
    }

    if (!isdetached) {
        // For joinable threads, wait until the thread stops;
        // We can't wait for detached threads.
        if(status != syTHREADSTATUS_EXITED) { Wait(); }
        if(rc) { *rc = m_Data->m_ExitCode; } // return the thread's exit code
    }

    return syTHREAD_NO_ERROR;
}

bool syThread::IsThisThread() const {
    return (this && this == This());
}


// ------------------------------------
// Begin syThread initialization module
// ------------------------------------

class syThreadModule {
    public:
        syThreadModule();
        bool OnInit();
        ~syThreadModule();
};

syThreadModule::syThreadModule() {
    // Set a flag indicating that thread variables were initialized correctly. Used by syThread::Create
    gs_ThreadsOk = OnInit();
}

bool syThreadModule::OnInit() {
    bool result;
    // allocate TLS index for storing the pointer to the current thread
    #ifdef __WIN32__
        gs_keySelf = ::TlsAlloc();
        result = (gs_keySelf != 0xFFFFFFFF);
        // main thread doesn't have associated wxThread object, so store 0 in the
        // TLS instead
        if ( !::TlsSetValue(gs_keySelf, (LPVOID)0) ) {
            ::TlsFree(gs_keySelf);
            gs_keySelf = 0xFFFFFFFF;
            result = false; // Error initializing thread module!!
        }
    #else
        int rc = pthread_key_create(&gs_keySelf, NULL);
        result = (rc == 0);
    #endif

    if(result) {
        // Initialize thread global variables
        gs_mutexAllThreads = new syMutex();
        gs_mutexDeleteThread = new syMutex();
        gs_condAllDeleted = new syCondition(*gs_mutexDeleteThread);
    }
    return result;
}

syThreadModule::~syThreadModule() {
    {
        syMutexLocker lock(*gs_mutexAllThreads);

        if(!syThread::IsMain()) { return; }
        if(!gs_ThreadsOk) { return; } // Thread globals weren't initialized correctly; No threads to delete.

        // Just in case, prevent new threads from being created by currently running threads ;-)
        gs_ThreadsOk = false;
    }

    // are there any threads left which are being deleted right now?
    size_t nThreadsBeingDeleted;
    {
        syMutexLocker lock( *gs_mutexDeleteThread );
        nThreadsBeingDeleted = gs_nThreadsBeingDeleted;

        if (nThreadsBeingDeleted > 0) {
            gs_condAllDeleted->Wait();
        }
    }

    std::set<syThread*>::iterator i;

    // First, delete all joinable threads.
    for(;;) {
        syThread* thread;
        {
            syMutexLocker lock(*gs_mutexAllThreads);
            if(gs_JoinableThreads.empty()) {
                break;
            }
            i = gs_JoinableThreads.begin();
             thread = *i;
        }
        if(thread) {
            thread->Delete();
            delete thread; // Delete only deletes the OS thread, we have to delete the C++ object too.
        }
    }

    // Now for the detached threads.
    for(;;) {
        syThread* thread;
        {
            syMutexLocker lock(*gs_mutexAllThreads);
            if(gs_DetachedThreads.empty()) {
                break;
            }
            i = gs_DetachedThreads.begin();
             thread = *i;
        }
        if(thread) {
            thread->Delete();
        }
    }

    // Let's repeat the thread deletion check
    {
        syMutexLocker lock( *gs_mutexDeleteThread );
        nThreadsBeingDeleted = gs_nThreadsBeingDeleted;

        if (nThreadsBeingDeleted > 0) {
            gs_condAllDeleted->Wait();
        }
    }

    delete gs_mutexAllThreads;
    delete gs_condAllDeleted;
    delete gs_mutexDeleteThread;

    // free TLD slot
    #ifdef __WIN32__
        TlsFree(gs_keySelf);
    #else
        (void)pthread_key_delete(gs_keySelf);
    #endif
}

// ----------------------------------
// End syThread initialization module
// ----------------------------------

// Initialize threads module
syThreadModule StaticThreadModule;
