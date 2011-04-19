/*********************************************************************
 * Name:      projectpane.cpp
 * Purpose:   Implementation of the Project (Resources / Effects) pane
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2009-03-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ********************************************************************/

#include <saya/core/intl.h>
#include <saya/core/dialogs.h>
#include "projectpane.h"
#include "projectpane.ui.h"
#include <QMenu>
#include <ui/widgets/generic/action.h>
#include <saya/core/app.h>
#include <saya/core/sigslot.h>
#include <saya/core/events.h>

using namespace sigslot;

extern unsigned int idFileImport;
extern unsigned int idProjectRescanProjectDir;

// -----------------------
// Begin ProjectPane::Data
// -----------------------

class ProjectPane::Data : public has_slots {
    public:
        Data(ProjectPane *parent = 0);
        virtual ~Data();
        Ui::projectPane* m_Ui;
        void OnResourceTreeContextMenu(QContextMenuEvent * ev);
        void OnRefreshResourceList();

    private:
        ProjectPane* m_Parent;
        syAction* action_import;
        syAction* action_rescan;
};

ProjectPane::Data::Data(ProjectPane* parent) :
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

    // Setup the signals and slots for the Project Pane

    action_import = new syAction(_("&Import..."),m_Parent);
    action_import->setActionId(idFileImport);
    action_rescan = new syAction(_("&Rescan project directory"),m_Parent);
    action_rescan->setActionId(idProjectRescanProjectDir);

    m_Parent->sigRefresh.connect(this,&ProjectPane::Data::OnRefreshResourceList);
}

ProjectPane::Data::~Data() {
    delete m_Ui;
    delete action_import;
    delete action_rescan;
    m_Parent->m_Data = 0;
    m_Parent = 0;
    action_import = 0;
    action_rescan = 0;
    m_Ui = 0;
}

void ProjectPane::Data::OnResourceTreeContextMenu(QContextMenuEvent * ev) {

    QList<QAction*> actions;

    actions.append(action_import);
    actions.append(action_rescan);

    if(actions.count() > 0) {
        syAction* action = dynamic_cast<syAction*>(QMenu::exec(actions, QCursor::pos()));
        if(action && action->getActionId()) {
            syActionEvent evt(action->getActionId());
            syApp::Get()->GetEventHandler()->ProcessEvent(evt);
        }
    }
}

void ProjectPane::Data::OnRefreshResourceList() {
    #warning TODO: Implement ProjectPane::Data::OnRefreshResourceList
}

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
