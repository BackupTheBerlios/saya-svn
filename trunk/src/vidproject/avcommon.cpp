#include "avcommon.h"


wxString serializable::serialize() {
    return _T("<null />");
}

bool serializable::unserialize(const wxString& data) {
    return false;
}

wxString VideoSettings::serialize() {
    wxString result;
    wxString opentag = _T("<VideoSettings");
    wxString closetag = _T("</VideoSettings>");
    propertymap::iterator iter;

    wxString attributes;
    wxString attr;
    attributes += wxString::Format(_T(" width=\"%d\""),width);
    attributes += wxString::Format(_T(" height=\"%d\""),height);
    attributes += wxString::Format(_T(" aspectratio=\"%f\""),aspectratio);
    attributes += wxString::Format(_T(" fps=\"%f\""),fps);
    attributes += wxString::Format(_T(" fps=\"%f\""),fps);
    result = opentag + attributes + _T(">");
    result += wxString(_T("<vidformat type=\"")) + vidformat + _T("\">");
    for(iter = formatsettings.begin();iter != formatsettings.end(); iter++) {
        result += wxString(_T("<item name=\"")) + iter->first + _T("\">") + iter->second + _T("</item>");
    }
    result += _T("</vidformat>");
    result += wxString(_T("<videocodec type=\"")) + videocodec + _T("\">");
    for(iter = videocodecsettings.begin();iter != videocodecsettings.end(); iter++) {
        result += wxString(_T("<item name=\"")) + iter->first + _T("\">") + iter->second + _T("</item>");
    }
    result += _T("</videocodec>");
    result += wxString(_T("<audioocodec type=\"")) + audiocodec + _T("\">");
    for(iter = audiocodecsettings.begin();iter != audiocodecsettings.end(); iter++) {
        result += wxString(_T("<item name=\"")) + iter->first + _T("\">") + iter->second + _T("</item>");
    }
    result += _T("</audiocodec>");
    result += closetag;
    return result;
}

bool VideoSettings::unserialize(const wxString& data) {
// TODO (rick#1#): Implement me!
    return false;
}
