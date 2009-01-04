/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Implementation of the Debug Log
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__


#ifndef WX_PRECOMP
    #include <wx/string.h>
    #include <wx/textctrl.h>
    #include <wx/sizer.h>
    #include <wx/frame.h>
#endif

#include "../saya/core/systring.h"
#include "../saya/core/app.h"
#include "../saya/core/sythread.h"
#include "debuglog.h"
#include <wx/datetime.h>
#include <deque>

BEGIN_EVENT_TABLE(AppDebugLog, wxFrame)
    EVT_CLOSE(AppDebugLog::OnClose)
    EVT_IDLE(AppDebugLog::OnIdle)
END_EVENT_TABLE()

std::deque<wxString> PendingDebugLogMessages;
static volatile bool s_DebugLogPending;
syMutex s_MyDebugLogMutex;

AppDebugLog::AppDebugLog() :
wxFrame(NULL, wxID_ANY, wxT("Debug Log"), wxDefaultPosition, wxSize( 539,399 ), wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	bSizer1->Add( m_log, 1, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
	syApp::Get()->SetTopWindow(this);
    this->Show();
}


void AppDebugLog::DebugLog(const char* msg) {
    DebugLog(wxString(msg, wxConvUTF8));
}

void AppDebugLog::DebugLog(const syString& msg) {
    DebugLog(wxString(msg));
}

void AppDebugLog::DebugLog(const wxString& msg) {
    if(!syThread::IsMain()) {
        syMutexLocker lock(s_MyDebugLogMutex);
        PendingDebugLogMessages.push_back(msg);
        s_DebugLogPending = true;
        syApp::Get()->WakeUpIdle();
        return;
    }
    wxDateTime t = wxDateTime::Now();
    syString s;
    s << "[" << syString(t.Format(_T("%Y-%m-%d %H:%M:%S"))) << "] : " << syString(msg) << "\n";
    m_log->AppendText(s);
    if(!syApp::Get()->IsMainLoopRunning()) {
        this->Refresh();
        this->Update();
    }
}

void AppDebugLog::OnClose(wxCloseEvent& event) {
    if(event.CanVeto()) {
        event.Veto();
        this->Hide();
    } else {
        Destroy();
    }
}

void AppDebugLog::OnIdle(wxIdleEvent& event) {
    if(s_DebugLogPending) {
        syMutexLocker lock(s_MyDebugLogMutex);
        DebugLog(PendingDebugLogMessages[0]);
        PendingDebugLogMessages.pop_front();
        s_DebugLogPending = PendingDebugLogMessages.size() > 0;
    }
}

AppDebugLog::~AppDebugLog() {
}
