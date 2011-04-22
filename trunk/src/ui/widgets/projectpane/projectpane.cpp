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
#include <saya/projectmanager.h>
#include <saya/timeline/avresources.h>
#include <saya/timeline/avresources.h>
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

        class ResourceItem : public QListWidgetItem {
            public:
                ResourceItem(const AVResource* res);
                unsigned int m_ResourceId;
                virtual ~ResourceItem() { m_ResourceId = 0; }
        };

        Data(ProjectPane *parent = 0);
        virtual ~Data();
        Ui::projectPane* m_Ui;
        void OnResourceListContextMenu(const QPoint& pos);
        void OnRefreshResourceList();

    private:
        ProjectPane* m_Parent;
        syAction* action_import;
        syAction* action_rescan;
};

ProjectPane::Data::ResourceItem::ResourceItem(const AVResource* res) :
QListWidgetItem()
{
    if(res) {
        m_ResourceId = res->m_ResourceId;
        setText(res->m_Filename);
        if(res->m_Icon != "") {
            setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(res->m_Icon.c_str()),"JPG")));
        }
    }
}


ProjectPane::Data::Data(ProjectPane* parent) :
m_Ui(new Ui::projectPane),
m_Parent(parent)
{
    m_Ui->setupUi(dynamic_cast<QDockWidget*>(m_Parent));
    m_Ui->listSeq->setSortingEnabled(true);
    m_Ui->listVid->setSortingEnabled(true);
    m_Ui->listSnd->setSortingEnabled(true);
    m_Ui->listImg->setSortingEnabled(true);
    m_Ui->listOther->setSortingEnabled(true);

    m_Ui->listSeq->setViewMode(QListView::ListMode);
    m_Ui->listVid->setViewMode(QListView::IconMode);
    m_Ui->listSnd->setViewMode(QListView::ListMode);
    m_Ui->listImg->setViewMode(QListView::IconMode);
    m_Ui->listOther->setViewMode(QListView::ListMode);

    // Setup the signals and slots for the Project Pane

    action_import = new syAction(_("&Import..."),m_Parent);
    action_import->setActionId(idFileImport);
    action_rescan = new syAction(_("&Rescan project directory"),m_Parent);
    action_rescan->setActionId(idProjectRescanProjectDir);

    m_Ui->tabResources->setContextMenuPolicy(Qt::CustomContextMenu);
    m_Ui->tabResources->sigcustomContextMenuRequested.connect(this,&ProjectPane::Data::OnResourceListContextMenu);

    m_Parent->sigRefreshResourceList.connect(this,&ProjectPane::Data::OnRefreshResourceList);
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

void ProjectPane::Data::OnResourceListContextMenu(const QPoint& pos) {

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
    m_Ui->listSeq->clear();
    m_Ui->listVid->clear();
    m_Ui->listSnd->clear();
    m_Ui->listImg->clear();
    m_Ui->listOther->clear();
    unsigned int i;
    const AVResources* resources = ProjectManager::Get()->GetResources();
    syListWidget* curlist = 0;
    if(resources) {
        for(i = 0; i < resources->size(); ++i) {
            const AVResource* res = &(resources->operator[](i));
            switch(res->m_ResourceType) {
                case RTSequence:
                    curlist = m_Ui->listSeq;
                break;
                case RTVideoFile:
                    curlist = m_Ui->listVid;
                break;
                case RTAudioFile:
                    curlist = m_Ui->listSnd;
                break;
                case RTImageFile:
                    curlist = m_Ui->listImg;
                break;
                case RTOfflineFile:
                    curlist = m_Ui->listOther;
                break;
                case RTTitle:
                    curlist = m_Ui->listOther;
                break;
                case RTBarsAndTone:
                    curlist = m_Ui->listVid;
                break;
                case RTBlackVideo:
                    curlist = m_Ui->listVid;
                break;
                case RTColorMatte:
                    curlist = m_Ui->listImg;
                break;
                case RTUCLeader:
                    curlist = m_Ui->listVid;
                default:
                    curlist = m_Ui->listOther;
            }
            ResourceItem* item = new ResourceItem(res);
            curlist->addItem(static_cast<QListWidgetItem*>(item));
        }
    }
}

// ---------------------
// End ProjectPane::Data
// ---------------------


// -----------------
// Begin ProjectPane
// -----------------

ProjectPane::ProjectPane(QWidget* parent) :
syDockWidget(parent)
{
    m_Data = new Data(this);
}

ProjectPane::~ProjectPane() {
    delete m_Data;
    m_Data = 0;
}

syString ProjectPane::SavePaneState() const {
    if(!this) return ""; // Due to dynamic casting, "this" may be null.
    return "";
    #warning TODO: Implement ProjectPane::SavePaneState
//    QByteArray tmpdata = m_Data->m_Ui->splitter->saveState().toBase64();
//    return syString(tmpdata.data());
}

void ProjectPane::RestorePaneState(const syString& data) {
    if(!this) return; // Due to dynamic casting, "this" may be null.
    #warning TODO: Implement ProjectPane::RestorePaneState
//    if(!data.empty())
//        m_Data->m_Ui->splitter->restoreState(QByteArray::fromBase64(QByteArray(data.c_str())));
}

// ---------------
// End ProjectPane
// ---------------
