/**************************************************************************************
 * Name:      tubslider.cpp
 * Purpose:   Implementation of a Slider control with a prettier interface
 * Authors:   Ricardo Garcia
 * Created:   2010-11-16
 * Copyright: Ricardo Garcia
 * License:   LGPL license version 3 or later, with linking exception
 **************************************************************************************/

#include "tubeslider.h"
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QStyle>
#include <QtSvg>
#include <QSvgRenderer>

// ----------------------
// Begin TubeSlider::Data
// ----------------------

class TubeSlider::Data {
    public:
        static const char* svg_button;
        Data(TubeSlider* parent);
        virtual ~Data();
        void UpdateWidth();
        void ShowToolTip();
        void MoveToMouse();
    public:
        TubeSlider* m_Parent;
        QSvgRenderer m_ButtonSvg;
        int m_x1,m_x2,m_x;
        double m_CurrentTime;
        double m_StartTime, m_EndTime;
};

TubeSlider::Data::Data(TubeSlider* parent) :
m_Parent(parent),
m_CurrentTime(0),
m_StartTime(0),
m_EndTime(10)
{
    UpdateWidth();
    m_x = m_x1;
    m_ButtonSvg.load(QByteArray::fromBase64(QByteArray(svg_button)));

}

void TubeSlider::Data::UpdateWidth() {
    m_x1 = 9;
    if(m_x1 > m_Parent->width()) {
        m_x1 = m_Parent->width();
    }
    m_x2 = m_Parent->width() - 9;
    if(m_x2 < m_x1) {
        m_x2 = m_x1;
    }
}

TubeSlider::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
}

void TubeSlider::Data::ShowToolTip() {
    int local_x = m_Parent->mapFromGlobal(QCursor::pos()).x();
    if(local_x >= m_x1 && local_x <= m_x2 && m_x2 > m_x1) {
        double time = m_StartTime + ((double)(local_x - m_x1)*(m_EndTime - m_StartTime))/(m_x2 - m_x1);
        int secs = floor(time);
        int millisecs = floor((time-secs)*1000);
        int mins = secs / 60;secs %= 60;
        int hrs = mins / 60; hrs %= 60;
        QPoint tmppos = QPoint(QCursor::pos().x(),m_Parent->mapToGlobal(QPoint(0,-36)).y());
        QString s = QString("%1:%2:%3.%4").arg(hrs,2,10,QChar('0')).arg(mins,2,10,QChar('0')).arg(secs,2,10,QChar('0')).arg(millisecs,3,10,QChar('0'));
        QToolTip::showText (tmppos, s, m_Parent);
    }
}

void TubeSlider::Data::MoveToMouse() {
    int local_x = m_Parent->mapFromGlobal(QCursor::pos()).x();
    if(local_x >= m_x1 && local_x <= m_x2 && m_x2 > m_x1) {
        double time = m_StartTime + ((local_x - m_x1)*(m_EndTime - m_StartTime))/(m_x2 - m_x1);
        m_Parent->setCurrentTime(time);
        m_Parent->TimeChanged(m_CurrentTime);
        ShowToolTip();
    }
}


// ----------------
// Begin TubeSlider
// ----------------

TubeSlider::TubeSlider(QWidget *parent) :
syWidget(parent),
m_Data(new Data(this))
{
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed));
    setMouseTracking(true);
    setTimeRange(0,10);
}

TubeSlider::~TubeSlider() {
    delete m_Data;
    m_Data = 0;
}

double TubeSlider::getCurrentTime() {
    return m_Data->m_CurrentTime;
}

void TubeSlider::setTimeRange(double start, double finish) {
    if(start > finish) {
        double tmptime = finish;
        finish = start;
        start = tmptime;
    }
    m_Data->m_StartTime = start;
    m_Data->m_EndTime = finish;
    if(m_Data->m_CurrentTime < start || m_Data->m_CurrentTime > finish) {
        m_Data->m_CurrentTime = start;
        update();
        TimeChanged(m_Data->m_CurrentTime);
    } else {
        update();
    }
}

