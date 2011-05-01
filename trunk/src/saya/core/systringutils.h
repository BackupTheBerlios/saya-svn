/***************************************************************
 * Name:      systringutils.h
 * Purpose:   Declaration for the syString utility functions
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-24
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#ifndef systringutils_h
#define systringutils_h

#include "systring.h"
#include <vector>
#include "base64.h"

syString implode(const syString& joiner, std::vector<syString> vec);
std::vector<syString> explode(const syString& joiner, syString s);
syString strtolower(const syString& s);
syString strtoupper(const syString& s);

#endif