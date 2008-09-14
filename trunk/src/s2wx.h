/**********************************************************************
 * Name:      s2wx.h
 * Purpose:   Functions for converting between wxString and std::string
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 *********************************************************************/

#include <string>
#include <wx/string.h>

using std::string;

/** Converts a std::string into a wxString */
const wxString s2wx(const std::string& str);

/** Converts a const char* into a wxString */
const wxString s2wx(const char* str);

/** Converts a wxString into an std::string */
const std::string wx2s(const wxString& s);

/** Converts a wxString into a const char* */
const char* wx2c(const wxString& s);
