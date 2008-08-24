/***************************************************************
 * Name:      mutex.h
 * Purpose:   Implementation of a Cross-platform Mutex class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 **************************************************************/

#include "mutex.h"

#ifndef __WIN32__
    #include <unistd.h>
    #include <sys/syscall.h>
    #include <sys/time.h>
#endif

unsigned long syGetTime();

unsigned long syMainThreadId = syMutex::GetThreadId();
unsigned long sySecondsAtInit = syGetTime();

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

unsigned long syMutex::GetThreadId() {
    #ifdef __WIN32__
        return (unsigned long)GetCurrentThreadId();
    #else
        return (unsigned long)syscall(__NR_gettid);
    #endif
}

unsigned long syMutex::GetMainThreadId() {
    return syMainThreadId;
}

/** Is the current thread the main thread? */
bool syMutex::IsMainThread() {
    return (syMutex::GetThreadId() == syMainThreadId);
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

syMutexLocker::syMutexLocker(syMutex& mutex) {
    m_mutex = &mutex;
    m_mutex->Lock();
}

syMutexLocker::~syMutexLocker() {
    m_mutex->Unlock();
}

void syMilliSleep(unsigned long msec) {
    #ifdef __WIN32__
    ::Sleep(msec);
    #else
    // The following code was adapted from http://cc.byexamples.com/20070525/nanosleep-is-better-than-sleep-and-usleep/
    // WARNING: Not tested
    // TODO: Remove the warning notice after this function has been tested successfully.
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
