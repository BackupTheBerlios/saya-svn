/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Declaration for the Debug Log
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/
#ifndef debuglog_h
#define debuglog_h


#include "../saya/core/debuglog.h"

class syString;
class QString;

class AppDebugLog : public syDebugLog {
    public:
        AppDebugLog();

        virtual void DebugLog(const char* msg);

        virtual void DebugLog(const syString& msg);

        void DebugLog(const QString& msg);

        virtual ~AppDebugLog();
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
