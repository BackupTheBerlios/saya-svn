/************************************************************************
 * Name:      resources.h
 * Purpose:   Implements classes and functions for loading resource files
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-23
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***********************************************************************/

#include "resources.h"

#include "../saya/core/systring.h"
#include <wx/bitmap.h>

syString ResourcesPath;
syString ResourcesImgPath;

void syInitResourcesPaths() {
    ResourcesPath = "resources/";
    ResourcesImgPath = ResourcesPath + "img/";
}

wxBitmap* syLoadImage(const syString& filename) {
    wxBitmap* bmp = new wxBitmap(wxString(ResourcesImgPath + filename), wxBITMAP_TYPE_ANY);
    return bmp;
}

wxBitmap* syLoadImage(const char* filename) {
    return syLoadImage(syString(filename));
}
