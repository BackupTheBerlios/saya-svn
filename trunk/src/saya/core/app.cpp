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
#include "config.h"
#include "eventqueue.h"
#include "sentryfuncs.h"


namespace sayaStaticData {
    static syApp* TheApp = 0;
    static volatile bool s_IsAppShuttingDown = false;
    static syConfig* TheConfig = 0;
    static syEvtHandler* TheHandler = 0;
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

syApp::syApp(int argc, char** argv) :
m_argc(argc),
m_argv(argv)
{
}

syDebugLog* syApp::CreateDebugLog() const {
    return 0;
}

int syApp::Start() {
    Result = -1;
    syAppDestructor destructor(this);
    if(!sayaStaticData::TheApp) {
        sayaStaticData::TheApp = this;
        syDebugLog::SetDebugLog(CreateDebugLog());
        if(OnInit()) {
            Run();
            OnExit();
        }
    } else {
        Result = -2;
    }
    return Result;
}

syConfig* syApp::GetConfig() {
    if(IsAppShuttingDown()) return 0;
    if(!sayaStaticData::TheConfig) {
        sayaStaticData::TheConfig = syApp::Get()->CreateConfig();
    }
    return sayaStaticData::TheConfig;
}

bool syApp::OnInit() {
    return true;
}

void syApp::OnExit() {}

syApp::~syApp() {
    ShutDown();
    if(sayaStaticData::TheApp == this)
        sayaStaticData::TheApp = 0;
    delete sayaStaticData::TheConfig;
    sayaStaticData::TheConfig = 0;
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

bool syApp::Pending() const {
    return syEvtQueue::Pending();
}

void syApp::ProcessNextEvent() const {
    syEvtQueue::ProcessNextEvent();
}

/** Sets the application's main event handler. */
void syApp::SetEventHandler(syEvtHandler* handler) {
    sayaStaticData::TheHandler = handler;
}

syEvtHandler* syApp::GetEventHandler() const {
    return sayaStaticData::TheHandler;
}

