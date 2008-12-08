/***************************************************************
 * Name:      recentfileslist.h
 * Purpose:   Implementation of a Recent Files list class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "recentfileslist.h"
#include "core/systring.h"

// #define RECENTFILES_DEBUG

#ifdef RECENTFILES_DEBUG
#include "projectmanager.h"
#endif

/** Defines the maximum capacity for a Recent Files list */
const unsigned int RecentFilesListCapacity = 9;

// -------------------------
// begin RecentFilesListData
// -------------------------

/** A lightweight double-ended string queue replacing std::deque */
class RecentFilesListData {
    public:
        /** Constructor. */
        RecentFilesListData(unsigned long capacity);

        /** Destructor. */
        ~RecentFilesListData();

        /** Gets the update-counter. */
        unsigned long GetCounter();

        /** Gets the current list size. */
        unsigned long size();

        /** Gets the current list capacity. */
        unsigned long GetCapacity();

        /** Gets a specific item. */
        const syString items(unsigned int i);

        /** Adds an item to the front, optionally checking if it's not already duplicated. */
        bool push_front(const syString& s, bool unique = false);

        /** Adds an item to the back, optionally checking if it's not already duplicated. */
        bool push_back(const syString& s, bool unique = false);

        /** Adds an item to the front/back, optionally checking if it's not already duplicated. */
        bool push(const syString& s, bool unique = false, bool from_front = false);

        /** Finds an item in the list, returning -1 if it's not found. */
        int find(const syString& s);

        /** Removes the last item in the list. */
        void pop_back();

        /** Removes the first item in the list. */
        void pop_front();

        /** Removes all items in the list. */
        void clear();
    private:
        unsigned long m_Counter;
        unsigned int m_Capacity;
        unsigned int m_Beginning;
        unsigned int m_Size;
        syString* m_Items;
};

RecentFilesListData::RecentFilesListData(unsigned long capacity) {
    if(capacity < 1) capacity = 1;
    if(capacity > 100) capacity = 100;
    m_Capacity = capacity;
    m_Items = new syString[m_Capacity];
    m_Size = 0;
    m_Counter = 1;
    m_Beginning = 0;
}

RecentFilesListData::~RecentFilesListData() {
    delete[] m_Items;
    m_Items = 0;
}

unsigned long RecentFilesListData::GetCounter() {
    return m_Counter;
}

unsigned long RecentFilesListData::size() {
    return m_Size;
}

unsigned long RecentFilesListData::GetCapacity() {
    return m_Capacity;
}

const syString RecentFilesListData::items(unsigned int i) {
    if(i >= m_Size) return "";
    i = (i + m_Beginning) % (m_Capacity);
    return m_Items[i];
}

bool RecentFilesListData::push_front(const syString& s, bool unique) {
    if(unique && find(s)>=0) return false;
    if(m_Size >= m_Capacity) {
        pop_back(); // There's always a vacancy
    }
    if(!m_Beginning) {
        m_Beginning = m_Capacity - 1;
    } else {
        --m_Beginning;
    }
    ++m_Size;
    m_Items[m_Beginning] = s;
    ++m_Counter;
    return true;
}

bool RecentFilesListData::push_back(const syString& s, bool unique) {
    if(m_Size >= m_Capacity) return false;
    if(unique && find(s)>=0) return false;
    m_Items[(m_Beginning + m_Size) % m_Capacity] = s;
    ++m_Size;
    ++m_Counter;
    return true;
}

bool RecentFilesListData::push(const syString& s, bool unique, bool from_front) {
    if(from_front) {
        return push_front(s, unique);
    }
    return push_back(s, unique);
}

int RecentFilesListData::find(const syString& s) {
    if(!m_Size) return -1;
    int result = -1;
    for(unsigned int i = 0; i < m_Size; ++i) {
        if(items(i) == s) {
            result = i;
            break;
        }
    }
    return result;
}

void RecentFilesListData::pop_back() {
    if(m_Size) {
        --m_Size;
    }
    ++m_Counter;
}

void RecentFilesListData::pop_front() {
    if(m_Size) {
        --m_Size;
        m_Beginning = (m_Beginning + 1) % m_Capacity;
    }
    ++m_Counter;
}

void RecentFilesListData::clear() {
    m_Beginning = 0;
    m_Size = 0;
    ++m_Counter;
}

// -------------------------
// end RecentFilesListData
// -------------------------

RecentFilesList::RecentFilesList(unsigned int capacity) {
    m_Data = new RecentFilesListData(capacity);
}

RecentFilesList::~RecentFilesList() {
    delete m_Data;
}

void RecentFilesList::Add(const char* s, bool fromthebeginning) {
    if(!s || !s[0]) { return; }
    syString tmps(s, true);
    m_Data->push(tmps, true, fromthebeginning);
}

void RecentFilesList::clear() {
    m_Data->clear();
}

const syString RecentFilesList::item(unsigned int i) const {
    if(i > 0) --i; // We're one-based.
    return m_Data->items(i);
}

unsigned int RecentFilesList::size() const {
    return m_Data->size();
}

unsigned int RecentFilesList::Counter() const {
    return m_Data->GetCounter();
}

bool RecentFilesList::UpdateCounter(unsigned int& destinationcounter) {
    if(destinationcounter == m_Data->GetCounter()) {
        return false;
    }
    destinationcounter = m_Data->GetCounter();
    return true;
}
