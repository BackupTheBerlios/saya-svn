/********************************************************************************
** Form generated from reading ui file 'newproject.ui'
**
** Created: Sat Apr 25 15:47:54 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef NEWPROJECTDLG_UI_H
#define NEWPROJECTDLG_UI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_new_project_dialog
{
public:
    QVBoxLayout *vboxLayout;
    QGroupBox *m_TitleAndFilename;
    QFormLayout *formLayout;
    QLabel *m_label;
    QLineEdit *idNewPrjTitle;
    QLabel *m_label_3;
    QLineEdit *idNewPrjFilename;
    QCheckBox *idNewPrjUnderscores;
    QLabel *m_label_2;
    QHBoxLayout *m_horizontalLayout_2;
    QPushButton *btnPrjLocation;
    QSpacerItem *m_horizontalSpacer;
    QGroupBox *m_ProjectSettings;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *m_horizontalLayout;
    QLabel *m_label_4;
    QComboBox *idNewPrjPresets;
    QPushButton *btnNewPrjSaveSettingsAs;
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
    QComboBox *idNewPrjAVSettings_interlacing;
    QLabel *m_label_8;
    QLineEdit *idNewPrjAVSettings_pixelaspect;
    QHBoxLayout *m_horizontalLayout_6;
    QLineEdit *idNewPrjAVSettings_width;
    QLabel *m_label_13;
    QLineEdit *idNewPrjAVSettings_height;
    QComboBox *idNewPrjAVSettings_fps;
    QGroupBox *m_ProjectSettingsAudio;
    QFormLayout *formLayout_4;
    QLabel *m_label_9;
    QComboBox *idNewPrjAVSettings_samplerate;
    QLabel *m_label_10;
    QComboBox *idNewPrjAVSettings_samplesize;
    QLabel *m_label_11;
    QComboBox *idNewPrjAVSettings_surround;
    QLabel *m_label_12;
    QSpinBox *idNewPrjAVSettings_channels;
    QGroupBox *m_ProjectSettingsAbout;
    QVBoxLayout *verticalLayout_5;
    QTextEdit *idNewPrjAVSettings_description;
    QWidget *m_EncoderSettingsTab;
    QWidget *m_DefaultSequenceSettingsTab;
    QHBoxLayout *m_horizontalLayout_3;
    QSpacerItem *m_horizontalSpacer_2;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *new_project_dialog)
    {
    if (new_project_dialog->objectName().isEmpty())
        new_project_dialog->setObjectName(QString::fromUtf8("new_project_dialog"));
    new_project_dialog->setEnabled(true);
    new_project_dialog->resize(625, 730);
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

    idNewPrjTitle = new QLineEdit(m_TitleAndFilename);
    idNewPrjTitle->setObjectName(QString::fromUtf8("idNewPrjTitle"));
    idNewPrjTitle->setFont(font1);

    formLayout->setWidget(0, QFormLayout::FieldRole, idNewPrjTitle);

    m_label_3 = new QLabel(m_TitleAndFilename);
    m_label_3->setObjectName(QString::fromUtf8("m_label_3"));
    m_label_3->setFont(font1);

    formLayout->setWidget(2, QFormLayout::LabelRole, m_label_3);

    idNewPrjFilename = new QLineEdit(m_TitleAndFilename);
    idNewPrjFilename->setObjectName(QString::fromUtf8("idNewPrjFilename"));
    idNewPrjFilename->setFont(font1);

    formLayout->setWidget(2, QFormLayout::FieldRole, idNewPrjFilename);

    idNewPrjUnderscores = new QCheckBox(m_TitleAndFilename);
    idNewPrjUnderscores->setObjectName(QString::fromUtf8("idNewPrjUnderscores"));
    idNewPrjUnderscores->setEnabled(true);
    idNewPrjUnderscores->setFont(font1);
    idNewPrjUnderscores->setChecked(true);

    formLayout->setWidget(3, QFormLayout::FieldRole, idNewPrjUnderscores);

    m_label_2 = new QLabel(m_TitleAndFilename);
    m_label_2->setObjectName(QString::fromUtf8("m_label_2"));
    m_label_2->setFont(font1);

    formLayout->setWidget(1, QFormLayout::LabelRole, m_label_2);

    m_horizontalLayout_2 = new QHBoxLayout();
    m_horizontalLayout_2->setObjectName(QString::fromUtf8("m_horizontalLayout_2"));
    btnPrjLocation = new QPushButton(m_TitleAndFilename);
    btnPrjLocation->setObjectName(QString::fromUtf8("btnPrjLocation"));
    btnPrjLocation->setEnabled(true);
    btnPrjLocation->setFont(font1);

    m_horizontalLayout_2->addWidget(btnPrjLocation);

    m_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_horizontalLayout_2->addItem(m_horizontalSpacer);


    formLayout->setLayout(1, QFormLayout::FieldRole, m_horizontalLayout_2);


    vboxLayout->addWidget(m_TitleAndFilename);

    m_ProjectSettings = new QGroupBox(new_project_dialog);
    m_ProjectSettings->setObjectName(QString::fromUtf8("m_ProjectSettings"));
    m_ProjectSettings->setFont(font);
    verticalLayout = new QVBoxLayout(m_ProjectSettings);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_horizontalLayout = new QHBoxLayout();
    m_horizontalLayout->setObjectName(QString::fromUtf8("m_horizontalLayout"));
    m_label_4 = new QLabel(m_ProjectSettings);
    m_label_4->setObjectName(QString::fromUtf8("m_label_4"));
    m_label_4->setFont(font1);

    m_horizontalLayout->addWidget(m_label_4);

    idNewPrjPresets = new QComboBox(m_ProjectSettings);
    idNewPrjPresets->setObjectName(QString::fromUtf8("idNewPrjPresets"));
    idNewPrjPresets->setEnabled(false);
    idNewPrjPresets->setFont(font1);

    m_horizontalLayout->addWidget(idNewPrjPresets);

    btnNewPrjSaveSettingsAs = new QPushButton(m_ProjectSettings);
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

    idNewPrjAVSettings_interlacing = new QComboBox(m_ProjectSettingsVideo);
    idNewPrjAVSettings_interlacing->setObjectName(QString::fromUtf8("idNewPrjAVSettings_interlacing"));
    idNewPrjAVSettings_interlacing->setFont(font1);

    formLayout_3->setWidget(2, QFormLayout::FieldRole, idNewPrjAVSettings_interlacing);

    m_label_8 = new QLabel(m_ProjectSettingsVideo);
    m_label_8->setObjectName(QString::fromUtf8("m_label_8"));
    m_label_8->setEnabled(true);
    m_label_8->setFont(font1);

    formLayout_3->setWidget(3, QFormLayout::LabelRole, m_label_8);

    idNewPrjAVSettings_pixelaspect = new QLineEdit(m_ProjectSettingsVideo);
    idNewPrjAVSettings_pixelaspect->setObjectName(QString::fromUtf8("idNewPrjAVSettings_pixelaspect"));
    idNewPrjAVSettings_pixelaspect->setFont(font1);

    formLayout_3->setWidget(3, QFormLayout::FieldRole, idNewPrjAVSettings_pixelaspect);

    m_horizontalLayout_6 = new QHBoxLayout();
    m_horizontalLayout_6->setObjectName(QString::fromUtf8("m_horizontalLayout_6"));
    idNewPrjAVSettings_width = new QLineEdit(m_ProjectSettingsVideo);
    idNewPrjAVSettings_width->setObjectName(QString::fromUtf8("idNewPrjAVSettings_width"));
    idNewPrjAVSettings_width->setFont(font1);

    m_horizontalLayout_6->addWidget(idNewPrjAVSettings_width);

    m_label_13 = new QLabel(m_ProjectSettingsVideo);
    m_label_13->setObjectName(QString::fromUtf8("m_label_13"));
    m_label_13->setFont(font1);

    m_horizontalLayout_6->addWidget(m_label_13);

    idNewPrjAVSettings_height = new QLineEdit(m_ProjectSettingsVideo);
    idNewPrjAVSettings_height->setObjectName(QString::fromUtf8("idNewPrjAVSettings_height"));
    idNewPrjAVSettings_height->setFont(font1);

    m_horizontalLayout_6->addWidget(idNewPrjAVSettings_height);


    formLayout_3->setLayout(0, QFormLayout::FieldRole, m_horizontalLayout_6);

    idNewPrjAVSettings_fps = new QComboBox(m_ProjectSettingsVideo);
    idNewPrjAVSettings_fps->setObjectName(QString::fromUtf8("idNewPrjAVSettings_fps"));
    idNewPrjAVSettings_fps->setFont(font1);
    idNewPrjAVSettings_fps->setMinimumContentsLength(0);

    formLayout_3->setWidget(1, QFormLayout::FieldRole, idNewPrjAVSettings_fps);


    m_horizontalLayout_5->addWidget(m_ProjectSettingsVideo);

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

    idNewPrjAVSettings_samplerate = new QComboBox(m_ProjectSettingsAudio);
    idNewPrjAVSettings_samplerate->setObjectName(QString::fromUtf8("idNewPrjAVSettings_samplerate"));
    idNewPrjAVSettings_samplerate->setFont(font1);

    formLayout_4->setWidget(0, QFormLayout::FieldRole, idNewPrjAVSettings_samplerate);

    m_label_10 = new QLabel(m_ProjectSettingsAudio);
    m_label_10->setObjectName(QString::fromUtf8("m_label_10"));
    m_label_10->setFont(font1);

    formLayout_4->setWidget(1, QFormLayout::LabelRole, m_label_10);

    idNewPrjAVSettings_samplesize = new QComboBox(m_ProjectSettingsAudio);
    idNewPrjAVSettings_samplesize->setObjectName(QString::fromUtf8("idNewPrjAVSettings_samplesize"));
    idNewPrjAVSettings_samplesize->setFont(font1);

    formLayout_4->setWidget(1, QFormLayout::FieldRole, idNewPrjAVSettings_samplesize);

    m_label_11 = new QLabel(m_ProjectSettingsAudio);
    m_label_11->setObjectName(QString::fromUtf8("m_label_11"));
    m_label_11->setFont(font1);

    formLayout_4->setWidget(2, QFormLayout::LabelRole, m_label_11);

    idNewPrjAVSettings_surround = new QComboBox(m_ProjectSettingsAudio);
    idNewPrjAVSettings_surround->setObjectName(QString::fromUtf8("idNewPrjAVSettings_surround"));
    idNewPrjAVSettings_surround->setFont(font1);

    formLayout_4->setWidget(2, QFormLayout::FieldRole, idNewPrjAVSettings_surround);

    m_label_12 = new QLabel(m_ProjectSettingsAudio);
    m_label_12->setObjectName(QString::fromUtf8("m_label_12"));
    m_label_12->setEnabled(true);
    m_label_12->setFont(font1);

    formLayout_4->setWidget(3, QFormLayout::LabelRole, m_label_12);

    idNewPrjAVSettings_channels = new QSpinBox(m_ProjectSettingsAudio);
    idNewPrjAVSettings_channels->setObjectName(QString::fromUtf8("idNewPrjAVSettings_channels"));
    idNewPrjAVSettings_channels->setFont(font1);
    idNewPrjAVSettings_channels->setMinimum(1);
    idNewPrjAVSettings_channels->setMaximum(32);
    idNewPrjAVSettings_channels->setValue(2);

    formLayout_4->setWidget(3, QFormLayout::FieldRole, idNewPrjAVSettings_channels);


    m_horizontalLayout_5->addWidget(m_ProjectSettingsAudio);


    m_verticalLayout_2->addLayout(m_horizontalLayout_5);

    m_ProjectSettingsAbout = new QGroupBox(m_AudioVideoSettingsTab);
    m_ProjectSettingsAbout->setObjectName(QString::fromUtf8("m_ProjectSettingsAbout"));
    m_ProjectSettingsAbout->setFont(font);
    verticalLayout_5 = new QVBoxLayout(m_ProjectSettingsAbout);
    verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
    idNewPrjAVSettings_description = new QTextEdit(m_ProjectSettingsAbout);
    idNewPrjAVSettings_description->setObjectName(QString::fromUtf8("idNewPrjAVSettings_description"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(idNewPrjAVSettings_description->sizePolicy().hasHeightForWidth());
    idNewPrjAVSettings_description->setSizePolicy(sizePolicy);
    idNewPrjAVSettings_description->setFont(font1);

    verticalLayout_5->addWidget(idNewPrjAVSettings_description);


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

    btnOk = new QPushButton(new_project_dialog);
    btnOk->setObjectName(QString::fromUtf8("btnOk"));

    m_horizontalLayout_3->addWidget(btnOk);

    btnCancel = new QPushButton(new_project_dialog);
    btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

    m_horizontalLayout_3->addWidget(btnCancel);


    vboxLayout->addLayout(m_horizontalLayout_3);


    retranslateUi(new_project_dialog);

    m_tabWidget->setCurrentIndex(0);
    idNewPrjAVSettings_fps->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(new_project_dialog);
    } // setupUi

    void retranslateUi(QDialog *new_project_dialog)
    {
    new_project_dialog->setWindowTitle(QApplication::translate("new_project_dialog", "Create new project", 0, QApplication::UnicodeUTF8));
    m_TitleAndFilename->setTitle(QApplication::translate("new_project_dialog", "Title and filename", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    m_label->setToolTip(QApplication::translate("new_project_dialog", "This will be the Project's official name.", "This will be the project's official name.", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    m_label->setText(QApplication::translate("new_project_dialog", "Title:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    idNewPrjTitle->setToolTip(QApplication::translate("new_project_dialog", "This will be the Project's official name.", "This will be the project's official name.", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    m_label_3->setToolTip(QApplication::translate("new_project_dialog", "The project's filename. Note that replacing spaces with underscores is recommended.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    m_label_3->setText(QApplication::translate("new_project_dialog", "Filename:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    idNewPrjFilename->setToolTip(QApplication::translate("new_project_dialog", "The project's filename. Note that replacing spaces with underscores is recommended.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    idNewPrjUnderscores->setText(QApplication::translate("new_project_dialog", "Auto-replace spaces with underscores (recommended)", 0, QApplication::UnicodeUTF8));

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
    m_ProjectSettings->setTitle(QApplication::translate("new_project_dialog", "Project settings", 0, QApplication::UnicodeUTF8));
    m_label_4->setText(QApplication::translate("new_project_dialog", "Predefined settings:", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    btnNewPrjSaveSettingsAs->setToolTip(QApplication::translate("new_project_dialog", "Saves the current project's settings as predefined settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    btnNewPrjSaveSettingsAs->setText(QApplication::translate("new_project_dialog", "Save custom settings as ...", 0, QApplication::UnicodeUTF8));
    m_ProjectSettingsVideo->setTitle(QApplication::translate("new_project_dialog", "Video", 0, QApplication::UnicodeUTF8));
    m_label_5->setText(QApplication::translate("new_project_dialog", "Size (w x h):", 0, QApplication::UnicodeUTF8));
    m_label_6->setText(QApplication::translate("new_project_dialog", "Framerate (f / s):", 0, QApplication::UnicodeUTF8));
    m_label_7->setText(QApplication::translate("new_project_dialog", "Interlacing:", 0, QApplication::UnicodeUTF8));
    idNewPrjAVSettings_interlacing->clear();
    idNewPrjAVSettings_interlacing->insertItems(0, QStringList()
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
    idNewPrjAVSettings_pixelaspect->setToolTip(QApplication::translate("new_project_dialog", "Sets the pixels' aspect ratio.\n"
"2.0: The pixels are twice as wide as they're tall.\n"
"0.5: The pixels are half as wide as they're tall.\n"
"1.0: The pixels are square.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    m_label_13->setText(QApplication::translate("new_project_dialog", "x", 0, QApplication::UnicodeUTF8));
    idNewPrjAVSettings_fps->clear();
    idNewPrjAVSettings_fps->insertItems(0, QStringList()
     << QApplication::translate("new_project_dialog", "23.97", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "24", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "25", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "29.997", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "30", 0, QApplication::UnicodeUTF8)
    );
    m_ProjectSettingsAudio->setTitle(QApplication::translate("new_project_dialog", "Audio", 0, QApplication::UnicodeUTF8));
    m_label_9->setText(QApplication::translate("new_project_dialog", "Sample rate (Hz):", 0, QApplication::UnicodeUTF8));
    idNewPrjAVSettings_samplerate->clear();
    idNewPrjAVSettings_samplerate->insertItems(0, QStringList()
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
    idNewPrjAVSettings_samplesize->clear();
    idNewPrjAVSettings_samplesize->insertItems(0, QStringList()
     << QApplication::translate("new_project_dialog", "16 bit", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "8 bit", 0, QApplication::UnicodeUTF8)
    );
    m_label_11->setText(QApplication::translate("new_project_dialog", "Surround:", 0, QApplication::UnicodeUTF8));
    idNewPrjAVSettings_surround->clear();
    idNewPrjAVSettings_surround->insertItems(0, QStringList()
     << QApplication::translate("new_project_dialog", "Mono", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "Stereo", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "5.1 Surround", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "7.1 Surround", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "10.2 Surround", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("new_project_dialog", "Custom", 0, QApplication::UnicodeUTF8)
    );
    m_label_12->setText(QApplication::translate("new_project_dialog", "Channels:", 0, QApplication::UnicodeUTF8));
    m_ProjectSettingsAbout->setTitle(QApplication::translate("new_project_dialog", "About these settings", 0, QApplication::UnicodeUTF8));
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

#endif // NEWPROJECTDLG_H
