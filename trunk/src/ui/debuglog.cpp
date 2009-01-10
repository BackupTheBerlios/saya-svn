/***************************************************************
 * Name:      debuglog.h
 * Purpose:   Implementation of the Debug Log
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-06-17
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "../saya/core/systring.h"
#include "../saya/core/app.h"
#include "../saya/core/sythread.h"
#include "debuglog.h"

#include <qframe.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qdatetime.h>

#include <deque>

std::deque<QString> PendingDebugLogMessages;
static volatile bool s_DebugLogPending;
syMutex s_MyDebugLogMutex;

class AppDebugLog::Data: public QTextEdit {
    Q_OBJECT
    public:
        Data(AppDebugLog* parent);
        virtual ~Data();
        AppDebugLog* m_Parent;
        QTimer* m_Timer;
    public slots:
        void OnIdle();

};

AppDebugLog::Data::Data(AppDebugLog* parent) :
QTextEdit(0),
m_Parent(parent),
m_Timer(new QTimer(this))
{
	this->setWindowFlags(Qt::Window);
	this->setAttribute(Qt::WA_QuitOnClose, true);
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	this->setWindowTitle("Debug Log");
	this->setReadOnly(true);
	this->resize(450,300);
	syApp::Get()->SetTopWindow(this);
    this->show();
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnIdle()) );
    m_Timer->start(1);
}

AppDebugLog::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
    }
    if(m_Timer) {
        disconnect(m_Timer, SIGNAL(timeout()), this, SLOT(OnIdle()) );
        delete m_Timer;
        m_Timer = 0;
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
    if(!syApp::Get()->IsMainLoopRunning()) {
        m_Data->repaint();
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

#include "debuglog.moc.h"
