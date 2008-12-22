/****************************************************************
 * Name:      eventqueue.h
 *            (implemented in events.cpp)
 * Purpose:   Declaration of a UI-neutral event queue object.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-12
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   wxWindows license
 * Notes:     This was based mainly on the wxWidgets event loop,
 *            except that we only have one event loop. This
 *            simplifies things a lot.
 ****************************************************************/

#ifndef sy_eventqueue_h
#define sy_eventqueue_h

class syEvtHandler;
class syEvent;

class syEvtQueue {
    public:
        /** Adds an event into the global event queue. */
        static void PostEvent(syEvtHandler* handler, syEvent& event);

        /** @brief Processes the next event in the global event queue.
         *  @return true if there are more events waiting in the queue, false otherwise.
         *  @warning This function must be called by the main thread ONLY!
         */
        static bool ProcessNextEvent();

        /** @brief returns true if there are events waiting in the Event Queue. */
        static bool Pending();
};

#endif
