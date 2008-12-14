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

syString wxsyConfig::Read(const char* key, const char* defaultvalue) {
    return wx2s(m_config->Read(s2wx(key),s2wx(defaultvalue)));
}

bool wxsyConfig::Write(const char* key, const char* value) {
    return m_config->Write(s2wx(key),s2wx(value));
}

bool wxsyConfig::Exists(const char* key) {
    return m_config->Exists(s2wx(key));
}

wxsyConfig::~wxsyConfig() {
    delete m_config;
    m_config = NULL;
}
