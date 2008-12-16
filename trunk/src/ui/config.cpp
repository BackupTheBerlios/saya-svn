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
#include "../saya/core/systring.h"

wxsyConfig::wxsyConfig(const char* application_name) : syConfig(application_name) {
    m_config = new wxConfig(syString(application_name, true));
}

syString wxsyConfig::Read(const char* key, const char* defaultvalue) const {
    return syString(m_config->Read(syString(key, true),syString(defaultvalue, true)));
}

bool wxsyConfig::Write(const char* key, const char* value) {
    return m_config->Write(syString(key, true),syString(value, true));
}

bool wxsyConfig::Exists(const char* key) const {
    return m_config->Exists(syString(key, true));
}

wxsyConfig::~wxsyConfig() {
    delete m_config;
    m_config = NULL;
}

unsigned int wxsyConfig::ReadUint(const char* key, unsigned int defaultvalue) const {
    long result = 0;
    m_config->Read(syString(key, true), &result, defaultvalue);
    if(result < 0) result = defaultvalue;
    return result;
}

int wxsyConfig::ReadInt(const char* key, int defaultvalue) const {
    int result = defaultvalue;
    m_config->Read(syString(key, true), &result, defaultvalue);
    return result;
}

bool wxsyConfig::ReadBool(const char* key, bool defaultvalue) const {
    bool result = false;
    m_config->Read(syString(key, true), &result, defaultvalue);
    return result;
}

double wxsyConfig::ReadFloat(const char* key, double defaultvalue) const {
    double result = defaultvalue;
    m_config->Read(syString(key, true), &result, defaultvalue);
    return result;
}

bool wxsyConfig::WriteUint(const char* key, unsigned int value) {
    return m_config->Write(syString(key, true), (long)value);
}

bool wxsyConfig::WriteInt(const char* key, int value) {
    return m_config->Write(syString(key, true), (long)value);
}

bool wxsyConfig::WriteBool(const char* key, bool value) {
    return m_config->Write(syString(key, true), value);
}

bool wxsyConfig::WriteFloat(const char* key, double value) {
    return m_config->Write(syString(key, true), value);
}
