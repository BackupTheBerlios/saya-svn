// **************************************************************************************
// * Name:      jog_ctrl.cpp
// * Purpose:   Implementation of class JogControl
// * Authors:   Ricardo Garcia
// * Created:   2009-02-06
// * Copyright: Ricardo Garcia
// * License:   LGPL license version 3 or later.
// **************************************************************************************/

#include "tubeslider.h"
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QStyle>

TubeSlider::TubeSlider(QWidget *parent) :
QSlider(Qt::Horizontal,parent)
{
}

TubeSlider::~TubeSlider() {
}

void TubeSlider::paintEvent ( QPaintEvent * pe ) {
    QColor topleftcolor(140,140,140);
    QColor bottomrightcolor(210,210,210);
    QPen pen;
    int x1 = 0;
    int y1 = height()*1/4;
    int x2 = width();
    int y2 = height()*3/4;
    int ww = x2 - x1 - 1;
    int hh = y2 - y1 - 1;

    QPainter painter(this);
    QLinearGradient emptygradient(x1,y1,x1,y2);
    emptygradient.setColorAt(0,QColor(180,180,180));
    emptygradient.setColorAt(1,QColor(204,204,204));
    QLinearGradient fillgradient(x1,y1,x1,y2);
    fillgradient.setColorAt(0,QColor(160,160,160));
    fillgradient.setColorAt(1,QColor(80,80,80));
    QBrush fillbrush(fillgradient);
    QBrush emptybrush(emptygradient);

    painter.setPen(pen);
    pen.setColor(topleftcolor);painter.setPen(pen);
    painter.drawLine(x1,y1,x1,y2);
    painter.drawLine(x1,y1,x2,y1);
    pen.setColor(bottomrightcolor);painter.setPen(pen);
    painter.drawLine(x2,y1,x2,y2);
    painter.drawLine(x2,y2,x1,y2);
    int curpos = QStyle::sliderPositionFromValue(minimum (),maximum(),value(),width());
    painter.fillRect(x1+1,y1+1,curpos,hh,fillbrush);
    painter.fillRect(curpos,y1+1,x2 - 1,hh,emptybrush);
}
