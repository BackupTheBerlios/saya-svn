/***************************************************************
 * Name:      bitmapdialog.cpp
 * Purpose:   Implements BitmapDialog
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2011-04-29
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include <saya/core/systring.h>
#include <saya/core/sybitmap.h>
#include "bitmapdialog.h"
#include "ui/bitmapdialog.ui.h"

#include <saya/core/sigslot.h>
#include <QImage>

using namespace sigslot;

class BitmapDialog::Data : public has_slots {
    public:
        Data(BitmapDialog* parent);
        virtual ~Data();

        BitmapDialog* m_Parent;
        Ui::bitmap_dialog* m_Ui;
};

BitmapDialog::Data::Data(BitmapDialog* parent) :
m_Parent(parent),
m_Ui(new Ui::bitmap_dialog)
{
    m_Ui->setupUi(parent);
}

BitmapDialog::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
    delete m_Ui;
    m_Ui = 0;
}

BitmapDialog::BitmapDialog(QWidget *parent) : QDialog(parent)
{
    m_Data = new Data(this);
}

BitmapDialog::~BitmapDialog()
{
    delete m_Data;
    m_Data = 0;
}

void BitmapDialog::SetBitmap(const syBitmap* src) {
    QImage tmpimage(src->GetReadOnlyBuffer(), src->GetWidth(), src->GetHeight(), QImage::Format_RGB32);
    m_Data->m_Ui->image->setPixmap(QPixmap::fromImage(tmpimage));
    m_Data->m_Ui->image->setMinimumSize(QSize(src->GetWidth(),src->GetHeight()));
}