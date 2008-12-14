/**********************************************************************
 * Name:      s2wx.cpp
 * Purpose:   Functions for converting between wxString and syString
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 *********************************************************************/

#include "s2wx.h"
#include <wx/string.h>
#include "../saya/core/systring.h"

/** Converts a syString into a wxString */
const wxString s2wx(const syString& str) {
    return wxString(str.c_str(), wxConvUTF8);
}

/** Converts a const char* into a wxString */
const wxString s2wx(const char* str) {
    return wxString(str, wxConvUTF8);
}

/** Converts a wxString into a syString */
const syString wx2s(const wxString& s) {
    return syString(s.mb_str(wxConvUTF8));
}

/** Converts a wxString into a const char* */
const char* wx2c(const wxString& s) {
    return s.mb_str(wxConvUTF8);
}
