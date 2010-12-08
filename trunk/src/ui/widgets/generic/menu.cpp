/**************************************************************************************
 * Name:      menu.cpp
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-12-07
 * Modified:  2010-12-07
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "menu.h"
#include "moc/menu.moc.h"

syMenu::syMenu ( QWidget* parent) : QMenu(parent) { connectFakeSlots(); }
syMenu::syMenu ( const QString & title, QWidget* parent)  : QMenu(title, parent) { connectFakeSlots(); }

void syMenu::connectFakeSlots() {
    void sigaboutToHide ();
    void sigaboutToShow ();
    void sighovered ( QAction * action );
    void sigtriggered ( QAction * action );

    connect(this,SIGNAL(aboutToHide()),this, SLOT(sigaboutToHide()), Qt::DirectConnection);
    connect(this,SIGNAL(aboutToShow()),this, SLOT(sigaboutToShow()), Qt::DirectConnection);
    connect(this,SIGNAL(hovered(QAction*)),this, SLOT(sighovered(QAction*)), Qt::DirectConnection);
    connect(this,SIGNAL(triggered(QAction*)),this, SLOT(sigtriggered(QAction*)), Qt::DirectConnection);
}
