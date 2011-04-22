/***************************************************************
 * Name:      vidproject.cpp
 * Purpose:   Implementation of the VidProject class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "core/iocommon.h"
#include "core/systring.h"

#include "timeline/avcommon.h"
#include "timeline/avsettings.h"
#include "timeline/avresource.h"
#include "timeline/avresources.h"
#include "timeline/avsequence.h"
#include "timeline/avtimeline.h"
#include "timeline/smap.h"
#include "timeline/smapxstr.h"

#include "vidproject.h"
#include "undohistory.h"
#include "inputmonitor.h"

#include "projectmanager.h"

#include "tinyxml/tinyxml.h"
#include "core/intl.h"

// --------------------
// Begin VidProjectData
// --------------------
class VidProjectData {
    public:

        /** Constructor */
        VidProjectData(VidProject* parent);

        /** Destructor */
        ~VidProjectData();

        VidProject* m_Parent;

        /** Saves the project to a given filename. */
        bool SaveToFile(const char* filename);

        /** Saves project's current state. */
        void SaveState(syString& data);

        /** Loads specified state from the project. */
        bool LoadState(const syString& data);

        /** Refreshes the Resource Map */
        void RefreshResourceMap();

        /** Project's modified flag */
        bool m_IsModified;

        /** Project's original state as loaded from disk. */
        syString m_OriginalXML;

        /** Undo History Stack. @see UndoHistoryClass */
        UndoHistoryClass* m_UndoHistory;

        /** The data for the sequences and clips in the project. */
        AVTimeline* m_Timeline; // the data for all clips

        /** The data for the resources used in the project. */
        AVResources* m_Resources;

        /** A map for the resources used in the project. */
        SMapUintUint* m_ResourceMap;

        SMapStr<unsigned int>* m_ResourceFilenameMap;

        unsigned int m_MaxResourceId;

        /** Project's Title */
        syString m_Title;

        /** Project's Filename */
        syString m_Filename;
};

VidProjectData::VidProjectData(VidProject* parent) :
m_Parent(parent),
m_IsModified(false),
m_UndoHistory(NULL),
m_Timeline(NULL),
m_Resources(NULL),
m_MaxResourceId(0),
m_Title(""),
m_Filename("")
{
    m_UndoHistory = new UndoHistoryClass;
    m_Timeline = new AVTimeline;
    m_Resources = new AVResources;
    m_ResourceMap = new SMapUintUint;
    m_ResourceFilenameMap = new SMapStr<unsigned int>;
}

VidProjectData::~VidProjectData() {
    delete m_ResourceFilenameMap;
    delete m_ResourceMap;
    delete m_Resources;
    delete m_Timeline;
    delete m_UndoHistory;
}

void VidProjectData::SaveState(syString& data) {
    data = "";
// TODO: Implement project State saving
}

bool VidProjectData::LoadState(const syString& data) {
    m_MaxResourceId = 0;
    // TODO: Implement project State loading
    RefreshResourceMap();
    m_Parent->SetModified();
    return true;
}

void VidProjectData::RefreshResourceMap() {
    m_ResourceMap->clear();
    m_ResourceFilenameMap->clear();
    unsigned int i,resource_id;
    AVResource* tmpres;
    for(i = 0; i < m_Resources->size(); ++i) {
        tmpres = &(m_Resources->operator[](i));
        if(tmpres) {
            resource_id = tmpres->m_ResourceId;
            m_ResourceMap->operator[](resource_id) = i;
            m_ResourceFilenameMap->operator[](tmpres->m_Filename.c_str()) = resource_id;
            if(m_MaxResourceId < resource_id) {
                m_MaxResourceId = resource_id;
            }
        }
    }
}

bool VidProjectData::SaveToFile(const char* filename) {
    if(!filename || *filename == 0) {
        return false;
    }
    syString data;
    bool result = false;
    do {
        data = m_Parent->serialize();
        result = TempFile::Write(filename, data.c_str());
    }while(false);
    return result;
}

// --------------------
// End VidProjectData
// --------------------

// ----------------
// Begin VidProject
// ----------------

