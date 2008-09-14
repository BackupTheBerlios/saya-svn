/***************************************************************
 * Name:      recentfileslist.h
 * Purpose:   Implementation of a Recent Files list class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "recentfileslist.h"
#include <deque>

#define RECENTFILES_DEBUG

#ifdef RECENTFILES_DEBUG
#include "projectmanager.h"
#endif


/** Defines the maximum capacity for a Recent Files list */
const unsigned int RecentFilesListCapacity = 9;

class RecentFilesListData {
    public:
    std::deque<std::string> items;
    unsigned long Counter;
    unsigned int Capacity;
};

RecentFilesList::RecentFilesList(unsigned int capacity) {
    m_Data = new RecentFilesListData;
    if(capacity < 1) capacity = 1;
    if(capacity > 100) capacity = 100;
    m_Data->Capacity = capacity;
    m_Data->Counter = 1; // External counters are set to 0 - so we set it to one to indicate a refresh
}

RecentFilesList::~RecentFilesList() {
    delete m_Data;
}

void RecentFilesList::Add(const std::string& s, bool fromthebeginning) {

    if(s.empty()) { return; }

    std::deque<std::string>& items = m_Data->items;

    if(!fromthebeginning && items.size() >= m_Data->Capacity) { return; } // Queue full

    // First, check if it's in the list
    size_t i;
    for(i = 0; i < items.size(); i++) {
        if(s == items[i]) {
            return; // Found
        }
    }

    // Finally, add it
    if(fromthebeginning) {
        items.push_front(s); // Add to the beginning
        if(items.size() > m_Data->Capacity) {
            items.pop_back();
        }
    } else {
        items.push_back(s); // Add to the end
    }
    m_Data->Counter++;
}

void RecentFilesList::clear() {
    m_Data->items.clear();
    m_Data->Counter++;
}

const std::string& RecentFilesList::item(unsigned int i) {
    if(i > 0) --i;
    return m_Data->items[i];
}


unsigned int RecentFilesList::size() {
    return m_Data->items.size();
}

unsigned int RecentFilesList::Counter() {
    return m_Data->Counter;
}


bool RecentFilesList::UpdateCounter(unsigned int& destinationcounter) {
    if(destinationcounter == m_Data->Counter) {
        return false;
    }
    destinationcounter = m_Data->Counter;
    return true;
}
