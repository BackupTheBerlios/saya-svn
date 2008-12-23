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
// --------------------
// Begin AVPlayer::Data
// --------------------

class AVPlayer::Data : public syEvtHandler {
    public:
        Data(AVPlayer* parent);
        ~Data();
        AVPlayer* m_Parent;
        void OnStop(AVPlayerEvent& event);
        void OnPlay(AVPlayerEvent& event);
        void OnPause(AVPlayerEvent& event);
        void OnGotoFirstFrame(AVPlayerEvent& event);
        void OnGotoLastFrame(AVPlayerEvent& event);
        void OnGotoNextFrame(AVPlayerEvent& event);
        void OnGotoPrevFrame(AVPlayerEvent& event);
        void OnFastForward(AVPlayerEvent& event);
        void OnFastRewind(AVPlayerEvent& event);
        void OnGotoSpecificFrame(AVPlayerEvent& event);
        void OnGotoSpecificTime(AVPlayerEvent& event);
        void OnSetSpeed(AVPlayerEvent& event);
};

AVPlayer::Data::Data(AVPlayer* parent) : m_Parent(parent)
{
    m_Parent->m_Delegate = this;
    syConnect(this, AVPlayerEvent::idStop, &AVPlayer::Data::OnStop);
    syConnect(this, AVPlayerEvent::idPlay, &AVPlayer::Data::OnPlay);
    syConnect(this, AVPlayerEvent::idPause, &AVPlayer::Data::OnPause);
    syConnect(this, AVPlayerEvent::idGotoFirstFrame, &AVPlayer::Data::OnGotoFirstFrame);
    syConnect(this, AVPlayerEvent::idGotoLastFrame, &AVPlayer::Data::OnGotoLastFrame);
    syConnect(this, AVPlayerEvent::idGotoNextFrame, &AVPlayer::Data::OnGotoNextFrame);
    syConnect(this, AVPlayerEvent::idGotoPrevFrame, &AVPlayer::Data::OnGotoPrevFrame);
    syConnect(this, AVPlayerEvent::idFastForward, &AVPlayer::Data::OnFastForward);
    syConnect(this, AVPlayerEvent::idFastRewind, &AVPlayer::Data::OnFastRewind);
    syConnect(this, AVPlayerEvent::idGotoSpecificFrame, &AVPlayer::Data::OnGotoSpecificFrame);
    syConnect(this, AVPlayerEvent::idGotoSpecificTime, &AVPlayer::Data::OnGotoSpecificTime);
    syConnect(this, AVPlayerEvent::idSetSpeed, &AVPlayer::Data::OnSetSpeed);
}

AVPlayer::Data::~Data() {
    DisconnectEvents();
}

void AVPlayer::Data::OnStop(AVPlayerEvent& event) {
    syMessageBox("You pressed Stop.");
}

void AVPlayer::Data::OnPlay(AVPlayerEvent& event) {
    syMessageBox("You pressed Play.");
}

void AVPlayer::Data::OnPause(AVPlayerEvent& event) {
    syMessageBox("You pressed Paused.");
}

void AVPlayer::Data::OnGotoFirstFrame(AVPlayerEvent& event) {
    syMessageBox("You pressed [<.");
}

void AVPlayer::Data::OnGotoLastFrame(AVPlayerEvent& event) {
    syMessageBox("You pressed >].");
}

void AVPlayer::Data::OnGotoNextFrame(AVPlayerEvent& event) {
    syMessageBox("You pressed >.");
}

void AVPlayer::Data::OnGotoPrevFrame(AVPlayerEvent& event) {
    syMessageBox("You pressed <.");
}

void AVPlayer::Data::OnFastForward(AVPlayerEvent& event) {
    syMessageBox("You pressed >>.");
}

void AVPlayer::Data::OnFastRewind(AVPlayerEvent& event) {
    syMessageBox("You pressed <<.");
}

void AVPlayer::Data::OnGotoSpecificFrame(AVPlayerEvent& event) {
    syMessageBox("You wanted to go to a specific frame.");
}

void AVPlayer::Data::OnGotoSpecificTime(AVPlayerEvent& event) {
    syMessageBox("You wanted to go to a specific time.");
}

void AVPlayer::Data::OnSetSpeed(AVPlayerEvent& event) {
    syMessageBox("You wanted to set the playback speed.");
}

// ------------------
// End AVPlayer::Data
// ------------------

// --------------
// Begin AVPlayer
// --------------
AVPlayer::AVPlayer() : m_Data(new AVPlayer::Data(this))
{
}

AVPlayer::~AVPlayer() {
    delete m_Data;
}

// ------------
// End AVPlayer
// ------------

// ------------------------
// Begin InputMonitor::Data
// ------------------------

class InputMonitor::Data {
    public:
        syString m_File;
        InputMonitor* m_Parent;
        FileVID* m_VID;
        Data(InputMonitor* parent);
        ~Data();
};

InputMonitor::Data::Data(InputMonitor* parent) :
m_File(""),
m_Parent(parent),
m_VID(new FileVID())
{
}

InputMonitor::Data::~Data() {
    delete m_VID;
}

// ----------------------
// End InputMonitor::Data
// ----------------------

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