bool VidProject::unserialize(const syString& data) {
    // TODO: Implement VidProject::unserialize
    Clear();
    return true;
}

syString VidProject::serialize() {
    // TODO: Implement VidProject::serialize
    syString data;
    data = "<?xml version=\"1.0\"?>\n<xvidproject version=\"1.0\">\n</xvidproject>\n";
    return data;
}

VidProject::VidProject() {
    m_Data = new VidProjectData(this);
    m_ExportSettings = new AVSettings;
    m_ExportSettings->ResetToDefaults();
    m_NeedsExportSettings = true;
}

VidProject::~VidProject()
{
    // dtor
    delete m_ExportSettings;
}

void VidProject::Clear() {
    m_ExportSettings->ResetToDefaults();
}

const syString VidProject::GetOfflineProjectTitle(const char* filename) {
    syString result = "";
    syString data;
    do {
        if(!ioCommon::FileExists(filename)) {
            break;
        }
        TiXmlDocument mydoc;
        if(!mydoc.LoadFile(filename)) {
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
    unserialize(m_Data->m_OriginalXML);
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
    syString data = "";
    syString currentdata = serialize();
    syString original_xml = m_Data->m_OriginalXML;
    bool result = false;
    FFile myfile;

    do {
        if(!ioCommon::FileExists(m_Data->m_Filename)) {
            break;
        }
        if(!myfile.Open(m_Data->m_Filename.c_str())) {
            break;
        }
        if(!myfile.ReadAll(data)) {
            break;
        }
        myfile.Close();
        result = unserialize(data);
        if(result) {
            m_Data->m_OriginalXML = data;
            ResetModified();
        }
    } while(false);

    if(myfile.IsOpened()) {
        myfile.Close();
    }
    if(!result) {
        unserialize(currentdata);
        m_Data->m_OriginalXML = original_xml;
        SetModified();
    }
    ClearUndoHistory();
    return result;
}

void VidProject::ClearUndoHistory() {
    m_Data->m_UndoHistory->Clear();
}

bool VidProject::CanUndo() const {
    return m_Data->m_UndoHistory->CanUndo();
}

bool VidProject::CanRedo() const {
    return m_Data->m_UndoHistory->CanRedo();
}


void VidProject::Undo() {
    syString data,curdata;
    curdata = "";
    if(m_Data->m_UndoHistory->IsEof()) {
        m_Data->SaveState(curdata); // If there's no redo available, make one.
    }

    if(m_Data->m_UndoHistory->Undo(data,curdata)) {
        m_Data->LoadState(data);
    }
}

void VidProject::Redo() {
    syString data;
    if(m_Data->m_UndoHistory->Redo(data)) {
        m_Data->LoadState(data);
    }
}

void VidProject::PushUndo(const char* OpName) {
    syString data;
    m_Data->SaveState(data);
    m_Data->m_UndoHistory->PushUndo(OpName,data);
    if(m_Data->m_UndoHistory->Redo(data)) {
        m_Data->LoadState(data);
    }
}

const syString VidProject::GetUndoOpname() const {
    return m_Data->m_UndoHistory->GetUndoOpname();
}

const syString VidProject::GetRedoOpname() const {
    return m_Data->m_UndoHistory->GetRedoOpname();
}

const syString VidProject::GetUndoHistoryOpName(unsigned int idx) const {
    return m_Data->m_UndoHistory->GetOpname(idx);
}

unsigned int VidProject::GetUndoIdx() const {
    return m_Data->m_UndoHistory->CurState();
}

bool VidProject::IsModified() const {
    return m_Data->m_IsModified;
}

void VidProject::SetModified() {
    bool lastmodified = m_Data->m_IsModified;
    m_Data->m_IsModified = true;
    if(m_Data->m_IsModified != lastmodified) {
        ProjectManager::Get()->OnProjectStatusModified();
    }
}

void VidProject::ResetModified() {
    bool lastmodified = m_Data->m_IsModified;
    m_Data->m_IsModified = false;
    if(m_Data->m_IsModified != lastmodified) {
        ProjectManager::Get()->OnProjectStatusModified();
    }
}

bool VidProject::IsNew() const {
    return (m_Data->m_Filename.empty());
}

VidProject* VidProject::Load(const char* filename, syString &errortext) {
    syString data = "";
    VidProject* nextproject = new VidProject;
    bool result = false;
    FFile myfile;
    do {
        if(!ioCommon::FileExists(filename)) {
            errortext.Printf(_("Error: Could not find file '%s'!"),filename);
            break;
        }
        if(!myfile.Open(filename)) {
            errortext.Printf(_("Error: Could not open file '%s'!"),filename);
            break;
        }
        if(!myfile.ReadAll(data)) {
            errortext.Printf(_("Error: Could not read file '%s'!"),filename);
            break;
        }
        myfile.Close();
        result = nextproject->unserialize(data);
        if(!result) {
            errortext.Printf(_("Error: File '%s' contains invalid data!"),filename);
        } else {
            nextproject->m_Data->m_Filename = filename;
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

VidProject* VidProject::Load(const syString& filename, syString &errortext) {
    return VidProject::Load(filename.c_str(), errortext);
}

bool VidProject::Save() {
    bool result = m_Data->SaveToFile(m_Data->m_Filename.c_str());
    if(result) {
        m_Data->m_OriginalXML = serialize();
        ResetModified();
        if(ProjectManager::Get()->GetClearUndoHistoryOnSave()) {
            ClearUndoHistory();
        }
    }
    return result;
}

bool VidProject::SaveAs(const char* filename) {
    bool result = m_Data->SaveToFile(filename);
    if(result) {
        m_Data->m_Filename = filename;
        m_Data->m_IsModified = false;
        ProjectManager::Get()->OnProjectStatusModified(); // Update status and filename
        if(ProjectManager::Get()->GetClearUndoHistoryOnSave()) {
            ClearUndoHistory();
        }
    }
    return result;
}

bool VidProject::SaveAs(const syString& filename) {
    return SaveAs(filename.c_str());
}

bool VidProject::SaveCopy(const char* filename) {
    bool result = m_Data->SaveToFile(filename);
    return result;
}

bool VidProject::SaveCopy(const syString& filename) {
    return SaveCopy(filename.c_str());
}

const char* VidProject::GetTitle() const {
    return m_Data->m_Title.c_str();
}

void VidProject::SetTitle(const char* newtitle) {
    m_Data->m_Title = newtitle;

}

const char* VidProject::GetFilename() const {
    return m_Data->m_Filename.c_str();
}

// Resources functions

unsigned int VidProject::ImportFile(const syString& filename, syString &errortext) {

    // First check for a duplicate filename.
    if(m_Data->m_ResourceFilenameMap->data.find(filename) != m_Data->m_ResourceFilenameMap->data.end()) {
        errortext = _("This file has already been imported.");
        return 0;
    }

    AVResource newres;
    newres.m_ResourceType = RTOfflineFile;
    // TODO: Implement Resource Type detection based on the filename
    newres.m_Filename = filename;
    newres.m_RelativeFilename = "";
    // TODO: Get the relative filename of the resource
    newres.m_Icon = "";
    // TODO: Get the file's icon
    newres.m_AVSettings = 0;
    // TODO: Get the file's AV Settings.

    newres.m_ResourceId = GetNewResourceId();
    m_Data->m_Resources->data.push_back(newres);
    m_Data->m_ResourceMap->operator[](newres.m_ResourceId) = m_Data->m_Resources->size() - 1;
    m_Data->m_ResourceFilenameMap->operator[](filename.c_str()) = newres.m_ResourceId;

    return newres.m_ResourceId;
}

const AVResources* VidProject::GetResources() const {
    return m_Data->m_Resources;
}

AVResource* VidProject::GetResourceById(unsigned int id) const {
    AVResource* result = 0;
    unsigned int idx = m_Data->m_ResourceMap->operator[](id);
    if(m_Data->m_Resources && m_Data->m_Resources->size() > idx) {
        result = &(m_Data->m_Resources->operator[](idx));
    }
    return result;
}

unsigned int VidProject::GetNewResourceId() {
    return ++(m_Data->m_MaxResourceId);
}

// --------------
// End VidProject
// --------------
