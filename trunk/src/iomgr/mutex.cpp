/***************************************************************
 * Name:      mutex.h
 * Purpose:   Implementation of a Cross-platform Mutex class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   WxWindows License
 **************************************************************/

#include "mutex.h"

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

syMutexLocker::syMutexLocker(syMutex& mutex) {
    m_mutex = &mutex;
    m_mutex->Lock();
}

syMutexLocker::~syMutexLocker() {
    m_mutex->Unlock();
}
