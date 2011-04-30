/**************************************************************************************
 * Name:      listwidget.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2011-04-20
 * Modified:  2011-04-20
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#include "listwidget.h"
#include "moc/listwidget.moc.h"

syListWidget::syListWidget(QWidget * parent) : QListWidget(parent) { connectFakeSlots(); }

void syListWidget::connectFakeSlots() {
    connect(this,SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),SLOT(sigcurrentItemChanged(QListWidgetItem *, QListWidgetItem *)));
    connect(this,SIGNAL(currentRowChanged(int)),SLOT(sigcurrentRowChanged(int)));
    connect(this,SIGNAL(currentTextChanged(const QString &)),SLOT(sigcurrentTextChanged(const QString &)));
    connect(this,SIGNAL(itemActivated(QListWidgetItem*)),SLOT(sigitemActivated(QListWidgetItem*)));
    connect(this,SIGNAL(itemChanged(QListWidgetItem*)),SLOT(sigitemChanged(QListWidgetItem*)));
    connect(this,SIGNAL(itemClicked(QListWidgetItem*)),SLOT(sigitemClicked(QListWidgetItem*)));
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem*)),SLOT(sigitemDoubleClicked(QListWidgetItem*)));
    connect(this,SIGNAL(itemEntered(QListWidgetItem*)),SLOT(sigitemEntered(QListWidgetItem*)));
    connect(this,SIGNAL(itemPressed(QListWidgetItem*)),SLOT(sigitemPressed(QListWidgetItem*)));
    connect(this,SIGNAL(itemSelectionChanged()),SLOT(sigitemSelectionChanged()));
    connect(this,SIGNAL(indexesMoved(const QModelIndexList &)),SLOT(sigindexesMoved(const QModelIndexList &)));
    connect(this,SIGNAL(activated(const QModelIndex &)),SLOT(sigactivated(const QModelIndex &)));
    connect(this,SIGNAL(clicked(const QModelIndex &)),SLOT(sigclicked(const QModelIndex &)));
    connect(this,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(sigdoubleClicked(const QModelIndex &)));
    connect(this,SIGNAL(entered(const QModelIndex &)),SLOT(sigentered(const QModelIndex &)));
    connect(this,SIGNAL(pressed(const QModelIndex &)),SLOT(sigpressed(const QModelIndex &)));
    connect(this,SIGNAL(viewportEntered()),SLOT(sigviewportEntered()));
    connect(this,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(sigcustomContextMenuRequested(const QPoint &)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed(QObject*)),SLOT(sigobjdestroyed(QObject*)), Qt::DirectConnection);
    connect(this,SIGNAL(destroyed()),SLOT(sigdestroyed()), Qt::DirectConnection);
}
