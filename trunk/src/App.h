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

class App : public wxApp
{
    public:
        virtual bool OnInit();
        bool LoadConfig();
        bool LoadXRCResources();
        void InitManagers();
        DECLARE_EVENT_TABLE()
    private:
};

#endif // SAYAAPP_H
