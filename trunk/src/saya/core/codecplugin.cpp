/***************************************************************
 * Name:      codecplugin.cpp
 * Purpose:   Implementation of the Codec Plugin class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-24
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "intl.h"
#include "debuglog.h"
#include "codecplugin.h"
#include <map>
#include <set>
#include "systring.h"
#include "systringutils.h"
#include "iocommon.h"
#include "app.h"

// ------------------------
// begin register functions
// ------------------------

class CodecPluginFactory {
    public:
        typedef std::map<syString, CodecPluginFactoryFunction, ltsystr> CodecPluginFactoryMap;
        typedef std::map<syString, CodecPlugin*, ltsystr> CodecPluginMap;
        typedef std::set<syString, ltsystr> CodecNamesSet;
        typedef std::map<syString, CodecNamesSet, ltsystr> CodecFileTypesMap;
        CodecPluginFactoryMap m_FactoryMap;
        CodecPluginMap m_Map;
        CodecFileTypesMap m_FileTypesMap;
        CodecFileTypesMap m_MimeTypesMap;

        ~CodecPluginFactory() {
            UnloadAllPlugins();
            UnregisterAllPlugins();
        }

        static CodecPlugin* s_CurrentPlugin;

        /** Registers a codec plugin. */
        static bool RegisterPlugin(const char* name, CodecPluginFactoryFunction func);

        /** Unregisters a codec plugin. */
        static void UnregisterPlugin(const char* name);

        /** Unregisters all codec plugins. */
        static void UnregisterAllPlugins();

        /** Searches a directory for shared libraries containing codec code (under construction). */
        static void RegisterPlugins(const char* path);

        /** Registers a plugin's reading and writing capabilities */
        static void RegisterPluginCapabilities(CodecPlugin* plugin);

        /** Loads a given codec plugin. */
        static CodecPlugin* LoadPlugin(const char* name);

        /** Selects the given codec plugin. */
        static CodecPlugin* SelectPlugin(const char* name);

        /** Returns the currently selected codec plugin. */
        static CodecPlugin* GetCurrentPlugin();

        /** Finds the appropriate plugin for reading a specific file. */
        static CodecPlugin* FindReadPlugin(const char* filename);

        /** Finds the appropriate plugin for reading a specific mime type. */
        static CodecPlugin* FindReadPluginByMimeType(const char* mimetype);

        /** Finds the appropriate plugin for reading a specific mimetype. It also supports extensions. */
        static CodecPlugin* FindWritePluginByMimeType(const char* mimetype);

        /** Finds a codec plugin by name; returns null on failure. */
        static CodecPlugin* FindPlugin(const char* name);

        /** Unloads a given codec plugin. */
        static void UnloadPlugin(const char* name);

        /** Loads all the registered plugins. */
        static void LoadAllRegisteredPlugins();

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
    DebugLog(syString(_("Registered codec plugin: ")) + name);
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
            DebugLog(syString(_("Loading codec plugin: ")) + plugin_name);
            result = it2->second();
            if(result) {
                syString reportedPluginName = result->GetPluginName();
                if(reportedPluginName == plugin_name) {
                    DebugLog(syString(_("Successfully loaded codec plugin: ")) + plugin_name);
                    s_self->m_Map[plugin_name] = result;
                    CodecPluginFactory::RegisterPluginCapabilities(result);
                } else {
                    delete result;
                    syString tmps;
                    tmps.Printf(_("ERROR loading plugin: '%s' (name mismatch: '%s')"), name, reportedPluginName.c_str());
                    DebugLog(tmps);
                }
            } else {
                DebugLog(syString(_("ERROR loading codec plugin: ")) + plugin_name);
            }
        }
    }
    return result;
}

/** Registers a plugin's reading and writing capabilities */
void CodecPluginFactory::RegisterPluginCapabilities(CodecPlugin* plugin) {
    if(!plugin) {
        return;
    }
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    CodecFileTypesMap& themap = s_self->m_FileTypesMap;
    CodecFileTypesMap& mimemap = s_self->m_MimeTypesMap;

    std::vector<syString> curfiletypes = explode(",",plugin->GetSupportedFileTypes());
    for(unsigned int i = 0, ii = curfiletypes.size(); i < ii; ++i) {
        themap[curfiletypes[i]].insert(plugin->GetPluginName());
    }
    curfiletypes = explode(",",plugin->GetSupportedMimeTypes());
    for(unsigned int i = 0, ii = curfiletypes.size(); i < ii; ++i) {
        mimemap[curfiletypes[i]].insert(plugin->GetPluginName());
    }
}

CodecPlugin* CodecPluginFactory::FindReadPlugin(const char* filename) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    CodecPlugin* result = 0;
    CodecFileTypesMap& themap = s_self->m_FileTypesMap;
    syString basefilename = ioCommon::GetFilename(filename);
    int dotpos = basefilename.find(".");
    if(dotpos < 0) {
        return 0; // Unknown file extension!
    }
    syString file_ext = strtolower(basefilename.substr(dotpos + 1, basefilename.length()));
    CodecPlugin::CodecReadingSkills curskill = CodecPlugin::CannotRead;
    if(file_ext.length()) {
        for(CodecFileTypesMap::iterator it = themap.find(file_ext); it != themap.end() && !result; ++it) {
            CodecNamesSet& codecs = it->second;
            for(CodecNamesSet::iterator it2 = codecs.begin();it2 != codecs.end(); ++it2) {
                syString codecname = *it2;
                CodecPlugin* plugin = CodecPluginFactory::FindPlugin(codecname.c_str());
                if(plugin) {
                    CodecPlugin::CodecReadingSkills tmpskill = plugin->CanReadFile(filename);
                    if(tmpskill > curskill) {
                        result = plugin;
                        curskill = tmpskill;
                        if(curskill == CodecPlugin::CanReadBoth) {
                            break;
                        }
                    }
                }
            }
            if(curskill == CodecPlugin::CanReadBoth) {
                break;
            }
        }
    }
    return result;
}

