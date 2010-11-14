/***************************************************************
 * Name:      newprojectdlg.h
 * Purpose:   Defines New Project Dialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-29
 * Modified:  2010-11-13
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <QDialog>
#include <QWidget>

class NewProjectDlg : public QDialog
{
    public:
        NewProjectDlg(QWidget *parent = 0);
        virtual ~NewProjectDlg();
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
