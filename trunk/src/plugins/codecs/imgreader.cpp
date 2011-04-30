/***************************************************************
 * Name:      imgreader.h
 * Purpose:   Implementation of the Image Reader Codec Plugin
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-25
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "imgreader.h"
#include <saya/core/iocommon.h>
#include <saya/core/sythread.h>
#include <saya/core/sybitmap.h>
#include <saya/core/systringutils.h>
#include <saya/core/debuglog.h>
#include <QImageReader>

// ----------------------
// begin syImgReaderCodec
// ----------------------

class syImgReaderCodec : public CodecInstance {
        friend class syImgReaderPlugin;
    public:
        syImgReaderCodec(syImgReaderPlugin* parent, const syString& filename);
        virtual ~syImgReaderCodec();
        virtual bool OpenInput(const syString filename = syEmptyString);
        virtual void CloseInput();

        virtual bool OpenOutput(const syString filename = syEmptyString) { return false; }
        virtual void CloseOutput() {}

        // Input functions

        virtual avtime_t SeekVideo(avtime_t pos);
        virtual avtime_t GetCurrentVideoTime();

        virtual avtime_t GetVideoLength();
        virtual VideoColorFormat GetColorFormat();
        virtual unsigned long GetWidth();
        virtual unsigned long GetHeight();
        virtual float GetPixelAspect();
        virtual float GetFramesPerSecond();

        virtual unsigned long GetFrameIndex(avtime_t time);
        virtual avtime_t GetTimeFromFrameIndex(unsigned long frame, bool fromend = false);
        virtual void LoadCurrentFrame(syBitmap* dest);

    private:
        syImgReaderPlugin* m_Parent;
        syString m_Filename;
        QImageReader* m_Reader;
        QImage* m_Image;
        volatile bool m_ImageLoaded;
        sySafeMutex m_Mutex;
};


syImgReaderCodec::syImgReaderCodec(syImgReaderPlugin* parent, const syString& filename) :
m_Parent(parent),
m_Filename(filename),
m_Reader(new QImageReader()),
m_Image(0),
m_ImageLoaded(0)
{
    m_IsVideo = true;
    m_IsAudio = false;
    m_IsInput = true;
    m_IsOutput = false;
}

syImgReaderCodec::~syImgReaderCodec() {
    CloseInput();
    delete m_Image;
    m_Image = 0;
    delete m_Reader;
    m_Reader = 0;
}

bool syImgReaderCodec::OpenInput(const syString filename) {
    bool result = false;
    syString newfilename = filename;
    if(m_Filename != "" && filename.empty()) {
        newfilename = m_Filename;
    } else {
        newfilename = filename;
    }
    if(m_Reader && syString(m_Reader->fileName()) != newfilename) {
        CloseInput();
        if(m_Reader) {
            m_Reader->setFileName(newfilename);
            m_Filename = filename;
        }
    }
    if(!m_Reader) {
        m_Reader = new QImageReader(newfilename);
        m_Filename = newfilename;
    }
    result = m_Reader->canRead();
    if(result) {
        if(m_Image) {
            delete m_Image;
        }
        m_Image = new QImage(GetWidth(),GetHeight(),QImage::Format_RGB32);
    }
    return result;
}

void syImgReaderCodec::CloseInput() {
    if(m_Reader) {
        m_Reader->setDevice(0);
        m_Filename.clear();
        m_ImageLoaded = false;
    }
}

avtime_t syImgReaderCodec::SeekVideo(avtime_t pos) {
    return pos;
}

avtime_t syImgReaderCodec::GetCurrentVideoTime() {
    return 0;
}

avtime_t syImgReaderCodec::GetVideoLength() {
    return 0;
}

VideoColorFormat syImgReaderCodec::GetColorFormat() {
    return vcfRGB32;

}

unsigned long syImgReaderCodec::GetWidth() {
    if(!m_Reader) {
        return 0;
    }
    int width = m_Reader->size().width();
    if(width < 0) {
        return 0;
    }
    return width;
}

unsigned long syImgReaderCodec::GetHeight() {
    if(!m_Reader) {
        return 0;
    }
    int height = m_Reader->size().height();
    if(height < 0) {
        return 0;
    }
    return height;
}

float syImgReaderCodec::GetPixelAspect() {
    return 1.0;
}

float syImgReaderCodec::GetFramesPerSecond() {
    return 29.997;
}
unsigned long syImgReaderCodec::GetFrameIndex(avtime_t time) {
    return 0;
}

avtime_t syImgReaderCodec::GetTimeFromFrameIndex(unsigned long frame, bool fromend) {
    return 0;
}

void syImgReaderCodec::LoadCurrentFrame(syBitmap* dest) {
    if(!dest) {
        return;
    }
    sySafeMutexLocker lock(m_Mutex);
    if(lock.IsLocked()) {
        // 1. Check if m_Image is null. If not, abort (can't create the image, we could be in a worker thread)
        if(!m_Image) {
            return;
        }

        // 2. Read the image into m_Image.
        if(!m_ImageLoaded && m_Reader->read(m_Image)) {
            m_ImageLoaded = true;
        }

        // 3. Copy the image to dest.
        if(m_ImageLoaded) {
            const unsigned char* bits = m_Image->bits();
            dest->CopyFrom(bits,GetWidth(),GetHeight(),vcfRGB32,m_Image->numBytes());
        } else {
            dest->Clear();
        }
    }
}

// --------------------
// end syImgReaderCodec
// --------------------

syImgReaderPlugin::syImgReaderPlugin()
{
}

syImgReaderPlugin::~syImgReaderPlugin() {
}

const char* syImgReaderPlugin::GetPluginName() const           { return "syImgReaderPlugin"; }
const char* syImgReaderPlugin::GetPluginFullName() const       { return "Saya Integrated Image reader plugin"; }
const char* syImgReaderPlugin::GetPluginVersion() const        { return "1.0"; }
const char* syImgReaderPlugin::GetPluginAuthor() const         { return "Ricardo Garcia"; }
const char* syImgReaderPlugin::GetPluginLicense() const        { return "GPL version 3 or later"; }
const char* syImgReaderPlugin::GetPluginCreationDate() const   { return "2011-04-25"; }

syString syImgReaderPlugin::GetSupportedFileTypes()    { return "bmp,gif,png,jpg,jpeg"; }
syString syImgReaderPlugin::GetSupportedVideoReadCodecs() { return ""; }
syString syImgReaderPlugin::GetSupportedVideoWriteCodecs() { return ""; }
syString syImgReaderPlugin::GetSupportedAudioReadCodecs() { return ""; }
syString syImgReaderPlugin::GetSupportedAudioWriteCodecs() { return ""; }

CodecPlugin::CodecReadingSkills syImgReaderPlugin::CanReadFile(const syString& filename) {
    CodecPlugin::CodecReadingSkills result = CannotRead;
    std::vector<syString> supported_filetypes = explode(",",GetSupportedFileTypes());
    syString extension = ioCommon::GetExtension(filename, true);
    for(unsigned int i = 0; i < supported_filetypes.size(); ++i) {
        if(supported_filetypes[i] == extension) {
            result = CanReadVideo;
            break;
        }
    }
    return result;
}

CodecPlugin::CodecWritingSkills syImgReaderPlugin::CanWriteFile(const syString& filetype, const syString& videocodec, const syString& audiocodec) {
    return CannotWrite;
}

void syImgReaderPlugin::OnLoad() {
}

void syImgReaderPlugin::OnUnload() {
}

CodecInstance* syImgReaderPlugin::OpenFile(const syString& filename) {
    CodecInstance* result = 0;
    if (CanReadFile(filename)) {
        result = new syImgReaderCodec(this, filename);
        if(result && !result->OpenInput()) {
            delete result;
            result = 0; // Couldn't read!
        }
    }
    return result;
}

CodecPlugin* CreateImageReaderPlugin() {
    return new syImgReaderPlugin();
}

namespace syImageReaderPluginRegistration {
    bool tmpresult = CodecPlugin::RegisterPlugin("syImgReaderPlugin", &CreateImageReaderPlugin);
};
