// **************************************************************************************
// * Name:      jog_ctrl.cpp
// * Purpose:   Implementation of class JogControl
// * Authors:   Ricardo Garcia
// * Created:   2009-02-06
// * Copyright: Ricardo Garcia
// * License:   LGPL license version 3 or later.
// **************************************************************************************/

#include "jog_ctrl.h"
#include <math.h>
#include <QtSvg>
#include <QSvgRenderer>

class JogControl::Data : public QObject {
    public:
        static const char* base64_knob;
        static const char* base64_handle;
        static const unsigned int MinimumSteps = 3;
        Data(JogControl* parent);
        virtual ~Data();
        JogControl* m_Parent;
        QSvgRenderer *m_FgRenderer, *m_BgRenderer;
        QRectF GetHandleRect(double angle); // 0 = 12 o' clock, 180 = 6 o' clock)
        unsigned int m_TotalSteps, m_CurrentStep;
        double m_CurrentAngle,m_LastAngle,m_StepAngle;
        bool LoadSvg();
        double GetStepAngle() const;
        unsigned int GetStepCount() const;
        void SetStepCount(unsigned int numsteps);
        unsigned int GetCurrentStep() const; // From 0 to numsteps - 1
        void SetCurrentStep(unsigned int stepnum); // From 0 to numsteps - 1
        double GetCurrentAngle() const;
        void SetCurrentAngle(double angle);  // 0 = 12 o' clock, 180 = 6 o' clock)
        int ChangeAngleAndProcess(double angle); // Changes angle; returns number of steps to advance.
        void RecalcStep();
        void RecalcAngle();
        double CalculateAngle(double x, double y) const;
};

