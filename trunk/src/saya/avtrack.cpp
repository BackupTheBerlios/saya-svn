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
#include "smapxstr.h"

AVTrack::AVTrack() {
    m_Clips = new AVClips;
    m_TimeIndex = new SMapUintUint;
}

AVTrack::~AVTrack() {
    delete m_TimeIndex;
    delete m_Clips;
}

bool AVTrack::unserialize(const char* src) {
    // TODO: Implement AVTrack::unserialize
    return false;
}

void AVTrack::serialize(serialized& dest) const {
    // TODO: Implement AVTrack::serialize
}
