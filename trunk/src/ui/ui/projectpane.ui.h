/********************************************************************************
** Form generated from reading ui file 'projectpanec26709.ui'
**
** Created: Wed Mar 4 18:01:22 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef PROJECTPANEC26709_H
#define PROJECTPANEC26709_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QListWidget>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_projectPane
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget1;
    QWidget *tabResources;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QTreeWidget *resourcesTree;
    QListWidget *resourcesList;
    QWidget *tabEffects;

    void setupUi(QDockWidget *projectPane)
    {
    if (projectPane->objectName().isEmpty())
        projectPane->setObjectName(QString::fromUtf8("projectPane"));
    projectPane->resize(307, 473);
    dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
    verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
    tabWidget1 = new QTabWidget(dockWidgetContents);
    tabWidget1->setObjectName(QString::fromUtf8("tabWidget1"));
    tabResources = new QWidget();
    tabResources->setObjectName(QString::fromUtf8("tabResources"));
    verticalLayout = new QVBoxLayout(tabResources);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    splitter = new QSplitter(tabResources);
    splitter->setObjectName(QString::fromUtf8("splitter"));
    splitter->setOrientation(Qt::Vertical);
    resourcesTree = new QTreeWidget(splitter);
    resourcesTree->setObjectName(QString::fromUtf8("resourcesTree"));
    resourcesTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    resourcesTree->setSelectionMode(QAbstractItemView::SingleSelection);
    resourcesTree->setSelectionBehavior(QAbstractItemView::SelectItems);
    resourcesTree->setRootIsDecorated(true);
    resourcesTree->setHeaderHidden(true);
    splitter->addWidget(resourcesTree);
    resourcesList = new QListWidget(splitter);
    resourcesList->setObjectName(QString::fromUtf8("resourcesList"));
    resourcesList->setViewMode(QListView::IconMode);
    splitter->addWidget(resourcesList);

    verticalLayout->addWidget(splitter);

    tabWidget1->addTab(tabResources, QString());
    tabEffects = new QWidget();
    tabEffects->setObjectName(QString::fromUtf8("tabEffects"));
    tabWidget1->addTab(tabEffects, QString());

    verticalLayout_2->addWidget(tabWidget1);

    projectPane->setWidget(dockWidgetContents);

    retranslateUi(projectPane);

    tabWidget1->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(projectPane);
    } // setupUi

    void retranslateUi(QDockWidget *projectPane)
    {
    projectPane->setWindowTitle(QApplication::translate("projectPane", "Project", 0, QApplication::UnicodeUTF8));
    resourcesTree->headerItem()->setText(0, QApplication::translate("projectPane", "1", 0, QApplication::UnicodeUTF8));
    tabWidget1->setTabText(tabWidget1->indexOf(tabResources), QApplication::translate("projectPane", "Resources", 0, QApplication::UnicodeUTF8));
    tabWidget1->setTabText(tabWidget1->indexOf(tabEffects), QApplication::translate("projectPane", "Effects", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(projectPane);
    } // retranslateUi

};

namespace Ui {
    class projectPane: public Ui_projectPane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PROJECTPANEC26709_H

