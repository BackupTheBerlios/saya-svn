/**************************************************************************************
 * Name:      widget.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_DOCKWIDGET_H_INCLUDED
#define WIDGETS_GENERIC_DOCKWIDGET_H_INCLUDED

#include <QDockWidget>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syDockWidget : public QDockWidget, public has_slots {
    Q_OBJECT
    public:
        syDockWidget( QWidget * parent = 0, Qt::WindowFlags f = 0 );
        virtual ~syDockWidget() {}

    #ifndef Q_MOC_RUN
        signal1<Qt::DockWidgetAreas> sigallowedAreasChanged; // (Qt::DockWidgetAreas allowedAreas)
        signal1<Qt::DockWidgetArea> sigdockLocationChanged; // (Qt::DockWidgetArea area)
        signal1<QDockWidget::DockWidgetFeatures> sigdockfeaturesChanged; //  (QDockWidget::DockWidgetFeatures features)
        signal1<bool> sigtopLevelChanged; // (bool topLevel)
        signal1<bool> sigvisibilityChanged; // (bool visible)
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void allowedAreasChanged(Qt::DockWidgetAreas allowedAreas);
        void dockLocationChanged(Qt::DockWidgetArea area);
        void featuresChanged(QDockWidget::DockWidgetFeatures features);
        void topLevelChanged(bool topLevel);
        void visibilityChanged(bool visible);
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