void TubeSlider::setCurrentTime(double time) {
    m_Data->m_CurrentTime = time;
    if(time < m_Data->m_StartTime) {
        m_Data->m_StartTime = time;
    }
    if(time > m_Data->m_EndTime) {
        m_Data->m_EndTime = time;
    }
    int curpos = m_Data->m_x1;
    if(m_Data->m_EndTime > m_Data->m_StartTime) {
        curpos += m_Data->m_CurrentTime*(m_Data->m_x2 - m_Data->m_x1)/(m_Data->m_EndTime - m_Data->m_StartTime);
    }
    if(curpos > m_Data->m_x2) { curpos = m_Data->m_x2; }
    if(curpos < m_Data->m_x1) { curpos = m_Data->m_x1; }
    if(m_Data->m_x != curpos) {
        m_Data->m_x = curpos;
        update();
    }
}

QSize TubeSlider::sizeHint() const {
    QSize size = QSize(75,20);
    return size;
}

void TubeSlider::paintEvent ( QPaintEvent * pe ) {
    m_Data->UpdateWidth();
    QColor topleftcolor(140,140,140);
    QColor bottomrightcolor(210,210,210);
    QPen pen;
    int x1 = m_Data->m_x1;
    int y1 = 5;
    int x2 = m_Data->m_x2;
    if(x2 < x1) { x2 = x1; }
    int y2 = 13;
    int hh = y2 - y1 - 1;

    QPainter painter(this);
    QLinearGradient emptygradient(x1,y1,x1,y2);
    emptygradient.setColorAt(0,QColor(180,180,180));
    emptygradient.setColorAt(1,QColor(204,204,204));
    QLinearGradient fillgradient(x1,y1,x1,y2);
    fillgradient.setColorAt(0,QColor(230,32,32));
    fillgradient.setColorAt(1,QColor(160,32,32));
    QBrush fillbrush(fillgradient);
    QBrush emptybrush(emptygradient);

    painter.setPen(pen);
    pen.setColor(topleftcolor);painter.setPen(pen);
    painter.drawLine(x1,y1,x1,y2);
    painter.drawLine(x1,y1,x2,y1);
    pen.setColor(bottomrightcolor);painter.setPen(pen);
    painter.drawLine(x2,y1,x2,y2);
    painter.drawLine(x2,y2,x1,y2);
    int curpos = x1;
    if(m_Data->m_EndTime > m_Data->m_StartTime) {
        curpos += m_Data->m_CurrentTime*(x2 - x1)/(m_Data->m_EndTime - m_Data->m_StartTime);
    }
    if(curpos > x2) { curpos = x2; }
    if(curpos < x1) { curpos = x1; }
    painter.fillRect(x1,y1+1,curpos - 9,hh,fillbrush);
    if(x2 > curpos + 9) {
        painter.fillRect(curpos,y1+1,x2 - curpos - 1,hh,emptybrush);
    }
    m_Data->m_ButtonSvg.render(&painter,QRectF(curpos -9, 0, 19, 19));
}

