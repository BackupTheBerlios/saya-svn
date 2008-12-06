/***************************************************************
 * Name:      sayadebuglogger.cpp
 * Purpose:   Implementation of the sayaDebugLogger class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <cstddef>
#include "sayadebuglogger.h"

sayaDebugLogger* TheDebugLogger = NULL;

void sayaDebugLogger::SetDebugLogger(sayaDebugLogger* logger) {
    TheDebugLogger = logger;
}

void DebugLog(const char* msg) {
    if(TheDebugLogger) {
        TheDebugLogger->DebugLog(msg);
    }
}

void DebugLog(const syString& msg) {
    if(TheDebugLogger) {
        TheDebugLogger->DebugLog(msg);
    }
}

sayaDebugLogger::~sayaDebugLogger() {
    if(TheDebugLogger == this) {
        TheDebugLogger = NULL;
    }
}
