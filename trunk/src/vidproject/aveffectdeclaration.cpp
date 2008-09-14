/************************************************************
 * Name:      aveffectdeclaration.cpp
 * Purpose:   Implementation of the AVEffectDeclaration class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************/

#include "aveffectdeclaration.h"
#include "aveffectparamdeclaration.h"
#include "aveffectparamdeclarations.h"

using namespace std;

AVEffectDeclaration::AVEffectDeclaration() {
    ParametersInfo = new AVEffectParamDeclarations;
}

AVEffectDeclaration::~AVEffectDeclaration() {
    delete ParametersInfo;
}

bool AVEffectDeclaration::unserialize(const string& src) {
    // TODO: Implement AVEffectDeclaration::unserialize
    return false;
}

string AVEffectDeclaration::serialize() {
    // TODO: Implement AVEffectDeclaration::serialize
    return "";
}
