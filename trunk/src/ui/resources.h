/*****************************************************************************
 * Name:      resources.h
 * Purpose:   Defines classes classes and functions for loading resource files
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-23
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ****************************************************************************/

#ifndef RESOURCES_H
#define RESOURCES_H

class syString;
class wxBitmap;
extern syString ResourcesPath;
extern syString ResourcesImgPath;

wxBitmap* syLoadImage(const syString& filename);
wxBitmap* syLoadImage(const char* filename);

void syInitResourcesPaths();

#endif