// cleaned-up and base64-encoded version of resources/jog_ctrl_knob.svg
// (Original made with Inkscape)
const char* JogControl::Data::base64_knob =
"PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8"
"c3ZnIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6Y2M9"
"Imh0dHA6Ly9jcmVhdGl2ZWNvbW1vbnMub3JnL25zIyIgeG1sbnM6cmRmPSJodHRwOi8vd3d3Lncz"
"Lm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIiB4bWxuczpzdmc9Imh0dHA6Ly93d3cudzMu"
"b3JnLzIwMDAvc3ZnIiB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHhtbG5zOnhs"
"aW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5rIiB2ZXJzaW9uPSIxLjEiIHdpZHRoPSI1"
"MDAiIGhlaWdodD0iNTAwIiBpZD0ic3ZnMiI+DQo8ZGVmcyBpZD0iZGVmczQiPg0KPGxpbmVhckdy"
"YWRpZW50IGlkPSJsaW5lYXJHcmFkaWVudDQ0ODgiPjxzdG9wIGlkPSJzdG9wNDQ5MCIgc3R5bGU9"
"InN0b3AtY29sb3I6IzU5NTk1OTtzdG9wLW9wYWNpdHk6MSIgb2Zmc2V0PSIwIiAvPjxzdG9wIGlk"
"PSJzdG9wNDQ5MiIgc3R5bGU9InN0b3AtY29sb3I6IzkwOTA5MDtzdG9wLW9wYWNpdHk6MSIgb2Zm"
"c2V0PSIxIiAvPjwvbGluZWFyR3JhZGllbnQ+DQo8bGluZWFyR3JhZGllbnQgaWQ9ImxpbmVhckdy"
"YWRpZW50Mjk4NyI+PHN0b3AgaWQ9InN0b3AyOTg5IiBzdHlsZT0ic3RvcC1jb2xvcjojNjc2NzY3"
"O3N0b3Atb3BhY2l0eToxIiBvZmZzZXQ9IjAiIC8+PHN0b3AgaWQ9InN0b3AyOTkxIiBzdHlsZT0i"
"c3RvcC1jb2xvcjojYmZiZmJmO3N0b3Atb3BhY2l0eToxIiBvZmZzZXQ9IjEiIC8+PC9saW5lYXJH"
"cmFkaWVudD4NCjxsaW5lYXJHcmFkaWVudCB4MT0iMjg0LjExMjg1IiB5MT0iNTYwLjkyNjg4IiB4"
"Mj0iNTczLjE3MzE2IiB5Mj0iOTQ5LjIyNjA3IiBpZD0ibGluZWFyR3JhZGllbnQ1MjI1IiB4bGlu"
"azpocmVmPSIjbGluZWFyR3JhZGllbnQyOTg3IiBncmFkaWVudFVuaXRzPSJ1c2VyU3BhY2VPblVz"
"ZSIgZ3JhZGllbnRUcmFuc2Zvcm09Im1hdHJpeCgwLjk2MTMxNjYyLDAsMCwwLjk2MTMxNjYxLC0x"
"NjAuNjE5NSw4My4yNjgyMjQpIiAvPg0KPGxpbmVhckdyYWRpZW50IHgxPSI2MDguNzE2ODYiIHkx"
"PSI5MzYuMTk4NTUiIHgyPSIyNTAuNTk4ODMiIHkyPSI1NjQuOTg0NSIgaWQ9ImxpbmVhckdyYWRp"
"ZW50NDQ5NCIgeGxpbms6aHJlZj0iI2xpbmVhckdyYWRpZW50NDQ4OCIgZ3JhZGllbnRVbml0cz0i"
"dXNlclNwYWNlT25Vc2UiIGdyYWRpZW50VHJhbnNmb3JtPSJtYXRyaXgoMC45NjEzMTY2MiwwLDAs"
"MC45NjEzMTY2MSwtMTYwLjYxOTUsODMuMjY4MjI0KSIgLz4NCjwvZGVmcz4NCjxtZXRhZGF0YSBp"
"ZD0ibWV0YWRhdGE3Ij48cmRmOlJERj48Y2M6V29yayByZGY6YWJvdXQ9IiI+PGRjOmZvcm1hdD5p"
"bWFnZS9zdmcreG1sPC9kYzpmb3JtYXQ+PGRjOnR5cGUgcmRmOnJlc291cmNlPSJodHRwOi8vcHVy"
"bC5vcmcvZGMvZGNtaXR5cGUvU3RpbGxJbWFnZSIgLz48ZGM6dGl0bGU+PC9kYzp0aXRsZT48L2Nj"
"Oldvcms+PC9yZGY6UkRGPjwvbWV0YWRhdGE+DQo8ZyB0cmFuc2Zvcm09InRyYW5zbGF0ZSgtOS4x"
"NTc2MDE0ZS02LC01NTIuMzYyMTcpIiBpZD0ibGF5ZXIxIj48cGF0aCBkPSJtIDQ5MC4zMjkxNyw4"
"MDIuNDA2NTcgYSAyNDAuMzI5MTYsMjQwLjMyOTE1IDAgMCAxIC00ODAuNjU4MzExOCwwIDI0MC4z"
"MjkxNiwyNDAuMzI5MTUgMCAxIDEgNDgwLjY1ODMxMTgsMCB6IiBpZD0icGF0aDI5ODUiIHN0eWxl"
"PSJmaWxsOnVybCgjbGluZWFyR3JhZGllbnQ1MjI1KTtmaWxsLW9wYWNpdHk6MTtzdHJva2U6dXJs"
"KCNsaW5lYXJHcmFkaWVudDQ0OTQpO3N0cm9rZS13aWR0aDoxOS4zNDE2OTAwNjtzdHJva2UtbWl0"
"ZXJsaW1pdDo0O3N0cm9rZS1vcGFjaXR5OjE7c3Ryb2tlLWRhc2hhcnJheTpub25lIiAvPjwvZz4N"
"Cjwvc3ZnPg0K";

// cleaned-up and base64-encoded version of resources/jog_ctrl_handle.svg
// (Original made with Inkscape)

