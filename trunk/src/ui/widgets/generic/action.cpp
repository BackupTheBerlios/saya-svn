/**************************************************************************************
 * Name:      action.cpp
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-12-07
 * Modified:  2010-12-07
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "action.h"
#include "moc/action.moc.h"

syAction::syAction ( QObject* parent) : QAction(parent) { connectFakeSlots(); }
syAction::syAction ( const QString & text, QObject* parent)  : QAction(text, parent) { connectFakeSlots(); }
syAction::syAction ( const QIcon & icon, const QString & text, QObject* parent) : QAction(icon, text, parent) { connectFakeSlots(); }

void syAction::connectFakeSlots() {
    connect(this,SIGNAL(changed()),this, SLOT(sigchanged()), Qt::DirectConnection);
    connect(this,SIGNAL(hovered()),this, SLOT(sighovered()), Qt::DirectConnection);
    connect(this,SIGNAL(toggled(bool)),this, SLOT(sigtoggled(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(triggered()),this, SLOT(sigtriggered()), Qt::DirectConnection);
    connect(this,SIGNAL(triggered(bool)),this, SLOT(sigtriggeredcheckable(bool)), Qt::DirectConnection);
}
