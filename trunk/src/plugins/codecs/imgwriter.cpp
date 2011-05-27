/***************************************************************
 * Name:      imgwriter.h
 * Purpose:   Implementation of the Image Writer Codec Plugin
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-05-26
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "imgwriter.h"
#include <saya/core/iocommon.h>
#include <saya/core/sythread.h>
#include <saya/core/sybitmap.h>
#include <saya/core/systringutils.h>
#include <saya/core/debuglog.h>
#include <QImage>
#include <QImageWriter>
#include <QBuffer>

// ----------------------
// begin syImgWriterCodec
// ----------------------

class syImgWriterCodec : public CodecInstance {
        friend class syImgWriterPlugin;
    public:
        syImgWriterCodec(syImgWriterPlugin* parent, const syString& filename);
        virtual ~syImgWriterCodec();

        virtual bool OpenOutput(const BasicAVSettings* settings, const syString filename = syEmptyString);
        virtual bool OpenMemoryOutput(const BasicAVSettings* settings, syString& dest);
        virtual void CloseOutput();

        // Output functions

        virtual avtime_t SaveCurrentFrame(const syBitmap* src);

    private:
        syImgWriterPlugin* m_Parent;
        syString m_Filename;
        QImageWriter* m_Writer;
        QBuffer* m_Buffer;
        QImage* m_Image;
        sySafeMutex m_Mutex;
};

syImgWriterCodec::syImgWriterCodec(syImgWriterPlugin* parent, const syString& filename) :
m_Parent(parent),
m_Filename(filename),
m_Writer(new QImageWriter()),
m_Buffer(0),
m_Image(0)
{
    m_IsVideo = true;
    m_IsAudio = false;
    m_IsInput = false;
    m_IsOutput = true;
}

syImgWriterCodec::~syImgWriterCodec() {
    CloseOutput();
    delete m_Image;
    m_Image = 0;
    delete m_Writer;
    m_Writer = 0;
    delete m_Buffer;
    m_Buffer = 0;
}


// --------------------
// end syImgWriterCodec
// --------------------

syImgWriterPlugin::syImgWriterPlugin()
{
}

syImgWriterPlugin::~syImgWriterPlugin() {
}

const char* syImgWriterPlugin::GetPluginName() const           { return "syImgWriterPlugin"; }
const char* syImgWriterPlugin::GetPluginFullName() const       { return "Saya Integrated Image reader plugin"; }
const char* syImgWriterPlugin::GetPluginVersion() const        { return "1.0"; }
const char* syImgWriterPlugin::GetPluginAuthor() const         { return "Ricardo Garcia"; }
const char* syImgWriterPlugin::GetPluginLicense() const        { return "GPL version 3 or later"; }
const char* syImgWriterPlugin::GetPluginCreationDate() const   { return "2011-04-25"; }

syString syImgWriterPlugin::GetSupportedFileTypes()    { return "bmp,gif,png,jpg,jpeg"; }
syString syImgWriterPlugin::GetSupportedVideoReadCodecs() { return ""; }
syString syImgWriterPlugin::GetSupportedVideoWriteCodecs() { return ""; }
syString syImgWriterPlugin::GetSupportedAudioReadCodecs() { return ""; }
syString syImgWriterPlugin::GetSupportedAudioWriteCodecs() { return ""; }
syString syImgWriterPlugin::GetSupportedMimeTypes() {
    return "image/bmp,image/gif,image/png,image/jpeg";
}

CodecPlugin::CodecWritingSkills syImgWriterPlugin::CanWriteFile(const syString& filetype, const syString& videocodec, const syString& audiocodec) {
    CodecPlugin::CodecWritingSkills result = CannotWrite;
    std::vector<syString> supported_filetypes = explode(",",GetSupportedFileTypes());
    for(unsigned int i = 0; i < supported_filetypes.size(); ++i) {
        if(supported_filetypes[i] == filetype) {
            result = CanWriteVideo;
            break;
        }
    }
    return result;
}

void syImgWriterPlugin::OnLoad() {
}

void syImgWriterPlugin::OnUnload() {
}

CodecInstance* syImgWriterPlugin::OpenFileForWriting(const BasicAVSettings* settings, const syString& filename)
{
    CodecInstance* result = 0;
    return result;
}
CodecInstance* syImgWriterPlugin::OpenStringForWriting(const BasicAVSettings* settings, syString& dest) {
    CodecInstance* result = 0;
    return result;
}

CodecPlugin* CreateImageWriterPlugin() {
    return new syImgWriterPlugin();
}

namespace syImageWriterPluginRegistration {
    bool tmpresult = CodecPlugin::RegisterPlugin("syImgWriterPlugin", &CreateImageWriterPlugin);
};
