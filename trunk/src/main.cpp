/***************************************************************
 * Name:      /main.cpp
 * Purpose:   This is our main application program.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   This file is Public Domain
 **************************************************************/

#include "main.h"

int main(int argc, char** argv) {
    syApp* app = new wxSayaApp();
    return app->Start(argc, (const char**)argv);
}