CodecPlugin* CodecPluginFactory::FindReadPluginByMimeType(const char* mimetype) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    CodecPlugin* result = 0;
    CodecFileTypesMap& themap = s_self->m_MimeTypesMap;
    CodecPlugin::CodecReadingSkills curskill = CodecPlugin::CannotRead;
    syString smimetype(mimetype, true);
    if(smimetype.length()) {
        for(CodecFileTypesMap::iterator it = themap.find(smimetype); it != themap.end() && !result; ++it) {
            CodecNamesSet& codecs = it->second;
            for(CodecNamesSet::iterator it2 = codecs.begin();it2 != codecs.end(); ++it2) {
                syString codecname = *it2;
                CodecPlugin* plugin = CodecPluginFactory::FindPlugin(codecname.c_str());
                if(plugin) {
                    CodecPlugin::CodecReadingSkills tmpskill = plugin->CanReadMimeType(smimetype);
                    if(tmpskill > curskill) {
                        result = plugin;
                        curskill = tmpskill;
                        if(curskill == CodecPlugin::CanReadBoth) {
                            break;
                        }
                    }
                }
            }
            if(curskill == CodecPlugin::CanReadBoth) {
                break;
            }
        }
    }
    return result;
}

CodecPlugin* CodecPluginFactory::FindWritePluginByMimeType(const char* mimetype) {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    CodecPlugin* result = 0;
    CodecFileTypesMap& themap = s_self->m_MimeTypesMap;
    CodecPlugin::CodecWritingSkills curskill = CodecPlugin::CannotWrite;
    CodecPlugin::CodecWritingSkills maxskill = CodecPlugin::CanWriteBoth;
    syString smimetype(mimetype, true);
    if(smimetype.substr(0,6) == "image/") {
        maxskill = CodecPlugin::CanWriteVideo;
    }

    if(smimetype.length()) {
        for(CodecFileTypesMap::iterator it = themap.find(smimetype); it != themap.end() && !result; ++it) {
            CodecNamesSet& codecs = it->second;
            for(CodecNamesSet::iterator it2 = codecs.begin();it2 != codecs.end(); ++it2) {
                syString codecname = *it2;
                CodecPlugin* plugin = CodecPluginFactory::FindPlugin(codecname.c_str());
                if(plugin) {
                    CodecPlugin::CodecWritingSkills tmpskill = plugin->CanWriteMimeType(smimetype);
                    if(tmpskill > curskill) {
                        result = plugin;
                        curskill = tmpskill;
                        if(curskill == maxskill) {
                            break;
                        }
                    }
                }
            }
            if(curskill == maxskill) {
                break;
            }
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

void CodecPluginFactory::LoadAllRegisteredPlugins() {
    if(!s_self) {
        s_self = new CodecPluginFactory;
    }
    for(CodecPluginFactoryMap::iterator it = s_self->m_FactoryMap.begin();it != s_self->m_FactoryMap.end(); ++it) {
        LoadPlugin(it->first.c_str()); // LoadPlugin always checks if the plugin has been loaded.
    }
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

void CodecPlugin::LoadAllRegisteredPlugins() {
    return CodecPluginFactory::LoadAllRegisteredPlugins();
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

CodecPlugin* CodecPlugin::FindReadPlugin(const char* filename) {
    return CodecPluginFactory::FindReadPlugin(filename);
}

CodecPlugin* CodecPlugin::FindReadPlugin(const syString& filename) {
    return CodecPluginFactory::FindReadPlugin(filename.c_str());
}

CodecPlugin* CodecPlugin::FindReadPluginByMimeType(const char* mimetype) {
    return CodecPluginFactory::FindReadPluginByMimeType(mimetype);
}

CodecPlugin* CodecPlugin::FindWritePluginByMimeType(const char* mimetype) {
    return CodecPluginFactory::FindWritePluginByMimeType(mimetype);
}

CodecInstance::CodecInstance():
m_IsVideo(false),
m_IsAudio(false),
m_IsInput(false),
m_IsOutput(false)
{
}

bool CodecInstance::IsVideo() const {
    return m_IsVideo;
}
bool CodecInstance::IsAudio() const {
    return m_IsAudio;
}
bool CodecInstance::IsInput() const {
    return m_IsInput;
}
bool CodecInstance::IsOutput() const {
    return m_IsOutput;
}

avtime_t CodecInstance::SeekVideo(avtime_t pos) { return 0; }
avtime_t CodecInstance::SeekAudio(avtime_t pos) { return 0; }
avtime_t CodecInstance::GetCurrentVideoTime() { return 0; }
avtime_t CodecInstance::GetCurrentAudioTime() { return 0; }

avtime_t CodecInstance::GetVideoLength() { return 0; }
avtime_t CodecInstance::GetAudioLength() { return 0; }
VideoColorFormat CodecInstance::GetColorFormat() { return vcfRGB32; }
unsigned long CodecInstance::GetWidth() { return 0; }
unsigned long CodecInstance::GetHeight() { return 0; }
float CodecInstance::GetPixelAspect() { return 1.0; }
float CodecInstance::GetFramesPerSecond() { return 29.997; }

unsigned long CodecInstance::GetFrameIndex(avtime_t time) { return 0; }
avtime_t CodecInstance::GetTimeFromFrameIndex(unsigned long frame, bool fromend) { return 0; }
void CodecInstance::LoadCurrentFrame(syBitmap* dest) {}
void CodecInstance::LoadAudioBuffer(syAudioBuffer* dest, unsigned long numsamples) {}
