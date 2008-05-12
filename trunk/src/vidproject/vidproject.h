#ifndef VIDPROJECT_H
#define VIDPROJECT_H

#include <wx/wx.h>
#include <map>

class VidProject;
class ExportSettings;

typedef std::map<wxString,wxString> propertymap;

class VideoSettings {
    public:
        VideoSettings();
        virtual ~VideoSettings();
        void ResetToDefaults();
        unsigned int width;
        unsigned int height;
        float aspectratio;
        float fps;
        wxString vidformat;
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
        bool Save();
        bool SaveAs(const wxString filename);
        bool SaveCopy(const wxString filename);

        bool IsModified();
        void SetModified();
        void ResetModified();
        bool m_NeedsExportSettings;
        wxString m_Title;
        wxString m_Filename;
        bool IsNew();
        VideoSettings m_ExportSettings;


    protected:
    private:
        bool m_IsModified;
        bool SaveToFile(const wxString &filename);
};

#endif // VIDPROJECT_H
