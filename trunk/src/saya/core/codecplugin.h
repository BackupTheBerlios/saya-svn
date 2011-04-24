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

typedef CodecPlugin* (*CodecPluginFactoryFunction)();

class CodecPlugin {
    public:
        friend class CodecPluginFactory;

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

    public:
        virtual const char* GetPluginName() { return ""; }
        virtual const char* GetPluginVersion() { return ""; }
        virtual const char* GetPluginAuthor() { return ""; }
        virtual const char* GetPluginLicense() { return ""; }
        virtual const char* GetPluginCreationDate() { return ""; }

    protected:

        /** Called when the plugin is loaded. */
        virtual void OnLoad() {}

        /** Called when the plugin is unloaded. */
        virtual void OnUnload() {}
};