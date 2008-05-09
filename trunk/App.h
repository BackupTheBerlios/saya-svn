/***************************************************************
 * Name:      sayaApp.h
 * Purpose:   Defines Application Class
 * Author:    Ricardo Garcia ()
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia ()
 * License:
 **************************************************************/

#ifndef APP_H
#define APP_H

#include <wx/app.h>

class App : public wxApp
{
    public:
        virtual bool OnInit();
        bool LoadConfig();
        void OnShowStartupDialog(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
    private:
};

#endif // SAYAAPP_H
