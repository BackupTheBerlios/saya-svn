/***************************************************************
 * Name:      codecplugin.h
 * Purpose:   Definition for the Codec Plugin class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-23
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

/** CodecPluginFactory is the class that manages all the codec plugins behind the scenes. */
class CodecPluginFactory;
class CodecPlugin;
#include "systring.h"

typedef CodecPlugin* (*CodecPluginFactoryFunction)();

/** A Codec Instance. This is the base class that encodes and decodes our files. */
class CodecInstance {
    public:
        friend class CodecPlugin;

        CodecInstance(CodecPlugin* plugin) {}
        virtual ~CodecInstance() {}
};


class CodecPlugin {
    public:
        friend class CodecPluginFactory;

        enum CodecReadingSkills {
            CannotRead = 0,
            CanReadAudio = 1,
            CanReadVideo = 2,
            CanReadBoth = 3
        };

        enum CodecWritingSkills {
            CannotWrite = 0,
            CanWriteAudio = 1,
            CanWriteVideo = 2,
            CanWriteBoth = 3
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

        /** Unregisters all plugins */
        static void UnregisterAllPlugins();

        /** Loads a plugin with a given name. The plugin MUST be deleted by function UnloadPlugin(). */
        static CodecPlugin* LoadPlugin(const char* name);

        /** Unloads a plugin with a given name. */
        static void UnloadPlugin(const char* name);

        /** Unloads all the plugins */
        static void UnloadAllPlugins();

        static const char* GetCodecPluginsPath();

        /** Selects the given codec plugin. */
        static CodecPlugin* SelectPlugin(const char* name);

        /** Returns the currently selected codec plugin. */
        static CodecPlugin* GetCurrentPlugin();

        /** Finds the appropriate plugin for reading a specific file. */
        static CodecPlugin* FindReadPlugin(const char* filename);

    public:
        virtual const char* GetPluginName() { return ""; }
        virtual const char* GetPluginVersion() { return ""; }
        virtual const char* GetPluginAuthor() { return ""; }
        virtual const char* GetPluginLicense() { return ""; }
        virtual const char* GetPluginCreationDate() { return ""; }

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

        /** Tests if the codec can read the video and audio from the given filename. */
        virtual CodecReadingSkills CanReadFile(const syString& filename) { return CannotRead; }

        /** Tests if the codec can read the video and audio from the given filename. */
        virtual CodecWritingSkills CanWriteFile(const syString& filetype, const syString& videocodec, const syString& audiocodec) { return CannotWrite; }

        /** Opens the given file for reading.
         *  @return A CodecInstance object dedicated to reading the file; 0 on failure.
         */
        virtual CodecInstance* OpenFile(const syString& filename) { return 0; }

    protected:

        /** Called when the plugin is loaded. */
        virtual void OnLoad() {}

        /** Called when the plugin is unloaded. */
        virtual void OnUnload() {}
};