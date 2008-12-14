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
#include <cstddef>

class PlaybackManager::Data {
    public:
        PlaybackManager* m_Parent;
        AVController* m_InputMonitor;
        AVController* m_PreviewMonitor;
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


PlaybackManager::PlaybackManager() {
    m_Data = new Data(this);
}

PlaybackManager::~PlaybackManager() {
    delete m_Data;
}

AVController* PlaybackManager::GetInputMonitor() const {
    return m_Data->m_InputMonitor;
}

AVController* PlaybackManager::GetPreviewMonitor() const {
    return NULL;
}
