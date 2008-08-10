#include "avcommon.h"
#include <sstream>
using namespace std;

std::string serializable::serialize() {
    return "<null />";
}

bool serializable::unserialize(const std::string& data) {
    return false;
}

std::string AVSettings::serialize() {
    std::string result;
    std::string opentag = "<AVSettings";
    std::string closetag = "</AVSettings>";
    propertymap::iterator iter;

    std::stringstream attributes;
    std::string attr;
    attributes << " width=\"" << width << "\"";
    attributes << " hegiht=\"" << height << "\"";
    attributes << " pixelaspect=\"" << pixelaspect << "\"";
    attributes << " fps=\"" << fps << "\"";
    result = opentag + attributes.str() + ">";
    result += std::string("<vidformat type=\"") + vidformat + "\">";
    for(iter = formatsettings.begin();iter != formatsettings.end(); iter++) {
        result += std::string("<item name=\"") + iter->first + "\">" + iter->second + "</item>";
    }
    result += "</vidformat>";
    result += std::string("<videocodec type=\"") + videocodec + "\">";
    for(iter = videocodecsettings.begin();iter != videocodecsettings.end(); iter++) {
        result += std::string("<item name=\"") + iter->first + "\">" + iter->second + "</item>";
    }
    result += "</videocodec>";
    result += std::string("<audioocodec type=\"") + audiocodec + "\">";
    for(iter = audiocodecsettings.begin();iter != audiocodecsettings.end(); iter++) {
        result += std::string("<item name=\"") + iter->first + "\">" + iter->second + "</item>";
    }
    result += "</audiocodec>";
    result += closetag;
    return result;
}

bool AVSettings::unserialize(const std::string& data) {
// FIXME (rick#1#): Implement me!
    return false;
}
