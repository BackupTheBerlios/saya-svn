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
        unsigned int GetStepCount() const;
        void SetStepCount(unsigned int numsteps);
        unsigned int GetCurrentStep() const; // From 0 to numsteps - 1
        void SetCurrentStep(unsigned int stepnum); // From 0 to numsteps - 1
        double GetCurrentAngle() const;
        void SetCurrentAngle(double angle);  // 0 = 12 o' clock, 180 = 6 o' clock)
        void ChangeAngleAndProcess(double angle); // Changes angle and triggers any signals
        void RecalcStep();
        void RecalcAngle();
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
}

bool JogControl::Data::LoadSvg() {
    // TODO: Incluir la imagen en los resources.
    bool result = true;
    result &= m_BgRenderer->load(QString("/home/rick/projects/saya/trunk/src/ui/widgets/jog_ctrl/resources/jog_ctrl_knob.svg"));
    result &= m_FgRenderer->load(QString("/home/rick/projects/saya/trunk/src/ui/widgets/jog_ctrl/resources/jog_ctrl_handle.svg"));
    return result;
}

void JogControl::Data::RecalcAngle() {
    if(m_TotalSteps < MinimumSteps) m_TotalSteps = MinimumSteps;
    m_CurrentStep %= m_TotalSteps;
    m_CurrentAngle = (360.0*m_CurrentStep)/m_TotalSteps;
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
    m_LastAngle = angle;
}

void JogControl::Data::ChangeAngleAndProcess(double angle) {
    if(!this) { return; }
    if(angle > 360) {
        angle = fmod(angle,360.0);
    }
    m_CurrentAngle = angle;
    if(m_LastAngle != m_CurrentAngle) {
        #warning "TODO: Calculate delta, including the cases where the angle was nearby 360 and now it's less, and viceversa."
    }
    m_LastAngle = m_CurrentAngle;

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
    // TODO: Calculate angle based on the mouse position.
}

void JogControl::mouseMoveEvent(QMouseEvent *event) {
    // TODO: Recalculate angle based on the mouse position, and call Data::ChangeAngleAndProcess
}

void JogControl::paintEvent ( QPaintEvent * pe ) {
    int w = width();
    int h = height();
    QPainter painter(this);
    m_Data->m_BgRenderer->render(&painter);
    m_Data->m_FgRenderer->render(&painter,QRectF(0,0,w/2,h/2));
//    painter.setClipRect(0,0,w / 2, h /2);


}

void JogControl::mouseWheelEvent(QWheelEvent *event) {
     int numDegrees = event->delta() / 8;
     int numSteps = numDegrees / 15;
     JogStep(numSteps);
}

