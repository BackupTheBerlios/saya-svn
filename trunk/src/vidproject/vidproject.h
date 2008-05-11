#ifndef VIDPROJECT_H
#define VIDPROJECT_H

#include <wx/wx.h>
#include <map>

class VidProject;
class ExportSettings;

typedef std::map<wxString,wxString> propertymap;

class ExportSettings {
    public:
        ExportSettings();
        virtual ~ExportSettings();
        void ResetToDefaults();
        unsigned int width;
        unsigned int height;
        float aspectratio;
        float fps;
        wxString exportformat;
        wxString videocodec;
        wxString audiocodec;
        propertymap videocodecsettings;
        propertymap audiocodecsettings;
        propertymap formatsettings;
};

class VidProject
{
    public:
        VidProject();
        virtual ~VidProject();
        static VidProject* Load(const wxString filename,wxString &errortext);
        bool LoadFromXml(const wxString &data);
        bool SaveToXml(wxString &data);
        bool IsModified();
        void SetModified();
        void ResetModified();
        bool m_NeedsExportSettings;
        wxString m_Title;
        wxString m_Filename;
        bool IsNew();
        ExportSettings m_ExportSettings;

    protected:
    private:
        bool m_IsModified;
};

#endif // VIDPROJECT_H
