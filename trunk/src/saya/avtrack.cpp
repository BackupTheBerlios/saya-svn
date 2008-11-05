/***************************************************************
 * Name:      avtrack.h
 * Purpose:   Implementation of the Timeline track class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avtrack.h"
#include "avclip.h"
#include "avclips.h"
#include "smap.h"

AVTrack::AVTrack() {
    m_Clips = new AVClips;
    m_TimeIndex = new SMapUintUint;
}

AVTrack::~AVTrack() {
    delete m_TimeIndex;
    delete m_Clips;
}

bool AVTrack::unserialize(const std::string& src) {
    // TODO: Implement AVTrack::unserialize
    return false;
}

std::string AVTrack::serialize() {
    // TODO: Implement AVTrack::serialize
    return "";
}
