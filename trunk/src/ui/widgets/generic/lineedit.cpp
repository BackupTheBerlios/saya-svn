/**************************************************************************************
 * Name:      lineedit.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "lineedit.h"
#include "moc/lineedit.moc.h"

syLineEdit::syLineEdit ( QWidget * parent) : QLineEdit(parent) { connectFakeSlots(); }
syLineEdit::syLineEdit ( const QString & contents, QWidget * parent) : QLineEdit(contents, parent) { connectFakeSlots(); }

void syLineEdit::connectFakeSlots() {
    connect(this,SIGNAL(cursorPositionChanged(int, int)),SLOT(sigcursorPositionChanged(int, int)), Qt::DirectConnection);
    connect(this,SIGNAL(editingFinished()),SLOT(sigeditingFinished()), Qt::DirectConnection);
    connect(this,SIGNAL(returnPressed()),SLOT(sigreturnPressed()), Qt::DirectConnection);
    connect(this,SIGNAL(selectionChanged()),SLOT(sigselectionChanged()), Qt::DirectConnection);
    connect(this,SIGNAL(textChanged(const QString &)),SLOT(sigtextChanged(const QString &)), Qt::DirectConnection);
    connect(this,SIGNAL(textEdited(const QString &)),SLOT(sigtextEdited(const QString &)), Qt::DirectConnection);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
