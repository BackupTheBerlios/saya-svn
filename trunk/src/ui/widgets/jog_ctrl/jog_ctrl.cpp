// **************************************************************************************
// * Name:      jog_ctrl.cpp
// * Purpose:   Implementation of class JogControl
// * Authors:   Ricardo Garcia
// * Created:   2009-02-06
// * Copyright: Ricardo Garcia
// * License:   LGPL license version 3 or later.
// **************************************************************************************/

#include "jog_ctrl.h"
#include <math.h>
#include <QtSvg>
#include <QSvgRenderer>

class JogControl::Data : public QObject {
/* Q_OBJECT */
    public:
        static const unsigned int MinimumSteps = 3;
        Data(JogControl* parent);
        JogControl* m_Parent;
        QSvgRenderer *m_FgRenderer, *m_BgRenderer;
        QRectF GetHandleRect(double angle); // 0 = 12 o' clock, 180 = 6 o' clock)
        unsigned int m_TotalSteps, m_CurrentStep;
        double m_CurrentAngle,m_LastAngle,m_StepAngle;
        bool LoadSvg();
        double GetStepAngle() const;
        unsigned int GetStepCount() const;
        void SetStepCount(unsigned int numsteps);
        unsigned int GetCurrentStep() const; // From 0 to numsteps - 1
        void SetCurrentStep(unsigned int stepnum); // From 0 to numsteps - 1
        double GetCurrentAngle() const;
        void SetCurrentAngle(double angle);  // 0 = 12 o' clock, 180 = 6 o' clock)
        void ChangeAngleAndProcess(double angle); // Changes angle and triggers any signals
        void RecalcStep();
        void RecalcAngle();
        double CalculateAngle(double x, double y) const;
        virtual ~Data();
};

JogControl::Data::Data(JogControl* parent) :
m_Parent(parent),
m_FgRenderer(0),
m_BgRenderer(0),
m_TotalSteps(12), m_CurrentStep(0),
m_CurrentAngle(0),m_LastAngle(0),m_StepAngle(120.0)
{
    m_FgRenderer = new QSvgRenderer(this);
    m_BgRenderer = new QSvgRenderer(this);
    m_StepAngle = 360.0 / m_TotalSteps;
}

bool JogControl::Data::LoadSvg() {
    #warning TODO: Include the image in the resources.
    bool result = true;
    result &= m_BgRenderer->load(QString("/home/rick/projects/saya/trunk/src/ui/widgets/jog_ctrl/resources/jog_ctrl_knob.svg"));
    result &= m_FgRenderer->load(QString("/home/rick/projects/saya/trunk/src/ui/widgets/jog_ctrl/resources/jog_ctrl_handle.svg"));
    return result;
}

double JogControl::Data::GetStepAngle() const {
    return m_StepAngle;
}

unsigned int JogControl::Data::GetStepCount() const {
    if(!this) { return 0; }
    return m_TotalSteps;
}

void JogControl::Data::SetStepCount(unsigned int numsteps) {
    if(!this) { return; }
    if(numsteps < MinimumSteps) {
        numsteps = MinimumSteps;
    }
    m_TotalSteps = numsteps;
    m_StepAngle = 360.0 / numsteps;
}

unsigned int JogControl::Data::GetCurrentStep() const {
    if(!this) { return 0; }
    return m_CurrentStep;
}

void JogControl::Data::SetCurrentStep(unsigned int stepnum) {
    if(!this) { return; }
    stepnum = stepnum % m_TotalSteps;
    m_CurrentStep = stepnum;
}

double JogControl::Data::GetCurrentAngle() const {
    if(!this) { return 0; }
    return m_CurrentAngle;
}

void JogControl::Data::SetCurrentAngle(double angle) {
    if(!this) { return; }
    if(angle > 360) {
        angle = fmod(angle,360.0);
    }
    m_CurrentAngle = angle;
    m_LastAngle = angle - fmod(angle,m_StepAngle);
}

void JogControl::Data::ChangeAngleAndProcess(double angle) {
    if(!this) { return; }
    if(m_LastAngle != angle) {
        // Calculate delta based on the angle difference
        double angle_delta = (angle - m_LastAngle);
        while(angle_delta < -360) { angle_delta += 360; }
        while(angle_delta > 360) { angle_delta -= 360; }
        if(angle_delta > 180) { angle_delta -=360; } // Assume angles greater than 180 deg. are 360 - angle
        if(angle_delta < -180) { angle_delta += 360; }
        int delta = round((angle_delta)/m_StepAngle);
        while(angle < 0) { angle += 360; }
        m_CurrentAngle = fmod(angle,360.0);
        RecalcStep();
        if(delta != 0) {
            // There was at least one click, process.

            m_LastAngle = m_CurrentAngle - fmod(m_CurrentAngle,m_StepAngle);
            // Calculate the next position of the dial

            // Emit the delta.
            emit m_Parent->JogStep(delta);
        }
        m_Parent->update();
    }
}

