/************************************************************************
 * Name:      resources.h
 * Purpose:   Implements classes and functions for loading resource files
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-23
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************************/

#include "resources.h"


#include <wx/string.h>
#include <wx/bitmap.h>

wxString ResourcesPath;
wxString ResourcesImgPath;

void syInitResourcesPaths() {
    ResourcesPath = wxString(wxT("resources/"));
    ResourcesImgPath = ResourcesPath + wxT("img/");
}

wxBitmap* syLoadImage(const wxString& filename) {
    wxBitmap* bmp = new wxBitmap(ResourcesImgPath + filename, wxBITMAP_TYPE_ANY);
    return bmp;
}
