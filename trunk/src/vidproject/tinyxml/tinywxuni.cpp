/***************************************************************
 * Name:      tinywxuni.cpp
 * Purpose:   wxWidgets wrapper functions for TinyXML class
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

#include <wx/file.h>
#include <wx/string.h>
#include "tinywxuni.h"
#include "tinyxml.h"

bool TinyXML::LoadDocument(const wxString& filename, TiXmlDocument *doc)
{

    if(!doc || !wxFile::Access(filename, wxFile::read))
        return false;

    wxFile file(filename);
    size_t len = file.Length();

    char *input = new char[len+1];
    input[len] = '\0';
    file.Read(input, len);

    doc->Parse(input);
    delete[] input;
    return true;
}

TiXmlDocument* TinyXML::LoadDocument(const wxString& filename)
{
    TiXmlDocument* doc = new TiXmlDocument();

    if(TinyXML::LoadDocument(filename, doc))
    {
        return doc;
    }
    else
    {
        delete doc;
        return 0;
    }
}

bool TinyXML::SaveDocument(const wxString& filename, TiXmlDocument* doc)
{
    if (!doc)
        return false;

    TiXmlPrinter printer;
    printer.SetIndent("\t");
    doc->Accept(&printer);

    wxTempFile file(filename);
    if(file.IsOpened())
        if(file.Write(printer.CStr(), printer.Size()) && file.Commit())
            return true;
    return false;
}