void TubeSlider::mouseMoveEvent ( QMouseEvent * event ) {
    m_Data->UpdateWidth();
    int local_x = mapFromGlobal(QCursor::pos()).x();
    int buttons = QApplication::mouseButtons();
    if(buttons != Qt::NoButton || (local_x >= m_Data->m_x - 3 && local_x <= m_Data->m_x + 3)) {
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
    m_Data->ShowToolTip();
    if(buttons != Qt::NoButton) {
        m_Data->MoveToMouse();
        update();
    }
}

void TubeSlider::mousePressEvent ( QMouseEvent * event ) {
    setCursor(Qt::PointingHandCursor);
    m_Data->UpdateWidth();
    m_Data->MoveToMouse();
    m_Data->ShowToolTip();
    update();
}

void TubeSlider::mouseReleaseEvent ( QMouseEvent * event ) {
    int local_x = mapFromGlobal(QCursor::pos()).x();
    if(local_x >= m_Data->m_x - 3 && local_x <= m_Data->m_x + 3) {
        setCursor(Qt::PointingHandCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

const char* TubeSlider::Data::svg_button =
"PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+Cjwh"
"LS0gQ3JlYXRlZCB3aXRoIElua3NjYXBlIChodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy8pIC0tPgoK"
"PHN2ZwogICB4bWxuczpkYz0iaHR0cDovL3B1cmwub3JnL2RjL2VsZW1lbnRzLzEuMS8iCiAgIHht"
"bG5zOmNjPSJodHRwOi8vY3JlYXRpdmVjb21tb25zLm9yZy9ucyMiCiAgIHhtbG5zOnJkZj0iaHR0"
"cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyIKICAgeG1sbnM6c3ZnPSJo"
"dHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIw"
"MDAvc3ZnIgogICB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayIKICAg"
"dmVyc2lvbj0iMS4xIgogICB3aWR0aD0iNDg1LjU3NjY2IgogICBoZWlnaHQ9IjQ5MS43NjY1NCIK"
"ICAgaWQ9InN2ZzIiPgogIDxkZWZzCiAgICAgaWQ9ImRlZnM0Ij4KICAgIDxsaW5lYXJHcmFkaWVu"
"dAogICAgICAgaWQ9ImxpbmVhckdyYWRpZW50NDQzMyI+CiAgICAgIDxzdG9wCiAgICAgICAgIGlk"
"PSJzdG9wNDQ0MSIKICAgICAgICAgc3R5bGU9InN0b3AtY29sb3I6Izc1NzU3NTtzdG9wLW9wYWNp"
"dHk6MSIKICAgICAgICAgb2Zmc2V0PSIwIiAvPgogICAgICA8c3RvcAogICAgICAgICBpZD0ic3Rv"
"cDQ0MzciCiAgICAgICAgIHN0eWxlPSJzdG9wLWNvbG9yOiNlM2UzZTM7c3RvcC1vcGFjaXR5OjEi"
"CiAgICAgICAgIG9mZnNldD0iMSIgLz4KICAgIDwvbGluZWFyR3JhZGllbnQ+CiAgICA8bGluZWFy"
"R3JhZGllbnQKICAgICAgIGlkPSJsaW5lYXJHcmFkaWVudDQ0MjEiPgogICAgICA8c3RvcAogICAg"
"ICAgICBpZD0ic3RvcDQ0MjciCiAgICAgICAgIHN0eWxlPSJzdG9wLWNvbG9yOiNjNGM0YzQ7c3Rv"
"cC1vcGFjaXR5OjEiCiAgICAgICAgIG9mZnNldD0iMCIgLz4KICAgICAgPHN0b3AKICAgICAgICAg"
"aWQ9InN0b3A0NDI5IgogICAgICAgICBzdHlsZT0ic3RvcC1jb2xvcjojNzU3NTc1O3N0b3Atb3Bh"
"Y2l0eToxIgogICAgICAgICBvZmZzZXQ9IjEiIC8+CiAgICA8L2xpbmVhckdyYWRpZW50PgogICAg"
"PGxpbmVhckdyYWRpZW50CiAgICAgICBpZD0ibGluZWFyR3JhZGllbnQ0Mjk1Ij4KICAgICAgPHN0"
"b3AKICAgICAgICAgaWQ9InN0b3A0MzM5IgogICAgICAgICBzdHlsZT0ic3RvcC1jb2xvcjojODg4"
"ODg4O3N0b3Atb3BhY2l0eToxIgogICAgICAgICBvZmZzZXQ9IjAiIC8+CiAgICAgIDxzdG9wCiAg"
"ICAgICAgIGlkPSJzdG9wNDM4NyIKICAgICAgICAgc3R5bGU9InN0b3AtY29sb3I6IzVhNWE1YTtz"
"dG9wLW9wYWNpdHk6MSIKICAgICAgICAgb2Zmc2V0PSIwLjY3NDcwODM3IiAvPgogICAgICA8c3Rv"
"cAogICAgICAgICBpZD0ic3RvcDQyOTkiCiAgICAgICAgIHN0eWxlPSJzdG9wLWNvbG9yOiM0NDQ0"
"NDQ7c3RvcC1vcGFjaXR5OjEiCiAgICAgICAgIG9mZnNldD0iMSIgLz4KICAgIDwvbGluZWFyR3Jh"
"ZGllbnQ+CiAgICA8ZmlsdGVyCiAgICAgICBjb2xvci1pbnRlcnBvbGF0aW9uLWZpbHRlcnM9InNS"
"R0IiCiAgICAgICBpZD0iZmlsdGVyMzkyNyI+CiAgICAgIDxmZUdhdXNzaWFuQmx1cgogICAgICAg"
"ICBzdGREZXZpYXRpb249IjEuMTE0Mjg1NyIKICAgICAgICAgaWQ9ImZlR2F1c3NpYW5CbHVyMzky"
"OSIgLz4KICAgIDwvZmlsdGVyPgogICAgPGZpbHRlcgogICAgICAgY29sb3ItaW50ZXJwb2xhdGlv"
"bi1maWx0ZXJzPSJzUkdCIgogICAgICAgaWQ9ImZpbHRlcjQzODkiPgogICAgICA8ZmVHYXVzc2lh"
"bkJsdXIKICAgICAgICAgaWQ9ImZlR2F1c3NpYW5CbHVyNDM5MSIKICAgICAgICAgc3RkRGV2aWF0"
"aW9uPSIzLjIzNzUiIC8+CiAgICA8L2ZpbHRlcj4KICAgIDxmaWx0ZXIKICAgICAgIGNvbG9yLWlu"
"dGVycG9sYXRpb24tZmlsdGVycz0ic1JHQiIKICAgICAgIGlkPSJmaWx0ZXI0NDUzIj4KICAgICAg"
"PGZlR2F1c3NpYW5CbHVyCiAgICAgICAgIGlkPSJmZUdhdXNzaWFuQmx1cjQ0NTUiCiAgICAgICAg"
"IHN0ZERldmlhdGlvbj0iMTEuMTQyODU5IiAvPgogICAgPC9maWx0ZXI+CiAgICA8bGluZWFyR3Jh"
"ZGllbnQKICAgICAgIHgxPSIzMTAuMjA2MTUiCiAgICAgICB5MT0iNTc0LjY0ODgiCiAgICAgICB4"
"Mj0iMzAyLjgyMTg0IgogICAgICAgeTI9Ii0zOC40MDE4NTkiCiAgICAgICBpZD0ibGluZWFyR3Jh"
"ZGllbnQ0NzEzIgogICAgICAgeGxpbms6aHJlZj0iI2xpbmVhckdyYWRpZW50NDQzMyIKICAgICAg"
"IGdyYWRpZW50VW5pdHM9InVzZXJTcGFjZU9uVXNlIgogICAgICAgZ3JhZGllbnRUcmFuc2Zvcm09"
"InRyYW5zbGF0ZSg3MS42OTYyMTcsMjU3LjE2NjA1KSIgLz4KICAgIDxyYWRpYWxHcmFkaWVudAog"
"ICAgICAgY3g9Ijc3Ljk2MDkwNyIKICAgICAgIGN5PSI1OTkuMTEyMjQiCiAgICAgICByPSIxMjku"
"NSIKICAgICAgIGZ4PSI3Ny45NjA5MDciCiAgICAgICBmeT0iNTk5LjExMjI0IgogICAgICAgaWQ9"
"InJhZGlhbEdyYWRpZW50NDczMCIKICAgICAgIHhsaW5rOmhyZWY9IiNsaW5lYXJHcmFkaWVudDQy"
"OTUiCiAgICAgICBncmFkaWVudFVuaXRzPSJ1c2VyU3BhY2VPblVzZSIKICAgICAgIGdyYWRpZW50"
"VHJhbnNmb3JtPSJtYXRyaXgoLTAuMzY0NjYyNjMsMC44NTMzODMwNiwtMC44Mjc5NjAzNSwtMC4z"
"NTM3OTkyMSw4MjMuMTIsNjI5LjE4MikiIC8+CiAgICA8bGluZWFyR3JhZGllbnQKICAgICAgIHgx"
"PSIyNzYuNTgwNDciCiAgICAgICB5MT0iNDQ1LjM4OTEzIgogICAgICAgeDI9IjI4MC41NTQ2MyIK"
"ICAgICAgIHkyPSIyNS4yMDE4ODEiCiAgICAgICBpZD0ibGluZWFyR3JhZGllbnQ0NzMyIgogICAg"
"ICAgeGxpbms6aHJlZj0iI2xpbmVhckdyYWRpZW50NDQyMSIKICAgICAgIGdyYWRpZW50VW5pdHM9"
"InVzZXJTcGFjZU9uVXNlIgogICAgICAgZ3JhZGllbnRUcmFuc2Zvcm09InRyYW5zbGF0ZSg5MC45"
"MTMxOTcsMzI2LjA5Mjk5KSIgLz4KICA8L2RlZnM+CiAgPG1ldGFkYXRhCiAgICAgaWQ9Im1ldGFk"
"YXRhNyI+CiAgICA8cmRmOlJERj4KICAgICAgPGNjOldvcmsKICAgICAgICAgcmRmOmFib3V0PSIi"
"PgogICAgICAgIDxkYzpmb3JtYXQ+aW1hZ2Uvc3ZnK3htbDwvZGM6Zm9ybWF0PgogICAgICAgIDxk"
"Yzp0eXBlCiAgICAgICAgICAgcmRmOnJlc291cmNlPSJodHRwOi8vcHVybC5vcmcvZGMvZGNtaXR5"
"cGUvU3RpbGxJbWFnZSIgLz4KICAgICAgICA8ZGM6dGl0bGU+PC9kYzp0aXRsZT4KICAgICAgPC9j"
"YzpXb3JrPgogICAgPC9yZGY6UkRGPgogIDwvbWV0YWRhdGE+CiAgPGcKICAgICBpZD0iZzQ3MzQi"
"PgogICAgPHBhdGgKICAgICAgIGQ9Im0gNTk3LjE0Mjg3LDUxMC45MzM2MiBhIDIyMS40Mjg1Nywy"
"MjQuMjg1ODggMCAxIDEgLTQ0Mi44NTcxNSwwIDIyMS40Mjg1NywyMjQuMjg1ODggMCAxIDEgNDQy"
"Ljg1NzE1LDAgeiIKICAgICAgIHRyYW5zZm9ybT0ibWF0cml4KDEuMDgzMzc5LDAsMCwxLjA4MzM3"
"NjYsLTE2NC4yNTI2NCwtMzA3LjY1MDI1KSIKICAgICAgIGlkPSJwYXRoMzc3Ni04IgogICAgICAg"
"c3R5bGU9ImZpbGw6dXJsKCNsaW5lYXJHcmFkaWVudDQ3MTMpO2ZpbGwtb3BhY2l0eToxO2ZpbGwt"
"cnVsZTpldmVub2RkO3N0cm9rZTpub25lO2ZpbHRlcjp1cmwoI2ZpbHRlcjM5MjcpIiAvPgogICAg"
"PHBhdGgKICAgICAgIGQ9Im0gNTk3LjE0Mjg3LDUxMC45MzM2MiBhIDIyMS40Mjg1NywyMjQuMjg1"
"OTEgMCAxIDEgLTQ0Mi44NTcxNSwwIDIyMS40Mjg1NywyMjQuMjg1OTEgMCAxIDEgNDQyLjg1NzE1"
"LDAgeiIKICAgICAgIHRyYW5zZm9ybT0ibWF0cml4KDAuODU0Mzc3MzQsMCwwLDAuODU0MzgxMDYs"
"LTc2LjgyNzA2NSwtMTkzLjkxNTIyKSIKICAgICAgIGlkPSJwYXRoMzc3NiIKICAgICAgIHN0eWxl"
"PSJmaWxsOnVybCgjbGluZWFyR3JhZGllbnQ0NzMyKTtmaWxsLW9wYWNpdHk6MTtmaWxsLXJ1bGU6"
"ZXZlbm9kZDtzdHJva2U6bm9uZTtmaWx0ZXI6dXJsKCNmaWx0ZXI0NDUzKSIgLz4KICAgIDxwYXRo"
"CiAgICAgICBkPSJtIDQ3Myw1MjkuODYyMTggYSAxMjkuNSwxMjkuNSAwIDEgMSAtMjU5LDAgMTI5"
"LjUsMTI5LjUgMCAxIDEgMjU5LDAgeiIKICAgICAgIHRyYW5zZm9ybT0ibWF0cml4KDAuODQzMzc0"
"MzcsMCwwLDAuODQzMzc0MzcsLTQ0LjM3MzI3MSwtMTkyLjExNzY3KSIKICAgICAgIGlkPSJwYXRo"
"NDI5MyIKICAgICAgIHN0eWxlPSJmaWxsOnVybCgjcmFkaWFsR3JhZGllbnQ0NzMwKTtmaWxsLW9w"
"YWNpdHk6MTtmaWxsLXJ1bGU6ZXZlbm9kZDtzdHJva2U6bm9uZTtmaWx0ZXI6dXJsKCNmaWx0ZXI0"
"Mzg5KSIgLz4KICA8L2c+Cjwvc3ZnPgo=";
