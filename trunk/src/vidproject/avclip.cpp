/***************************************************************
 * Name:      avclip.cpp
 * Purpose:   Code for Timeline clips and tracks
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avclip.h"
#include "avtransition.h"

AVClip::AVClip()
{
    //ctor
}

AVClip::~AVClip()
{
    //dtor
}

bool AVClip::unserialize(const std::string& data) {
    return false;
}

std::string AVClip::serialize() {
    return "";
}

AVTimeline::AVTimeline() {
}

AVTimeline::~AVTimeline() {
}

bool AVTimeline::unserialize(const std::string& data) {
    return false;
}

std::string AVTimeline::serialize() {
    return "";
}

AVResource::AVResource() {
}

AVResource::~AVResource() {
}

bool AVResource::unserialize(const std::string& data) {
    //TODO: Implement AVResource::unserialize
    return false;
}

std::string AVResource::serialize() {
    //TODO: Implement AVResource::serialize
    return "";
}

AVResources::AVResources() {
}

AVResources::~AVResources() {
}

bool AVResources::unserialize(const std::string& data) {
    //TODO: Implement AVResources::unserialize
    return false;
}

std::string AVResources::serialize() {
    //TODO: Implement AVResources::serialize
    return "";
}
