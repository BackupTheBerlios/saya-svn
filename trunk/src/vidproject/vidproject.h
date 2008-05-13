#ifndef VIDPROJECT_H
#define VIDPROJECT_H

#include <wx/string.h>
#include <map>
#include <deque>
#include "undohistory.h"

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
        void SaveToXml(wxString &data);
        bool Save();
        bool SaveAs(const wxString filename);
        bool SaveCopy(const wxString filename);
        void Revert(); // Reverts to its original state (after loading from disk)
        void Clear();

        // Undo History management functions
        void ClearUndoHistory();
        void Undo();
        void Redo();
        void PushUndo(const wxString OpName);
        const wxString GetUndoOpname();
        const wxString GetRedoOpname();
        const wxString GetUndoHistoryOpName(unsigned int idx);
        unsigned int GetUndoIdx();

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
        wxString m_OriginalXML;
        UndoHistoryClass m_UndoHistory;
        void SaveState(wxString& data);
        bool LoadState(const wxString& data);
};

#endif // VIDPROJECT_H
