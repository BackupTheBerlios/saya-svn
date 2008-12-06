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
    #include <wx/slider.h>
    #include <wx/gdicmn.h>
    #include <wx/font.h>
    #include <wx/colour.h>
    #include <wx/settings.h>
    #include <wx/string.h>
    #include <wx/bitmap.h>
    #include <wx/image.h>
    #include <wx/icon.h>
    #include <wx/bmpbuttn.h>
    #include <wx/button.h>
    #include <wx/stattext.h>
    #include <wx/panel.h>
    #include <wx/fs_zip.h>
    #include <wx/fs_mem.h>
    #include <wx/log.h>
#endif // WX_PRECOMP

#endif // WX_PCH_H_INCLUDED
