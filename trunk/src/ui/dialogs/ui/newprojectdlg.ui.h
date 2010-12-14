/********************************************************************************
** Form generated from reading ui file 'newprojectjd6120.ui'
**
** Created: Mon Dec 13 22:47:39 2010
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef NEWPROJECTJD6120_H
#define NEWPROJECTJD6120_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <ui/widgets/generic/checkbox.h>
#include <ui/widgets/generic/combobox.h>
#include <ui/widgets/generic/lineedit.h>
#include <ui/widgets/generic/pushbutton.h>
#include <ui/widgets/generic/spinbox.h>
#include <ui/widgets/generic/textedit.h>

QT_BEGIN_NAMESPACE

class Ui_new_project_dialog
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *m_TitleAndFilename;
    QFormLayout *formLayout;
    QLabel *m_label;
    syLineEdit *edNewPrjTitle;
    QLabel *m_label_3;
    syLineEdit *edNewPrjFilename;
    syCheckBox *chkNewPrjUnderscores;
    QLabel *m_label_2;
    QHBoxLayout *m_horizontalLayout_2;
    syPushButton *btnPrjLocation;
    QSpacerItem *m_horizontalSpacer;
    QFrame *line;
    QGroupBox *m_ProjectSettings;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *m_horizontalLayout;
    QLabel *m_label_4;
    syComboBox *cmbNewPrjPresets;
    syPushButton *btnNewPrjSaveSettingsAs;
    QTabWidget *m_tabWidget;
    QWidget *m_AudioVideoSettingsTab;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *m_verticalLayout_2;
    QHBoxLayout *m_horizontalLayout_5;
    QGroupBox *m_ProjectSettingsVideo;
    QFormLayout *formLayout_3;
    QLabel *m_label_5;
    QLabel *m_label_6;
    QLabel *m_label_7;
    syComboBox *cmbNewPrjAVSettings_interlacing;
    QLabel *m_label_8;
    syLineEdit *edNewPrjAVSettings_pixelaspect;
    QHBoxLayout *m_horizontalLayout_6;
    syLineEdit *edNewPrjAVSettings_width;
    QLabel *m_label_13;
    syLineEdit *edNewPrjAVSettings_height;
    syComboBox *cmbNewPrjAVSettings_fps;
    QFrame *line_3;
    QGroupBox *m_ProjectSettingsAudio;
    QFormLayout *formLayout_4;
    QLabel *m_label_9;
    syComboBox *cmbNewPrjAVSettings_samplerate;
    QLabel *m_label_10;
    syComboBox *cmbNewPrjAVSettings_samplesize;
    QLabel *m_label_11;
    syComboBox *cmbNewPrjAVSettings_surround;
    QLabel *m_label_12;
    sySpinBox *spnNewPrjAVSettings_channels;
    QGroupBox *m_ProjectSettingsAbout;
    QVBoxLayout *verticalLayout_5;
    syTextEdit *txtNewPrjAVSettings_description;
    QWidget *m_EncoderSettingsTab;
    QWidget *m_DefaultSequenceSettingsTab;
    QHBoxLayout *m_horizontalLayout_3;
    QSpacerItem *m_horizontalSpacer_2;
    syPushButton *btnOk;
    syPushButton *btnCancel;

    void setupUi(QDialog *new_project_dialog)
    {
        if (new_project_dialog->objectName().isEmpty())
            new_project_dialog->setObjectName(QString::fromUtf8("new_project_dialog"));
        new_project_dialog->setEnabled(true);
        new_project_dialog->resize(679, 730);
        vboxLayout = new QVBoxLayout(new_project_dialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        m_TitleAndFilename = new QGroupBox(new_project_dialog);
        m_TitleAndFilename->setObjectName(QString::fromUtf8("m_TitleAndFilename"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        m_TitleAndFilename->setFont(font);
        formLayout = new QFormLayout(m_TitleAndFilename);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        m_label = new QLabel(m_TitleAndFilename);
        m_label->setObjectName(QString::fromUtf8("m_label"));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        m_label->setFont(font1);

        formLayout->setWidget(0, QFormLayout::LabelRole, m_label);

        edNewPrjTitle = new syLineEdit(m_TitleAndFilename);
        edNewPrjTitle->setObjectName(QString::fromUtf8("edNewPrjTitle"));
        edNewPrjTitle->setFont(font1);

        formLayout->setWidget(0, QFormLayout::FieldRole, edNewPrjTitle);

        m_label_3 = new QLabel(m_TitleAndFilename);
        m_label_3->setObjectName(QString::fromUtf8("m_label_3"));
        m_label_3->setFont(font1);

        formLayout->setWidget(2, QFormLayout::LabelRole, m_label_3);

        edNewPrjFilename = new syLineEdit(m_TitleAndFilename);
        edNewPrjFilename->setObjectName(QString::fromUtf8("edNewPrjFilename"));
        edNewPrjFilename->setFont(font1);

        formLayout->setWidget(2, QFormLayout::FieldRole, edNewPrjFilename);

        chkNewPrjUnderscores = new syCheckBox(m_TitleAndFilename);
        chkNewPrjUnderscores->setObjectName(QString::fromUtf8("chkNewPrjUnderscores"));
        chkNewPrjUnderscores->setEnabled(true);
        chkNewPrjUnderscores->setFont(font1);
        chkNewPrjUnderscores->setChecked(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, chkNewPrjUnderscores);

        m_label_2 = new QLabel(m_TitleAndFilename);
        m_label_2->setObjectName(QString::fromUtf8("m_label_2"));
        m_label_2->setFont(font1);

        formLayout->setWidget(1, QFormLayout::LabelRole, m_label_2);

        m_horizontalLayout_2 = new QHBoxLayout();
        m_horizontalLayout_2->setObjectName(QString::fromUtf8("m_horizontalLayout_2"));
        btnPrjLocation = new syPushButton(m_TitleAndFilename);
        btnPrjLocation->setObjectName(QString::fromUtf8("btnPrjLocation"));
        btnPrjLocation->setEnabled(true);
        btnPrjLocation->setFont(font1);

        m_horizontalLayout_2->addWidget(btnPrjLocation);

        m_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        m_horizontalLayout_2->addItem(m_horizontalSpacer);


        formLayout->setLayout(1, QFormLayout::FieldRole, m_horizontalLayout_2);


        vboxLayout->addWidget(m_TitleAndFilename);

        line = new QFrame(new_project_dialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line);

        m_ProjectSettings = new QGroupBox(new_project_dialog);
        m_ProjectSettings->setObjectName(QString::fromUtf8("m_ProjectSettings"));
        m_ProjectSettings->setFont(font);
        m_ProjectSettings->setFlat(false);
        verticalLayout = new QVBoxLayout(m_ProjectSettings);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        m_horizontalLayout = new QHBoxLayout();
        m_horizontalLayout->setObjectName(QString::fromUtf8("m_horizontalLayout"));
        m_label_4 = new QLabel(m_ProjectSettings);
        m_label_4->setObjectName(QString::fromUtf8("m_label_4"));
        m_label_4->setFont(font1);

        m_horizontalLayout->addWidget(m_label_4);

        cmbNewPrjPresets = new syComboBox(m_ProjectSettings);
        cmbNewPrjPresets->setObjectName(QString::fromUtf8("cmbNewPrjPresets"));
        cmbNewPrjPresets->setEnabled(false);
        cmbNewPrjPresets->setFont(font1);

        m_horizontalLayout->addWidget(cmbNewPrjPresets);

        btnNewPrjSaveSettingsAs = new syPushButton(m_ProjectSettings);
        btnNewPrjSaveSettingsAs->setObjectName(QString::fromUtf8("btnNewPrjSaveSettingsAs"));
        btnNewPrjSaveSettingsAs->setEnabled(true);
        btnNewPrjSaveSettingsAs->setFont(font1);

        m_horizontalLayout->addWidget(btnNewPrjSaveSettingsAs);


        verticalLayout->addLayout(m_horizontalLayout);

        m_tabWidget = new QTabWidget(m_ProjectSettings);
        m_tabWidget->setObjectName(QString::fromUtf8("m_tabWidget"));
        m_tabWidget->setEnabled(true);
        m_tabWidget->setFont(font1);
        m_tabWidget->setTabPosition(QTabWidget::North);
        m_AudioVideoSettingsTab = new QWidget();
        m_AudioVideoSettingsTab->setObjectName(QString::fromUtf8("m_AudioVideoSettingsTab"));
        m_AudioVideoSettingsTab->setEnabled(true);
        verticalLayout_3 = new QVBoxLayout(m_AudioVideoSettingsTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        m_verticalLayout_2 = new QVBoxLayout();
        m_verticalLayout_2->setObjectName(QString::fromUtf8("m_verticalLayout_2"));
        m_horizontalLayout_5 = new QHBoxLayout();
        m_horizontalLayout_5->setObjectName(QString::fromUtf8("m_horizontalLayout_5"));
        m_ProjectSettingsVideo = new QGroupBox(m_AudioVideoSettingsTab);
        m_ProjectSettingsVideo->setObjectName(QString::fromUtf8("m_ProjectSettingsVideo"));
        m_ProjectSettingsVideo->setFont(font);
        formLayout_3 = new QFormLayout(m_ProjectSettingsVideo);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        m_label_5 = new QLabel(m_ProjectSettingsVideo);
        m_label_5->setObjectName(QString::fromUtf8("m_label_5"));
        m_label_5->setFont(font1);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, m_label_5);

        m_label_6 = new QLabel(m_ProjectSettingsVideo);
        m_label_6->setObjectName(QString::fromUtf8("m_label_6"));
        m_label_6->setFont(font1);

        formLayout_3->setWidget(1, QFormLayout::LabelRole, m_label_6);

        m_label_7 = new QLabel(m_ProjectSettingsVideo);
        m_label_7->setObjectName(QString::fromUtf8("m_label_7"));
        m_label_7->setFont(font1);

        formLayout_3->setWidget(2, QFormLayout::LabelRole, m_label_7);

        cmbNewPrjAVSettings_interlacing = new syComboBox(m_ProjectSettingsVideo);
        cmbNewPrjAVSettings_interlacing->setObjectName(QString::fromUtf8("cmbNewPrjAVSettings_interlacing"));
        cmbNewPrjAVSettings_interlacing->setFont(font1);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, cmbNewPrjAVSettings_interlacing);

        m_label_8 = new QLabel(m_ProjectSettingsVideo);
        m_label_8->setObjectName(QString::fromUtf8("m_label_8"));
        m_label_8->setEnabled(true);
        m_label_8->setFont(font1);

        formLayout_3->setWidget(3, QFormLayout::LabelRole, m_label_8);

        edNewPrjAVSettings_pixelaspect = new syLineEdit(m_ProjectSettingsVideo);
        edNewPrjAVSettings_pixelaspect->setObjectName(QString::fromUtf8("edNewPrjAVSettings_pixelaspect"));
        edNewPrjAVSettings_pixelaspect->setFont(font1);

        formLayout_3->setWidget(3, QFormLayout::FieldRole, edNewPrjAVSettings_pixelaspect);

        m_horizontalLayout_6 = new QHBoxLayout();
        m_horizontalLayout_6->setObjectName(QString::fromUtf8("m_horizontalLayout_6"));
        edNewPrjAVSettings_width = new syLineEdit(m_ProjectSettingsVideo);
        edNewPrjAVSettings_width->setObjectName(QString::fromUtf8("edNewPrjAVSettings_width"));
        edNewPrjAVSettings_width->setFont(font1);

        m_horizontalLayout_6->addWidget(edNewPrjAVSettings_width);

        m_label_13 = new QLabel(m_ProjectSettingsVideo);
        m_label_13->setObjectName(QString::fromUtf8("m_label_13"));
        m_label_13->setFont(font1);

        m_horizontalLayout_6->addWidget(m_label_13);

        edNewPrjAVSettings_height = new syLineEdit(m_ProjectSettingsVideo);
        edNewPrjAVSettings_height->setObjectName(QString::fromUtf8("edNewPrjAVSettings_height"));
        edNewPrjAVSettings_height->setFont(font1);

        m_horizontalLayout_6->addWidget(edNewPrjAVSettings_height);


        formLayout_3->setLayout(0, QFormLayout::FieldRole, m_horizontalLayout_6);

        cmbNewPrjAVSettings_fps = new syComboBox(m_ProjectSettingsVideo);
        cmbNewPrjAVSettings_fps->setObjectName(QString::fromUtf8("cmbNewPrjAVSettings_fps"));
        cmbNewPrjAVSettings_fps->setFont(font1);
        cmbNewPrjAVSettings_fps->setMinimumContentsLength(0);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, cmbNewPrjAVSettings_fps);


        m_horizontalLayout_5->addWidget(m_ProjectSettingsVideo);

        line_3 = new QFrame(m_AudioVideoSettingsTab);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        m_horizontalLayout_5->addWidget(line_3);

        m_ProjectSettingsAudio = new QGroupBox(m_AudioVideoSettingsTab);
        m_ProjectSettingsAudio->setObjectName(QString::fromUtf8("m_ProjectSettingsAudio"));
        m_ProjectSettingsAudio->setFont(font);
        formLayout_4 = new QFormLayout(m_ProjectSettingsAudio);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        m_label_9 = new QLabel(m_ProjectSettingsAudio);
        m_label_9->setObjectName(QString::fromUtf8("m_label_9"));
        m_label_9->setFont(font1);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, m_label_9);

        cmbNewPrjAVSettings_samplerate = new syComboBox(m_ProjectSettingsAudio);
        cmbNewPrjAVSettings_samplerate->setObjectName(QString::fromUtf8("cmbNewPrjAVSettings_samplerate"));
        cmbNewPrjAVSettings_samplerate->setFont(font1);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, cmbNewPrjAVSettings_samplerate);

        m_label_10 = new QLabel(m_ProjectSettingsAudio);
        m_label_10->setObjectName(QString::fromUtf8("m_label_10"));
        m_label_10->setFont(font1);

        formLayout_4->setWidget(1, QFormLayout::LabelRole, m_label_10);

        cmbNewPrjAVSettings_samplesize = new syComboBox(m_ProjectSettingsAudio);
        cmbNewPrjAVSettings_samplesize->setObjectName(QString::fromUtf8("cmbNewPrjAVSettings_samplesize"));
        cmbNewPrjAVSettings_samplesize->setFont(font1);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, cmbNewPrjAVSettings_samplesize);

        m_label_11 = new QLabel(m_ProjectSettingsAudio);
        m_label_11->setObjectName(QString::fromUtf8("m_label_11"));
        m_label_11->setFont(font1);

        formLayout_4->setWidget(2, QFormLayout::LabelRole, m_label_11);

        cmbNewPrjAVSettings_surround = new syComboBox(m_ProjectSettingsAudio);
        cmbNewPrjAVSettings_surround->setObjectName(QString::fromUtf8("cmbNewPrjAVSettings_surround"));
        cmbNewPrjAVSettings_surround->setFont(font1);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, cmbNewPrjAVSettings_surround);

        m_label_12 = new QLabel(m_ProjectSettingsAudio);
        m_label_12->setObjectName(QString::fromUtf8("m_label_12"));
        m_label_12->setEnabled(true);
        m_label_12->setFont(font1);

        formLayout_4->setWidget(3, QFormLayout::LabelRole, m_label_12);

        spnNewPrjAVSettings_channels = new sySpinBox(m_ProjectSettingsAudio);
        spnNewPrjAVSettings_channels->setObjectName(QString::fromUtf8("spnNewPrjAVSettings_channels"));
        spnNewPrjAVSettings_channels->setFont(font1);
        spnNewPrjAVSettings_channels->setMinimum(1);
        spnNewPrjAVSettings_channels->setMaximum(32);
        spnNewPrjAVSettings_channels->setValue(2);

        formLayout_4->setWidget(3, QFormLayout::FieldRole, spnNewPrjAVSettings_channels);


        m_horizontalLayout_5->addWidget(m_ProjectSettingsAudio);


        m_verticalLayout_2->addLayout(m_horizontalLayout_5);

        m_ProjectSettingsAbout = new QGroupBox(m_AudioVideoSettingsTab);
        m_ProjectSettingsAbout->setObjectName(QString::fromUtf8("m_ProjectSettingsAbout"));
        m_ProjectSettingsAbout->setFont(font);
        verticalLayout_5 = new QVBoxLayout(m_ProjectSettingsAbout);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        txtNewPrjAVSettings_description = new syTextEdit(m_ProjectSettingsAbout);
        txtNewPrjAVSettings_description->setObjectName(QString::fromUtf8("txtNewPrjAVSettings_description"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(txtNewPrjAVSettings_description->sizePolicy().hasHeightForWidth());
        txtNewPrjAVSettings_description->setSizePolicy(sizePolicy);
        txtNewPrjAVSettings_description->setFont(font1);

        verticalLayout_5->addWidget(txtNewPrjAVSettings_description);


        m_verticalLayout_2->addWidget(m_ProjectSettingsAbout);


        verticalLayout_3->addLayout(m_verticalLayout_2);

        m_tabWidget->addTab(m_AudioVideoSettingsTab, QString());
        m_EncoderSettingsTab = new QWidget();
        m_EncoderSettingsTab->setObjectName(QString::fromUtf8("m_EncoderSettingsTab"));
        m_EncoderSettingsTab->setEnabled(true);
        m_tabWidget->addTab(m_EncoderSettingsTab, QString());
        m_DefaultSequenceSettingsTab = new QWidget();
        m_DefaultSequenceSettingsTab->setObjectName(QString::fromUtf8("m_DefaultSequenceSettingsTab"));
        m_DefaultSequenceSettingsTab->setEnabled(true);
        m_tabWidget->addTab(m_DefaultSequenceSettingsTab, QString());

        verticalLayout->addWidget(m_tabWidget);


        vboxLayout->addWidget(m_ProjectSettings);

        m_horizontalLayout_3 = new QHBoxLayout();
        m_horizontalLayout_3->setObjectName(QString::fromUtf8("m_horizontalLayout_3"));
        m_horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        m_horizontalLayout_3->addItem(m_horizontalSpacer_2);

        btnOk = new syPushButton(new_project_dialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        m_horizontalLayout_3->addWidget(btnOk);

        btnCancel = new syPushButton(new_project_dialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        m_horizontalLayout_3->addWidget(btnCancel);


        vboxLayout->addLayout(m_horizontalLayout_3);

#ifndef QT_NO_SHORTCUT
        m_label->setBuddy(edNewPrjTitle);
        m_label_3->setBuddy(edNewPrjFilename);
        m_label_2->setBuddy(btnPrjLocation);
        m_label_5->setBuddy(edNewPrjAVSettings_width);
        m_label_6->setBuddy(cmbNewPrjAVSettings_fps);
        m_label_7->setBuddy(cmbNewPrjAVSettings_interlacing);
        m_label_8->setBuddy(edNewPrjAVSettings_pixelaspect);
        m_label_9->setBuddy(cmbNewPrjAVSettings_samplerate);
        m_label_10->setBuddy(cmbNewPrjAVSettings_samplesize);
        m_label_11->setBuddy(cmbNewPrjAVSettings_surround);
        m_label_12->setBuddy(spnNewPrjAVSettings_channels);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(edNewPrjTitle, btnPrjLocation);
        QWidget::setTabOrder(btnPrjLocation, edNewPrjFilename);
        QWidget::setTabOrder(edNewPrjFilename, chkNewPrjUnderscores);
        QWidget::setTabOrder(chkNewPrjUnderscores, cmbNewPrjPresets);
        QWidget::setTabOrder(cmbNewPrjPresets, btnNewPrjSaveSettingsAs);
        QWidget::setTabOrder(btnNewPrjSaveSettingsAs, m_tabWidget);
        QWidget::setTabOrder(m_tabWidget, edNewPrjAVSettings_width);
        QWidget::setTabOrder(edNewPrjAVSettings_width, edNewPrjAVSettings_height);
        QWidget::setTabOrder(edNewPrjAVSettings_height, cmbNewPrjAVSettings_fps);
        QWidget::setTabOrder(cmbNewPrjAVSettings_fps, cmbNewPrjAVSettings_interlacing);
        QWidget::setTabOrder(cmbNewPrjAVSettings_interlacing, edNewPrjAVSettings_pixelaspect);
        QWidget::setTabOrder(edNewPrjAVSettings_pixelaspect, cmbNewPrjAVSettings_samplerate);
        QWidget::setTabOrder(cmbNewPrjAVSettings_samplerate, cmbNewPrjAVSettings_samplesize);
        QWidget::setTabOrder(cmbNewPrjAVSettings_samplesize, cmbNewPrjAVSettings_surround);
        QWidget::setTabOrder(cmbNewPrjAVSettings_surround, spnNewPrjAVSettings_channels);
        QWidget::setTabOrder(spnNewPrjAVSettings_channels, txtNewPrjAVSettings_description);
        QWidget::setTabOrder(txtNewPrjAVSettings_description, btnOk);
        QWidget::setTabOrder(btnOk, btnCancel);

        retranslateUi(new_project_dialog);

        m_tabWidget->setCurrentIndex(0);
        cmbNewPrjAVSettings_fps->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(new_project_dialog);
    } // setupUi

    void retranslateUi(QDialog *new_project_dialog)
    {
        new_project_dialog->setWindowTitle(QApplication::translate("new_project_dialog", "Create new project", 0, QApplication::UnicodeUTF8));
        new_project_dialog->setStyleSheet(QApplication::translate("new_project_dialog", "QGroupBox { border:none; padding-top: 12px;  }", 0, QApplication::UnicodeUTF8));
        m_TitleAndFilename->setTitle(QApplication::translate("new_project_dialog", "Title and filename:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_label->setToolTip(QApplication::translate("new_project_dialog", "This will be the Project's official name.", "This will be the project's official name.", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_label->setText(QApplication::translate("new_project_dialog", "Title:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        edNewPrjTitle->setToolTip(QApplication::translate("new_project_dialog", "This will be the Project's official name.", "This will be the project's official name.", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        m_label_3->setToolTip(QApplication::translate("new_project_dialog", "The project's filename. Note that replacing spaces with underscores is recommended.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_label_3->setText(QApplication::translate("new_project_dialog", "Filename:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        edNewPrjFilename->setToolTip(QApplication::translate("new_project_dialog", "The project's filename. Note that replacing spaces with underscores is recommended.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chkNewPrjUnderscores->setText(QApplication::translate("new_project_dialog", "Auto-replace spaces with underscores (recommended)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_label_2->setToolTip(QApplication::translate("new_project_dialog", "The directory where the project file will be created.\n"
"For portability purposes, all resources (video, audio, etc.) SHOULD be beneath this directory.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_label_2->setText(QApplication::translate("new_project_dialog", "Location:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnPrjLocation->setToolTip(QApplication::translate("new_project_dialog", "The directory where the project file will be created.\n"
"For portability purposes, all resources (video, audio, etc.) SHOULD be beneath this directory.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnPrjLocation->setText(QApplication::translate("new_project_dialog", "Browse...", 0, QApplication::UnicodeUTF8));
        m_ProjectSettings->setStyleSheet(QString());
        m_ProjectSettings->setTitle(QApplication::translate("new_project_dialog", "Project settings:", 0, QApplication::UnicodeUTF8));
        m_label_4->setText(QApplication::translate("new_project_dialog", "Predefined settings:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnNewPrjSaveSettingsAs->setToolTip(QApplication::translate("new_project_dialog", "Saves the current project's settings as predefined settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnNewPrjSaveSettingsAs->setText(QApplication::translate("new_project_dialog", "Save custom settings as ...", 0, QApplication::UnicodeUTF8));
        m_ProjectSettingsVideo->setTitle(QApplication::translate("new_project_dialog", "Video:", 0, QApplication::UnicodeUTF8));
        m_label_5->setText(QApplication::translate("new_project_dialog", "Size (w x h):", 0, QApplication::UnicodeUTF8));
        m_label_6->setText(QApplication::translate("new_project_dialog", "Framerate (f / s):", 0, QApplication::UnicodeUTF8));
        m_label_7->setText(QApplication::translate("new_project_dialog", "Interlacing:", 0, QApplication::UnicodeUTF8));
        cmbNewPrjAVSettings_interlacing->clear();
        cmbNewPrjAVSettings_interlacing->insertItems(0, QStringList()
         << QApplication::translate("new_project_dialog", "Progressive (non interlaced)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "Interlaced; bottom field first", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "interlaced; top field first", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        m_label_8->setToolTip(QApplication::translate("new_project_dialog", "Sets the pixels' aspect ratio.\n"
"2.0: The pixels are twice as wide as they're tall.\n"
"0.5: The pixels are half as wide as they're tall.\n"
"1.0: The pixels are square.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_label_8->setText(QApplication::translate("new_project_dialog", "Pixel aspect:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        edNewPrjAVSettings_pixelaspect->setToolTip(QApplication::translate("new_project_dialog", "Sets the pixels' aspect ratio.\n"
"2.0: The pixels are twice as wide as they're tall.\n"
"0.5: The pixels are half as wide as they're tall.\n"
"1.0: The pixels are square.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_label_13->setText(QApplication::translate("new_project_dialog", "x", 0, QApplication::UnicodeUTF8));
        cmbNewPrjAVSettings_fps->clear();
        cmbNewPrjAVSettings_fps->insertItems(0, QStringList()
         << QApplication::translate("new_project_dialog", "23.97", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "24", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "25", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "29.997", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "30", 0, QApplication::UnicodeUTF8)
        );
        m_ProjectSettingsAudio->setTitle(QApplication::translate("new_project_dialog", "Audio:", 0, QApplication::UnicodeUTF8));
        m_label_9->setText(QApplication::translate("new_project_dialog", "Sample rate (Hz):", 0, QApplication::UnicodeUTF8));
        cmbNewPrjAVSettings_samplerate->clear();
        cmbNewPrjAVSettings_samplerate->insertItems(0, QStringList()
         << QApplication::translate("new_project_dialog", "8000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "11025", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "16000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "22050", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "24000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "32000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "44100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "48000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "96000", 0, QApplication::UnicodeUTF8)
        );
        m_label_10->setText(QApplication::translate("new_project_dialog", "Sample size:", 0, QApplication::UnicodeUTF8));
        cmbNewPrjAVSettings_samplesize->clear();
        cmbNewPrjAVSettings_samplesize->insertItems(0, QStringList()
         << QApplication::translate("new_project_dialog", "16 bit", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "8 bit", 0, QApplication::UnicodeUTF8)
        );
        m_label_11->setText(QApplication::translate("new_project_dialog", "Surround:", 0, QApplication::UnicodeUTF8));
        cmbNewPrjAVSettings_surround->clear();
        cmbNewPrjAVSettings_surround->insertItems(0, QStringList()
         << QApplication::translate("new_project_dialog", "Mono", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "Stereo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "5.1 Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "7.1 Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "10.2 Surround", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("new_project_dialog", "Custom", 0, QApplication::UnicodeUTF8)
        );
        m_label_12->setText(QApplication::translate("new_project_dialog", "Channels:", 0, QApplication::UnicodeUTF8));
        m_ProjectSettingsAbout->setTitle(QApplication::translate("new_project_dialog", "About these settings:", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(m_AudioVideoSettingsTab), QApplication::translate("new_project_dialog", "A/V Settings", 0, QApplication::UnicodeUTF8));
        m_tabWidget->setTabText(m_tabWidget->indexOf(m_EncoderSettingsTab), QApplication::translate("new_project_dialog", "Encoder Settings", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_DefaultSequenceSettingsTab->setToolTip(QApplication::translate("new_project_dialog", "When you create a new sequence, it will adopt these settings.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_tabWidget->setTabText(m_tabWidget->indexOf(m_DefaultSequenceSettingsTab), QApplication::translate("new_project_dialog", "Default Sequence Settings", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("new_project_dialog", "Ok", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("new_project_dialog", "Cancel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(new_project_dialog);
    } // retranslateUi

};

namespace Ui {
    class new_project_dialog: public Ui_new_project_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // NEWPROJECTJD6120_H
