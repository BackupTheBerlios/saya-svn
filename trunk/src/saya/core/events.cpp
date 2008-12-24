/************************************************************************************
 * Name:      evemts.cpp
 * Purpose:   Implementation of a UI-neutral event handling system
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 * Notes:     The event handling code used here was taken from the GameDev article
 *            entitled  "Effective Event Handling in C++", located at
 *            http://www.gamedev.net/reference/programming/features/effeventcpp/
 *            The same concept is explained at http://www.newty.de/fpt/functor.html
 ************************************************************************************/
#include <typeinfo>
#include <set>
#include <map>
#include <deque>
#include "systring.h"
#include "sythread.h"
#include "evtregistry.h" // evtregistry.h includes events.h
#include "eventqueue.h"

// -----------------
// begin syEventItem
// -----------------

class syEventItem {
    public:
        syEvent* m_Event;
        syEvtHandler* m_Recipient;
        syEventItem() : m_Event(0), m_Recipient(0) {}
        syEventItem(syEvtHandler* recipient, syEvent& event) : m_Event(event.clone()), m_Recipient(recipient) {}
        syEventItem(const syEventItem& copy) : m_Event(copy.m_Event->clone()), m_Recipient(copy.m_Recipient) {}
        syEventItem& operator=(const syEventItem& copy) {
            delete m_Event;
            m_Event = 0;
            m_Event = copy.m_Event->clone();
            m_Recipient = copy.m_Recipient;
            return *this;
        }
        void Execute() {
            if(m_Recipient) {
                m_Recipient->ProcessEvent(*m_Event);
            }
        }
        ~syEventItem() {
            delete m_Event;
        }
};

typedef std::deque<syEventItem> syEvtInnerQueue;
static syEvtInnerQueue g_Queue;
static syMutex g_EventsMutex;

// ---------------
// end syEventItem
// ---------------

// ------------------------
// begin syEvtHandler::Data
// ------------------------

class syEvtHandler::Data {
    public:
        typedef std::map<int, syFunctorBase*>intFunctorMap;
        typedef std::map<syString, intFunctorMap, ltsystr> FunctorMap;
        typedef std::set<syEvtHandler*> EventHandlerSet;
        FunctorMap m_Handlers;
        unsigned int m_DisabledCount;
        Data() : m_DisabledCount(0) {}
        ~Data();
        void AddEventHandler(const char* eventclass, int EventId, syFunctorBase* handlerfunc);
        void RemoveEventHandler(const char* eventclass, int EventId);
        void RemoveAllHandlers();
        void ProcessEvent(syEvent& evt);
};

inline syEvtHandler::Data::~Data() {
    RemoveAllHandlers();
}

inline void syEvtHandler::Data::AddEventHandler(const char* eventclass, int EventId, syFunctorBase* handlerfunc) {
    RemoveEventHandler(eventclass, EventId);
    m_Handlers[eventclass][EventId] = handlerfunc;
}

inline void syEvtHandler::Data::RemoveEventHandler(const char* eventclass, int EventId) {
    FunctorMap::iterator i = m_Handlers.find(syString(eventclass));
    if(i != m_Handlers.end()) {
        intFunctorMap::iterator j = i->second.find(EventId);
        if(j != i->second.end()) {
            delete j->second;
            j->second = 0;
        }
    }
}

inline void syEvtHandler::Data::RemoveAllHandlers() {
    FunctorMap::iterator i;
    for(i = m_Handlers.begin(); i != m_Handlers.end(); ++i) {
        intFunctorMap::iterator j;
        for(j = i->second.begin(); j != i->second.end(); ++j) {
            delete j->second;
            j->second = 0;
        }
    }
    m_Handlers.clear();
}

inline void syEvtHandler::Data::ProcessEvent(syEvent& evt) {
    syString evtname = typeid(evt).name();
    FunctorMap::iterator i = m_Handlers.find(evtname);
    if(i != m_Handlers.end()) {
        intFunctorMap::iterator j = i->second.find(evt.EventId);
        if(j == i->second.end()) {
            j = i->second.find(-1); // -1 by default catches all event ids.
        }
        if(j != i->second.end()) {
            syFunctorBase* functor = j->second;
            if(functor) {
                functor->Exec(evt);
            }
        }
    }
}

// ----------------------
// end syEvtHandler::Data
// ----------------------

// ------------------
// begin syEvtHandler
// ------------------

syEvtHandler::syEvtHandler() : m_Delegate(0), m_Data(new Data) {
}

syEvtHandler::~syEvtHandler() {
    m_Delegate = 0;
    DisconnectEvents();
    delete m_Data;
    m_Data = 0;
}

void syEvtHandler::ProcessEvent(syEvent& evt) {
    if(!this) return;
    if(!m_Data || m_Data->m_DisabledCount > 0) return;
    if(m_Delegate) {
        m_Delegate->ProcessEvent(evt);
    } else {
        m_Data->ProcessEvent(evt);
    }
}

void syEvtHandler::DisableEventHandling() {
    ++(m_Data->m_DisabledCount);
}

void syEvtHandler::RestoreEventHandling() {
    if(m_Data->m_DisabledCount > 0) {
        --(m_Data->m_DisabledCount);
    }
}

void syEvtHandler::DisconnectEvents() {
    m_Data->RemoveAllHandlers();
}

void syEvtHandler::AddEventHandler(const char* eventclass, int EventId, syFunctorBase* handlerfunc) {
    m_Data->AddEventHandler(eventclass, EventId, handlerfunc);
}

void syEvtHandler::RemoveEventHandler(const char* eventclass, int EventId) {
    m_Data->RemoveEventHandler(eventclass, EventId);
}

// ----------------
// end syEvtHandler
// ----------------

// ---------------
// begin syEvtQueue
// ---------------
void syEvtQueue::PostEvent(syEvtHandler* handler, syEvent& event) {
    if(!handler) return;
    syMutexLocker lock(g_EventsMutex);
    g_Queue.push_back(syEventItem(handler,event));
}

bool syEvtQueue::ProcessNextEvent() {
    syMutexLocker lock(g_EventsMutex);
    if(g_Queue.size()) {
        syEventItem item(g_Queue[0]);
        g_Queue.pop_front();
        item.Execute();
    }
    return g_Queue.size() > 0;
}

bool syEvtQueue::Pending() {
    syMutexLocker lock(g_EventsMutex);
    return g_Queue.size() > 0;
}

// --------------
// end syEvtQueue
// --------------
