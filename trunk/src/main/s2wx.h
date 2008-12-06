/**********************************************************************
 * Name:      s2wx.h
 * Purpose:   Functions for converting between wxString and syString
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 *********************************************************************/

class wxString;
class syString;

/** Converts a syString into a wxString */
const wxString s2wx(const syString& str);

/** Converts a const char* into a wxString */
const wxString s2wx(const char* str);

/** Converts a wxString into a syString */
const syString wx2s(const wxString& s);

/** Converts a wxString into a const char* */
const char* wx2c(const wxString& s);
