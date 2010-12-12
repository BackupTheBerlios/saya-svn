/***************************************************************
 * Name:      ui/config.cpp
 * Purpose:   Implements the Application Configuration Handler
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "config.h"
#include <qsettings.h>
#include <saya/core/systring.h>

QsyConfig::QsyConfig(const char* application_name) : syConfig(application_name) {
    m_config = new QSettings;
}

syString QsyConfig::Read(const char* key, const char* defaultvalue) const {
    return syString(m_config->value(key, defaultvalue).toString().toUtf8().constData());
}

bool QsyConfig::Write(const char* key, const char* value) {
    m_config->setValue(key, value);
    return true;
}

bool QsyConfig::Exists(const char* key) const {
    return m_config->contains(key);
}

QsyConfig::~QsyConfig() {
    delete m_config;
    m_config = NULL;
}

unsigned int QsyConfig::ReadUint(const char* key, unsigned int defaultvalue) const {
    long result = 0;
    result = m_config->value(key, defaultvalue).toInt();
    if(result < 0) result = defaultvalue;
    return result;
}

int QsyConfig::ReadInt(const char* key, int defaultvalue) const {
    int result = defaultvalue;
    result = m_config->value(key, defaultvalue).toInt();
    return result;
}

bool QsyConfig::ReadBool(const char* key, bool defaultvalue) const {
    bool result = false;
    result = m_config->value(key, defaultvalue).toBool();
    return result;
}

double QsyConfig::ReadFloat(const char* key, double defaultvalue) const {
    double result = defaultvalue;
    result = m_config->value(key, defaultvalue).toDouble();
    return result;
}

bool QsyConfig::WriteUint(const char* key, unsigned int value) {
    m_config->setValue(key, value);
    return true;
}

bool QsyConfig::WriteInt(const char* key, int value) {
    m_config->setValue(key, value);
    return true;
}

bool QsyConfig::WriteBool(const char* key, bool value) {
    m_config->setValue(key, value);
    return true;
}

bool QsyConfig::WriteFloat(const char* key, double value) {
    m_config->setValue(key, value);
    return true;
}
