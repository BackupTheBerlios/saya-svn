/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Declaration for the Debug Log
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/
#ifndef debuglog_h
#define debuglog_h

#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/frame.h>
class AppDebugLog : public wxFrame {
    protected:
		wxTextCtrl* m_log;

    public:
        AppDebugLog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Debug Log"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 539,399 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
        void Log(const char* msg);
        void Log(const wxString& msg);
        virtual ~AppDebugLog();
    private:
        void OnClose(wxCloseEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif
