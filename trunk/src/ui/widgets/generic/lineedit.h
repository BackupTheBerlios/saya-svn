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

#ifndef WIDGETS_GENERIC_LINEEDIT_H_INCLUDED
#define WIDGETS_GENERIC_LINEEDIT_H_INCLUDED

#include <QLineEdit>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syLineEdit : public QLineEdit, public has_slots {
    Q_OBJECT
    public:
        syLineEdit( QWidget * parent = 0 );
        syLineEdit ( const QString & contents, QWidget * parent = 0 );
        virtual ~syLineEdit() {}

    #ifndef Q_MOC_RUN
        signal2<int,int> sigcursorPositionChanged;
        signal0 sigeditingFinished;
        signal0 sigreturnPressed;
        signal0 sigselectionChanged;
        signal1<const QString &> sigtextChanged;
        signal1<const QString &> sigtextEdited;
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigcursorPositionChanged ( int old, int new );
        void sigeditingFinished ();
        void sigreturnPressed ();
        void sigselectionChanged ();
        void sigtextChanged ( const QString & text );
        void sigtextEdited ( const QString & text );
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
