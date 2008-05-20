#ifndef TINYWXUNI_H
#define TINYWXUNI_H

/***************************************************************
 * Name:      tinywxuni.h
 * Purpose:   wxWidgets wrapper function declarations for TinyXML class
 * Author:    Thanks to the Code::Blocks team  (specially
 *            mortenmacfly, killerbot, mandrav and thomasdenk)
 *            for making this file and making all the necessary
 *            adaptations to the TinyXML class so it can work
 *            with wxWidgets.
 * Created:   2008-05-16
 * Copyright: I guess Yiannis Mandravelos (mandrav), since he's
 *            the project leader for the Code::Blocks IDE.
 * License:   GPL version 3
 **************************************************************/

class wxString;
class TiXmlDocument;

namespace TinyXML
{
    bool LoadDocument(const wxString& filename, TiXmlDocument *doc);
    TiXmlDocument* LoadDocument(const wxString& filename);

    bool SaveDocument(const wxString& filename, TiXmlDocument* doc);
}

#endif
