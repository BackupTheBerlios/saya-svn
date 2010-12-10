/**************************************************************************************
 * Name:      spinbox.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "spinbox.h"
#include "moc/spinbox.moc.h"

sySpinBox::sySpinBox ( QWidget * parent) : QSpinBox(parent) { connectFakeSlots(); }

void sySpinBox::connectFakeSlots() {
    connect(this,SIGNAL(valueChanged(int i)),SLOT(sigvalueChanged_int(int)), Qt::DirectConnection);
    connect(this,SIGNAL(valueChanged(const QString &)),SLOT(sigvalueChanged_text(const QString &)), Qt::DirectConnection);
    connect(this,SIGNAL(editingFinished()),SLOT(sigeditingFinished()), Qt::DirectConnection);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
