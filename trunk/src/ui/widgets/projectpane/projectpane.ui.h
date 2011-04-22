/********************************************************************************
** Form generated from reading ui file 'projectpaneEa7725.ui'
**
** Created: Thu Apr 21 21:04:52 2011
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef PROJECTPANEEA7725_H
#define PROJECTPANEEA7725_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <ui/widgets/generic/listwidget.h>
#include <ui/widgets/generic/widget.h>

QT_BEGIN_NAMESPACE

class Ui_projectPane
{
public:
    syWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTabWidget *tabWidget1;
    syWidget *tabResources;
    QVBoxLayout *verticalLayout_3;
    QTabWidget *tabWidget2;
    syWidget *tabSeq;
    QVBoxLayout *verticalLayout;
    syListWidget *listSeq;
    syWidget *tabVid;
    QVBoxLayout *verticalLayout_4;
    syListWidget *listVid;
    syWidget *tabImg;
    QVBoxLayout *verticalLayout_5;
    syListWidget *listImg;
    syWidget *tabSnd;
    QVBoxLayout *verticalLayout_6;
    syListWidget *listSnd;
    syWidget *tabOther;
    QVBoxLayout *verticalLayout_7;
    syListWidget *listOther;
    QWidget *tabEffects;

    void setupUi(QDockWidget *projectPane)
    {
        if (projectPane->objectName().isEmpty())
            projectPane->setObjectName(QString::fromUtf8("projectPane"));
        projectPane->resize(267, 536);
        dockWidgetContents = new syWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        tabWidget1 = new QTabWidget(dockWidgetContents);
        tabWidget1->setObjectName(QString::fromUtf8("tabWidget1"));
        tabResources = new syWidget();
        tabResources->setObjectName(QString::fromUtf8("tabResources"));
        verticalLayout_3 = new QVBoxLayout(tabResources);
        verticalLayout_3->setMargin(0);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tabWidget2 = new QTabWidget(tabResources);
        tabWidget2->setObjectName(QString::fromUtf8("tabWidget2"));
        QFont font;
        font.setPointSize(7);
        tabWidget2->setFont(font);
        tabWidget2->setLayoutDirection(Qt::LeftToRight);
        tabWidget2->setAutoFillBackground(false);
        tabWidget2->setTabPosition(QTabWidget::North);
        tabWidget2->setTabShape(QTabWidget::Rounded);
        tabWidget2->setElideMode(Qt::ElideNone);
        tabWidget2->setUsesScrollButtons(false);
        tabWidget2->setDocumentMode(true);
        tabWidget2->setTabsClosable(false);
        tabSeq = new syWidget();
        tabSeq->setObjectName(QString::fromUtf8("tabSeq"));
        verticalLayout = new QVBoxLayout(tabSeq);
#ifndef Q_OS_MAC
        verticalLayout->setMargin(9);
#endif
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        listSeq = new syListWidget(tabSeq);
        listSeq->setObjectName(QString::fromUtf8("listSeq"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(listSeq->sizePolicy().hasHeightForWidth());
        listSeq->setSizePolicy(sizePolicy);
        listSeq->setFrameShape(QFrame::StyledPanel);
        listSeq->setViewMode(QListView::IconMode);

        verticalLayout->addWidget(listSeq);

        tabWidget2->addTab(tabSeq, QString());
        tabVid = new syWidget();
        tabVid->setObjectName(QString::fromUtf8("tabVid"));
        verticalLayout_4 = new QVBoxLayout(tabVid);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        listVid = new syListWidget(tabVid);
        listVid->setObjectName(QString::fromUtf8("listVid"));
        sizePolicy.setHeightForWidth(listVid->sizePolicy().hasHeightForWidth());
        listVid->setSizePolicy(sizePolicy);
        listVid->setAcceptDrops(true);
        listVid->setFrameShape(QFrame::StyledPanel);
        listVid->setViewMode(QListView::IconMode);

        verticalLayout_4->addWidget(listVid);

        tabWidget2->addTab(tabVid, QString());
        tabImg = new syWidget();
        tabImg->setObjectName(QString::fromUtf8("tabImg"));
        verticalLayout_5 = new QVBoxLayout(tabImg);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        listImg = new syListWidget(tabImg);
        listImg->setObjectName(QString::fromUtf8("listImg"));
        sizePolicy.setHeightForWidth(listImg->sizePolicy().hasHeightForWidth());
        listImg->setSizePolicy(sizePolicy);
        listImg->setAcceptDrops(true);
        listImg->setFrameShape(QFrame::StyledPanel);
        listImg->setViewMode(QListView::IconMode);

        verticalLayout_5->addWidget(listImg);

        tabWidget2->addTab(tabImg, QString());
        tabSnd = new syWidget();
        tabSnd->setObjectName(QString::fromUtf8("tabSnd"));
        verticalLayout_6 = new QVBoxLayout(tabSnd);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        listSnd = new syListWidget(tabSnd);
        listSnd->setObjectName(QString::fromUtf8("listSnd"));
        sizePolicy.setHeightForWidth(listSnd->sizePolicy().hasHeightForWidth());
        listSnd->setSizePolicy(sizePolicy);
        listSnd->setAcceptDrops(true);
        listSnd->setFrameShape(QFrame::StyledPanel);
        listSnd->setViewMode(QListView::IconMode);

        verticalLayout_6->addWidget(listSnd);

        tabWidget2->addTab(tabSnd, QString());
        tabOther = new syWidget();
        tabOther->setObjectName(QString::fromUtf8("tabOther"));
        verticalLayout_7 = new QVBoxLayout(tabOther);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        listOther = new syListWidget(tabOther);
        listOther->setObjectName(QString::fromUtf8("listOther"));
        sizePolicy.setHeightForWidth(listOther->sizePolicy().hasHeightForWidth());
        listOther->setSizePolicy(sizePolicy);
        listOther->setAcceptDrops(true);
        listOther->setFrameShape(QFrame::StyledPanel);
        listOther->setViewMode(QListView::IconMode);

        verticalLayout_7->addWidget(listOther);

        tabWidget2->addTab(tabOther, QString());

        verticalLayout_3->addWidget(tabWidget2);

        tabWidget1->addTab(tabResources, QString());
        tabEffects = new QWidget();
        tabEffects->setObjectName(QString::fromUtf8("tabEffects"));
        tabWidget1->addTab(tabEffects, QString());

        verticalLayout_2->addWidget(tabWidget1);

        projectPane->setWidget(dockWidgetContents);

        retranslateUi(projectPane);

        tabWidget1->setCurrentIndex(0);
        tabWidget2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(projectPane);
    } // setupUi

    void retranslateUi(QDockWidget *projectPane)
    {
        projectPane->setWindowTitle(QApplication::translate("projectPane", "Project", 0, QApplication::UnicodeUTF8));
        tabWidget2->setStyleSheet(QString());
        listSeq->setStyleSheet(QString());
        tabWidget2->setTabText(tabWidget2->indexOf(tabSeq), QApplication::translate("projectPane", "Seq", 0, QApplication::UnicodeUTF8));
        tabWidget2->setTabToolTip(tabWidget2->indexOf(tabSeq), QApplication::translate("projectPane", "Sequences", 0, QApplication::UnicodeUTF8));
        listVid->setStyleSheet(QString());
        tabWidget2->setTabText(tabWidget2->indexOf(tabVid), QApplication::translate("projectPane", "Vid", 0, QApplication::UnicodeUTF8));
        tabWidget2->setTabToolTip(tabWidget2->indexOf(tabVid), QApplication::translate("projectPane", "Videos", 0, QApplication::UnicodeUTF8));
        listImg->setStyleSheet(QString());
        tabWidget2->setTabText(tabWidget2->indexOf(tabImg), QApplication::translate("projectPane", "Img", 0, QApplication::UnicodeUTF8));
        tabWidget2->setTabToolTip(tabWidget2->indexOf(tabImg), QApplication::translate("projectPane", "Images", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabSnd->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        listSnd->setStyleSheet(QString());
        tabWidget2->setTabText(tabWidget2->indexOf(tabSnd), QApplication::translate("projectPane", "Snd", 0, QApplication::UnicodeUTF8));
        tabWidget2->setTabToolTip(tabWidget2->indexOf(tabSnd), QApplication::translate("projectPane", "Sounds", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tabOther->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        listOther->setStyleSheet(QString());
        tabWidget2->setTabText(tabWidget2->indexOf(tabOther), QApplication::translate("projectPane", "Other", 0, QApplication::UnicodeUTF8));
        tabWidget2->setTabToolTip(tabWidget2->indexOf(tabOther), QApplication::translate("projectPane", "Other Resources", 0, QApplication::UnicodeUTF8));
        tabWidget1->setTabText(tabWidget1->indexOf(tabResources), QApplication::translate("projectPane", "Resources", 0, QApplication::UnicodeUTF8));
        tabWidget1->setTabText(tabWidget1->indexOf(tabEffects), QApplication::translate("projectPane", "Effects", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(projectPane);
    } // retranslateUi

};

namespace Ui {
    class projectPane: public Ui_projectPane {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PROJECTPANEEA7725_H
