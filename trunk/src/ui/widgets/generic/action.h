/**************************************************************************************
 * Name:      action.h
 * Purpose:   Declaration of widgets with new signal implementations.
 *            These widgets will allow us to declare new signals and slots
 *            without having to rely on Qt's MOC.
 * Authors:   Ricardo Garcia
 * Created:   2010-12-07
 * Modified:  2010-12-07
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3.0 or later
 **************************************************************************************/

#ifndef WIDGETS_GENERIC_ACTION_H_INCLUDED
#define WIDGETS_GENERIC_ACTION_H_INCLUDED

#include <QAction>

#include <saya/core/sigslot.h>
using namespace sigslot;

#define SY_FAKE_SLOTS
#ifndef Q_MOC_RUN
    #undef SY_FAKE_SLOTS
#endif

class syAction : public QAction, public has_slots {
    Q_OBJECT
    public:
        syAction( QObject * parent = 0 );
        syAction( const QString & text, QObject * parent = 0 );
        syAction( const QIcon & icon, const QString & text, QObject * parent = 0 );
        /** Sets a custom Id for Action Events. To be used with syActionEvent. */
        void setActionId(unsigned int id) { m_ActionId = id; }
        unsigned int getActionId() { return m_ActionId; }
        virtual ~syAction() {}

    #ifndef Q_MOC_RUN
        signal0 sigchanged;
        signal0 sighovered;
        signal1<bool> sigtoggled;
        signal0 sigtriggered;
        signal1<bool> sigtriggeredcheckable;
    #endif
    #ifdef SY_FAKE_SLOTS
    public slots:
        void sigchanged();
        void sighovered();
        void sigtoggled(bool checked);
        void sigtriggered();
        void sigtriggeredcheckable(bool checked);
    #endif

    protected:
        unsigned int m_ActionId;

    private:
        void connectFakeSlots();
};

#undef SY_FAKE_SLOTS

#endif
