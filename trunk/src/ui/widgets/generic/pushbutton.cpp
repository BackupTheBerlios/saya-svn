/**************************************************************************************
 * Name:      generic.h
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "pushbutton.h"
#include "moc/pushbutton.moc.h"

syPushButton::syPushButton ( QWidget * parent) : QPushButton(parent) { connectFakeSlots(); }
syPushButton::syPushButton ( const QString & text, QWidget * parent)  : QPushButton(text, parent) { connectFakeSlots(); }
syPushButton::syPushButton ( const QIcon & icon, const QString & text, QWidget * parent) : QPushButton(icon, text, parent) { connectFakeSlots(); }

void syPushButton::connectFakeSlots() {
    connect(this,SIGNAL(pressed()),this, SLOT(sigpressed()), Qt::DirectConnection);
    connect(this,SIGNAL(released()),this, SLOT(sigreleased()), Qt::DirectConnection);
    connect(this,SIGNAL(toggled(bool)),this, SLOT(sigtoggled(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(clicked(bool)),this, SLOT(sigclicked(bool)), Qt::DirectConnection);
}
