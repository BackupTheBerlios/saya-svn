/*************************************************************************************
 * Name:      events.h
 * Purpose:   Declaration of event-related classes: syEvent, syEvtQueue, syEvtHandler.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 * Notes:     To connect and disconnect events to a handler, see evtregistry.h
 *************************************************************************************/

#ifndef syevents_h
#define syevents_h

class syFunctorBase;
class syEvtHandler;

class syEvent {
    public:
        unsigned int EventId;
        syEvent(unsigned int id = 0) : EventId(id) {}
        virtual ~syEvent() {}
        virtual syEvent* clone() = 0;
};

/** syEvtHandler is the base for all our event handlers. */
class syEvtHandler {
    friend class syEvtRegistry;
    public:
        /** Constructor. */
        syEvtHandler();

        /** Processes an event, dispatching it to the appropriate member function. */
        void ProcessEvent(syEvent& evt);

        /** Disconnects all events from the current object. */
        void syDisconnectEvents();

        /** Destructor. */
        virtual ~syEvtHandler();

    private:
        void AddEventHandler(const char* eventclass, int EventId, syFunctorBase* handlerfunc);
        void RemoveEventHandler(const char* eventclass, int EventId);
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
