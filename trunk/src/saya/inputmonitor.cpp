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
#include "core/evtregistry.h"
#include "core/dialogs.h"
// ----------------------
// Begin InputMonitorData
// ----------------------

class InputMonitor::Data : public syEvtHandler {
    public:
        Data(InputMonitor* parent);
        ~Data();
        InputMonitor* m_Parent;
        syString m_File;
        FileVID* m_VID;
        void OnStop(syMonitorEvent& event);
        void OnPlay(syMonitorEvent& event);
        void OnPause(syMonitorEvent& event);
        void OnGotoFirstFrame(syMonitorEvent& event);
        void OnGotoLastFrame(syMonitorEvent& event);
        void OnGotoNextFrame(syMonitorEvent& event);
        void OnGotoPrevFrame(syMonitorEvent& event);
        void OnFastForward(syMonitorEvent& event);
        void OnFastRewind(syMonitorEvent& event);
        void OnGotoSpecificFrame(syMonitorEvent& event);
        void OnGotoSpecificTime(syMonitorEvent& event);
        void OnSetSpeed(syMonitorEvent& event);
};

InputMonitor::Data::Data(InputMonitor* parent) : m_Parent(parent)
{
    m_Parent->m_Delegate = this;
    m_VID = new FileVID();
    syConnect(this, syMonitorEvent::idStop, &InputMonitor::Data::OnStop);
    syConnect(this, syMonitorEvent::idPlay, &InputMonitor::Data::OnPlay);
    syConnect(this, syMonitorEvent::idPause, &InputMonitor::Data::OnPause);
    syConnect(this, syMonitorEvent::idGotoFirstFrame, &InputMonitor::Data::OnGotoFirstFrame);
    syConnect(this, syMonitorEvent::idGotoLastFrame, &InputMonitor::Data::OnGotoLastFrame);
    syConnect(this, syMonitorEvent::idGotoNextFrame, &InputMonitor::Data::OnGotoNextFrame);
    syConnect(this, syMonitorEvent::idGotoPrevFrame, &InputMonitor::Data::OnGotoPrevFrame);
    syConnect(this, syMonitorEvent::idFastForward, &InputMonitor::Data::OnFastForward);
    syConnect(this, syMonitorEvent::idFastRewind, &InputMonitor::Data::OnFastRewind);
    syConnect(this, syMonitorEvent::idGotoSpecificFrame, &InputMonitor::Data::OnGotoSpecificFrame);
    syConnect(this, syMonitorEvent::idGotoSpecificTime, &InputMonitor::Data::OnGotoSpecificTime);
    syConnect(this, syMonitorEvent::idSetSpeed, &InputMonitor::Data::OnSetSpeed);
}

InputMonitor::Data::~Data() {
    delete m_VID;
    DisconnectEvents();
}
void InputMonitor::Data::OnStop(syMonitorEvent& event) {
    syMessageBox("You pressed Stop.");
}

void InputMonitor::Data::OnPlay(syMonitorEvent& event) {
    syMessageBox("You pressed Play.");
}

void InputMonitor::Data::OnPause(syMonitorEvent& event) {
    syMessageBox("You pressed Paused.");
}

void InputMonitor::Data::OnGotoFirstFrame(syMonitorEvent& event) {
    syMessageBox("You pressed [<.");
}

void InputMonitor::Data::OnGotoLastFrame(syMonitorEvent& event) {
    syMessageBox("You pressed >].");
}

void InputMonitor::Data::OnGotoNextFrame(syMonitorEvent& event) {
    syMessageBox("You pressed >.");
}

void InputMonitor::Data::OnGotoPrevFrame(syMonitorEvent& event) {
    syMessageBox("You pressed <.");
}

void InputMonitor::Data::OnFastForward(syMonitorEvent& event) {
    syMessageBox("You pressed >>.");
}

void InputMonitor::Data::OnFastRewind(syMonitorEvent& event) {
    syMessageBox("You pressed <<.");
}

void InputMonitor::Data::OnGotoSpecificFrame(syMonitorEvent& event) {
    syMessageBox("You wanted to go to a specific frame.");
}

void InputMonitor::Data::OnGotoSpecificTime(syMonitorEvent& event) {
    syMessageBox("You wanted to go to a specific time.");
}

void InputMonitor::Data::OnSetSpeed(syMonitorEvent& event) {
    syMessageBox("You wanted to set the playback speed.");
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
    m_Data = new Data(this);
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
