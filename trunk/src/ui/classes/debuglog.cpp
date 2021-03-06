/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Implementation of the Debug Log
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <saya/core/iocommon.h>
#include <saya/core/systring.h>
#include <saya/core/app.h>
#include <saya/core/sythread.h>
#include "debuglog.h"

#include <qframe.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qdatetime.h>

#include <deque>

std::deque<QString> PendingDebugLogMessages;
static volatile bool s_DebugLogPending = false;
syMutex s_MyDebugLogMutex;

class AppDebugLog::Data: public QTextEdit {
    public:
        Data(AppDebugLog* parent);
        virtual ~Data();
        AppDebugLog* m_Parent;
        void OnIdle();
        void timerEvent(QTimerEvent* event) { OnIdle(); }
    private:
        int m_TimerId;

};

AppDebugLog::Data::Data(AppDebugLog* parent) :
QTextEdit(0),
m_Parent(parent),
m_TimerId(0)
{
	this->setWindowFlags(Qt::Window);
	this->setAttribute(Qt::WA_QuitOnClose, true);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowTitle("Debug Log");
	this->setReadOnly(true);
	this->resize(450,300);
	syApp::Get()->SetTopWindow(this);
    this->show();
    m_TimerId = startTimer(3);
}

AppDebugLog::Data::~Data() {
    killTimer(m_TimerId);
    if(m_Parent) {
        m_Parent->m_Data = 0;
    }
    m_Parent = 0;

}

AppDebugLog::AppDebugLog() :
m_Data(new Data(this))
{
}

void AppDebugLog::DebugLog(const char* msg) {
    DebugLog(QString(msg));
}

void AppDebugLog::DebugLog(const syString& msg) {
    DebugLog(QString(msg));
}

void AppDebugLog::DebugLog(const QString& msg) {
    if(!m_Data) {
        return;
    }
    if(!syThread::IsMain()) {
        syMutexLocker lock(s_MyDebugLogMutex);
        PendingDebugLogMessages.push_back(msg);
        s_DebugLogPending = true;
        syApp::Get()->WakeUpIdle();
        return;
    }
    QDateTime t = QDateTime::currentDateTime();
    syString s;
    s << "[" << syString(t.toString("yy:MM:dd hh:mm:ss")) << "] : " << syString(msg);
    m_Data->append(s);
    s << "\n";
    if(!syApp::Get()->IsMainLoopRunning()) {
        ioCommon::Print(s.c_str());
    }
}

void AppDebugLog::Data::OnIdle() {
    if(s_DebugLogPending) {
        syMutexLocker lock(s_MyDebugLogMutex);
        m_Parent->DebugLog(PendingDebugLogMessages[0]);
        PendingDebugLogMessages.pop_front();
        s_DebugLogPending = PendingDebugLogMessages.size() > 0;
    }
}

AppDebugLog::~AppDebugLog() {
    delete m_Data;
    m_Data = 0;
}
