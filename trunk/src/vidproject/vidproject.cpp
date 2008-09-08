/***************************************************************
 * Name:      vidproject.h
 * Purpose:   Implementation of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "../iomgr/iocommon.h"
#include "vidproject.h"
#include "projectmanager.h"
#include "tinyxml/tinyxml.h"
#include <libintl.h>

AVSettings::AVSettings() {
    ResetToDefaults();
}

AVSettings::~AVSettings() {
}

void AVSettings::ResetToDefaults() {
    width = 720;
    height = 480;
    fps = 29.997;
    pixelaspect = 1.0;
    vidformat = "avi";
    videocodec = "";
    audiocodec = "";
    videocodecsettings.clear();
    audiocodecsettings.clear();
    formatsettings.clear();
}

bool VidProject::unserialize(const std::string& data) {
    // TODO: Implement VidProject::unserialize
    Clear();
    return true;
}

std::string VidProject::serialize() {
    // TODO: Implement VidProject::serialize
    std::string data;
    data = "<?xml version=\"1.0\"?>\n<xvidproject version=\"1.0\">\n</xvidproject>\n";
    return data;
}

VidProject::VidProject()
{
    //ctor
    m_Title = "";
    m_ExportSettings.ResetToDefaults();
    m_NeedsExportSettings = true;
    m_Filename = "";
}

VidProject::~VidProject()
{
    //dtor
}

void VidProject::Clear() {
    m_ExportSettings.ResetToDefaults();
}

const std::string VidProject::GetOfflineProjectTitle(const std::string& filename) {
    std::string result = "";
    std::string data;
    do {
        if(!ioCommon::FileExists(filename)) {
            break;
        }
        TiXmlDocument mydoc;
        if(!mydoc.LoadFile(filename.c_str())) {
            break;
        }
        TiXmlHandle hRoot(0);
        hRoot = mydoc.FirstChildElement("xvidproject");
        TiXmlElement* pElem = hRoot.FirstChild("title").ToElement();
        if(pElem) {
            const char *pKey = pElem->GetText();
            if(pKey) {
                result = pKey;
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
    std::string data = "";
    std::string currentdata = serialize();
    std::string original_xml = m_OriginalXML;
    bool result = false;
    FFile myfile;

    do {
        if(!ioCommon::FileExists(m_Filename)) {
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

void VidProject::SaveState(std::string& data) {
    data = "";
// TODO: Implement project State saving
}

bool VidProject::LoadState(const std::string& data) {
// TODO: Implement project State loading
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
    std::string data,curdata;
    curdata = "";
    if(m_UndoHistory.IsEof()) {
        SaveState(curdata); // If there's no redo available, make one.
    }

    if(m_UndoHistory.Undo(data,curdata)) {
        LoadState(data);
    }
}

void VidProject::Redo() {
    std::string data;
    if(m_UndoHistory.Redo(data)) {
        LoadState(data);
    }
}

void VidProject::PushUndo(const std::string OpName) {
    std::string data;
    SaveState(data);
    m_UndoHistory.PushUndo(OpName,data);
    if(m_UndoHistory.Redo(data)) {
        LoadState(data);
    }
}

const std::string VidProject::GetUndoOpname() {
    return m_UndoHistory.GetUndoOpname();
}

const std::string VidProject::GetRedoOpname() {
    return m_UndoHistory.GetRedoOpname();
}

const std::string VidProject::GetUndoHistoryOpName(unsigned int idx) {
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
    return (m_Filename.empty());
}

VidProject* VidProject::Load(const std::string filename, std::string &errortext) {
    std::string data = "";
    VidProject* nextproject = new VidProject;
    bool result = false;
    FFile myfile;
    do {
        if(!ioCommon::FileExists(filename)) {
            errortext = ioCommon::Printf(gettext("Error: Could not find file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.Open(filename)) {
            errortext = ioCommon::Printf(gettext("Error: Could not open file '%s'!"),filename.c_str());
            break;
        }
        if(!myfile.ReadAll(&data)) {
            errortext = ioCommon::Printf(gettext("Error: Could not read file '%s'!"),filename.c_str());
            break;
        }
        myfile.Close();
        result = nextproject->unserialize(data);
        if(!result) {
            errortext = ioCommon::Printf(gettext("Error: File '%s' contains invalid data!"),filename.c_str());
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

bool VidProject::SaveAs(const std::string filename) {
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

bool VidProject::SaveCopy(const std::string filename) {
    bool result = SaveToFile(filename);
    return result;
}

bool VidProject::SaveToFile(const std::string &filename) {
    if(filename.empty()) {
        return false;
    }
    std::string data;
    bool result = false;
    do {
        data = serialize();
        TempFile tmpfile(filename);
        if(!tmpfile.IsOpened()) break;
        if(!tmpfile.Write(data)) {
            tmpfile.Discard();
            break;
        }
        result = tmpfile.Commit();
    }while(false);
    return result;
}

