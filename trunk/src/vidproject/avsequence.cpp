/***************************************************************
 * Name:      avsequence.cpp
 * Purpose:   Implementation of the Timeline Sequence class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avsequence.h"
#include "avclip.h"
#include "avclips.h"
#include "avtrack.h"
#include "avtracks.h"

AVSequence::AVSequence() {
    m_VideoTracks = new AVTracks;
    m_AudioTracks = new AVTracks;
}

AVSequence::~AVSequence() {
    delete m_AudioTracks;
    delete m_VideoTracks;
}

bool AVSequence::unserialize(const std::string& src) {
    // TODO: Implement AVSequence::unserialize
    return false;
}

std::string AVSequence::serialize() {
    // TODO: Implement AVSequence::serialize
    return "";
}
