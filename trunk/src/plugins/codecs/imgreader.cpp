/***************************************************************
 * Name:      imgreader.h
 * Purpose:   Implementation of the Image Reader Codec Plugin
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-25
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "imgreader.h"


// ----------------------
// begin syImgReaderCodec
// ----------------------

class syImgReaderCodec : public CodecInstance {
    friend class syImgReaderPlugin;
    public:
        syImgReaderCodec(syImgReaderPlugin* parent, const syString& filename);
        virtual ~syImgReaderCodec() {}
        virtual bool OpenInput(const syString& filename);
        virtual void CloseInput();

        virtual bool OpenOutput(const syString& filename) { return false; }
        virtual void CloseOutput() {}

        // Input functions

        virtual avtime_t SeekVideo(avtime_t pos);
        virtual avtime_t SeekAudio(avtime_t pos);
        virtual avtime_t GetCurrentVideoTime() const;
        virtual avtime_t GetCurrentAudioTime() const;

        virtual avtime_t GetVideoLength() const;
        virtual avtime_t GetAudioLength() const;
        virtual VideoColorFormat GetColorFormat() const;
        virtual unsigned long GetWidth() const;
        virtual unsigned long GetHeight() const;
        virtual float GetPixelAspect() const;
        virtual float GetFramesPerSecond() const;

        virtual unsigned long GetFrameIndex(avtime_t time);
        virtual avtime_t GetTimeFromFrameIndex(unsigned long frame, bool fromend = false);
        virtual void LoadCurrentFrame(syBitmap* dest);
        virtual void LoadAudioBuffer(syAudioBuffer* dest, unsigned long numsamples = 0);

    private:
        syImgReaderPlugin* m_Parent;
        syString m_Filename;
};

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
    #warning TODO: Implement syImgReaderPlugin::CanReadFile
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
    if(CanReadFile(filename)) {
        // result = new syImgReaderCodec(this, filename);
    }
    return result;
}
