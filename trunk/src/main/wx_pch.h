/***************************************************************
 * Name:      wx_pch.h
 * Purpose:   Header to create Pre-Compiled Header (PCH)
 * Author:    Ricardo Garcia ()
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia ()
 * License:
 **************************************************************/

#ifndef WX_PCH_H_INCLUDED
#define WX_PCH_H_INCLUDED

// basic wxWidgets headers
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#ifdef WX_PRECOMP
    // put here all your rarely-changing header files
    #include <wx/xrc/xmlres.h>
    #include <wx/config.h>
    #include <wx/filedlg.h>
    #include <wx/aui/aui.h>
    #include <wx/splitter.h>
    #include <wx/treectrl.h>
    #include <wx/spinctrl.h>
    #include <wx/sizer.h>
    #include <wx/html/htmlwin.h>
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
