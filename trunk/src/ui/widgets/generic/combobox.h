/**************************************************************************************
 * Name:      combobox.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_COMBOBOX_H_INCLUDED
#define WIDGETS_GENERIC_COMBOBOX_H_INCLUDED

#include <QComboBox>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syComboBox : public QComboBox, public has_slots {
    Q_OBJECT
    public:
        syComboBox( QWidget * parent = 0 );
        virtual ~syComboBox() {}

    #ifndef Q_MOC_RUN
        signal1<int> sigactivated_index;
        signal1<const QString&> sigactivated_text;
        signal1<int> sigcurrentIndexChanged_index;
        signal1<const QString&> sigcurrentIndexChanged_text;
        signal1<const QString&> sigeditTextChanged;
        signal1<int> sighighlighted_index;
        signal1<const QString&> sighighlighted_text;
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigactivated_index ( int index );
        void sigactivated_text ( const QString & text );
        void sigcurrentIndexChanged_index ( int index );
        void sigcurrentIndexChanged_text ( const QString & text );
        void sigeditTextChanged ( const QString & text );
        void sighighlighted_index ( int index );
        void sighighlighted_text ( const QString & text );
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
