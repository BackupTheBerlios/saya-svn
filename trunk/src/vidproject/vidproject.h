#ifndef VIDPROJECT_H
#define VIDPROJECT_H

/***************************************************************
 * Name:      vidproject.h
 * Purpose:   Definition of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <wx/string.h>
#include <map>
#include <deque>
#include "avclip.h"
#include "undohistory.h"

class VidProject;
class ExportSettings;

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
        bool CanUndo();
        bool CanRedo();
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
        AVTimeline m_Timeline; // the data for all clips
        AVResources m_Resources;
};

#endif // VIDPROJECT_H
