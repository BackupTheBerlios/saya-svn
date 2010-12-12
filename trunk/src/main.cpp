/***************************************************************
 * Name:      /main.cpp
 * Purpose:   This is our main application program.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   This file is Public Domain
 **************************************************************/

#include "main.h"
#include <saya/core/debuglog.h>
#include <saya/core/intl.h>
#include <saya/playbackmanager.h>
#include <saya/projectmanager.h>
#include <cstdio>

const char* APP_NAME = "SayaVideoEditor";
const char* APP_VENDOR = "Rick Garcia";
const char* APP_SHOWNAME = "Saya";
const char* APP_SHOWOFFNAME = "SayaVE Ain't Yet Another Video Editor";

SayaApplication::SayaApplication(int argc, char** argv) : qsyApp(argc, argv) {}

bool SayaApplication::InitializeApplicationObjects () {
    bool result = true;
    DebugLog(_("Initializing Playback Manager..."));
    PlaybackManager::Get();

    DebugLog(_("Initializing Project Manager..."));
    ProjectManager::Get();

    if(!ProjectManager::Get()->LoadConfig()) {
        ErrorMessageBox(_("WARNING: Could not read configuration!"));
    }
    return result;
}

bool SayaApplication::LoadResources() {
    bool result = false;
    do {
        if(!LoadResource("welcomedlg.rcc")) break;
        if(!LoadResource("playbackcontrol.rcc")) break;
        result = true;
    }while(false);
    return result;
}

void SayaApplication::OnBeforeExit() {
    ProjectManager::Get()->SaveConfig();
    ProjectManager::Get()->CloseProject(true);
}

SayaApplication::~SayaApplication() {
    PlaybackManager::Unload();
    ProjectManager::Unload();
}

const char* SayaApplication::GetApplicationName() const {
    return APP_NAME;
}

const char* SayaApplication::GetApplicationDisplayName() const {
    return APP_SHOWNAME;
}

const char* SayaApplication::GetApplicationVendor() const {
    return APP_VENDOR;
}

const char* SayaApplication::GetApplicationShowOffName() const {
    return APP_SHOWOFFNAME;
}


int main(int argc, char** argv) {
    syApp* app = new SayaApplication(argc, argv);
    int result = app->Start();
    return result;
}