void JogControl::Data::RecalcStep() {
    if(m_TotalSteps < MinimumSteps) m_TotalSteps = MinimumSteps;
    if(m_CurrentAngle <0) {
        m_CurrentAngle  = 0;
    }
    if(m_CurrentAngle  > 360) {
        m_CurrentAngle  = fmod(m_CurrentAngle ,360.0);
    }
    m_CurrentStep = (m_CurrentAngle *m_TotalSteps)/360;
}

void JogControl::Data::RecalcAngle() {
    if(m_TotalSteps < MinimumSteps) m_TotalSteps = MinimumSteps;
    m_CurrentStep %= m_TotalSteps;
    m_CurrentAngle = (360.0*m_CurrentStep)/m_TotalSteps;
}

double JogControl::Data::CalculateAngle(double x, double y) const {
    double angle;
    if(fabs(x)<=0.1) {
        if(y <= 0) {
            angle = 0;
        } else {
            angle = 180;
        }
    } else if(fabs(y)<=0.1) {
        if(x <= 0) {
            angle = 270;
        } else {
            angle = 90;
        }
    } else {
        angle = atan2(x,-y)*180.0/3.141592653589793;
    }
    return angle;
}


JogControl::Data::~Data() {
    delete m_BgRenderer;
    m_BgRenderer = 0;
    delete m_FgRenderer;
    m_FgRenderer = 0;
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
}


JogControl::JogControl(QWidget *parent) :
m_Data(0)
{
    m_Data = new Data(this);
    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    if(m_Data->LoadSvg()) {
    } else {
        QMessageBox::warning(this,"Error","Could not load the JogControl image files!");
    }
}

JogControl::~JogControl() {
    delete m_Data;
    m_Data = 0;
}

QSize JogControl::sizeHint() const
{
    QSize size = QSize(48,48);
    return size;
}

unsigned int JogControl::GetStepCount() const {
    return m_Data->GetStepCount();
}

void JogControl::SetStepCount(unsigned int numsteps) {
    m_Data->SetStepCount(numsteps);
}

unsigned int JogControl::GetCurrentStep() const {
    return m_Data->GetCurrentStep();
}

void JogControl::SetCurrentStep(unsigned int stepnum) {
    m_Data->SetCurrentStep(stepnum);
}

double JogControl::GetCurrentAngle() const {
    return m_Data->GetCurrentAngle();
}

void JogControl::SetCurrentAngle(double angle) {
    m_Data->SetCurrentAngle(angle);
}

void JogControl::mousePressEvent(QMouseEvent *event) {
    double angle = 0;
    QPointF curpos = event->posF();
    // (0,0) is at top-left.
    qreal x = curpos.x() - (0.5*width());
    qreal y = curpos.y() - (0.5*height());
    angle = m_Data->CalculateAngle(x,y);
    SetCurrentAngle(angle);
    update();
}

void JogControl::mouseMoveEvent(QMouseEvent *event) {
    double angle = 0;
    QPointF curpos = event->posF();
    // (0,0) is at top-left.
    qreal x = curpos.x() - (0.5*width());
    qreal y = curpos.y() - (0.5*height());
    angle = m_Data->CalculateAngle(x,y);
    m_Data->ChangeAngleAndProcess(angle);
}

void JogControl::paintEvent ( QPaintEvent * pe ) {
    int w = width();
    int h = height();
    QPainter painter(this);
    double x0 = (0.5*w);
    double y0 = (0.5*h);
    double x = 0;
    double y = 0;
    double angle = m_Data->m_CurrentAngle;
    if(angle == 0) {
        x = x0;
        y = y0 - 0.275*h;
    } else {
        double rad_angle = angle*3.141592653589793/180;
        x = x0 + (0.275*w)*sin(rad_angle);
        y = y0 - (0.275*h)*cos(rad_angle);
    }
    m_Data->m_BgRenderer->render(&painter);
    m_Data->m_FgRenderer->render(&painter,QRectF(x - 0.125*w,y - 0.125*h,0.25*w,0.25*h));
}

void JogControl::wheelEvent(QWheelEvent *event) {
     event->accept();
     double numDegrees = event->delta() / 8.0;
     double numSteps = numDegrees / 15.0;
     if(numSteps != 0) {
        double angle = m_Data->GetCurrentAngle() + m_Data->GetStepAngle()*numSteps;
        m_Data->ChangeAngleAndProcess(angle);
     }
}

void JogControl::JogStep(int numSteps) {
}

#include "moc/jog_ctrl.moc.h"