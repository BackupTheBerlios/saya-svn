/*********************************************************************
 * Name:      projectpane.cpp
 * Purpose:   Implementation of the Project (Resources / Effects) pane
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2009-03-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 ********************************************************************/
#include "projectpane.h"
#include "ui/projectpane.ui.h"

// -----------------------
// Begin ProjectPane::Data
// -----------------------

class ProjectPane::Data : public QObject {
    Q_OBJECT

    public:
        Data(ProjectPane *parent = 0);
        virtual ~Data();
        Ui::projectPane* m_Ui;

    public slots:
//        void OnResourceTreeContextMenu(wxTreeEvent& event);

//
//        void OnUpdateProjectPaneUI();

    private:
        ProjectPane* m_Parent;
};

ProjectPane::Data::Data(ProjectPane* parent) :
QObject(0),
m_Ui(new Ui::projectPane),
m_Parent(parent)
{
    m_Ui->setupUi(dynamic_cast<QDockWidget*>(m_Parent));

    // TODO: Create the Resources tree here.

//  Here's the old wxWidgets code for reference.
//	wxTreeItemId daroot = m_ResourcesTree->AddRoot(_w("Resources"), -1, -1, NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Sequences"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Videos"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Images"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Sound"),-1,-1,NULL);
//	m_ResourcesTree->AppendItem(daroot, _w("Other"),-1,-1,NULL);


    // Here we setup the signals and slots.
}

ProjectPane::Data::~Data() {
    delete m_Ui;
    m_Ui = 0;
}

// TODO: Reimplement the context menus and UpdateUI events with Qt.
// here's the old wxWidgets code for reference.
//
//void AppFrame::OnResourceTreeContextMenu(wxTreeEvent& event) {
//
//    wxMenu *menu = wxXmlResource::Get()->LoadMenu(_T("resources_tree_menu"));
//    std::auto_ptr<wxMenu> tmpptr(menu);
//    if(menu) {
//        PopupMenu(menu);
//    }
//}
//
//void AppFrame::OnUpdateProjectPaneUI() {
//    if(IsAppShuttingDown())
//        return;
//    bool enablePane = ProjectManager::Get()->HasProject();
//    wxWindow* thepane = FindWindow(idProjectPane);
//    if(thepane) {
//        thepane->Enable(enablePane);
//    }
//}


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

// ---------------
// End ProjectPane
// ---------------
