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


#include "../saya/core/debuglog.h"
#include <wx/frame.h>

class syString;
class wxString;
class wxCloseEvent;
class wxTextCtrl;

class AppDebugLog : public wxFrame, public syDebugLog {

    protected:
		wxTextCtrl* m_log;

    public:

        AppDebugLog();

        virtual void DebugLog(const char* msg);

        virtual void DebugLog(const syString& msg);

        void DebugLog(const wxString& msg);

        virtual ~AppDebugLog();
    private:
        void OnClose(wxCloseEvent& event);
        void OnIdle(wxIdleEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif
