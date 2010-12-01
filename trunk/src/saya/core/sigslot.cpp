/**************************************************************************************
 * Name:      sigslot.cpp
 * Purpose:   Implementation of a platform-independent signal and slot library.
 * Author:    Sarah Thompson
 * Modified
 * by:        Miguel A. Gavidia, Rick Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Miguel A. Gavidia, Rick Garcia
 * License:   LGPL Licence version 2.1 or later
 * Notes:
 *              Modified by juvinious adding in chaining support 01/01/2007:
 *              - Modded to add in specific slot disconnections 01/24/2007
 *              - Added in stuff to be inclusive to saggui
 *              Modified by Rick Garcia 28/11/2010:
 *              - Removed saggui stuff that we don't need
 *              - Moved some classes to an external .cpp file
 *              - Made multithreaded by default using a special shared mutex template.
 *              - Removed now deprecated thread policies.
 *              - Replaced multiple template class definitions with CPP macros
 **************************************************************************************/

#include "sigslot.h"

#include <set>

namespace sigslot {

typedef std::set<_signal_base *> sender_set;

class has_slots::Data {
    public:
        sender_set m_senders;
        sySharedMutex<has_slots> m_Mutex;
};

DEFINE_SHAREDMUTEXDATA(has_slots);


has_slots::has_slots() : m_Data(new Data) {
}

has_slots::has_slots(const has_slots& hs) : m_Data(new Data) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
   sender_set::const_iterator it = hs.m_Data->m_senders.begin();
   sender_set::const_iterator itEnd = hs.m_Data->m_senders.end();

    while (it != itEnd) {
        (*it)->slot_duplicate(&hs, this);
        m_Data->m_senders.insert(*it);
        ++it;
    }
}

void has_slots::signal_connect(_signal_base* sender)
{
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    m_Data->m_senders.insert(sender);
}

void has_slots::signal_disconnect(_signal_base* sender)
{
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    m_Data->m_senders.erase(sender);
}

has_slots::~has_slots()
{
    disconnect_all_slots();
    delete m_Data;
    m_Data = 0;
}

void has_slots::disconnect_all_slots()
{
   sySafeMutexLocker lock(*(m_Data->m_Mutex()));
   sender_set::const_iterator it = m_Data->m_senders.begin();
   sender_set::const_iterator itEnd = m_Data->m_senders.end();

    while (it != itEnd)
    {
        (*it)->slot_disconnect(this);
        ++it;
    }

    m_Data->m_senders.erase(m_Data->m_senders.begin(), m_Data->m_senders.end());
}


// -----------------
// Begin has_signals
// -----------------

has_signals::has_signals() {}

has_signals::~has_signals() {
    remove_connections();
}

void has_signals::bind_signal(has_signals* chainsig) {
    this->bound_signals.push_back(chainsig);
}

void has_signals::connect_signal(has_signals* chainsig) {
    if (chainsig == this)return;
    sig_iterator it = connected_signals.begin();
    sig_iterator itEnd = connected_signals.end();
    sig_iterator itFind;

    if ((itFind = std::find(it, itEnd, chainsig)) == itEnd) {
        this->connected_signals.push_back(chainsig);
        chainsig->bind_signal(this);
    }
}

void has_signals::disconnect_connected(has_signals* chainsig) {
    sig_iterator it = connected_signals.begin();
    sig_iterator itEnd = connected_signals.end();
    sig_iterator itFind;

    if ((itFind = std::find(it, itEnd, chainsig)) != itEnd) {
        if ((*itFind)) {
            (*itFind)->bound_signals.remove(this);
        }
        connected_signals.erase(itFind);
    }
}

void has_signals::disconnect_bound(has_signals* chainsig) {
    sig_iterator it = bound_signals.begin();
    sig_iterator itEnd = bound_signals.end();
    sig_iterator itFind;

    if ((itFind = std::find(it, itEnd, chainsig)) != itEnd) {
        if ((*itFind)) {
            (*itFind)->connected_signals.remove(this);
        }
        bound_signals.erase(itFind);
    }
}

void has_signals::remove_connections() {
    if (!connected_signals.empty()) {
        sig_iterator it = connected_signals.begin();
        sig_iterator itEnd = connected_signals.end();
        while (it != itEnd) {
            if ((*it))(*it)->bound_signals.remove(this);
            ++it;
        }
        connected_signals.resize(0);
    }

    if (!bound_signals.empty()) {
        sig_iterator it = bound_signals.begin();
        sig_iterator itEnd = bound_signals.end();
        while (it != itEnd) {
            if ((*it))(*it)->connected_signals.remove(this);
            ++it;
        }
        bound_signals.resize(0);
    }
}

// -----------------
// End has_signals
// -----------------


} // namespace sigslot
