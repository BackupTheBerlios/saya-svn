/**************************************************************************************
 * Name:      listwidget.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2011-04-20
 * Modified:  2011-04-20
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_LISTWIDGET_H_INCLUDED
#define WIDGETS_GENERIC_LISTWIDGET_H_INCLUDED

#include <QListWidget>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syListWidget : public QListWidget, public has_slots {
    Q_OBJECT
    public:
        syListWidget( QWidget * parent = 0 );
        virtual ~syListWidget() {}

    #ifndef Q_MOC_RUN
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
        signal2<QListWidgetItem*, QListWidgetItem*> sigcurrentItemChanged;
        signal1<int> sigcurrentRowChanged;
        signal1<const QString&> sigcurrentTextChanged;
        signal1<QListWidgetItem*> sigitemActivated;
        signal1<QListWidgetItem*> sigitemChanged;
        signal1<QListWidgetItem*> sigitemClicked;
        signal1<QListWidgetItem*> sigitemDoubleClicked;
        signal1<QListWidgetItem*> sigitemEntered;
        signal1<QListWidgetItem*> sigitemPressed;
        signal0 sigitemSelectionChanged;
        signal1<const QModelIndexList&> sigindexesMoved;
        signal1<const QModelIndex&> sigactivated;
        signal1<const QModelIndex&> sigclicked;
        signal1<const QModelIndex&> sigdoubleClicked;
        signal1<const QModelIndex&> sigentered;
        signal1<const QModelIndex&> sigpressed;
        signal0 sigviewportEntered;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigcurrentItemChanged ( QListWidgetItem * current, QListWidgetItem * previous );
        void sigcurrentRowChanged ( int currentRow );
        void sigcurrentTextChanged ( const QString & currentText );
        void sigitemActivated ( QListWidgetItem * item );
        void sigitemChanged ( QListWidgetItem * item );
        void sigitemClicked ( QListWidgetItem * item );
        void sigitemDoubleClicked ( QListWidgetItem * item );
        void sigitemEntered ( QListWidgetItem * item );
        void sigitemPressed ( QListWidgetItem * item );
        void sigitemSelectionChanged ();
        void sigindexesMoved ( const QModelIndexList & indexes );
        void sigactivated ( const QModelIndex & index );
        void sigclicked ( const QModelIndex & index );
        void sigdoubleClicked ( const QModelIndex & index );
        void sigentered ( const QModelIndex & index );
        void sigpressed ( const QModelIndex & index );
        void sigviewportEntered ();
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
