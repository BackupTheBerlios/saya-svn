/**************************************************************************************
 * Name:      sigslot.cpp
 * Purpose:   Implementation of a platform-independent signal and slot library.
 * Original
 *   author:  Sarah Thompson, 2002.
 * Modified
 *   by:      Rick Garcia
 * Created:   2010-11-28
 * Modified:  2010-12-05
 * Copyright: (c) 2010, Rick Garcia
 * License:   LGPL Licence version 3.0 or later
 * Notes:
 *              Original code available at http://sigslot.sourceforge.net/
 *              Modified by Rick Garcia 05/12/2010:
 *              - Completely discarded Miguel A. Gavidia's modifications (sorry, found a better way)
 *              - Moved the emitting code to a _signal_call_base class to get rid of many templates
 *              - Replaced multiple template class definitions with CPP macros
 *              - Added signal chaining support through the _connection_base class
 *              - Replaced thread policies with a special shared mutex template.
 **************************************************************************************/

#include "sythread.h"
#include "sigslot.h"

#include <set>
#include <list>

namespace sigslot {

typedef std::set<_signal_base *> signal_set;

// ------------------------
// begin _signal_base::Data
// ------------------------

class _signal_base::Data {
    public:
        typedef std::list<_connection_base*> connections_list;
        typedef connections_list::const_iterator const_iterator;
        typedef connections_list::iterator iterator;
        connections_list m_connections;
        sySharedMutex<_signal_base> m_Mutex;
};

// ------------------------
// end _signal_base::Data
// ------------------------

// ------------------
// begin _signal_base
// ------------------

_signal_base::_signal_base() : m_Data(new Data) {}

_signal_base::~_signal_base() {
    disconnect_all();
    delete m_Data;
    m_Data = 0;
}

_signal_base::_signal_base(const _signal_base& s) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        Data::const_iterator  it = s.m_Data->m_connections.begin(); Data::const_iterator itEnd = s.m_Data->m_connections.end();
        while (it != itEnd) { (*it)->getdest()->connect_signal(this); m_Data->m_connections.push_back((*it)->clone()); ++it; }
    }
}

void _signal_base::slot_duplicate(const has_slots* oldtarget, has_slots* newtarget) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        Data::iterator it = m_Data->m_connections.begin();Data::iterator itEnd = m_Data->m_connections.end();
        while (it != itEnd) { if ((*it)->getdest() == oldtarget) { m_Data->m_connections.push_back((*it)->duplicate(newtarget)); } ++it; }
    }
}

void _signal_base::disconnect(has_slots* target) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    Data::connections_list::iterator it = m_Data->m_connections.begin();
    Data::connections_list::iterator itEnd = m_Data->m_connections.end();

    while(it != itEnd) {
        if((*it)->getdest() == target) {
            delete *it;
            m_Data->m_connections.erase(it);
            target->disconnect_signal(this);
            return;
        }
        ++it;
    }
}

void _signal_base::disconnect_all() {
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    Data::connections_list::const_iterator it = m_Data->m_connections.begin();
    Data::connections_list::const_iterator itEnd = m_Data->m_connections.end();

    while(it != itEnd) {
        (*it)->getdest()->disconnect_signal(this);
        delete *it;
        ++it;
    }
    m_Data->m_connections.erase(m_Data->m_connections.begin(), m_Data->m_connections.end());
}

void _signal_base::emit_base(signal_call_base* pcall) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        Data::const_iterator itNext, it = m_Data->m_connections.begin();
        Data::const_iterator itEnd = m_Data->m_connections.end();

        while (it != itEnd) {
            itNext = it;
            ++itNext;
            (*it)->emit(pcall);
            it = itNext;
        }
    }

}


void _signal_base::add_connection(const _connection_base &conn) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        _connection_base* pconn = const_cast<_connection_base*>(&conn)->clone();
        m_Data->m_connections.push_back(pconn);
        pconn->getdest()->connect_signal(this);
    }
}

void _signal_base::remove_connection(const _connection_base &conn) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        has_slots* pobj = conn.getdest();
        Data::iterator itNext, it = m_Data->m_connections.begin(); \
        Data::iterator itEnd = m_Data->m_connections.end();
        bool wasconnected = false;
        while (it != itEnd) {
            itNext = it;
            ++itNext;
            if((*it)->equals(&conn)) {
                delete (*it);
                m_Data->m_connections.erase(it);
                wasconnected = true;
                break;
            }
            it = itNext;
        }
        if(wasconnected) {
            pobj->disconnect_signal(this);
        }
    }
}

void _signal_base::kill_connection(has_slots* target) {
    sySafeMutexLocker lock(*(m_Data->m_Mutex())); if(lock.IsLocked()) {
        Data::iterator it = m_Data->m_connections.begin(); Data::iterator itEnd = m_Data->m_connections.end();
        while (it != itEnd) { Data::iterator itNext = it; ++itNext; if ((*it)->getdest() == target) { delete *it; m_Data->m_connections.erase(it); } it = itNext; }
    }
}

// ----------------
// end _signal_base
// ----------------


class has_slots::Data {
    public:
        signal_set m_signals;
        sySharedMutex<has_slots> m_Mutex;
};

has_slots::has_slots() : m_Data(new Data) {
}

has_slots::has_slots(const has_slots& hs) : m_Data(new Data) {
   sySafeMutexLocker lock(*(m_Data->m_Mutex()));
   if(lock.IsLocked()) {
       signal_set::const_iterator it = hs.m_Data->m_signals.begin();
       signal_set::const_iterator itEnd = hs.m_Data->m_signals.end();

        while (it != itEnd) {
            (*it)->slot_duplicate(&hs, this);
            m_Data->m_signals.insert(*it);
            ++it;
        }
   }
}

has_slots::~has_slots()
{
    disconnect_all();
    delete m_Data;
    m_Data = 0;
}

void has_slots::connect_signal(_signal_base* signal)
{
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    if(lock.IsLocked()) {
        m_Data->m_signals.insert(signal);
    }
}

void has_slots::disconnect_signal(_signal_base* signal)
{
    sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    m_Data->m_signals.erase(signal);
}

void has_slots::disconnect_all()
{
   sySafeMutexLocker lock(*(m_Data->m_Mutex()));
    signal_set::const_iterator it = m_Data->m_signals.begin();
    signal_set::const_iterator itEnd = m_Data->m_signals.end();

    while (it != itEnd) {
        (*it)->kill_connection(this);
        ++it;
    }
    m_Data->m_signals.erase(m_Data->m_signals.begin(), m_Data->m_signals.end());
}

}; // namespace sigslot
