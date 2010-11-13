// **************************************************************************************
// * Name:      jog_ctrl.h
// * Purpose:   Declaration of class JogControl
// * Authors:   Ricardo Garcia
// * Created:   2009-02-06
// * Copyright: Ricardo Garcia
// * License:   LGPL license version 3 or later.
// **************************************************************************************/

#include <QWidget>

class JogControl : public QWidget {
    // Q_OBJECT
    public:
        JogControl(QWidget *parent = 0);
        QSize sizeHint() const;
        unsigned int GetStepCount() const;
        void SetStepCount(unsigned int numsteps); // Minimum 3.
        unsigned int GetCurrentStep() const; // From 0 to numsteps - 1
        void SetCurrentStep(unsigned int stepnum); // From 0 to numsteps - 1
        double GetCurrentAngle() const;
        void SetCurrentAngle(double angle);  // 0 = 12 o' clock, 180 = 6 o' clock)
        virtual ~JogControl();

    signals:
        void JogStep(int numsteps); // Can be negative

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);
        void paintEvent ( QPaintEvent * pe );

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
