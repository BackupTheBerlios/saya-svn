/****************************************************************
 * Name:      app.h
 * Purpose:   Implementation of a UI-neutral syApplication class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ***************************************************************/

#include "app.h"
#include "systring.h"
#include "debuglog.h"

namespace sayaStaticData {
    static syApp* TheApp = 0;
    static volatile bool s_IsAppShuttingDown = false;
};

/** Destroys an syApp object along with it. This class is exception-safe and is guaranteed to succeed. */
class syAppDestructor {
    public:
        syApp* m_App;
        syAppDestructor(syApp* app) : m_App(app) {}
        ~syAppDestructor() { delete m_App; }
};

int syApp::Result = 0;
syApp* syApp::Get() {
    return sayaStaticData::TheApp;
}

syApp::syApp() {
}

syDebugLog* syApp::CreateDebugLog() const {
    return 0;
}

int syApp::Start(int argc, const char** argv) {
    Result = -1;
    syAppDestructor destructor(this);
    if(!sayaStaticData::TheApp) {
        sayaStaticData::TheApp = this;
        syDebugLog::SetDebugLog(CreateDebugLog());
        if(OnInit(argc, argv)) {
            Run();
            OnExit();
        }
    } else {
        Result = -2;
    }
    return Result;
}

bool syApp::OnInit(int argc, const char** argv) {
    return true;
}

void syApp::OnExit() {}

syApp::~syApp() {
    if(sayaStaticData::TheApp == this)
        sayaStaticData::TheApp = 0;
    syDebugLog::DeleteDebugLog();
}

bool IsAppShuttingDown() {
    return sayaStaticData::s_IsAppShuttingDown;
}

bool syApp::IsAppShuttingDown() {
    return sayaStaticData::s_IsAppShuttingDown;
}

void syApp::ShutDown() {
    sayaStaticData::s_IsAppShuttingDown = true;
}