/***************************************************************
 * Name:      welcomedlg.h
 * Purpose:   Defines Welcome Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-26
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef WELCOMEDLG_H
#define WELCOMEDLG_H

#include <qframe.h>


class WelcomeDialog: public QFrame
{
    Q_OBJECT
    public:
        WelcomeDialog(QWidget* parent);
        virtual ~WelcomeDialog();
    protected:
        virtual void showEvent(QShowEvent * event);
        virtual void closeEvent(QCloseEvent *event);
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
