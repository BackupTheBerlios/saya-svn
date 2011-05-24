/***************************************************************
 * Name:      newprojectdlg.cpp
 * Purpose:   Implements New Project Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-29
 * Modified:  2010-11-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <saya/core/systring.h>
#include <saya/projectmanager.h>
#include <saya/presetmanager.h>
#include <saya/timeline/sstrvector.h>
#include <saya/timeline/smapxstr.h>

#include "newprojectdlg.h"
#include "ui/newprojectdlg.ui.h"

#include <QFileDialog>
#include <QInputDialog>

#include <saya/core/sigslot.h>
using namespace sigslot;

class NewProjectDlg::Data : public has_slots {
    public:
        Data(NewProjectDlg* parent);
        void ConnectSignalsAndSlots();
        virtual ~Data();
        bool LoadPresets();

        NewProjectDlg* m_Parent;
        Ui::new_project_dialog* m_Ui;

    // slots
        void OnCancel();
        void OnAVSettingsUpdateUI();
        void OnAVSettingsUpdateUI_int(int x) { OnAVSettingsUpdateUI(); }
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
    m_Ui->edNewPrjAVSettings_width->setValidator(new QIntValidator(m_Ui->edNewPrjAVSettings_width));
    m_Ui->edNewPrjAVSettings_height->setValidator(new QIntValidator(m_Ui->edNewPrjAVSettings_height));
    m_Ui->edNewPrjAVSettings_pixelaspect->setValidator(new QDoubleValidator(m_Ui->edNewPrjAVSettings_pixelaspect));
    m_Ui->cmbNewPrjPresets->setEnabled(true);
    LoadPresets();
}

void NewProjectDlg::Data::ConnectSignalsAndSlots() {

    m_Ui->cmbNewPrjPresets->sigactivated_index.connect(this, &NewProjectDlg::Data::OnPrjPresetsChanged);

    m_Ui->btnPrjLocation->sigclicked.connect(this, &NewProjectDlg::Data::OnBrowseDir);
    m_Ui->btnNewPrjSaveSettingsAs->sigclicked.connect(this, &NewProjectDlg::Data::OnPrjSaveSettingsAsClicked);
    m_Ui->btnOk->sigclicked.connect(this, &NewProjectDlg::Data::OnPressOk);
    m_Ui->btnCancel->sigclicked.connect(this, &NewProjectDlg::Data::OnCancel);

    m_Ui->edNewPrjFilename->sigeditingFinished.connect(this, &NewProjectDlg::Data::OnEditingFinished);

    m_Ui->edNewPrjAVSettings_width->sigeditingFinished.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI);
    m_Ui->edNewPrjAVSettings_height->sigeditingFinished.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI);
    m_Ui->cmbNewPrjAVSettings_fps->sigcurrentIndexChanged_index.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->cmbNewPrjAVSettings_interlacing->sigcurrentIndexChanged_index.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->edNewPrjAVSettings_pixelaspect->sigeditingFinished.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI);
    m_Ui->cmbNewPrjAVSettings_samplerate->sigcurrentIndexChanged_index.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->cmbNewPrjAVSettings_samplesize->sigcurrentIndexChanged_index.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->cmbNewPrjAVSettings_surround->sigcurrentIndexChanged_index.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->spnNewPrjAVSettings_channels->sigvalueChanged_int.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI_int);
    m_Ui->txtNewPrjAVSettings_description->sigtextChanged.connect(this, &NewProjectDlg::Data::OnAVSettingsUpdateUI);
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

    m_Ui->cmbNewPrjPresets->addItem("<Custom>");
    for (i=0, imax=presets.size(); i<imax; i++) {
        m_Ui->cmbNewPrjPresets->addItem(presets[i]);
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

void NewProjectDlg::Data::OnCancel() {
    m_Parent->close();
}

void NewProjectDlg::Data::OnPrjPresetsChanged(int i)
{
    if (i < 0) return;

    if (i == 0) {
        m_Ui->edNewPrjAVSettings_width->setEnabled(true);
        m_Ui->edNewPrjAVSettings_height->setEnabled(true);
        m_Ui->cmbNewPrjAVSettings_fps->setEnabled(true);
        m_Ui->cmbNewPrjAVSettings_interlacing->setEnabled(true);
        m_Ui->edNewPrjAVSettings_pixelaspect->setEnabled(true);
        m_Ui->cmbNewPrjAVSettings_samplerate->setEnabled(true);
        m_Ui->cmbNewPrjAVSettings_samplesize->setEnabled(true);
        m_Ui->cmbNewPrjAVSettings_surround->setEnabled(true);
        m_Ui->spnNewPrjAVSettings_channels->setEnabled(true);
        m_Ui->txtNewPrjAVSettings_description->setEnabled(true);
        return;
    }

    QString itemText = m_Ui->cmbNewPrjPresets->itemText(i);
    SMapStrStr configs = ProjectManager::Get()->GetPresets()->GetPresetData(itemText.toStdString().c_str());

    const syString *value;

    value = configs.find("edNewPrjAVSettings_width");
    if (value) m_Ui->edNewPrjAVSettings_width->setText(value->c_str());

    value = configs.find("edNewPrjAVSettings_height");
    if (value) m_Ui->edNewPrjAVSettings_height->setText(value->c_str());

    value = configs.find("cmbNewPrjAVSettings_fps");
    if (value) m_Ui->cmbNewPrjAVSettings_fps->setCurrentIndex(m_Ui->cmbNewPrjAVSettings_fps->findText(value->c_str()));

    value = configs.find("cmbNewPrjAVSettings_interlacing");
    if (value) m_Ui->cmbNewPrjAVSettings_interlacing->setCurrentIndex(m_Ui->cmbNewPrjAVSettings_interlacing->findText(value->c_str()));

    value = configs.find("edNewPrjAVSettings_pixelaspect");
    if (value) m_Ui->edNewPrjAVSettings_pixelaspect->setText(value->c_str());

    value = configs.find("cmbNewPrjAVSettings_samplerate");
    if (value) m_Ui->cmbNewPrjAVSettings_samplerate->setCurrentIndex(m_Ui->cmbNewPrjAVSettings_samplerate->findText(value->c_str()));

    value = configs.find("cmbNewPrjAVSettings_samplesize");
    if (value) m_Ui->cmbNewPrjAVSettings_samplesize->setCurrentIndex(m_Ui->cmbNewPrjAVSettings_samplesize->findText(value->c_str()));

    value = configs.find("cmbNewPrjAVSettings_surround");
    if (value) m_Ui->cmbNewPrjAVSettings_surround->setCurrentIndex(m_Ui->cmbNewPrjAVSettings_surround->findText(value->c_str()));

    value = configs.find("spnNewPrjAVSettings_channels");
    if (value) {
        bool ok;
        int intvalue = (static_cast<QString>(*value)).toInt(&ok, 10);
        if (ok) m_Ui->spnNewPrjAVSettings_channels->setValue(intvalue);
    }

    value = configs.find("txtNewPrjAVSettings_description");
    if (value) m_Ui->txtNewPrjAVSettings_description->setText(value->c_str());

    m_Ui->edNewPrjAVSettings_width->setEnabled(false);
    m_Ui->edNewPrjAVSettings_height->setEnabled(false);
    m_Ui->cmbNewPrjAVSettings_fps->setEnabled(false);
    m_Ui->cmbNewPrjAVSettings_interlacing->setEnabled(false);
    m_Ui->edNewPrjAVSettings_pixelaspect->setEnabled(false);
    m_Ui->cmbNewPrjAVSettings_samplerate->setEnabled(false);
    m_Ui->cmbNewPrjAVSettings_samplesize->setEnabled(false);
    m_Ui->cmbNewPrjAVSettings_surround->setEnabled(false);
    m_Ui->spnNewPrjAVSettings_channels->setEnabled(false);
    m_Ui->txtNewPrjAVSettings_description->setEnabled(false);
}

void NewProjectDlg::Data::OnPrjSaveSettingsAsClicked()
{
    bool ok;
    QString text;
    SMapStrStr configs;

    configs["edNewPrjAVSettings_width"] = m_Ui->edNewPrjAVSettings_width->text();
    configs["edNewPrjAVSettings_height"] = m_Ui->edNewPrjAVSettings_height->text();
    configs["cmbNewPrjAVSettings_fps"] = m_Ui->cmbNewPrjAVSettings_fps->currentText();
    configs["cmbNewPrjAVSettings_interlacing"] = m_Ui->cmbNewPrjAVSettings_interlacing->currentText();
    configs["edNewPrjAVSettings_pixelaspect"] = m_Ui->edNewPrjAVSettings_pixelaspect->text();
    configs["cmbNewPrjAVSettings_samplerate"] = m_Ui->cmbNewPrjAVSettings_samplerate->currentText();
    configs["cmbNewPrjAVSettings_samplesize"] = m_Ui->cmbNewPrjAVSettings_samplesize->currentText();
    configs["cmbNewPrjAVSettings_surround"] = m_Ui->cmbNewPrjAVSettings_surround->currentText();
    configs["spnNewPrjAVSettings_channels"] = QString::number(m_Ui->spnNewPrjAVSettings_channels->value(), 10);
    configs["txtNewPrjAVSettings_description"] = m_Ui->txtNewPrjAVSettings_description->toPlainText();

    text = QInputDialog::getText(m_Parent, tr("Pick name dialog"), tr("Predefined settings name:"),
        QLineEdit::Normal, m_Ui->edNewPrjTitle->text(), &ok);

    /*programmatically it's possible to insert duplicates so we need to assure
    that in combobox it's only one item with a specific text*/
    if (ok && !text.isEmpty() && m_Ui->cmbNewPrjPresets->findText(text) < 0 &&
        ProjectManager::Get()->GetPresets()->SaveNewPreset(text.toStdString().c_str(), configs)) {
        m_Ui->cmbNewPrjPresets->addItem(text);
        m_Ui->cmbNewPrjPresets->setCurrentIndex(m_Ui->cmbNewPrjPresets->count() - 1);
    }
}

void NewProjectDlg::Data::OnBrowseDir()
{
    QString dirName = QFileDialog::getExistingDirectory(m_Parent, tr("Choose directory"));
    //TODO: dirName should be private in class
}

void NewProjectDlg::Data::OnPressOk()
{
    if (m_Ui->edNewPrjTitle->text().isEmpty()) return;
    if (m_Ui->edNewPrjFilename->text().isEmpty()) return;
    //TODO: save data in a .xml or .json file through ProjectManager, close the dialog and open the project.
}

void NewProjectDlg::Data::OnEditingFinished()
{
    if (m_Ui->chkNewPrjUnderscores->isChecked()) {
        m_Ui->edNewPrjFilename->setText(m_Ui->edNewPrjFilename->text().replace(' ', '_'));
    }
}
