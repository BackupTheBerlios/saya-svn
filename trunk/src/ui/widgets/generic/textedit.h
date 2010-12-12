/**************************************************************************************
 * Name:      textedit.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_TEXTEDIT_H_INCLUDED
#define WIDGETS_GENERIC_TEXTEDIT_H_INCLUDED

#include <QTextEdit>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syTextEdit : public QTextEdit, public has_slots {
    Q_OBJECT
    public:
        syTextEdit( QWidget * parent = 0 );
        syTextEdit ( const QString & text, QWidget * parent = 0 );
        virtual ~syTextEdit() {}

    #ifndef Q_MOC_RUN
        signal1<bool> sigcopyAvailable;
        signal1<const QTextCharFormat &> sigcurrentCharFormatChanged;
        signal0 sigcursorPositionChanged;
        signal1<bool> sigredoAvailable;
        signal0 sigselectionChanged;
        signal0 sigtextChanged;
        signal1<bool> sigundoAvailable;
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigcopyAvailable ( bool yes );
        void sigcurrentCharFormatChanged ( const QTextCharFormat & f );
        void sigcursorPositionChanged ();
        void sigredoAvailable ( bool available );
        void sigselectionChanged ();
        void sigtextChanged ();
        void sigundoAvailable ( bool available );
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
