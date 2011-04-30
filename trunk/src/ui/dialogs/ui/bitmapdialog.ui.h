/********************************************************************************
** Form generated from reading UI file 'bitmapdialogzm9623.ui'
**
** Created: Sat Apr 30 17:27:45 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BITMAPDIALOGZM9623_H
#define BITMAPDIALOGZM9623_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bitmap_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *image;

    void setupUi(QDialog *bitmap_dialog)
    {
        if (bitmap_dialog->objectName().isEmpty())
            bitmap_dialog->setObjectName(QString::fromUtf8("bitmap_dialog"));
        bitmap_dialog->setWindowModality(Qt::NonModal);
        bitmap_dialog->resize(447, 375);
        bitmap_dialog->setSizeGripEnabled(true);
        bitmap_dialog->setModal(false);
        verticalLayout = new QVBoxLayout(bitmap_dialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(bitmap_dialog);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 425, 353));
        scrollAreaWidgetContents->setAutoFillBackground(false);
        scrollAreaWidgetContents->setStyleSheet(QString::fromUtf8("background-color:#000;"));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        image = new QLabel(scrollAreaWidgetContents);
        image->setObjectName(QString::fromUtf8("image"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(image->sizePolicy().hasHeightForWidth());
        image->setSizePolicy(sizePolicy);
        image->setMinimumSize(QSize(16, 16));

        verticalLayout_2->addWidget(image);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(bitmap_dialog);

        QMetaObject::connectSlotsByName(bitmap_dialog);
    } // setupUi

    void retranslateUi(QDialog *bitmap_dialog)
    {
        bitmap_dialog->setWindowTitle(QApplication::translate("bitmap_dialog", "Image", 0, QApplication::UnicodeUTF8));
        image->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class bitmap_dialog: public Ui_bitmap_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BITMAPDIALOGZM9623_H
