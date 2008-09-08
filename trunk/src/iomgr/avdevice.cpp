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

AVDevice::AVDevice() :
m_Busy(NULL),
m_IsOk(false),
m_IsShuttingDown(false)
{
    m_Busy = new sySafeMutex;
}

AVDevice::~AVDevice() {
    delete m_Busy;
}

bool AVDevice::Init() {
    bool result = Connect();
    if(result) {
        result = AllocateResources();
    }
    m_IsOk = result;
    return m_IsOk;
}

bool AVDevice::IsOk() {
    return m_IsOk;
}

void AVDevice::ShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    m_Busy->Wait();
    Disconnect();
    FreeResources();
    m_IsOk = false;
    m_IsShuttingDown = false;
}

void AVDevice::StartShutDown() {
    m_IsShuttingDown = true;
}

void AVDevice::WaitForShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    m_Busy->Wait();
}

void AVDevice::FinishShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    if(m_Busy->IsUnlocked()) {
        Disconnect();
        FreeResources();
        m_IsOk = false;
        m_IsShuttingDown = false;
    }
}

bool AVDevice::IsBusy() {
    return !m_Busy->IsUnlocked();
}

bool AVDevice::InnerMustAbort() {
    return !m_IsOk || m_IsShuttingDown;
}

bool AVDevice::IsVideo() {
    return m_IsVideo;
}

bool AVDevice::IsAudio() {
    return m_IsAudio;
}

bool AVDevice::IsInput() {
    return m_IsInput;
}

bool AVDevice::IsOutput() {
    return m_IsOutput;
}


