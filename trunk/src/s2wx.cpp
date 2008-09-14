/**********************************************************************
 * Name:      s2wx.cpp
 * Purpose:   Functions for converting between wxString and std::string
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 *********************************************************************/

#include "s2wx.h"

const wxString s2wx(const std::string& str) {
    return wxString(str.c_str(), wxConvUTF8);
}

const wxString s2wx(const char* str) {
    return wxString(str, wxConvUTF8);
}

const std::string wx2s(const wxString& s) {
    return std::string(s.mb_str(wxConvUTF8));
}

const char* wx2c(const wxString& s) {
    return s.mb_str(wxConvUTF8);
}
