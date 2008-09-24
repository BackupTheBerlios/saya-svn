/***************************************************************
 * Name:      avdevice.cpp
 * Purpose:   Implementation of the AVDevice class
 *            and related classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  AVDevice is a generic template for Audio/Video
 *            input/output buffering classes.
 **************************************************************/

#include "avdevice.h"
#include "sythread.h"
#include <cstddef>

AVDevice::AVDevice() :
m_InputMutex(NULL),
m_OutputMutex(NULL),
m_Stop(false),
m_SoftStop(false),
m_IsOk(false),
m_IsShuttingDown(false)
{
    m_InputMutex = new sySafeMutex;
    m_OutputMutex = new sySafeMutex;
}

AVDevice::~AVDevice() {
    delete m_OutputMutex;
    delete m_InputMutex;
}

bool AVDevice::Init() {
    if(!m_IsOk) {
        FreeResources();
        bool result = Connect();
        if(result) {
            result = AllocateResources();
        }
        m_IsOk = result;
    }
    return m_IsOk;
}

bool AVDevice::IsShuttingDown() const {
    return m_IsShuttingDown;
}

bool AVDevice::IsOk() const {
    return m_IsOk;
}

void AVDevice::ShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    while(!m_InputMutex->IsUnlocked() || !m_OutputMutex->IsUnlocked()) {
        m_InputMutex->Wait();
        m_OutputMutex->Wait();
    }
    Disconnect();
    FreeResources();
    m_IsOk = false;
    m_IsShuttingDown = false;
    m_Stop = false;
    m_SoftStop = false;

}

void AVDevice::StartShutDown() {
    m_IsShuttingDown = true;
}

void AVDevice::WaitForShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    while(!m_InputMutex->IsUnlocked() || !m_OutputMutex->IsUnlocked()) {
        m_InputMutex->Wait();
        m_OutputMutex->Wait();
    }
}

void AVDevice::FinishShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    if(m_InputMutex->IsUnlocked() && m_OutputMutex->IsUnlocked()) {
        Disconnect();
        FreeResources();
        m_IsOk = false;
        m_IsShuttingDown = false;
        m_Stop = false;
        m_SoftStop = false;
    }
}

bool AVDevice::IsPlaying() const {
    return !(m_InputMutex->IsUnlocked() && m_OutputMutex->IsUnlocked());
}

bool AVDevice::InnerMustAbort() const {
    return !m_IsOk || m_IsShuttingDown || m_Stop;
}

bool AVDevice::MustStop() const {
    return !m_IsOk || m_IsShuttingDown || m_Stop || m_SoftStop;
}

bool AVDevice::IsVideo() const {
    return m_IsVideo;
}

bool AVDevice::IsAudio() const {
    return m_IsAudio;
}

bool AVDevice::IsInput() const {
    return m_IsInput;
}

bool AVDevice::IsOutput() const {
    return m_IsOutput;
}

void AVDevice::Abort() const {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_Stop = true;
}

void AVDevice::Stop() const {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_SoftStop = true;
}
