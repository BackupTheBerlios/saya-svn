/***************************************************************
 * Name:      ui/config.cpp
 * Purpose:   Implements the Application Configuration Handler
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <wx/config.h>
#include "config.h"
#include "s2wx.h"

wxsyConfig::wxsyConfig(const char* application_name) : syConfig(application_name) {
    m_config = new wxConfig(s2wx(application_name));
}

syString wxsyConfig::Read(const char* key, const char* defaultvalue) const {
    return syString(m_config->Read(s2wx(key),s2wx(defaultvalue)));
}

bool wxsyConfig::Write(const char* key, const char* value) {
    return m_config->Write(s2wx(key),s2wx(value));
}

bool wxsyConfig::Exists(const char* key) const {
    return m_config->Exists(s2wx(key));
}

wxsyConfig::~wxsyConfig() {
    delete m_config;
    m_config = NULL;
}

unsigned int wxsyConfig::ReadUint(const char* key, unsigned int defaultvalue) const {
    long result = 0;
    m_config->Read(s2wx(key), &result, defaultvalue);
    if(result < 0) result = defaultvalue;
    return result;
}

int wxsyConfig::ReadInt(const char* key, int defaultvalue) const {
    int result = defaultvalue;
    m_config->Read(s2wx(key), &result, defaultvalue);
    return result;
}

bool wxsyConfig::ReadBool(const char* key, bool defaultvalue) const {
    bool result = false;
    m_config->Read(s2wx(key), &result, defaultvalue);
    return result;
}

double wxsyConfig::ReadFloat(const char* key, double defaultvalue) const {
    double result = defaultvalue;
    m_config->Read(s2wx(key), &result, defaultvalue);
    return result;
}

bool wxsyConfig::WriteUint(const char* key, unsigned int value) {
    return m_config->Write(s2wx(key), (long)value);
}

bool wxsyConfig::WriteInt(const char* key, int value) {
    return m_config->Write(s2wx(key), (long)value);
}

bool wxsyConfig::WriteBool(const char* key, bool value) {
    return m_config->Write(s2wx(key), value);
}

bool wxsyConfig::WriteFloat(const char* key, double value) {
    return m_config->Write(s2wx(key), value);
}
