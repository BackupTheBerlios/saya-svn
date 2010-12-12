/**************************************************************************************
 * Name:      menu.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-12-07
 * Modified:  2010-12-07
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_MENU_H_INCLUDED
#define WIDGETS_GENERIC_MENU_H_INCLUDED

#include <QMenu>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class QAction;

class syMenu : public QMenu, public has_slots {
    Q_OBJECT
    public:
        syMenu( QWidget * parent = 0 );
        syMenu( const QString & title, QWidget * parent = 0 );
        virtual ~syMenu() {}

    #ifndef Q_MOC_RUN
        signal0 sigaboutToHide;
        signal0 sigaboutToShow;
        signal1<QAction*> sighovered;
        signal1<QAction*> sigtriggered;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigaboutToHide ();
        void sigaboutToShow ();
        void sighovered ( QAction * action );
        void sigtriggered ( QAction * action );
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
