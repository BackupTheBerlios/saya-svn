// **************************************************************************************
// * Name:      tubslider.h
// * Purpose:   Declaration of a Slider control with a prettier interface
// * Authors:   Ricardo Garcia
// * Created:   2010-11-16
// * Copyright: Ricardo Garcia
// * License:   LGPL license version 3 or later.
// **************************************************************************************/

#include <QWidget>
#include <QSlider>
class TubeSlider : public QSlider {
    public:
        TubeSlider(QWidget *parent = 0);
        virtual ~TubeSlider();

    protected:
        void paintEvent ( QPaintEvent * pe );
};
