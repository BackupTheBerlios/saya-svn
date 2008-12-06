/***************************************************************
 * Name:      sayaApp.h
 * Purpose:   Defines Application Class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef APP_H
#define APP_H

#include <wx/app.h>

class AppDebugLog;
class syString;
class wxConfig;
class AppConfig;
class AppConfigProvider;
extern wxFrame* CreateMainFrame();

// *** Our App ***
class App : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
        bool LoadConfig();
        bool LoadXRCResources();
        void InitManagers();
        DECLARE_EVENT_TABLE()
        virtual ~App();
    private:
        void OnExitApp(wxCommandEvent& event);
        AppConfigProvider* m_configprovider;
        AppDebugLog* m_debuglog;
};

#endif // SAYAAPP_H
