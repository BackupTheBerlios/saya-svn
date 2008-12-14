/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Declaration for the sayaDebugLogger class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef sydebuglog_h
#define sydebuglog_h

class syString;

/** Adds a message to the currently-assigned debug log */
void DebugLog(const char* msg);

/** Adds a message to the currently-assigned debug log (syString version) */
void DebugLog(const syString& msg);

class syDebugLog {
    public:

        /** Adds a message to the debug log */
        virtual void DebugLog(const char* msg) = 0;

        /** Adds a message to the debug log (syString version) */
        virtual void DebugLog(const syString& msg) = 0;

        /** @brief sets the pointer for the program's debug logger
          *
          * Call this from your main program to specify an object which will do the
          * debug logging.
          */
        static void SetDebugLog(syDebugLog* log);

        /** Deletes the current debug log from memory. */
        static void DeleteDebugLog();

        /** Standard constructor */
        syDebugLog() {}

        /** Standard destructor */
        virtual ~syDebugLog();
};

#endif
