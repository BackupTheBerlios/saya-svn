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
        Data(ProjectPane *parent = 0);
        virtual ~Data();
        Ui::projectPane* m_Ui;
        void OnResourceListContextMenu(QContextMenuEvent * ev);
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

void ProjectPane::Data::OnResourceListContextMenu(QContextMenuEvent * ev) {

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
//    m_Ui->resourcesList->clear();
//    QTreeWidgetItem* category = m_Ui->resourcesTree->currentItem();

    unsigned int i;
    const AVResources* resources = ProjectManager::Get()->GetResources();
//    QTreeWidgetItem* allowedcategory = null;
//    if(resources) {
//        for(i = 0; i < resources.size(); ++i) {
//            const AVResource* res = &resources[i];
//            switch(res->m_ResourceType) {
//                case RTSequence:
//                    allowedcategory = item_sequences;
//                break;
//                case RTVideoFile:
//                    allowedcategory = item_videos;
//                break;
//                case RTAudioFile:
//                    allowedcategory = item_sound;
//                break;
//                case RTImageFile:
//                    allowedcategory = item_images;
//                break;
//                case RTOfflineFile:
//                    allowedcategory = item_other;
//                break;
//                case RTTitle:
//                    allowedcategory = item_other;
//                break;
//                case RTBarsAndTone:
//                    allowedcategory = item_videos;
//                break;
//                case RTBlackVideo:
//                    allowedcategory = item_videos;
//                break;
//                case RTColorMatte:
//                    allowedcategory = item_images;
//                break;
//                case RTUCLeader:
//                    allowedcategory = item_videos;
//                default:
//                    allowedcategory = item_other;
//            }
//            if(category == allowedcategory) {
//
//            }
//        }
//    }
//
//
//
//
//
//    if(item == item_sequences) {
//
//    } else if(item == item_videos) {
//    } else if(item == item_images) {
//    } else if(item == item_sound) {
//    } else if(item == item_other) {
//    }

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

void ProjectPane::contextMenuEvent(QContextMenuEvent * ev) {
    m_Data->OnResourceListContextMenu(ev);
}

// ---------------
// End ProjectPane
// ---------------
