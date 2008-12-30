/***************************************************************
 * Name:      nullvid.cpp
 * Purpose:   Implementation for the NullVID class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "nullvid.h"
#include "sybitmap.h"
#include "systring.h"

NullVID::NullVID() {
    m_Width = 32;
    m_Height = 32;
    m_ColorFormat = vcfBGR24;
}

NullVID::~NullVID(){
}

void NullVID::LoadCurrentFrame(){
    m_Bitmap->Clear();
}

VideoInputDevice* CreateNullVID() {
    return new NullVID;
}

namespace DummyNullVID{
    bool dummybool = VideoInputDevice::RegisterVID("", &CreateNullVID);
};
