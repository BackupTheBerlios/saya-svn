/***************************************************************
 * Name:      sythread.cpp
 * Purpose:   Implementation of a Cross-platform Thread class
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
 *
 **************************************************************/

#include "sythread.h"

#ifndef __WIN32__
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <sys/time.h>
    #include <errno.h>
#endif
#include <limits.h>

unsigned long syGetTime();

unsigned long syMainThreadId = syThread::GetThreadId();
unsigned long sySecondsAtInit = syGetTime();

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

// ---------------------------------------
// Begin syCondition class (POSIX only)
// This class was almost copied completely
// From the wxwidgets threadpsx.cpp
// See the copyright notice for details.
// ---------------------------------------

#ifndef __WIN32__
syCondition::syCondition(syMutex& mutex) :
m_mutex(mutex)
{
    int err = pthread_cond_init(&m_cond, NULL);
    m_isOk = (err == 0);
}

syCondition::~syCondition() {
    if ( m_isOk ) {
        /* int err = */ pthread_cond_destroy(&m_cond);
    }
}

bool syCondition::IsOk() const {
    return m_isOk;
}

syCondError syCondition::Wait() {
    int err = pthread_cond_wait(&m_cond, GetPMutex());
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    return syCOND_NO_ERROR;
}

syCondError syCondition::WaitTimeout(unsigned long msec) {
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
    syCondError result = syCOND_MISC_ERROR;
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
}

syCondError syCondition::Signal() {
    int err = pthread_cond_signal(&m_cond);
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    return syCOND_NO_ERROR;
}

syCondError syCondition::Broadcast() {
    int err = pthread_cond_broadcast(&m_cond);
    if (err != 0) {
        return syCOND_MISC_ERROR;
    }
    return syCOND_NO_ERROR;
}

#endif

// End syCondition class

// -----------------------
// Begin sySemaphore class
// -----------------------

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

// End sySemaphore class


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

// --------------------
// Begin syThread class
// --------------------

syThread::syThread(syThreadKind kind) :
m_ThreadId(0),
m_ThreadKind(kind),
m_ThreadStatus(syTHREADSTATUS_NOT_CREATED),
m_PauseRequested(false),
m_StopRequested(false),
m_ExitCode(0)
{
    m_StatusMutex = new syMutex;
}

syThread::~syThread() {
    delete m_StatusMutex;
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
    // TODO: Implement syThread::Create
    return syTHREAD_MISC_ERROR;
}


bool syThread::IsAlive() const {
    syMutexLocker lock(*m_StatusMutex);
    return (m_ThreadStatus == syTHREADSTATUS_CREATED) ||
    (m_ThreadStatus == syTHREADSTATUS_PAUSED) ||
    (m_ThreadStatus == syTHREADSTATUS_RUNNING);
}

bool syThread::IsDetached() const {
    return (m_ThreadKind == syTHREAD_DETACHED);
}

bool syThread::IsPaused() const {
    syMutexLocker lock(*m_StatusMutex);
    return (m_ThreadStatus == syTHREADSTATUS_PAUSED);
}

bool syThread::IsRunning() const {
    syMutexLocker lock(*m_StatusMutex);
    return (m_ThreadStatus == syTHREADSTATUS_RUNNING);
}

syThreadError syThread::Kill() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    // TODO: Implement syThread::Kill
    return syTHREAD_MISC_ERROR;
}

void syThread::OnExit() {
    // TODO: Implement syThread::OnExit
}

syThreadError syThread::Pause() {
    if(IsThisThread()) { return syTHREAD_WRONG_THREAD_CONTEXT; }
    if(!IsRunning()) { return syTHREAD_NOT_RUNNING; }

    while(m_ThreadStatus == syTHREADSTATUS_RUNNING) {
        // There might be two threads trying to pause/resume a third thread.
        // To avoid a race condition, just keep requesting the pause.
        m_PauseRequested = true;
        syMicroSleep(50);
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
    m_StatusMutex->Lock();
    if(m_ThreadKind == syTHREAD_JOINABLE && m_ThreadStatus == syTHREADSTATUS_EXITED) {
        m_ThreadStatus = syTHREADSTATUS_CREATED;
        m_PauseRequested = false;
        m_StopRequested = false;
    }
    if(m_ThreadStatus != syTHREADSTATUS_CREATED) {
        result = syTHREAD_RUNNING;
        m_StatusMutex->Unlock();
    } else {
        m_ThreadStatus = syTHREADSTATUS_RUNNING;
        result = syTHREAD_NO_ERROR;
        m_StatusMutex->Unlock();
        m_StartSemaphore.Post();
    }
    return result;
}

void syThread::SetPriority(int priority) {
    if(m_ThreadStatus == syTHREADSTATUS_CREATED) {
        if(priority < 0) { priority = 0; }
        if(priority > 100) { priority = 100; }
        m_Priority = priority;
    }
}

int syThread::GetPriority() const {
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
    m_StatusMutex->Lock();
    syThreadError result;
    switch(m_ThreadStatus) {
        case syTHREADSTATUS_PAUSED:
            m_PauseRequested = false;
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
    m_StatusMutex->Unlock();
    return result;
}

bool syThread::SetConcurrency(size_t level) {
    return false; // This function is kept only for wxWidgets API compatibility
}

bool syThread::TestDestroy() {
    if(!IsThisThread()) return false;
    if(m_ThreadStatus != syTHREADSTATUS_RUNNING) return false;
    if(m_PauseRequested) {
        {
            syMutexLocker lock(*m_StatusMutex);
            if(m_ThreadStatus!= syTHREADSTATUS_RUNNING) return false;
            m_ThreadStatus = syTHREADSTATUS_PAUSED;
        }
        m_ResumeSemaphore.Wait();
        {
            syMutexLocker lock(*m_StatusMutex);
            m_ThreadStatus = syTHREADSTATUS_RUNNING;
            m_PauseRequested = false;
        }
    }
    return m_StopRequested;
}

syThread* syThread::This() {
    // TODO: Implement syThread::This()
    return NULL;
}

void syThread::Yield() {
    if(!IsThisThread() || IsMain()) { return; }
    #ifdef __WIN32__
    ::SwitchToThread();
    #else
    ::sched_yield();
    #endif
}

int syThread::Wait() const {
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
    {
        // First lock the mutex, and get the status.
        syMutexLocker lock(*m_StatusMutex);
        status = m_ThreadStatus;
        m_PauseRequested = false;
        m_StopRequested = true;
        isdetached = (m_ThreadKind == syTHREAD_DETACHED);
    }

    if(status == syTHREADSTATUS_CREATED) { Run(); } // Run the thread (we can't stop it if it keeps waiting)
    if(status == syTHREADSTATUS_PAUSED) { Resume(); } // Resume the thread for the same reason.
    if (!isdetached) {
        // For joinable threads, wait until the thread stops;
        // We can't wait for detached threads.
        if(status != syTHREADSTATUS_EXITED) { Wait(); }
        if(rc) { *rc = m_ExitCode; } // return the thread's exit code
    }
    return syTHREAD_NO_ERROR;
}

void syThread::Exit(int exitcode) {
    m_ExitCode = exitcode;
}

bool syThread::IsThisThread() const {
    return (this == This());
}
