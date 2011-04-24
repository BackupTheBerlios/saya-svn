/***************************************************************
 * Name:      playbackmanager.cpp
 * Purpose:   Implementation of the Playback Manager class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-12-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "playbackmanager.h"
#include "inputmonitor.h"
#include "core/app.h"
#include <cstddef>

// ---------------------------
// begin PlaybackManager::Data
// ---------------------------

static PlaybackManager* g_PlaybackManager = 0;

class PlaybackManager::Data {
    public:
        PlaybackManager* m_Parent;
        AVPlayer* m_InputMonitor;
        AVPlayer* m_PreviewMonitor;
        Data(PlaybackManager* parent);
        ~Data();
};

PlaybackManager::Data::Data(PlaybackManager* parent) :
m_Parent(parent),
m_InputMonitor(new InputMonitor),
m_PreviewMonitor(NULL) {
}

PlaybackManager::Data::~Data() {
    delete m_InputMonitor;
    m_InputMonitor = 0;
    delete m_PreviewMonitor;
    m_PreviewMonitor = 0;
}

// -------------------------
// end PlaybackManager::Data
// -------------------------

// ---------------------
// begin PlaybackManager
// ---------------------

PlaybackManager* PlaybackManager::Get() {
    if(!g_PlaybackManager) {
        if(!IsAppShuttingDown()) {
            g_PlaybackManager = new PlaybackManager();
        }
    }
    return g_PlaybackManager;
}

void PlaybackManager::Unload() {
    delete g_PlaybackManager;
    g_PlaybackManager = 0;
}

PlaybackManager::PlaybackManager() {
    m_Data = new Data(this);
}

PlaybackManager::~PlaybackManager() {
    g_PlaybackManager = 0;
    delete m_Data;
}

AVPlayer* PlaybackManager::GetInputMonitor() const {
    if(IsAppShuttingDown()) return 0;
    return m_Data->m_InputMonitor;
}

AVPlayer* PlaybackManager::GetPreviewMonitor() const {
    if(IsAppShuttingDown()) return 0;
    return 0;
}

// -------------------
// end PlaybackManager
// -------------------
