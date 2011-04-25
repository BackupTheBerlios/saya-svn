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

#include <cstddef>
#include <set>
#include "avdevice.h"
#include "sythread.h"
#include "app.h"

// ----------------------
// begin AVDeviceRegistry
// ----------------------

class AVDeviceRegistry {
    public:
        typedef std::set<AVDevice*> AVDeviceSet;
        AVDeviceSet m_Devices;
        AVDeviceRegistry();
        ~AVDeviceRegistry();
        void Register(AVDevice* device);
        void Unregister(AVDevice* device);
        volatile bool m_DevicesBeingShutDown;
        void UnregisterAll();
};

AVDeviceRegistry::AVDeviceRegistry() {
    m_Devices.clear();
    m_DevicesBeingShutDown = false;
}

AVDeviceRegistry::~AVDeviceRegistry() {
    UnregisterAll();
}

void AVDeviceRegistry::UnregisterAll() {
    AVDeviceSet::iterator i = m_Devices.begin();
    AVDeviceSet::iterator imax = m_Devices.end();
    while(m_Devices.size()) {
        delete *(m_Devices.begin());
    }
}

void AVDeviceRegistry::Register(AVDevice* device) {
    m_Devices.insert(device);
}

void AVDeviceRegistry::Unregister(AVDevice* device) {
    m_Devices.erase(device);
}

AVDeviceRegistry s_DeviceRegistry;

// --------------------
// end AVDeviceRegistry
// --------------------



AVDevice::AVDevice() :
m_InputVideoMutex(NULL),
m_InputAudioMutex(NULL),
m_OutputVideoMutex(NULL),
m_OutputAudioMutex(NULL),
m_Stop(false),
m_SoftStop(false),
m_IsOk(false),
m_IsShuttingDown(false)
{
    m_InputVideoMutex = new sySafeMutex;
    m_OutputVideoMutex = new sySafeMutex;
    m_InputAudioMutex = new sySafeMutex;
    m_OutputAudioMutex = new sySafeMutex;
    s_DeviceRegistry.Register(this);
}

AVDevice::~AVDevice() {
    ShutDown();
    delete m_OutputAudioMutex;
    delete m_InputAudioMutex;
    delete m_OutputVideoMutex;
    delete m_InputVideoMutex;
    s_DeviceRegistry.Unregister(this);
}

bool AVDevice::Init() {
    if(!m_IsOk) {
        FreeResources();
        if(syApp::Get()->IsAppShuttingDown()) {
            return false;
        }
        bool result = Connect();
        if(result) {
            result = AllocateResources();
        }
        m_IsOk = result;
    }
    return m_IsOk;
}

bool AVDevice::IsShuttingDown() const {
    return m_IsShuttingDown || s_DeviceRegistry.m_DevicesBeingShutDown;
}

bool AVDevice::IsOk() const {
    return m_IsOk;
}

void AVDevice::ShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    m_IsShuttingDown = true;
    while(!m_InputVideoMutex->IsUnlocked() ||
          !m_OutputVideoMutex->IsUnlocked() ||
          !m_InputAudioMutex->IsUnlocked() ||
          !m_OutputAudioMutex->IsUnlocked()) {
        m_InputVideoMutex->Wait();
        m_OutputVideoMutex->Wait();
        m_InputAudioMutex->Wait();
        m_OutputAudioMutex->Wait();
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
    while(!m_InputVideoMutex->IsUnlocked() ||
          !m_OutputVideoMutex->IsUnlocked() ||
          !m_InputAudioMutex->IsUnlocked() ||
          !m_OutputAudioMutex->IsUnlocked()) {
        m_InputVideoMutex->Wait();
        m_OutputVideoMutex->Wait();
        m_InputAudioMutex->Wait();
        m_OutputAudioMutex->Wait();
    }
}

void AVDevice::FinishShutDown() {
    if(!syThread::IsMain()) { return; } // Can only be called from the main thread!
    if(m_InputVideoMutex->IsUnlocked() && m_OutputVideoMutex->IsUnlocked() &&
       m_InputAudioMutex->IsUnlocked() && m_OutputAudioMutex->IsUnlocked()) {
        Disconnect();
        FreeResources();
        m_IsOk = false;
        m_IsShuttingDown = false;
        m_Stop = false;
        m_SoftStop = false;
    }
}

bool AVDevice::IsPlaying() const {
    return !(m_InputVideoMutex->IsUnlocked() && m_OutputVideoMutex->IsUnlocked() &&
       m_InputAudioMutex->IsUnlocked() && m_OutputAudioMutex->IsUnlocked());
}

bool AVDevice::InnerMustAbort() const {
    return !m_IsOk || m_IsShuttingDown || s_DeviceRegistry.m_DevicesBeingShutDown || m_Stop;
}

bool AVDevice::MustStop() const {
    return !m_IsOk || m_IsShuttingDown || s_DeviceRegistry.m_DevicesBeingShutDown || m_Stop || m_SoftStop;
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

void AVDevice::ShutDownAll() {
    s_DeviceRegistry.m_DevicesBeingShutDown = true;
}
