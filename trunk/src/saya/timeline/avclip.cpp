/***************************************************************
 * Name:      avclip.cpp
 * Purpose:   Implementation of the Timeline clips class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "smapxstr.h"
#include "avclip.h"
#include "avtransition.h"
#include "aveffect.h"
#include "aveffects.h"

AVClip::AVClip()
{
    //ctor
    m_Effects = new AVEffects;
    m_EndingTransition = new AVTransition;
    m_Markers = new SMapUintUint;
}

AVClip::~AVClip()
{
    //dtor
    delete m_Markers;
    delete m_EndingTransition;
    delete m_Effects;
}

bool AVClip::unserialize(const char* data) {
    return false;
}

void AVClip::serialize(serialized& dest) const {
    // TODO: Implement AVClip::serialize
}
