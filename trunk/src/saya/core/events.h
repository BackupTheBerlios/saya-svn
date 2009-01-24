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
        void DisconnectEvents();

        /** Destructor. */
        virtual ~syEvtHandler();

        /** @brief Temporarily disables event handling.
         *  This function has a counter that stores how many times in a row it's been disabled,
         *  so you don't need to worry about checking the state.
         */
        void DisableEventHandling();

        /** @brief Restores event handling. */
        void RestoreEventHandling();

    protected:
        /** @brief The delegate handler for the current object.
         * Event handlers can have a delegate that will take care of their events for them.
         * This can be useful for extending the Private-Implementation pattern to event handling.
         */
        syEvtHandler* m_Delegate;
    private:
        void AddEventHandler(const char* eventclass, int EventId, syFunctorBase* handlerfunc);
        void RemoveEventHandler(const char* eventclass, int EventId);
        class Data;
        friend class Data;
        Data* m_Data;
};

class syActionEvent : public syEvent {
    public:
        /** Constructor. */
        syActionEvent(unsigned int id);

        /** Standard Destructor. */
        virtual ~syActionEvent() {}
        virtual syEvent* clone();
        /** @brief Allocates a new Event ID.
         *  @warning This function is NOT thread-safe, it must be called by the main thread only!
         */
        static unsigned int NewId(const char* userstring = 0);

        /** @brief Returns the next available Action ID.
         *  @warning This function is NOT thread-safe, it must be called by the main thread only!
         *  @param userstring A user string to be related to this id. Used for Qt as a slot name.
         */
        static unsigned int CurrentId();

        /** @brief Reserves an Id to avoid collisions with NewId().
         *  @warning This function is NOT thread-safe, it must be called by the main thread only!
         *  @param userstring A user string to be related to this id. Used for Qt as a slot name.
         */
        static unsigned int RegisterId(unsigned int id, const char* userstring = 0);

        /** @brief Maps an Action Event Id to a string, and to a user string.
         *  Subsequent calls to RegisterId with this string will return the same numeric id.
         *  @param idname The registered name of the Event.
         *  @param userstring A user string to be related to this id. Used for Qt as a slot name.
         */
        static unsigned int RegisterId(const char* idname, const char* userstring = 0);

        /** @brief Maps an Action Event Id to a user string.
         *  @param id The registered event id#.
         *  @param userstring A user string to be related to this id. Used for Qt as a slot name.
         */
        static void RegisterUserString(unsigned int id, const char* userstring);

        /** @brief returns the user string associated to the given id.
         *  This can be used to help assign Qt slots to Menu actions automatically.
         */
        static const char* GetUserStringFromId(unsigned int id);
    private:
        class Data;
};

#endif

