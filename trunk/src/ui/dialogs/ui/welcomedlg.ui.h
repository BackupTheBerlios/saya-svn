/********************************************************************************
** Form generated from reading ui file 'welcomedlgHf5162.ui'
**
** Created: Fri Dec 10 21:59:18 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef WELCOMEDLGHF5162_H
#define WELCOMEDLGHF5162_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

#include <ui/widgets/generic/pushbutton.h>
#include <ui/widgets/generic/textbrowser.h>

QT_BEGIN_NAMESPACE

class Ui_welcomedlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblRecentProjects;
    syTextBrowser *browser1;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_2;
    syPushButton *btnNew;
    QLabel *lblNew;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    syPushButton *btnOpen;
    QLabel *lblOpen;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_4;
    syPushButton *btnQuit;
    QLabel *lblQuit;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *welcomedlg)
    {
        if (welcomedlg->objectName().isEmpty())
            welcomedlg->setObjectName(QString::fromUtf8("welcomedlg"));
        welcomedlg->setWindowModality(Qt::NonModal);
        welcomedlg->resize(460, 455);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(welcomedlg->sizePolicy().hasHeightForWidth());
        welcomedlg->setSizePolicy(sizePolicy);
        welcomedlg->setMaximumSize(QSize(460, 480));
        verticalLayout = new QVBoxLayout(welcomedlg);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lblRecentProjects = new QLabel(welcomedlg);
        lblRecentProjects->setObjectName(QString::fromUtf8("lblRecentProjects"));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        lblRecentProjects->setFont(font);
        lblRecentProjects->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblRecentProjects);

        browser1 = new syTextBrowser(welcomedlg);
        browser1->setObjectName(QString::fromUtf8("browser1"));

        verticalLayout->addWidget(browser1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        btnNew = new syPushButton(welcomedlg);
        btnNew->setObjectName(QString::fromUtf8("btnNew"));
        btnNew->setMaximumSize(QSize(96, 96));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/new_video.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnNew->setIcon(icon);
        btnNew->setIconSize(QSize(96, 96));
        btnNew->setFlat(false);

        verticalLayout_2->addWidget(btnNew);

        lblNew = new QLabel(welcomedlg);
        lblNew->setObjectName(QString::fromUtf8("lblNew"));
        lblNew->setFont(font);
        lblNew->setTextFormat(Qt::PlainText);
        lblNew->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_2->addWidget(lblNew);


        horizontalLayout->addLayout(verticalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        btnOpen = new syPushButton(welcomedlg);
        btnOpen->setObjectName(QString::fromUtf8("btnOpen"));
        btnOpen->setMaximumSize(QSize(96, 96));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/folder_video.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnOpen->setIcon(icon1);
        btnOpen->setIconSize(QSize(96, 96));
        btnOpen->setFlat(false);

        verticalLayout_3->addWidget(btnOpen);

        lblOpen = new QLabel(welcomedlg);
        lblOpen->setObjectName(QString::fromUtf8("lblOpen"));
        lblOpen->setFont(font);
        lblOpen->setTextFormat(Qt::PlainText);
        lblOpen->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_3->addWidget(lblOpen);


        horizontalLayout->addLayout(verticalLayout_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        btnQuit = new syPushButton(welcomedlg);
        btnQuit->setObjectName(QString::fromUtf8("btnQuit"));
        btnQuit->setMaximumSize(QSize(96, 96));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnQuit->setIcon(icon2);
        btnQuit->setIconSize(QSize(96, 96));
        btnQuit->setFlat(false);

        verticalLayout_4->addWidget(btnQuit);

        lblQuit = new QLabel(welcomedlg);
        lblQuit->setObjectName(QString::fromUtf8("lblQuit"));
        lblQuit->setFont(font);
        lblQuit->setTextFormat(Qt::PlainText);
        lblQuit->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

        verticalLayout_4->addWidget(lblQuit);


        horizontalLayout->addLayout(verticalLayout_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(welcomedlg);

        QMetaObject::connectSlotsByName(welcomedlg);
    } // setupUi

    void retranslateUi(QWidget *welcomedlg)
    {
        welcomedlg->setWindowTitle(QApplication::translate("welcomedlg", "Welcome to Saya!", 0, QApplication::UnicodeUTF8));
        lblRecentProjects->setText(QApplication::translate("welcomedlg", "Recent Projects:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnNew->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnNew->setText(QString());
        lblNew->setText(QApplication::translate("welcomedlg", "Create new\n"
"Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnOpen->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnOpen->setText(QString());
        lblOpen->setText(QApplication::translate("welcomedlg", "Open existing\n"
"Project", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnQuit->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnQuit->setText(QString());
        lblQuit->setText(QApplication::translate("welcomedlg", "Quit", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(welcomedlg);
    } // retranslateUi

};

namespace Ui {
    class welcomedlg: public Ui_welcomedlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WELCOMEDLGHF5162_H
