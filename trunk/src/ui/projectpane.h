/***************************************************************
 * Name:      projectpane.h
 * Purpose:   Declaration for the Project (Resources / Effects) pane
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2009-03-04
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/
#ifndef projectpane_h
#define projectpane_h

#include <QDockWidget>

class ProjectPane : public QDockWidget {
    public:
        ProjectPane(QWidget* parent = 0);
        virtual ~ProjectPane();
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
