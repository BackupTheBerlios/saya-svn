/*********************************************************************
 * Name:      projectpane.cpp
 * Purpose:   Implementation of the Project (Resources / Effects) pane
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2009-03-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ********************************************************************/
#include "../saya/core/intl.h"
#include "../saya/core/dialogs.h"
#include "projectpane.h"
#include "ui/projectpane.ui.h"
#include <QMenu>
// -----------------------
// Begin ProjectPane::Data
// -----------------------

class ProjectPane::Data : public QObject {
    Q_OBJECT

    public:
        Data(ProjectPane *parent = 0);
        virtual ~Data();
        Ui::projectPane* m_Ui;
        void OnResourceTreeContextMenu(QContextMenuEvent * ev);

    public slots:

    private:
        ProjectPane* m_Parent;
        QAction* action_import;
        QAction* action_rescan;
};

ProjectPane::Data::Data(ProjectPane* parent) :
QObject(0),
m_Ui(new Ui::projectPane),
m_Parent(parent)
{
    m_Ui->setupUi(dynamic_cast<QDockWidget*>(m_Parent));

    QTreeWidgetItem* item_sequences = new QTreeWidgetItem(m_Ui->resourcesTree);
    QTreeWidgetItem* item_videos = new QTreeWidgetItem(m_Ui->resourcesTree);
    QTreeWidgetItem* item_images = new QTreeWidgetItem(m_Ui->resourcesTree);
    QTreeWidgetItem* item_sound = new QTreeWidgetItem(m_Ui->resourcesTree);
    QTreeWidgetItem* item_other = new QTreeWidgetItem(m_Ui->resourcesTree);

    item_sequences->setText(0,_("Sequences"));
    item_videos->setText(0,_("Videos"));
    item_images->setText(0,_("Images"));
    item_sound->setText(0,_("Sound"));
    item_other->setText(0,_("Other"));

    m_Ui->resourcesTree->addTopLevelItem(item_sequences);
    m_Ui->resourcesTree->addTopLevelItem(item_videos);
    m_Ui->resourcesTree->addTopLevelItem(item_images);
    m_Ui->resourcesTree->addTopLevelItem(item_sound);
    m_Ui->resourcesTree->addTopLevelItem(item_other);

    // TODO: Setup the signals and slots for the Project Pane
    // TODO: Connect the actions to the corresponding slots in the main window, or use syEvents (preferred)

    action_import = new QAction(_("&Import..."),this);
    action_rescan = new QAction(_("&Rescan project directory"),this);

}

ProjectPane::Data::~Data() {
    delete m_Ui;
    delete action_import;
    delete action_rescan;
    action_import = 0;
    action_rescan = 0;
    m_Ui = 0;
}

void ProjectPane::Data::OnResourceTreeContextMenu(QContextMenuEvent * ev) {

    QList<QAction*> actions;

    actions.append(action_import);
    actions.append(action_rescan);

    if(actions.count() > 0) {
        QMenu::exec(actions, QCursor::pos());
    }
}


#include "moc/projectpane.moc.h"

// ---------------------
// End ProjectPane::Data
// ---------------------


// -----------------
// Begin ProjectPane
// -----------------

ProjectPane::ProjectPane(QWidget* parent) :
QDockWidget(parent)
{
    m_Data = new Data(this);
}

ProjectPane::~ProjectPane() {
    delete m_Data;
    m_Data = 0;
}

syString ProjectPane::SaveSplitterState() const {
    if(!this) return ""; // Due to dynamic casting, "this" may be null.
    QByteArray tmpdata = m_Data->m_Ui->splitter->saveState().toBase64();
    return syString(tmpdata.data());
}

void ProjectPane::RestoreSplitterState(const syString& data) {
    if(!this) return; // Due to dynamic casting, "this" may be null.
    if(!data.empty())
        m_Data->m_Ui->splitter->restoreState(QByteArray::fromBase64(QByteArray(data.c_str())));
}

void ProjectPane::contextMenuEvent(QContextMenuEvent * ev) {
    m_Data->OnResourceTreeContextMenu(ev);
}

// ---------------
// End ProjectPane
// ---------------
