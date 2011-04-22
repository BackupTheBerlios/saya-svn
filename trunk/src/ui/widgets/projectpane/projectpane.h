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

#include <ui/widgets/generic/dockwidget.h>
#include <saya/core/systring.h>
#include <saya/core/sigslot.h>

class ProjectPane : public syDockWidget {
    public:
        ProjectPane(QWidget* parent = 0);
        virtual ~ProjectPane();
        syString SavePaneState() const;
        void RestorePaneState(const syString& data);
        sigslot::signal0 sigRefresh;
    private:
        virtual void contextMenuEvent(QContextMenuEvent * ev);
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
