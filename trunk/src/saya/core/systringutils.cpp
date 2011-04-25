/***************************************************************
 * Name:      systringutils.cpp
 * Purpose:   Implementation of the syString utility functions
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-24
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 **************************************************************/

#include "systringutils.h"

syString implode(const syString& joiner, std::vector<syString> vec) {
    syString result = "";
    for(unsigned int i = 0; i < vec.size(); ++i) {
        if(i > 0) {
            result << joiner;
        }
        result << vec[i];
    }
    return result;
}

std::vector<syString> explode(const syString& joiner, syString s) {
    std::vector<syString> result;
    int curpos = 0;
    unsigned int lastpos = 0;
    int joinerlength = joiner.length();
    int slength = s.length();
    if(joinerlength) {
        while(curpos >= 0 && curpos < slength) {
            curpos = s.find(joiner, lastpos);
            if(curpos == syString::npos) {
                result.push_back(s.substr(lastpos,s.length()));
                break;
            } else {
                result.push_back(s.substr(lastpos,curpos - lastpos));
                result.push_back(joiner);
                lastpos = curpos + joinerlength;
            }

        }
    }
    return result;
}

syString strtolower(const syString& s) {
    syString result(s);
    for(unsigned int i = 0, ii = result.length(); i < ii; ++i) {
        char c = result[i];
        if(c >= 'A' && c <= 'Z') {
            result[i] = c + ('a' - 'A');
        }
    }
    return result;
}

syString strtoupper(const syString& s) {
    syString result(s);
    for(unsigned int i = 0, ii = result.length(); i < ii; ++i) {
        char c = result[i];
        if(c >= 'a' && c <= 'z') {
            result[i] = c - ('a' - 'A');
        }
    }
    return result;
}