const char* JogControl::Data::base64_handle =
"PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+DQo8"
"c3ZnDQogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iDQogICB4"
"bWxuczpjYz0iaHR0cDovL2NyZWF0aXZlY29tbW9ucy5vcmcvbnMjIg0KICAgeG1sbnM6cmRmPSJo"
"dHRwOi8vd3d3LnczLm9yZy8xOTk5LzAyLzIyLXJkZi1zeW50YXgtbnMjIg0KICAgeG1sbnM6c3Zn"
"PSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyINCiAgIHhtbG5zPSJodHRwOi8vd3d3LnczLm9y"
"Zy8yMDAwL3N2ZyINCiAgIHhtbG5zOnhsaW5rPSJodHRwOi8vd3d3LnczLm9yZy8xOTk5L3hsaW5r"
"Ig0KICAgdmVyc2lvbj0iMS4xIg0KICAgd2lkdGg9IjEwMCINCiAgIGhlaWdodD0iMTAwLjEzMjYz"
"Ig0KICAgaWQ9InN2ZzU0OTEiPg0KPGRlZnMgaWQ9ImRlZnM1NDkzIj48bGluZWFyR3JhZGllbnQg"
"aWQ9ImxpbmVhckdyYWRpZW50Mzc4OCI+PHN0b3AgaWQ9InN0b3AzNzkwIiBzdHlsZT0ic3RvcC1j"
"b2xvcjojZmZmZmZmO3N0b3Atb3BhY2l0eToxIiBvZmZzZXQ9IjAiIC8+PHN0b3AgaWQ9InN0b3Az"
"NzkyIiBzdHlsZT0ic3RvcC1jb2xvcjojY2ZjZmNmO3N0b3Atb3BhY2l0eToxIiBvZmZzZXQ9IjEi"
"IC8+PC9saW5lYXJHcmFkaWVudD4NCjxsaW5lYXJHcmFkaWVudCBpZD0ibGluZWFyR3JhZGllbnQ1"
"MjU2Ij48c3RvcCBpZD0ic3RvcDUyNTgiIHN0eWxlPSJzdG9wLWNvbG9yOiMzZDNkM2Q7c3RvcC1v"
"cGFjaXR5OjEiIG9mZnNldD0iMCIgLz48c3RvcCBpZD0ic3RvcDUyNjAiIHN0eWxlPSJzdG9wLWNv"
"bG9yOiNhMGEwYTA7c3RvcC1vcGFjaXR5OjEiIG9mZnNldD0iMSIgLz48L2xpbmVhckdyYWRpZW50"
"Pg0KPGZpbHRlciB4PSItMC4xMjYxOTQ5NyIgeT0iLTAuMjE3NjY3MDQiIHdpZHRoPSIxLjI1MjM4"
"OTkiIGhlaWdodD0iMS40MzUzMzQxIiBjb2xvci1pbnRlcnBvbGF0aW9uLWZpbHRlcnM9InNSR0Ii"
"IGlkPSJmaWx0ZXIzODI4Ij4NCjxmZUdhdXNzaWFuQmx1ciBzdGREZXZpYXRpb249IjIuNDczMDcw"
"NiIgaWQ9ImZlR2F1c3NpYW5CbHVyMzgzMCIgLz48L2ZpbHRlcj4NCjxsaW5lYXJHcmFkaWVudCB4"
"MT0iMzgxLjUiIHkxPSI1MjYuMzYyMTgiIHgyPSI0MzAuNSIgeTI9IjYxNS4zNjIxOCIgaWQ9Imxp"
"bmVhckdyYWRpZW50MzgzNiIgeGxpbms6aHJlZj0iI2xpbmVhckdyYWRpZW50NTI1NiIgZ3JhZGll"
"bnRVbml0cz0idXNlclNwYWNlT25Vc2UiIC8+DQo8cmFkaWFsR3JhZGllbnQgY3g9IjM5MS45MzQ4"
"NCIgY3k9IjU5NC40NzkxMyIgcj0iMjMuNSIgZng9IjM5MS45MzQ4NCIgZnk9IjU5NC40NzkxMyIg"
"aWQ9InJhZGlhbEdyYWRpZW50MzgzOCIgeGxpbms6aHJlZj0iI2xpbmVhckdyYWRpZW50Mzc4OCIg"
"Z3JhZGllbnRVbml0cz0idXNlclNwYWNlT25Vc2UiIGdyYWRpZW50VHJhbnNmb3JtPSJtYXRyaXgo"
"MC43ODU3MjYxLC0wLjM5ODA5NjY2LDAuMzA1NDExMzYsMC41NjcwNDU3MiwtOTYuMzUzNTE4LDQx"
"Mi4wNjkyNSkiIC8+DQo8L2RlZnM+DQo8bWV0YWRhdGEgaWQ9Im1ldGFkYXRhNTQ5NiI+PHJkZjpS"
"REY+PGNjOldvcmsgcmRmOmFib3V0PSIiPjxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9y"
"bWF0PjxkYzp0eXBlIHJkZjpyZXNvdXJjZT0iaHR0cDovL3B1cmwub3JnL2RjL2RjbWl0eXBlL1N0"
"aWxsSW1hZ2UiIC8+PGRjOnRpdGxlPjwvZGM6dGl0bGU+PC9jYzpXb3JrPjwvcmRmOlJERj48L21l"
"dGFkYXRhPg0KPGcgdHJhbnNmb3JtPSJ0cmFuc2xhdGUoLTAuOTk5OTkzNzgsLTk1Mi4yMzE2Myki"
"IGlkPSJsYXllcjEiPjxwYXRoIGQ9Im0gNDYyLDU3MS4zNjIxOCBhIDU3LjUsNTYgMCAxIDEgLTEx"
"NSwwIDU3LjUsNTYgMCAxIDEgMTE1LDAgeiIgdHJhbnNmb3JtPSJtYXRyaXgoMC44NTQ3MDA4NSww"
"LDAsMC44NzgzNTY0MSwtMjk0LjcyNjUsNTAwLjQyNzA1KSIgaWQ9InBhdGg1MjQ2IiBzdHlsZT0i"
"ZmlsbDp1cmwoI2xpbmVhckdyYWRpZW50MzgzNik7ZmlsbC1vcGFjaXR5OjE7c3Ryb2tlOiM0YTRh"
"NGE7c3Ryb2tlLXdpZHRoOjIuMzA4Mjc0OTg7c3Ryb2tlLW1pdGVybGltaXQ6NDtzdHJva2Utb3Bh"
"Y2l0eToxO3N0cm9rZS1kYXNoYXJyYXk6bm9uZSIgLz4NCjxwYXRoIGQ9Im0gNDE2LDU5MC44NjIx"
"OCBhIDIzLjUsMTIuNSAwIDEgMSAtNDcsMCAyMy41LDEyLjUgMCAxIDEgNDcsMCB6IiB0cmFuc2Zv"
"cm09Im1hdHJpeCgwLjkyMjg5MDM0LC0wLjIxODUzMDQ2LC0wLjAwMzQxMDg2LDEuMTUwNTU0OSwt"
"MjkzLjA2MDA1LDQzMi41NjU2OSkiIGlkPSJwYXRoNTI2NCIgc3R5bGU9ImZpbGw6dXJsKCNyYWRp"
"YWxHcmFkaWVudDM4MzgpO2ZpbGwtb3BhY2l0eToxO3N0cm9rZTpub25lO2ZpbHRlcjp1cmwoI2Zp"
"bHRlcjM4MjgpIiAvPjwvZz4NCjwvc3ZnPg0K";


