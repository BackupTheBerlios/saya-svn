/**************************************************************************************
 * Name:      combobox.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "combobox.h"
#include "moc/combobox.moc.h"

syComboBox::syComboBox ( QWidget * parent) : QComboBox(parent) { connectFakeSlots(); }

void syComboBox::connectFakeSlots() {
    connect(this,SIGNAL(activated(int)),SLOT(sigactivated_index(int)),Qt::DirectConnection);
    connect(this,SIGNAL(activated(const QString &)),SLOT(sigactivated_text(const QString &)),Qt::DirectConnection);
    connect(this,SIGNAL(currentIndexChanged(int)),SLOT(sigcurrentIndexChanged_index(int)),Qt::DirectConnection);
    connect(this,SIGNAL(currentIndexChanged(const QString &)),SLOT(sigcurrentIndexChanged_text(const QString &)),Qt::DirectConnection);
    connect(this,SIGNAL(editTextChanged(const QString &)),SLOT(sigeditTextChanged(const QString &)),Qt::DirectConnection);
    connect(this,SIGNAL(highlighted(int)),SLOT(sighighlighted_index(int)),Qt::DirectConnection);
    connect(this,SIGNAL(highlighted(const QString &)),SLOT(sighighlighted_text(const QString &)),Qt::DirectConnection);

    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
