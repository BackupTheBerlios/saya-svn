/***************************************************************
 * Name:      vidproject.h
 * Purpose:   Implementation of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "vidproject.h"
#include "projectmanager.h"
#include <wx/ffile.h>
#include "tinyxml/tinywxuni.h"
#include "tinyxml/tinyxml.h"

AVSettings::AVSettings() {
    ResetToDefaults();
}

AVSettings::~AVSettings() {
}

void AVSettings::ResetToDefaults() {
    width = 720;
    height = 480;
    aspectratio = ARWide;
    fps = 29.997;
    vidformat = _T("avi");
    videocodec = _T("");
    audiocodec = _T("");
    videocodecsettings.clear();
    audiocodecsettings.clear();
    formatsettings.clear();
}

bool VidProject::unserialize(const wxString& data) {
    // TODO (rick#1#): Implement me!
    Clear();
    return true;
}

wxString VidProject::serialize() {
    // TODO (rick#1#): Implement me!
    wxString data;
    data = _T("<?xml version=\"1.0\"?>\n<xvidproject version=\"1.0\">\n</xvidproject>\n");
    return data;
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

const wxString VidProject::GetOfflineProjectTitle(const wxString& filename) {
    wxString result = wxEmptyString;
    wxString data;
    do {
        if(!wxFileExists(filename)) {
            break;
        }
        TiXmlDocument* mydoc = TinyXML::LoadDocument(filename);
        if(!mydoc) {
            break;
        }
        // TODO: Obtain the project's name from the XML
        TiXmlHandle hRoot(0);
        hRoot = mydoc->FirstChildElement("xvidproject");
        TiXmlElement* pElem = hRoot.FirstChild("title").ToElement();
        if(pElem) {
            const char *pKey = pElem->GetText();
            if(pKey) {
                result = wxString(pKey,wxConvUTF8);
            }
        }
    } while(false);

    return result;
}


void VidProject::Revert() {
    if(IsNew()) {
        return;
    }
    unserialize(m_OriginalXML);
    ResetModified();
    ClearUndoHistory();
    // At this point the relation between the Undo history
    // and the original is lost; plus, the clips in the resources area may have changed.
    // Therefore, the Undo history must be cleared.
}

bool VidProject::Reload() {
    if(IsNew()) {
        return false;
    }
    wxString data = wxEmptyString;
    wxString currentdata = serialize();
    wxString original_xml = m_OriginalXML;
    bool result = false;
    wxFFile myfile;

    do {
        if(!wxFileExists(m_Filename)) {
            break;
        }
        if(!myfile.Open(m_Filename)) {
            break;
        }
        if(!myfile.ReadAll(&data)) {
            break;
        }
        myfile.Close();
        result = unserialize(data);
        if(result) {
            m_OriginalXML = data;
            ResetModified();
        }
    } while(false);

    if(myfile.IsOpened()) {
        myfile.Close();
    }
    if(!result) {
        unserialize(currentdata);
        m_OriginalXML = original_xml;
        SetModified();
    }
    ClearUndoHistory();
    return result;
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

bool VidProject::CanUndo() {
    return m_UndoHistory.CanUndo();
}

bool VidProject::CanRedo() {
    return m_UndoHistory.CanRedo();
}


void VidProject::Undo() {
    wxString data,curdata;
    curdata = wxEmptyString;
    if(m_UndoHistory.IsEof()) {
        SaveState(curdata); // If there's no redo available, make one.
    }

    if(m_UndoHistory.Undo(data,curdata)) {
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
    return m_UndoHistory.CurState();
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
        result = nextproject->unserialize(data);
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
        m_OriginalXML = serialize();
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
        data = serialize();
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

