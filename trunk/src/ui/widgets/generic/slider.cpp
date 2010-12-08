/**************************************************************************************
 * Name:      slider.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "slider.h"
#include "moc/slider.moc.h"

sySlider::sySlider ( QWidget * parent) : QSlider(parent) { connectFakeSlots(); }
sySlider::sySlider ( Qt::Orientation orientation, QWidget * parent)  : QSlider(orientation, parent) { connectFakeSlots(); }

void sySlider::connectFakeSlots() {
    connect(this,SIGNAL(actionTriggered(int)),SLOT(sigactionTriggered(int)), Qt::DirectConnection);
    connect(this,SIGNAL(rangeChanged(int,int)),SLOT(sigrangeChanged(int,int)), Qt::DirectConnection);
    connect(this,SIGNAL(sliderMoved(int)),SLOT(sigsliderMoved(int)), Qt::DirectConnection);
    connect(this,SIGNAL(sliderPressed()),SLOT(sigsliderPressed()), Qt::DirectConnection);
    connect(this,SIGNAL(sliderReleased()),SLOT(sigsliderReleased()), Qt::DirectConnection);
    connect(this,SIGNAL(valueChanged(int)),SLOT(sigvalueChanged(int)), Qt::DirectConnection);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
