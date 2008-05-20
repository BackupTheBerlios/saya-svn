/***************************************************************
 * Name:      avclip.cpp
 * Purpose:   Code for Timeline clips and tracks
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avclip.h"

AVClip::AVClip()
{
    //ctor
}

AVClip::~AVClip()
{
    //dtor
}

bool AVClip::unserialize(const wxString& data) {
    return false;
}

wxString AVClip::serialize() {
    return wxEmptyString;
}

AVTimeline::AVTimeline() {
}

AVTimeline::~AVTimeline() {
}

bool AVTimeline::unserialize(const wxString& data) {
    return false;
}

wxString AVTimeline::serialize() {
    return wxEmptyString;
}

AVResource::AVResource() {
}

AVResource::~AVResource() {
}

bool AVResource::unserialize(const wxString& data) {
    return false;
}

wxString AVResource::serialize() {
    return wxEmptyString;
}

