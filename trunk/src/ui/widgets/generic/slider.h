/**************************************************************************************
 * Name:      slider.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-11-28
 * Modified:  2010-11-28
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_SLIDER_H_INCLUDED
#define WIDGETS_GENERIC_SLIDER_H_INCLUDED

#include <QSlider>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class sySlider : public QSlider, public has_slots {
    Q_OBJECT
    public:
        sySlider( QWidget * parent = 0 );
        sySlider( Qt::Orientation orientation, QWidget * parent = 0 );
        virtual ~sySlider() {}

    #ifndef Q_MOC_RUN
        signal1<int> sigactionTriggered;
        signal2<int, int> sigrangeChanged;
        signal1<int> sigsliderMoved;
        signal0 sigsliderPressed;
        signal0 sigsliderReleased;
        signal1<int> sigvalueChanged;
        signal1<const QPoint&> sigcustomContextMenuRequested;
        signal1<QObject*> sigobjdestroyed;
        signal0 sigdestroyed;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigactionTriggered(int action);
        void sigrangeChanged(int min,int max);
        void sigsliderMoved(int value);
        void sigsliderPressed();
        void sigsliderReleased();
        void sigvalueChanged(int value);
        void sigcustomContextMenuRequested(const QPoint &pos);
        void sigobjdestroyed(QObject* obj);
        void sigdestroyed();
    #endif

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
