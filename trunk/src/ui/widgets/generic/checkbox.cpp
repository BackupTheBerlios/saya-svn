/**************************************************************************************
 * Name:      checkbox.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "checkbox.h"
#include "moc/checkbox.moc.h"

syCheckBox::syCheckBox ( QWidget * parent) : QCheckBox(parent) { connectFakeSlots(); }
syCheckBox::syCheckBox ( const QString & text, QWidget * parent)  : QCheckBox(text, parent) { connectFakeSlots(); }

void syCheckBox::connectFakeSlots() {
    connect(this,SIGNAL(stateChanged(int)),this, SLOT(sigstateChanged(int)), Qt::DirectConnection);
    connect(this,SIGNAL(pressed()),this, SLOT(sigpressed()), Qt::DirectConnection);
    connect(this,SIGNAL(released()),this, SLOT(sigreleased()), Qt::DirectConnection);
    connect(this,SIGNAL(toggled(bool)),this, SLOT(sigtoggled(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(clicked(bool)),this, SLOT(sigclickedcheckable(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(clicked()),this, SLOT(sigclicked()), Qt::DirectConnection);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
