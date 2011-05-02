/***************************************************************
 * Name:      imgreader.h
 * Purpose:   Definition for the Image Reader Codec Plugin
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-25
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef imgreader_h
#define imgreader_h

#include <saya/core/codecplugin.h>

class syImgReaderPlugin : public CodecPlugin {
    public:
        syImgReaderPlugin();
        virtual ~syImgReaderPlugin();

        const char* GetPluginName() const;
        const char* GetPluginFullName() const;
        const char* GetPluginVersion() const;
        const char* GetPluginAuthor() const;
        const char* GetPluginLicense() const;
        const char* GetPluginCreationDate() const;

        syString GetSupportedFileTypes();
        syString GetSupportedVideoReadCodecs();
        syString GetSupportedVideoWriteCodecs();
        syString GetSupportedAudioReadCodecs();
        syString GetSupportedAudioWriteCodecs();
        syString GetSupportedMimeTypes();

        CodecReadingSkills CanReadFile(const syString& filename);
        CodecReadingSkills CanReadMimeType(const syString& mimetype);
        CodecWritingSkills CanWriteFile(const syString& filetype, const syString& videocodec, const syString& audiocodec);
        CodecInstance* OpenFile(const syString& filename);

    protected:
        void OnLoad();
        void OnUnload();

    private:
        class Data;
        friend class Data;
        Data* m_Data;

};

#endif
