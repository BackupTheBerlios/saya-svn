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

    private:
        ProjectPane* m_Parent;
};

ProjectPane::Data::Data(ProjectPane* parent) :
QObject(0),
m_Ui(new Ui::projectPane),
m_Parent(parent)
{
    m_Ui->setupUi(dynamic_cast<QDockWidget*>(m_Parent));

    // Here we setup the signals and slots.
}

ProjectPane::Data::~Data() {
    delete m_Ui;
    m_Ui = 0;
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

// ---------------
// End ProjectPane
// ---------------
