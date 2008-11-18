/***************************************************************
 * Name:      filevid.cpp
 * Purpose:   Implementation for the FileVID class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "filevid.h"
#include "sybitmap.h"

// -----------------
// Begin FileVIDData
// -----------------

class FileVIDData {
    public:
        FileVIDData(FileVID* parent);
        FileVID* m_Parent;
        std::string m_Filename;
};

FileVIDData::FileVIDData(FileVID* parent) : m_Parent(parent),
m_Filename("")
{
}

// ---------------
// End FileVIDData
// ---------------

FileVID::FileVID() {
    m_Width = 320;
    m_Height = 200;
    m_ColorFormat = vcfBGR24;
    m_Data = new FileVIDData(this);
    m_Data->m_Filename.clear();
}

FileVID::~FileVID(){
    delete m_Data;
}

void FileVID::LoadCurrentFrame(){
    // TODO: Implement FileVID::LoadCurrentFrame
    m_Bitmap->Clear();
}

bool FileVID::SetFile(const char* filename) {
    if(IsOk()) { return false; } // File can't be changed while playing!
    m_Data->m_Filename = std::string(filename);
    return true;
}

bool FileVID::SetFile(const std::string& filename) {
    if(IsOk()) { return false; } // File can't be changed while playing!
    m_Data->m_Filename = filename;
    return true;
}

std::string FileVID::GetFile() {
    return m_Data->m_Filename;
}

bool FileVID::AllocateResources() {
    bool result = false;
    if(!VideoInputDevice::AllocateResources()) {
        result = false;
    } else {
        // Allocate Resources (open file) here
        // TODO: Integrate the file handlers inside FileVID::AllocateResources()
    }
    return result;
}

void FileVID::FreeResources() {
    // Free Resources here

    VideoInputDevice::FreeResources();
}

