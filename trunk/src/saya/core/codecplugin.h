/***************************************************************
 * Name:      codecplugin.h
 * Purpose:   Definition for the Codec Plugin class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-23
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef codecplugin_h
#define codecplugin_h

#include "systring.h"
#include "videocolorformat.h"
#include "avtypes.h"

/** CodecPluginFactory is the class that manages all the codec plugins behind the scenes. */
class CodecPluginFactory;
class CodecPlugin;
class syBitmap;
class syAudioBuffer;


typedef CodecPlugin* (*CodecPluginFactoryFunction)();

/** A Codec Instance. This is the base class that encodes and decodes our files. */
class CodecInstance {
    public:
        friend class CodecPlugin;

        CodecInstance();
        virtual ~CodecInstance() {}
        /** @brief Opens a file for reading.
         *  @return true on success; false otherwise.
         */
        virtual bool OpenInput(const syString filename = syEmptyString) { return false; }

        /** @brief Opens a memory buffer for reading.
         *  @param buf The address of the memory buffer.
         *  @param size The size of the memory buffer.
         *  @param mimetype The mime type the buffer supposedly holds.
         *  @return true on success; false otherwise.
         *  @warning For security reasons, implementors MUST FAIL to read the file if the actual format does not match the given mimetype.
         */
        virtual bool OpenMemoryInput(const unsigned char* buf, unsigned int size, const char* mimetype) { return false; }

        /** @brief Closes the input resource. */
        virtual void CloseInput() {}

        /** @brief Opens a file for writing.
         *  @return true on success; false otherwise,
         */
        virtual bool OpenOutput(const syString filename = syEmptyString) { return false; }

        /** @brief Closes the output file. */
        virtual void CloseOutput() {}

        // Input functions

        /** Seeks the Video to a determinate position in time. */
        virtual avtime_t SeekVideo(avtime_t pos);

        /** Seeks the Audio to a determinate position in time. */
        virtual avtime_t SeekAudio(avtime_t pos);

        /** Gets the current position in time for the Video. */
        virtual avtime_t GetCurrentVideoTime();

        /** Gets the current position in time for the Audio. */
        virtual avtime_t GetCurrentAudioTime();

        /** Gets the total Video duration. */
        virtual avtime_t GetVideoLength();

        /** Gets the total Audio duration. */
        virtual avtime_t GetAudioLength();

        /** Gets the Video color format used to return the current file's video frame. */
        virtual VideoColorFormat GetColorFormat();

        /** Gets the width in pixels of the current file. */
        virtual unsigned long GetWidth();

        /** Gets the height in pixels of the current file. */
        virtual unsigned long GetHeight();

        /** Gets the pixel aspect of the current file (default 1.0). */
        virtual float GetPixelAspect();

        /** Gets the framerate for the current file, in case of a fixed framerate. */
        virtual float GetFramesPerSecond();

        /** Gets the frame index (0-based) corresponding to a given time. */
        virtual unsigned long GetFrameIndex(avtime_t time);

        /** Gets the time corresponding to a given frame index. */
        virtual avtime_t GetTimeFromFrameIndex(unsigned long frame, bool fromend = false);

        /** Loads the current frame into a syBitmap. */
        virtual void LoadCurrentFrame(syBitmap* dest);

        /** Loads the audio into a syAudioBuffer object. */
        virtual void LoadAudioBuffer(syAudioBuffer* dest, unsigned long numsamples = 0);

        // Output functions

        /** @return true if this is a video resource. */
        bool IsVideo() const;
        /** @return true if this is an audio resource. */
        bool IsAudio() const;
        /** @return true if this is an input resource. */
        bool IsInput() const;
        /** @return true if this is an output resource. */
        bool IsOutput() const;

    protected:
        bool m_IsVideo;
        bool m_IsAudio;
        bool m_IsInput;
        bool m_IsOutput;
};


class CodecPlugin {
    public:
        friend class CodecPluginFactory;

        enum CodecReadingSkills {
            CannotRead   = 0x00, // These are bitmasks, so that if a plugin can read both video and audio, it
            CanReadAudio = 0x01, // will be chosen before one that can only read video.
            CanReadVideo = 0x02, // Video-only is considered superior to audio-only.
            CanReadBoth  = 0x03
        };

        enum CodecWritingSkills {
            CannotWrite   = 0x00,
            CanWriteAudio = 0x01,
            CanWriteVideo = 0x02,
            CanWriteBoth  = 0x03
        };

