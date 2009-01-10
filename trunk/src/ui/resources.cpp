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
#include <qbitmap.h>

syString ResourcesPath;
syString ResourcesImgPath;

void syInitResourcesPaths() {
    ResourcesPath = "resources/";
    ResourcesImgPath = ResourcesPath + "img/";
}

QBitmap* syLoadImage(const syString& filename) {
    return 0;
    // TODO: Find out if we're still gonna need this function
//    wxBitmap* bmp = new wxBitmap(wxString(ResourcesImgPath + filename), wxBITMAP_TYPE_ANY);
//    return bmp;
}

QBitmap* syLoadImage(const char* filename) {
    return syLoadImage(syString(filename));
}
