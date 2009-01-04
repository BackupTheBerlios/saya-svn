/***************************************************************
 * Name:      sybitmapsink.cpp
 * Purpose:   Implementation of the syBitmapSink class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-08-03
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#include "sybitmapsink.h"

VideoColorFormat syBitmapSink::GetColorFormat() const { return vcfRGB32; }
unsigned int syBitmapSink::GetWidth() const { return 0; }
unsigned int syBitmapSink::GetHeight() const { return 0; }
int syBitmapSink::GetTop() const { return 0; }
int syBitmapSink::GetLeft() const { return 0; }