        CodecPlugin() {}
        virtual ~CodecPlugin() {}
        /** @brief Registers a Video Codec Plugin (a factory function) with a specific name.
         *  @param url The string to register the factory with.
         *  @param func The factory function to register.
         *  @return true always; The return value was added to help initializer functions.
         */
        static bool RegisterPlugin(const char* name, CodecPluginFactoryFunction func);

        /** @brief Unregisters a Codec Plugin with a specific name.
         *  @param url The string to unregister the plugin.
         */
        static void UnregisterPlugin(const char* name);

        /** Searches a directory for shared libraries containing codec code. */
        static void RegisterPlugins(const char* path);

        /** Unregisters all plugins. Called at the end of the program. */
        static void UnregisterAllPlugins();

        /** Loads a plugin with a given name. The plugin musted be deleted ONLY by calling function UnloadPlugin(). */
        static CodecPlugin* LoadPlugin(const char* name);

        /** Unloads a plugin with a given name. */
        static void UnloadPlugin(const char* name);

        /** Loads all the registered plugins. */
        static void LoadAllRegisteredPlugins();

        /** Unloads all the plugins. Called at the end of the program. */
        static void UnloadAllPlugins();

        static const char* GetCodecPluginsPath();

        /** Selects the given codec plugin.*/
        static CodecPlugin* SelectPlugin(const char* name);

        /** Returns the currently selected codec plugin. */
        static CodecPlugin* GetCurrentPlugin();

        /** Finds the appropriate plugin for reading a specific file. */
        static CodecPlugin* FindReadPlugin(const syString& filename);

        /** Finds the appropriate plugin for reading a specific file. */
        static CodecPlugin* FindReadPlugin(const char* filename);

        /** Finds the appropriate plugin for reading a specific mimetype. It also supports extensions. */
        static CodecPlugin* FindReadPluginByMimeType(const char* mimetype);

    public:
        virtual const char* GetPluginName() const { return ""; }
        virtual const char* GetPluginFullName() const { return ""; }
        virtual const char* GetPluginVersion() const { return ""; }
        virtual const char* GetPluginAuthor() const { return ""; }
        virtual const char* GetPluginLicense() const { return ""; }
        virtual const char* GetPluginCreationDate() const { return ""; }

        /**
         * @brief Gets the supported Filetypes (extensions) that this plugin can decode; separated by commas.
         * Example: "avi,mpg,mpeg,jpg"
         */
        virtual syString GetSupportedFileTypes() { return ""; }

        /** Gets the supported Codec strings that this plugin can decode; separated by commas */
        virtual syString GetSupportedVideoReadCodecs() { return ""; }

        /** Gets the supported Codec strings that this plugin can decode; separated by commas */
        virtual syString GetSupportedVideoWriteCodecs() { return ""; }

        /** Gets the supported Codec strings that this plugin can decode; separated by commas */
        virtual syString GetSupportedAudioReadCodecs() { return ""; }

        /** Gets the supported Codec strings that this plugin can decode; separated by commas */
        virtual syString GetSupportedAudioWriteCodecs() { return ""; }

        /** Gets the supported MIME types that this plugin can decode; separated by commas */
        virtual syString GetSupportedMimeTypes() { return ""; }

        /** Tests if the codec can read the video and audio from the given filename. */
        virtual CodecReadingSkills CanReadFile(const syString& filename) { return CannotRead; }

        /** Tests if the codec can write video and audio from the given filename. */
        virtual CodecWritingSkills CanWriteFile(const syString& filetype, const syString& videocodec, const syString& audiocodec) { return CannotWrite; }

        /** Tests if the codec can read the video and audio from the given mime type. */
        virtual CodecReadingSkills CanReadMimeType(const syString& mimetype) { return CannotRead; }

        /** Tests if the codec can write video and audio from the given mime type. */
        virtual CodecWritingSkills CanWriteMimeType(const syString& mimetype) { return CannotWrite; }

        /** Opens the given file for reading.
         *  @return A CodecInstance object dedicated to reading the file; 0 on failure.
         */
        virtual CodecInstance* OpenFile(const syString& filename) { return 0; }

        /** Opens an in-memory file for reading.
         *  @param data The string containing a binary image of the file.
         *  @param mimetype The mime type of the file in question.
         *  @return A CodecInstance object dedicated to reading the file; 0 on failure.
         *  @warning For security reasons, implementors MUST FAIL to read a file if it doesn't match with the given mime type.
         */
        virtual CodecInstance* OpenString(const syString& data, const char* mimetype) { return 0; }

    protected:

        /** Called when the plugin is loaded. */
        virtual void OnLoad() {}

        /** Called when the plugin is unloaded. */
        virtual void OnUnload() {}
};

#endif
