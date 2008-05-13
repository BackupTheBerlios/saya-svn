#include "vidproject.h"
#include "projectmanager.h"
#include <wx/ffile.h>

VideoSettings::VideoSettings() {
    ResetToDefaults();
}

VideoSettings::~VideoSettings() {
}

void VideoSettings::ResetToDefaults() {
    width = 720;
    height = 480;
    aspectratio = 16/9.0;
    fps = 29.997;
    vidformat = _T("avi");
    videocodec = _T("");
    audiocodec = _T("");
    videocodecsettings.clear();
    audiocodecsettings.clear();
    formatsettings.clear();
}


VidProject::VidProject()
{
    //ctor
    m_Title = wxEmptyString;
    m_ExportSettings.ResetToDefaults();
    m_NeedsExportSettings = true;
    m_Filename = wxEmptyString;
}

VidProject::~VidProject()
{
    //dtor
}

void VidProject::Clear() {
    m_ExportSettings.ResetToDefaults();
}

void VidProject::Revert() {
    LoadFromXml(m_OriginalXML);
    ResetModified();
    ClearUndoHistory();
    // At this point the relation between the Undo history
    // and the original is lost; plus, the clips in the resources area may have changed.
    // Therefore, the Undo history must be cleared.
}

void VidProject::SaveState(wxString& data) {
    data = wxEmptyString;
// TODO (rick#1#): Implement project State saving
}

bool VidProject::LoadState(const wxString& data) {
// TODO (rick#1#): Implement project State loading
    SetModified();
    return true;
}

void VidProject::ClearUndoHistory() {
    m_UndoHistory.Clear();
}

void VidProject::Undo() {
    wxString data;
    if(m_UndoHistory.Undo(data)) {
        LoadState(data);
    }
}

void VidProject::Redo() {
    wxString data;
    if(m_UndoHistory.Redo(data)) {
        LoadState(data);
    }
}

void VidProject::PushUndo(const wxString OpName) {
    wxString data;
    SaveState(data);
    m_UndoHistory.PushUndo(OpName,data);
    if(m_UndoHistory.Redo(data)) {
        LoadState(data);
    }
}

const wxString VidProject::GetUndoOpname() {
    return m_UndoHistory.GetUndoOpname();
}

const wxString VidProject::GetRedoOpname() {
    return m_UndoHistory.GetRedoOpname();
}

const wxString VidProject::GetUndoHistoryOpName(unsigned int idx) {
    return m_UndoHistory.GetOpname(idx);
}

unsigned int VidProject::GetUndoIdx() {
    return m_UndoHistory.GetStateIdx();
}

bool VidProject::IsModified() {
    return m_IsModified;
}

void VidProject::SetModified() {
    bool lastmodified = m_IsModified;
    m_IsModified = true;
    if(m_IsModified != lastmodified) {
        ProjectManager::Get()->OnProjectStatusModified();
    }
}

void VidProject::ResetModified() {
    bool lastmodified = m_IsModified;
    m_IsModified = false;
    if(m_IsModified != lastmodified) {
        ProjectManager::Get()->OnProjectStatusModified();
    }
}

bool VidProject::IsNew() {
    return (m_Filename.IsEmpty());
}

bool VidProject::LoadFromXml(const wxString &data) {
    Clear();
// TODO (rick#1#): Implement VidProject::LoadFromXml
    return true;
}

void VidProject::SaveToXml(wxString &data) {
// TODO (rick#1#): Implement VidProject::SaveToXml
    data = _T("<?xml version=\"1.0\"?>\n<xvidproject version=\"1.0\">\n</xvidproject>\n");
    return;
}

VidProject* VidProject::Load(const wxString filename, wxString &errortext) {
    wxString data = wxEmptyString;
    VidProject* nextproject = new VidProject;
    bool result = false;
    wxFFile myfile;
    do {
        if(!wxFileExists(filename)) {
            errortext.Printf(_("Error: Could not find file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.Open(filename)) {
            errortext.Printf(_("Error: Could not open file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.ReadAll(&data)) {
            errortext.Printf(_("Error: Could not read file '%s'!"),filename.c_str());
            break;
        }
        myfile.Close();
        result = nextproject->LoadFromXml(data);
        if(!result) {
            errortext.Printf(_("Error: File '%s' contains invalid data!"),filename.c_str());
        } else {
            nextproject->m_Filename = filename;
            nextproject->ResetModified();
        }
    } while(false);

    if(myfile.IsOpened()) {
        myfile.Close();
    }
    if(!result) {
        delete nextproject;
        nextproject = NULL;
    }
    return nextproject;
}

bool VidProject::Save() {
    bool result = SaveToFile(m_Filename);
    if(result) {
        SaveToXml(m_OriginalXML);
        ResetModified();
        if(ProjectManager::Get()->GetClearUndoHistoryOnSave()) {
            ClearUndoHistory();
        }
    }
    return result;
}

bool VidProject::SaveAs(const wxString filename) {
    bool result = SaveToFile(filename);
    if(result) {
        m_Filename = filename;
        m_IsModified = false;
        ProjectManager::Get()->OnProjectStatusModified(); // Update status and filename
        if(ProjectManager::Get()->GetClearUndoHistoryOnSave()) {
            ClearUndoHistory();
        }
    }
    return result;
}

bool VidProject::SaveCopy(const wxString filename) {
    bool result = SaveToFile(filename);
    return result;
}

bool VidProject::SaveToFile(const wxString &filename) {
    if(filename.IsEmpty()) {
        return false;
    }
    wxString data;
    bool result = false;
    do {
        SaveToXml(data);
        wxTempFile tmpfile(filename);
        if(!tmpfile.IsOpened()) break;
        if(!tmpfile.Write(data,wxConvUTF8)) {
            tmpfile.Discard();
            break;
        }
        result = tmpfile.Commit();
    }while(false);
    return result;
}