JogControl::Data::Data(JogControl* parent) :
m_Parent(parent),
m_FgRenderer(0),
m_BgRenderer(0),
m_TotalSteps(12), m_CurrentStep(0),
m_CurrentAngle(0),m_LastAngle(0),m_StepAngle(120.0)
{
    m_FgRenderer = new QSvgRenderer(this);
    m_BgRenderer = new QSvgRenderer(this);
    m_StepAngle = 360.0 / m_TotalSteps;
}

JogControl::Data::~Data() {
    delete m_BgRenderer;
    m_BgRenderer = 0;
    delete m_FgRenderer;
    m_FgRenderer = 0;
    if(m_Parent) {
        disconnect();
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
}

bool JogControl::Data::LoadSvg() {
    bool result = true;
    result &= m_BgRenderer->load(QByteArray::fromBase64(QByteArray(JogControl::Data::base64_knob)));
    result &= m_FgRenderer->load(QByteArray::fromBase64(QByteArray(JogControl::Data::base64_handle)));
//    result &= m_BgRenderer->load(QString("ui/widgets/jog_ctrl/resources/jog_ctrl_knob.svg"));
//    result &= m_FgRenderer->load(QString("ui/widgets/jog_ctrl/resources/jog_ctrl_handle.svg"));
    return result;
}

double JogControl::Data::GetStepAngle() const {
    return m_StepAngle;
}

unsigned int JogControl::Data::GetStepCount() const {
    if(!this) { return 0; }
    return m_TotalSteps;
}

void JogControl::Data::SetStepCount(unsigned int numsteps) {
    if(!this) { return; }
    if(numsteps < MinimumSteps) {
        numsteps = MinimumSteps;
    }
    m_TotalSteps = numsteps;
    m_StepAngle = 360.0 / numsteps;
}

unsigned int JogControl::Data::GetCurrentStep() const {
    if(!this) { return 0; }
    return m_CurrentStep;
}

void JogControl::Data::SetCurrentStep(unsigned int stepnum) {
    if(!this) { return; }
    stepnum = stepnum % m_TotalSteps;
    m_CurrentStep = stepnum;
}

double JogControl::Data::GetCurrentAngle() const {
    if(!this) { return 0; }
    return m_CurrentAngle;
}

void JogControl::Data::SetCurrentAngle(double angle) {
    if(!this) { return; }
    if(angle > 360) {
        angle = fmod(angle,360.0);
    }
    m_CurrentAngle = angle;
    m_LastAngle = angle - fmod(angle,m_StepAngle);
}

int JogControl::Data::ChangeAngleAndProcess(double angle) {
    if(!this) { return 0; }
    int delta = 0;
    if(m_LastAngle != angle) {
        // Calculate delta based on the angle difference
        double angle_delta = (angle - m_LastAngle);
        while(angle_delta < -360) { angle_delta += 360; }
        while(angle_delta > 360) { angle_delta -= 360; }
        if(angle_delta > 180) { angle_delta -=360; } // Assume angles greater than 180 deg. are 360 - angle
        if(angle_delta < -180) { angle_delta += 360; }
        delta = round((angle_delta)/m_StepAngle);
        while(angle < 0) { angle += 360; }
        m_CurrentAngle = fmod(angle,360.0);
        RecalcStep();
        if(delta != 0) {
            // There was at least one click, process.

            m_LastAngle = m_CurrentAngle - fmod(m_CurrentAngle,m_StepAngle);
            // Calculate the next position of the dial
        }
        m_Parent->update();
    }
    return delta;
}

void JogControl::Data::RecalcStep() {
    if(m_TotalSteps < MinimumSteps) m_TotalSteps = MinimumSteps;
    if(m_CurrentAngle <0) {
        m_CurrentAngle  = 0;
    }
    if(m_CurrentAngle  > 360) {
        m_CurrentAngle  = fmod(m_CurrentAngle ,360.0);
    }
    m_CurrentStep = (m_CurrentAngle *m_TotalSteps)/360;
}

void JogControl::Data::RecalcAngle() {
    if(m_TotalSteps < MinimumSteps) m_TotalSteps = MinimumSteps;
    m_CurrentStep %= m_TotalSteps;
    m_CurrentAngle = (360.0*m_CurrentStep)/m_TotalSteps;
}

double JogControl::Data::CalculateAngle(double x, double y) const {
    double angle;
    if(fabs(x)<=0.1) {
        if(y <= 0) {
            angle = 0;
        } else {
            angle = 180;
        }
    } else if(fabs(y)<=0.1) {
        if(x <= 0) {
            angle = 270;
        } else {
            angle = 90;
        }
    } else {
        angle = atan2(x,-y)*180.0/3.141592653589793;
    }
    return angle;
}

JogControl::JogControl(QWidget *parent, Qt::WindowFlags f) :
m_Data(0)
{
    m_Data = new Data(this);
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    if(m_Data->LoadSvg()) {
    } else {
        QMessageBox::warning(this,"Error","Could not load the JogControl image data!");
    }
    setCursor(Qt::PointingHandCursor);
}

JogControl::~JogControl() {
    delete m_Data;
    m_Data = 0;
}

QSize JogControl::sizeHint() const
{
    QSize size = QSize(48,48);
    return size;
}

unsigned int JogControl::GetStepCount() const {
    return m_Data->GetStepCount();
}

void JogControl::SetStepCount(unsigned int numsteps) {
    m_Data->SetStepCount(numsteps);
}

unsigned int JogControl::GetCurrentStep() const {
    return m_Data->GetCurrentStep();
}

void JogControl::SetCurrentStep(unsigned int stepnum) {
    m_Data->SetCurrentStep(stepnum);
}

double JogControl::GetCurrentAngle() const {
    return m_Data->GetCurrentAngle();
}

void JogControl::SetCurrentAngle(double angle) {
    m_Data->SetCurrentAngle(angle);
}

void JogControl::mousePressEvent(QMouseEvent *event) {
    double angle = 0;
    QPointF curpos = event->posF();
    // (0,0) is at top-left.
    qreal x = curpos.x() - (0.5*width());
    qreal y = curpos.y() - (0.5*height());
    angle = m_Data->CalculateAngle(x,y);
    SetCurrentAngle(angle);
    update();
}

void JogControl::mouseMoveEvent(QMouseEvent *event) {
    double angle = 0;
    QPointF curpos = event->posF();
    // (0,0) is at top-left.
    qreal x = curpos.x() - (0.5*width());
    qreal y = curpos.y() - (0.5*height());
    angle = m_Data->CalculateAngle(x,y);
    int delta = m_Data->ChangeAngleAndProcess(angle);
    if(delta) {
        int i;
        if(delta > 0) {
            for(i = 0; i < delta; ++i) {
                emit JogStepUp();
            }
        } else {
            for(i = 0; i > delta; --i) {
                emit JogStepDown();
            }
        }
    }
}

void JogControl::paintEvent ( QPaintEvent * pe ) {
    int w = width();
    int h = height();
    QPainter painter(this);
    double x0 = (0.5*w);
    double y0 = (0.5*h);
    double x = 0;
    double y = 0;
    double angle = m_Data->m_CurrentAngle;
    if(angle == 0) {
        x = x0;
        y = y0 - 0.275*h;
    } else {
        double rad_angle = angle*3.141592653589793/180;
        x = x0 + (0.275*w)*sin(rad_angle);
        y = y0 - (0.275*h)*cos(rad_angle);
    }
    m_Data->m_BgRenderer->render(&painter);
    m_Data->m_FgRenderer->render(&painter,QRectF(x - 0.125*w,y - 0.125*h,0.25*w,0.25*h));
}

void JogControl::wheelEvent(QWheelEvent *event) {
    event->accept();
    double numDegrees = event->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    int delta = 0;
    if(numSteps != 0) {
        double angle = m_Data->GetCurrentAngle() + m_Data->GetStepAngle()*numSteps;
        delta = m_Data->ChangeAngleAndProcess(angle);
    }
    if(delta) {
        int i;
        if(delta > 0) {
            for(i = 0; i < delta; ++i) {
                emit JogStepUp();
            }
        } else {
            for(i = 0; i > delta; --i) {
                emit JogStepDown();
            }
        }
    }
}

#include "moc/jog_ctrl.moc.h"
