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
#include "debuglog.h"
#include <wx/datetime.h>

BEGIN_EVENT_TABLE(AppDebugLog, wxFrame)
    EVT_CLOSE(AppDebugLog::OnClose)
END_EVENT_TABLE()

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
    wxDateTime t = wxDateTime::Now();
    syString s;
    s << "[" << syString(t.Format(_T("%Y-%m-%d %H:%M:%S"))) << "] : " << syString(msg) << "\n";
    m_log->AppendText(s);
    if(!syApp::Get()->IsMainLoopRunning()) {
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

AppDebugLog::~AppDebugLog() {
}
