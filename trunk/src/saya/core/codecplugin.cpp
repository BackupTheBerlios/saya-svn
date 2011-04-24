/***************************************************************
 * Name:      codecplugin.cpp
 * Purpose:   Implementation of the Codec Plugin class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-24
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "codecplugin.h"
#include <map>
#include "systring.h"
#include "app.h"

// ------------------------
// begin register functions
// ------------------------

class CodecPluginFactory {
    public:
        typedef std::map<syString, CodecPluginFactoryFunction, ltsystr> CodecPluginFactoryMap;
        typedef std::map<syString, CodecPlugin*, ltsystr> CodecPluginMap;
        CodecPluginFactoryMap m_FactoryMap;
        CodecPluginMap m_Map;
        static CodecPlugin* s_CurrentPlugin;

        /** Registers a codec plugin. */
        static bool RegisterPlugin(const char* name, CodecPluginFactoryFunction func);

        /** Unregisters a codec plugin. */
        static void UnregisterPlugin(const char* name);

        /** Unregisters all codec plugins. */
        static void UnregisterAllPlugins();

        /** Searches a directory for shared libraries containing codec code (under construction). */
        static void RegisterPlugins(const char* path);

        /** Loads a given codec plugin. */
        static CodecPlugin* LoadPlugin(const char* name);

        /** Selects the given codec plugin. */
        static CodecPlugin* SelectPlugin(const char* name);

        /** Returns the currently selected codec plugin. */
        static CodecPlugin* GetCurrentPlugin();

        /** Finds a codec plugin by name; returns null on failure. */
        static CodecPlugin* FindPlugin(const char* name);

        /** Unloads a given codec plugin. */
        static void UnloadPlugin(const char* name);

        /** Unloads all codec plugins. */
        static void UnloadAllPlugins();

        static CodecPluginFactory* s_self;
        class StaticDestructor {
            public:
                ~StaticDestructor() {
                    delete CodecPluginFactory::s_self;
                    s_self = 0;
                }
        };
        static StaticDestructor s_Destructor;
};

CodecPluginFactory* CodecPluginFactory::s_self = 0;
CodecPlugin* CodecPluginFactory::s_CurrentPlugin = 0;
CodecPluginFactory::StaticDestructor CodecPluginFactory::s_Destructor;

const char* CodecPlugin::GetCodecPluginsPath() {
    static syString s_CodecPluginsPath = syString();
    if(s_CodecPluginsPath.empty()) {
        s_CodecPluginsPath = syString(syApp::Get()->GetApplicationPath()) + "/plugins/codecs/";
    }
    return s_CodecPluginsPath.c_str();
}

bool CodecPluginFactory::RegisterPlugin(const char* name, CodecPluginFactoryFunction func) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    syString tmp(name);
    s_self->m_FactoryMap[tmp] = func;
    return true;
}

void CodecPluginFactory::RegisterPlugins(const char* path) {
    #warning TODO: Implement CodecPluginFactory::RegisterPlugins();
}


void CodecPluginFactory::UnregisterPlugin(const char* url) {
    if(!s_self) {
        return;
    }
    s_self->m_FactoryMap.erase(syString(url));
}

void CodecPluginFactory::UnregisterAllPlugins() {
    if(s_self) {
        s_self->m_FactoryMap.clear();
    }
}

CodecPlugin* CodecPluginFactory::LoadPlugin(const char* name) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    syString plugin_name(name);
    CodecPlugin* result = 0;

    // First check if the Plugin has been already loaded
    CodecPluginMap::iterator it = s_self->m_Map.find(plugin_name);
    if(it != s_self->m_Map.end()) {
        result = it->second;
    } else {
        // Try to create the plugin
        CodecPluginFactoryMap::const_iterator it2 = s_self->m_FactoryMap.find(plugin_name);
        if(it2 != s_self->m_FactoryMap.end()) {
            result = it2->second();
            s_self->m_Map[plugin_name] = result;
        }
    }
    return result;
}

CodecPlugin* CodecPluginFactory::FindPlugin(const char* name) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    syString plugin_name(name);
    CodecPlugin* result = 0;

    CodecPluginMap::iterator it = s_self->m_Map.find(plugin_name);
    if(it != s_self->m_Map.end()) {
        result = it->second;
    }
    return result;
}

void CodecPluginFactory::UnloadPlugin(const char* name) {
    if(!s_self) {
        return;
    }
    CodecPlugin* plugin = FindPlugin(name);
    if(plugin) {
        if(s_CurrentPlugin == plugin) {
            s_CurrentPlugin = 0;
        }
        plugin->OnUnload();
    }
    delete plugin;
    s_self->m_Map.erase(syString(name));
}

void CodecPluginFactory::UnloadAllPlugins() {
    if(!s_self) {
        return;
    }
    for(CodecPluginMap::iterator it = s_self->m_Map.begin();it != s_self->m_Map.end(); ++it) {
        CodecPlugin* plugin = it->second;
        plugin->OnUnload();
        delete plugin;
    }
    s_CurrentPlugin = 0;
    s_self->m_Map.clear();
}

CodecPlugin* CodecPluginFactory::SelectPlugin(const char* name) {
    if(!s_self) {
        return 0;
    }
    s_CurrentPlugin = FindPlugin(name);
    return s_CurrentPlugin;
}

CodecPlugin* CodecPluginFactory::GetCurrentPlugin() {
    return s_CurrentPlugin;
}



bool CodecPlugin::RegisterPlugin(const char* name, CodecPluginFactoryFunction func) {
    return CodecPluginFactory::RegisterPlugin(name, func);
}

void CodecPlugin::UnregisterPlugin(const char* name) {
    CodecPluginFactory::UnregisterPlugin(name);
}

void CodecPlugin::RegisterPlugins(const char* path) {
    CodecPluginFactory::RegisterPlugins(path);
}

void CodecPlugin::UnregisterAllPlugins() {
    CodecPluginFactory::UnregisterAllPlugins();
}

CodecPlugin* CodecPlugin::LoadPlugin(const char* name) {
    return CodecPluginFactory::LoadPlugin(name);
}

void CodecPlugin::UnloadPlugin(const char* name) {
    return CodecPluginFactory::UnloadPlugin(name);
}

void CodecPlugin::UnloadAllPlugins() {
    return CodecPluginFactory::UnloadAllPlugins();
}

CodecPlugin* CodecPlugin::SelectPlugin(const char* name) {
    return CodecPluginFactory::SelectPlugin(name);
}

CodecPlugin* CodecPlugin::GetCurrentPlugin() {
    return CodecPluginFactory::GetCurrentPlugin();
}

// ----------------------
// end register functions
// ----------------------
