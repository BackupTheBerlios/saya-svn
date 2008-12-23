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
m_InputMonitor(NULL),
m_PreviewMonitor(NULL) {
    m_InputMonitor = new InputMonitor;
}

PlaybackManager::Data::~Data() {
    delete m_InputMonitor;
    delete m_PreviewMonitor;
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
}

PlaybackManager::PlaybackManager() {
    m_Data = new Data(this);
}

PlaybackManager::~PlaybackManager() {
    delete m_Data;
    g_PlaybackManager = 0;
}

AVPlayer* PlaybackManager::GetInputMonitor() const {
    return m_Data->m_InputMonitor;
}

AVPlayer* PlaybackManager::GetPreviewMonitor() const {
    return NULL;
}

// -------------------
// end PlaybackManager
// -------------------
