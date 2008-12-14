/***************************************************************
 * Name:      debuglog.cpp
 * Purpose:   Implementation of the syDebugLog class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "debuglog.h"

syDebugLog* TheDebugLog = 0;

void syDebugLog::SetDebugLog(syDebugLog* log) {
    TheDebugLog = log;
}

void DebugLog(const char* msg) {
    if(TheDebugLog) {
        TheDebugLog->DebugLog(msg);
    }
}

void DebugLog(const syString& msg) {
    if(TheDebugLog) {
        TheDebugLog->DebugLog(msg);
    }
}

void syDebugLog::DeleteDebugLog() {
    delete TheDebugLog;
}

syDebugLog::~syDebugLog() {
    if(TheDebugLog == this) {
        TheDebugLog = 0;
    }
}
