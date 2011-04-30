/***************************************************************
 * Name:      bitmapdialog.h
 * Purpose:   Defines A Bitmap Dialog to show a bitmap
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-29
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef BITMAPDIALOG_H
#define BITMAPDIALOG_H

#include <QDialog>
#include <QWidget>
class syBitmap;

class BitmapDialog : public QDialog
{
    public:
        BitmapDialog(QWidget *parent = 0);
        void SetBitmap(const syBitmap* src);
        virtual ~BitmapDialog();
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};

#endif
