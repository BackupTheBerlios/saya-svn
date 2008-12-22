/************************************************************************************
 * Name:      evtregistry.h
 * Purpose:   Declaration and Implementation of the syConnect/syDisconnect functions.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 * Notes:     The event handling code used here was taken from the GameDev article
 *            entitled  "Effective Event Handling in C++", located at
 *            http://www.gamedev.net/reference/programming/features/effeventcpp/
 *            The same concept is explained at http://www.newty.de/fpt/functor.html
 ************************************************************************************/

#ifndef syevtregistry_h
#define syevtregistry_h

#include <typeinfo>
#include "events.h"

/** @brief This is a pure virtual class used by our event handler wrapper.
  *
  */
class syFunctorBase {
    public:
        virtual ~syFunctorBase() {};
        void Exec(syEvent& event) { Call(event);}
    private:
        virtual void Call(syEvent&) = 0;
};

/** This is the functor class that we'll use to actually call the event handlers. */
template <class T, class EventT>
class syFunctor : public syFunctorBase {
    public:
        typedef void (T::*MemberFunc)(EventT&);
        syFunctor(T* instance, MemberFunc memFn) : m_Obj(instance), m_Func(memFn) {};

        void Call(syEvent& event) {
            (m_Obj->*m_Func)(static_cast<EventT&>(event));
        }

    private:
        T* m_Obj;
        MemberFunc m_Func;
};

class syEvtRegistry {
    public:

        /** Connects an event to the event handlers map. */
        template <class T, class EventT>
        static void Connect(T* obj, int EventId, void (T::*memberFunc)(EventT&));

        /** Disconnects an event from the event handlers map. */
        template <class T, class EventT>
        static void Disconnect(T* obj, int EventId, void (T::*memberFunc)(EventT&));

};

template <class T, class EventT>
void syEvtRegistry::Connect(T* obj, int EventId, void (T::*memberFunc)(EventT&)) {
  obj->AddEventHandler(typeid(EventT).name(), EventId, new syFunctor<T, EventT>(obj, memberFunc));
}

template <class T, class EventT>
void syEvtRegistry::Disconnect(T* obj, int EventId, void (T::*memberFunc)(EventT&)) {
  obj->RemoveEventHandler(typeid(EventT).name(), EventId);
}

template <class T, class EventT>
void syConnect(T* obj, int EventId, void (T::*memberFunc)(EventT&)) {
  syEvtRegistry::Connect(obj, EventId, memberFunc);
}

template <class T, class EventT>
void syDisconnect(T* obj, int EventId, void (T::*memberFunc)(EventT&)) {
  syEvtRegistry::Disconnect(obj, EventId, memberFunc);
}

#endif
