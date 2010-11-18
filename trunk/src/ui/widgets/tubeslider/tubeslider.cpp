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

// ----------------------
// Begin TubeSlider::Data
// ----------------------

class TubeSlider::Data : public QObject {
    public:
        static const char* sliderbutton;
        Data(TubeSlider* parent);
        virtual ~Data();
        TubeSlider* m_Parent;
        QPixmap m_ButtonPixmap;
};

TubeSlider::Data::Data(TubeSlider* parent) :
m_Parent(parent)
{
    m_ButtonPixmap.loadFromData(QByteArray::fromBase64(sliderbutton));
}

TubeSlider::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;    }
}


// ----------------
// Begin TubeSlider
// ----------------

TubeSlider::TubeSlider(QWidget *parent) :
QSlider(Qt::Horizontal,parent),
m_Data(new Data(this))
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
    if(curpos >= x2) { curpos = x2; }
    painter.fillRect(x1+1,y1+1,curpos,hh,fillbrush);
    painter.fillRect(curpos,y1+1,x2 - 1,hh,emptybrush);
    if(curpos < 7) { curpos = 7; }
    painter.drawPixmap(curpos - 9, ((y1 + y2)/2) -8, m_Data->m_ButtonPixmap);
}

const char* TubeSlider::Data::sliderbutton =
"iVBORw0KGgoAAAANSUhEUgAAABIAAAASCAYAAABWzo5XAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A"
"/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB9oLEgQpMw4ubc0AAAOHSURBVDjL"
"RZTdT1RnEMZ/c8579ntZLOx2WVqIV160TRrq/2PSGBMr1htMbWsTe2HFSBMvAK3xfzImvVtCjCJL"
"AmWX5ZwDe96P6cWCztVMMvNMnmeejGxubgKwurrKZTz89XfixBCbGBEhiiK89xRFgbWWp0+ffurd"
"2trizp07GBFBVQXQn36+y2J3gZnmDM1Gk1qtSmRiNAScc2RZxvj0lMd/Pebj/ke2N7cBBFC5RH7+"
"/DlxHFMql5mfn9fZVktr1ZokpYTCWmxRcH52psfDoQxHQ/HOIwird1dZf/IEAfjxxg2+X1mh0WzS"
"6y1qo1FXVZUQAiLC5Pwcay0AkYhOnJWjwyPJ85x/377ln9eviQCWr17FGEN3YUHr9RpJkoi1ltFo"
"RJqmZFnG+RRMVSBJEtrttsZxzFdLSwDI2toa3W6XXq+n3W5Xq9WqjMdjDg4O6Pf7RFHEeDxGVfnu"
"229odzpqTCKVSkWP/juSvb09GewPMKVSmVarxZUrsyoiMhwOeffuHTs7Ozjn2NnZIc9z4jgG0IWF"
"Y1la+pokaUuz0dSZ5gzH5WOMMYZKuUKSlCSEQFEU5HnO7u4uk6IgzzK894QQ2Nv7IIPBgGazSaPR"
"pFIuy3Q2wXhvcd7hvSeKIlQD/X6f0WhEFEVYa3HOISIMBgfMzc2hqnjvQQQl4L3DiAAKGpSJnZBl"
"Oaenp1z4C1Wdiol8yp2bLp6WUweZOE4wJp5qcHGREALee5IkoVKp4JwjiiKcd4QQAKhWq5g4IhIh"
"igzGWsfZ+TmFLbRUKokxhmvXrmFMzP7+4FJkFJifnae32KPVmqFUKmGd0/PJZEqtKCacjE+oN+oy"
"98Wc1ut1uX79Or1ejzdv3pCmKc46arUqKz+saKfdlmqtRghBx+MTSbOUoiimBP989Ih2p8Py8rIa"
"Yy74K9Za0jSlmEyI45jZVkuTJBFEcM4xODiQ0XDILw8eYAB2+zuNufn59PDwUDqdL7VWq2sUReK9"
"o9FoEIJHgxJCQEAn1spwOBRVZf/DHp8lB7a3t1GFpJTQabe12ZzRcqkksYnx3mOtJc8yTbPsE0iS"
"JNy8eZO/NzYwL1++RFXl9u3bunrvHou9BWxRSKN+QrVWxRhDCGEKlOecpilnZznvP7znxdYLXr16"
"Jbdu3fr8Rp5tbHB/bQ2AP357SGxiIjO92OVjuzTn+vr6xcwz7q/dB+B/VJ7lMDYivOsAAAAASUVO"
"RK5CYII=";
