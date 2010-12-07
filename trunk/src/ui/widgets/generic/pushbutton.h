/**************************************************************************************
 * Name:      pushbutton.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_PUSHBUTTON_H_INCLUDED
#define WIDGETS_GENERIC_PUSHBUTTON_H_INCLUDED

#include <QPushButton>

#include "../../../saya/core/sigslot.h"
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syPushButton : public QPushButton, public has_slots {
    Q_OBJECT
    public:
        syPushButton( QWidget * parent = 0 );
        syPushButton( const QString & text, QWidget * parent = 0 );
        syPushButton( const QIcon & icon, const QString & text, QWidget * parent = 0 );

    #ifndef Q_MOC_RUN
        signal0 sigpressed;
        signal0 sigreleased;
        signal1<bool> sigtoggled;
        signal1<bool> sigclicked;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigpressed();
        void sigreleased();
        void sigtoggled(bool toggled);
        void sigclicked(bool checked);
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
