/***************************************************************
 * Name:      filevid.cpp
 * Purpose:   Implementation for the FileVID class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-11-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "systring.h"
#include "filevid.h"
#include "sybitmap.h"

// -------------------
// Begin FileVID::Data
// -------------------

class FileVID::Data {
    public:
        Data(FileVID* parent);
        ~Data();
        FileVID* m_Parent;
        AVSource* m_VirtualVID;
        syString m_Filename;
        void ClearVirtualVID();
        void SetFilename(const syString& filename);
};

FileVID::Data::Data(FileVID* parent) :
m_Parent(parent),
m_VirtualVID(0),
m_Filename("")
{
}

void FileVID::Data::ClearVirtualVID() {
    delete m_VirtualVID;
    m_VirtualVID = 0;
}

void FileVID::Data::SetFilename(const syString& filename) {
    ClearVirtualVID();
    m_Filename = filename;
    m_VirtualVID = AVSource::CreateSource(filename.c_str());
}

FileVID::Data::~Data() {
    ClearVirtualVID();
}

// -----------------
// End FileVID::Data
// -----------------

FileVID::FileVID() {
    m_Width = 320;
    m_Height = 200;
    m_ColorFormat = vcfBGR24;
    m_IsVideo = true;
    m_IsAudio = false; // Just for now
    m_Data = new Data(this);
    m_Data->m_Filename.clear();
}

FileVID::~FileVID(){
    delete m_Data;
}

bool FileVID::SetFile(const char* filename) {
    if(IsOk()) { return false; } // File can't be changed while playing!
    m_Data->SetFilename(filename);
    return true;
}

bool FileVID::SetFile(const syString& filename) {
    if(IsOk()) { return false; } // File can't be changed while playing!
    m_Data->SetFilename(filename);
    return true;
}

syString FileVID::GetFile() {
    return m_Data->m_Filename;
}

bool FileVID::AllocateResources() {
    bool result = false;
    if(!m_Data->m_Filename.empty() && !m_Data->m_VirtualVID) {
        m_Data->m_VirtualVID = AVSource::CreateSource(m_Data->m_Filename.c_str());
    }
    if(m_Data->m_VirtualVID) {
        // We'll mirror the VirtualVID by copying all of its parameters, even m_Bitmap.
        // This way we won't have to deal with copying the data.
        result = m_Data->m_VirtualVID->Init();
        m_CurrentVideoTime = m_Data->m_VirtualVID->GetVideoPos();
        m_VideoLength = m_Data->m_VirtualVID->GetVideoLength();
        m_Width = m_Data->m_VirtualVID->GetWidth();
        m_Height = m_Data->m_VirtualVID->GetHeight();
        m_ColorFormat = m_Data->m_VirtualVID->GetColorFormat();
        m_PixelAspect = m_Data->m_VirtualVID->GetPixelAspect();
        m_FramesPerSecond = m_Data->m_VirtualVID->GetFramesPerSecond();
    } else {
        if(!AVSource::AllocateResources()) {
            result = false;
        } else {
            #warning TODO implement the file part of FileVID::AllocateResources()
            // Allocate Resources (open file) here
        }
    }
    return result;
}

void FileVID::FreeResources() {
    // Free Resources here
    if(m_Data->m_VirtualVID) {
        m_Data->ClearVirtualVID();
    } else {
        #warning TODO implement the file part of FileVID::FreeResources()
        // Close file here
        AVSource::FreeResources();
    }
    m_Bitmap = 0;
}

unsigned long FileVID::GetFrameIndex(avtime_t time) {
    #warning TODO implement FileVID::GetFrameIndex(avtime_t time)
    // This is a stub.
    if(m_Data->m_VirtualVID) {
        return m_Data->m_VirtualVID->GetFrameIndex(time);
    }
    return AVSource::GetFrameIndex(time);
}

avtime_t FileVID::GetTimeFromFrameIndex(unsigned long  frame, bool fromend) {
    #warning TODO implement FileVID::GetTimeFromFrameIndex(unsigned long  frame, bool fromend)
    // This is a stub.
    if(m_Data->m_VirtualVID) {
        return m_Data->m_VirtualVID->GetTimeFromFrameIndex(frame, fromend);
    }
    return AVSource::GetTimeFromFrameIndex(frame, fromend);
}

void FileVID::LoadCurrentFrame() {
    if(m_Data->m_VirtualVID) {
        m_Data->m_VirtualVID->SendCurrentFrame(static_cast<syBitmap*>(0));
    } else {
        #warning TODO implement FileVID::LoadCurrentFrame()
        // TODO: Implement FileVID::LoadCurrentFrame
        if(m_Bitmap) {
            m_Bitmap->Clear();
        }
    }
}

avtime_t FileVID::SeekVideoResource(avtime_t time) {
    if(m_Data->m_VirtualVID) {
        return m_Data->m_VirtualVID->SeekVideo(time);
    } else {
        #warning TODO implement FileVID::SeekResource(avtime_t time)
        // This is a stub
        // here should go the CODEC call.
        return time;
    }
}

const syBitmap* FileVID::GetBitmap() {
    if(m_Data->m_VirtualVID) {
        return m_Data->m_VirtualVID->GetBitmap();
    } else {
        return m_Bitmap;
    }
}
