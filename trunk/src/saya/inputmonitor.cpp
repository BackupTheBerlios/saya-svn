/***************************************************************
 * Name:      inputmonitor.cpp
 * Purpose:   Implementation of the InputMonitor class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "core/sythread.h"
#include "inputmonitor.h"
#include "core/filevid.h"
#include "core/systring.h"

// ----------------------
// Begin InputMonitorData
// ----------------------

class InputMonitorData {
    public:
        InputMonitorData(InputMonitor* parent);
        ~InputMonitorData();
        InputMonitor* m_Parent;
        syString m_File;
        FileVID* m_VID;

};

InputMonitorData::InputMonitorData(InputMonitor* parent) : m_Parent(parent)
{
    m_VID = new FileVID();
}

InputMonitorData::~InputMonitorData() {
    delete m_VID;
}

// --------------------
// End InputMonitorData
// --------------------

// ------------------
// Begin InputMonitor
// ------------------

InputMonitor::InputMonitor() {
    m_ReservedVideoIn = true;
    m_ReservedAudioIn = true;
    m_Data = new InputMonitorData(this);
}

InputMonitor::~InputMonitor() {
    delete m_Data;
}

/** Sets the file to read */
bool InputMonitor::SetFile(syString filename) {
    if(!syThread::IsMain()) { return false; }
    if(IsPlaying()) { return false; }
    if(!(m_Data->m_VID->SetFile(filename))) { return false; }
    m_Data->m_File = filename;
    return true;
}

const syString InputMonitor::GetFile() const {
    return m_Data->m_File;
}

// ----------------
// End InputMonitor
// ----------------
