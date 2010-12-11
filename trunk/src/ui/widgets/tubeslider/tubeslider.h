/**************************************************************************************
 * Name:      tubslider.h
 * Purpose:   Declaration of a Slider control with a prettier interface
 * Authors:   Ricardo Garcia
 * Created:   2010-11-16
 * Copyright: Ricardo Garcia
 * License:   LGPL license version 3 or later, with linking exception
 **************************************************************************************/

#ifndef TUBESLIDER_H_INCLUDED
#define TUBESLIDER_H_INCLUDED

#include "../generic/widget.h"

class TubeSlider : public syWidget {
    public:
        TubeSlider(QWidget *parent = 0);
        virtual ~TubeSlider();
        QSize sizeHint() const;
        double getCurrentTime();

        signal1<double> TimeChanged;

    public: // slots
        void setTimeRange(double start, double finish); // In seconds
        void setCurrentTime(double time); // Update the seek bar's current position.

    protected:
        void paintEvent ( QPaintEvent * pe );
        void mouseMoveEvent ( QMouseEvent * event );
        void mousePressEvent ( QMouseEvent * event );
        void mouseReleaseEvent ( QMouseEvent * event );

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif
