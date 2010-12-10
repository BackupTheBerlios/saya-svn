/**************************************************************************************
 * Name:      lineedit.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_SPINBOX_H_INCLUDED
#define WIDGETS_GENERIC_SPINBOX_H_INCLUDED

#include <QSpinBox>

#include "sigslot.h"
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class sySpinBox : public QSpinBox, public has_slots {
    Q_OBJECT
    public:
        sySpinBox( QWidget * parent = 0 );
        virtual ~sySpinBox() {}

    #ifndef Q_MOC_RUN
        signal1<int> sigvalueChanged_int;
        signal1<const QString &> sigvalueChanged_text;
        signal0 sigeditingFinished;
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigvalueChanged_int(int i);
        void sigvalueChanged_text(const QString &text);
        void sigeditingFinished();
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
