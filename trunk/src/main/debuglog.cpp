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
    #include <wx/xrc/xmlres.h>
    #include <wx/string.h>
    #include <wx/textctrl.h>
    #include <wx/gdicmn.h>
    #include <wx/settings.h>
    #include <wx/sizer.h>
    #include <wx/config.h>
#endif

#include "s2wx.h"
#include "debuglog.h"
#include <wx/datetime.h>

BEGIN_EVENT_TABLE(AppDebugLog, wxFrame)
    EVT_CLOSE(AppDebugLog::OnClose)
END_EVENT_TABLE()

AppDebugLog::AppDebugLog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) :
wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_log = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	bSizer1->Add( m_log, 1, wxALL|wxEXPAND, 5 );

	this->SetSizer( bSizer1 );
	this->Layout();
    this->Show();
}


void AppDebugLog::DebugLog(const char* msg) {
    DebugLog(s2wx(msg));
}

void AppDebugLog::DebugLog(const syString& msg) {
    DebugLog(s2wx(msg));
}

void AppDebugLog::DebugLog(const wxString& msg) {
    wxDateTime t = wxDateTime::Now();
    wxString s;
    s = wxString(_T("[")) + t.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("] : ") + msg + _T("\n");
    m_log->AppendText(s);
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
