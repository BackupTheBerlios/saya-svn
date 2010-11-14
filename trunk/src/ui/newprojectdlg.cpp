/***************************************************************
 * Name:      newprojectdlg.cpp
 * Purpose:   Implements New Project Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-29
 * Modified:  2010-11-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "../saya/projectmanager.h"
#include "../saya/presetmanager.h"
#include "../saya/core/systring.h"
#include "../saya/timeline/sstrvector.h"
#include "../saya/timeline/smapxstr.h"

#include "newprojectdlg.h"
#include "ui/newprojectdlg.ui.h"

#include <QFileDialog>
#include <QInputDialog>

class NewProjectDlg::Data : public QObject {
    Q_OBJECT
    public:
        Data(NewProjectDlg* parent);
        void ConnectSignalsAndSlots();
        virtual ~Data();
        bool LoadPresets();

        NewProjectDlg* m_Parent;
        Ui::new_project_dialog* m_Ui;

    public slots:
        void OnAVSettingsUpdateUI();
        void OnPrjPresetsChanged(int i);
        void OnPrjSaveSettingsAsClicked();
        void OnBrowseDir();
        void OnPressOk();
        void OnEditingFinished();
};

NewProjectDlg::Data::Data(NewProjectDlg* parent) :
m_Parent(parent),
m_Ui(new Ui::new_project_dialog) {
    m_Ui->setupUi(parent);
    ConnectSignalsAndSlots();
    //adding validators for QLineEdit fields
    //TODO: set specific ranges for them
    m_Ui->idNewPrjAVSettings_width->setValidator(new QIntValidator(m_Ui->idNewPrjAVSettings_width));
    m_Ui->idNewPrjAVSettings_height->setValidator(new QIntValidator(m_Ui->idNewPrjAVSettings_height));
    m_Ui->idNewPrjAVSettings_pixelaspect->setValidator(new QDoubleValidator(m_Ui->idNewPrjAVSettings_pixelaspect));
    m_Ui->idNewPrjPresets->setEnabled(true);
    LoadPresets();
}

void NewProjectDlg::Data::ConnectSignalsAndSlots() {
    connect(m_Ui->idNewPrjPresets, SIGNAL(activated(int)), this, SLOT(OnPrjPresetsChanged(int)));

    connect(m_Ui->btnPrjLocation, SIGNAL(clicked()), this, SLOT(OnBrowseDir()));
    connect(m_Ui->btnNewPrjSaveSettingsAs, SIGNAL(clicked()), this, SLOT(OnPrjSaveSettingsAsClicked()));
    connect(m_Ui->btnOk, SIGNAL(clicked()), this, SLOT(OnPressOk()));
    connect(m_Ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(m_Ui->idNewPrjFilename, SIGNAL(editingFinished()), this, SLOT(OnEditingFinished()));

    connect(m_Ui->idNewPrjAVSettings_width, SIGNAL(editingFinished()), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_height, SIGNAL(editingFinished()), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_fps, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_interlacing, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_pixelaspect, SIGNAL(editingFinished()), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_samplerate, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_samplesize, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_surround, SIGNAL(currentIndexChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_channels, SIGNAL(valueChanged(int)), this, SLOT(OnAVSettingsUpdateUI()));
    connect(m_Ui->idNewPrjAVSettings_description, SIGNAL(textChanged()), this, SLOT(OnAVSettingsUpdateUI()));
}

NewProjectDlg::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
    delete m_Ui;
    m_Ui = 0;
}

bool NewProjectDlg::Data::LoadPresets()
{
    unsigned int i, imax;
    SStringVector presets = ProjectManager::Get()->GetPresets()->GetPresets();

    m_Ui->idNewPrjPresets->addItem("<Custom>");
    for (i=0, imax=presets.size(); i<imax; i++) {
        m_Ui->idNewPrjPresets->addItem(presets[i]);
    }
    return true;
}

NewProjectDlg::NewProjectDlg(QWidget *parent) : QDialog(parent)
{
    m_Data = new Data(this);
}

NewProjectDlg::~NewProjectDlg()
{
    delete m_Data;
    m_Data = 0;
}

void NewProjectDlg::Data::OnAVSettingsUpdateUI()
{
}

void NewProjectDlg::Data::OnPrjPresetsChanged(int i)
{
    if (i < 0) return;

    if (i == 0) {
        m_Ui->idNewPrjAVSettings_width->setEnabled(true);
        m_Ui->idNewPrjAVSettings_height->setEnabled(true);
        m_Ui->idNewPrjAVSettings_fps->setEnabled(true);
        m_Ui->idNewPrjAVSettings_interlacing->setEnabled(true);
        m_Ui->idNewPrjAVSettings_pixelaspect->setEnabled(true);
        m_Ui->idNewPrjAVSettings_samplerate->setEnabled(true);
        m_Ui->idNewPrjAVSettings_samplesize->setEnabled(true);
        m_Ui->idNewPrjAVSettings_surround->setEnabled(true);
        m_Ui->idNewPrjAVSettings_channels->setEnabled(true);
        m_Ui->idNewPrjAVSettings_description->setEnabled(true);
        return;
    }

    QString itemText = m_Ui->idNewPrjPresets->itemText(i);
    SMapStrStr configs = ProjectManager::Get()->GetPresets()->GetPresetData(itemText.toStdString().c_str());

    const syString *value;

    value = configs.find("idNewPrjAVSettings_width");
    if (value) m_Ui->idNewPrjAVSettings_width->setText(value->c_str());

    value = configs.find("idNewPrjAVSettings_height");
    if (value) m_Ui->idNewPrjAVSettings_height->setText(value->c_str());

    value = configs.find("idNewPrjAVSettings_fps");
    if (value) m_Ui->idNewPrjAVSettings_fps->setCurrentIndex(m_Ui->idNewPrjAVSettings_fps->findText(value->c_str()));

    value = configs.find("idNewPrjAVSettings_interlacing");
    if (value) m_Ui->idNewPrjAVSettings_interlacing->setCurrentIndex(m_Ui->idNewPrjAVSettings_interlacing->findText(value->c_str()));

    value = configs.find("idNewPrjAVSettings_pixelaspect");
    if (value) m_Ui->idNewPrjAVSettings_pixelaspect->setText(value->c_str());

    value = configs.find("idNewPrjAVSettings_samplerate");
    if (value) m_Ui->idNewPrjAVSettings_samplerate->setCurrentIndex(m_Ui->idNewPrjAVSettings_samplerate->findText(value->c_str()));

    value = configs.find("idNewPrjAVSettings_samplesize");
    if (value) m_Ui->idNewPrjAVSettings_samplesize->setCurrentIndex(m_Ui->idNewPrjAVSettings_samplesize->findText(value->c_str()));

    value = configs.find("idNewPrjAVSettings_surround");
    if (value) m_Ui->idNewPrjAVSettings_surround->setCurrentIndex(m_Ui->idNewPrjAVSettings_surround->findText(value->c_str()));

    value = configs.find("idNewPrjAVSettings_channels");
    if (value) {
        bool ok;
        int intvalue = (static_cast<QString>(*value)).toInt(&ok, 10);
        if (ok) m_Ui->idNewPrjAVSettings_channels->setValue(intvalue);
    }

    value = configs.find("idNewPrjAVSettings_description");
    if (value) m_Ui->idNewPrjAVSettings_description->setText(value->c_str());

    m_Ui->idNewPrjAVSettings_width->setEnabled(false);
    m_Ui->idNewPrjAVSettings_height->setEnabled(false);
    m_Ui->idNewPrjAVSettings_fps->setEnabled(false);
    m_Ui->idNewPrjAVSettings_interlacing->setEnabled(false);
    m_Ui->idNewPrjAVSettings_pixelaspect->setEnabled(false);
    m_Ui->idNewPrjAVSettings_samplerate->setEnabled(false);
    m_Ui->idNewPrjAVSettings_samplesize->setEnabled(false);
    m_Ui->idNewPrjAVSettings_surround->setEnabled(false);
    m_Ui->idNewPrjAVSettings_channels->setEnabled(false);
    m_Ui->idNewPrjAVSettings_description->setEnabled(false);
}

void NewProjectDlg::Data::OnPrjSaveSettingsAsClicked()
{
    bool ok;
    QString text;
    SMapStrStr configs;

    configs["idNewPrjAVSettings_width"] = m_Ui->idNewPrjAVSettings_width->text();
    configs["idNewPrjAVSettings_height"] = m_Ui->idNewPrjAVSettings_height->text();
    configs["idNewPrjAVSettings_fps"] = m_Ui->idNewPrjAVSettings_fps->currentText();
    configs["idNewPrjAVSettings_interlacing"] = m_Ui->idNewPrjAVSettings_interlacing->currentText();
    configs["idNewPrjAVSettings_pixelaspect"] = m_Ui->idNewPrjAVSettings_pixelaspect->text();
    configs["idNewPrjAVSettings_samplerate"] = m_Ui->idNewPrjAVSettings_samplerate->currentText();
    configs["idNewPrjAVSettings_samplesize"] = m_Ui->idNewPrjAVSettings_samplesize->currentText();
    configs["idNewPrjAVSettings_surround"] = m_Ui->idNewPrjAVSettings_surround->currentText();
    configs["idNewPrjAVSettings_channels"] = QString::number(m_Ui->idNewPrjAVSettings_channels->value(), 10);
    configs["idNewPrjAVSettings_description"] = m_Ui->idNewPrjAVSettings_description->toPlainText();

    text = QInputDialog::getText(m_Parent, tr("Pick name dialog"), tr("Predefined settings name:"),
        QLineEdit::Normal, m_Ui->idNewPrjTitle->text(), &ok);

    /*programmatically it's possible to insert duplicates so we need to assure
    that in combobox it's only one item with a specific text*/
    if (ok && !text.isEmpty() && m_Ui->idNewPrjPresets->findText(text) < 0 &&
        ProjectManager::Get()->GetPresets()->SaveNewPreset(text.toStdString().c_str(), configs)) {
        m_Ui->idNewPrjPresets->addItem(text);
        m_Ui->idNewPrjPresets->setCurrentIndex(m_Ui->idNewPrjPresets->count() - 1);
    }
}

void NewProjectDlg::Data::OnBrowseDir()
{
    QString dirName = QFileDialog::getExistingDirectory(m_Parent, tr("Choose directory"));
    //TODO: dirName should be private in class
}

void NewProjectDlg::Data::OnPressOk()
{
    if (m_Ui->idNewPrjTitle->text().isEmpty()) return;
    if (m_Ui->idNewPrjFilename->text().isEmpty()) return;
    #warning TODO: save data in a .xml or .json file through ProjectManager, close the dialog and open the project.
}

void NewProjectDlg::Data::OnEditingFinished()
{
    if (m_Ui->idNewPrjUnderscores->isChecked()) {
        m_Ui->idNewPrjFilename->setText(m_Ui->idNewPrjFilename->text().replace(' ', '_'));
    }
}

#ifndef Q_MOC_RUN
  #include "moc/newprojectdlg.moc.h"
#endif
