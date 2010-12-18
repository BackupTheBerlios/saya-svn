/**************************************************************************************
 * Name:      dockwidget.cpp
 * Purpose:   Implementation of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "dockwidget.h"
#include "moc/dockwidget.moc.h"

syDockWidget::syDockWidget( QWidget * parent, Qt::WindowFlags f) : QDockWidget(parent, f) { connectFakeSlots(); }

void syDockWidget::connectFakeSlots() {
    connect(this,SIGNAL(allowedAreasChanged(Qt::DockWidgetAreas)),SLOT(sigallowedAreasChanged(Qt::DockWidgetAreas)), Qt::DirectConnection);
    connect(this,SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),SLOT(sigdockLocationChanged(Qt::DockWidgetArea)), Qt::DirectConnection);
    connect(this,SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)),SLOT(sigfeaturesChanged(QDockWidget::DockWidgetFeatures)), Qt::DirectConnection);
    connect(this,SIGNAL(topLevelChanged(bool)),SLOT(sigtopLevelChanged(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(visibſilityChanged(bool)),SLOT(sigvisibilityChanged(bool)), Qt::DirectConnection);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
