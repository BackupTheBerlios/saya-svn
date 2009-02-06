// **************************************************************************************
// * Name:      playbackcontrol.cpp
// * Purpose:   Implementation of class PlaybackControl
// * Authors:   Rigoberto Calleja
// * Created:   2009-02-06
// * Copyright: Rigoberto Calleja
// * License:   GPL license
// **************************************************************************************/
#include <QtGui>
#include "playbackcontrol.h"
#include "playbackcontrol.ui.h"

PlaybackControl::PlaybackControl(QWidget* parent)
    : QWidget (parent)
{
    m_PlaybackSlider = new QSlider(Qt::Horizontal);

    m_btnFirstFrame = new QPushButton();
    m_btnFirstFrame->setIcon(QIcon(":../resources/img/btn_firstframe.png"));

    m_btnFastRewind = new QPushButton();
    m_btnFastRewind->setIcon(QIcon("../resources/img/btn_fastrewind.png"));

    m_btnPreviousFrame = new QPushButton();
    m_btnPreviousFrame->setIcon(QIcon("../resources/img/btn_previousframe.png"));

    m_btnPlay = new QPushButton();
    m_btnPlay->setIcon(QIcon("../resources/img/btn_play.png"));

    m_btnNextFrame = new QPushButton();
    m_btnNextFrame->setIcon(QIcon("../resources/img/btn_nextframe.png"));

    m_btnFastForward = new QPushButton();
    m_btnFastForward->setIcon(QIcon("../resources/img/btn_fastforward.png"));

    m_btnLastFrame = new QPushButton();
    m_btnLastFrame->setIcon(QIcon("../resources/img/btn_lastframe.png"));

    m_Shuttle = new QSlider(Qt::Horizontal);
    m_Shuttle->setMinimum(0);
    m_Shuttle->setMaximum(100);
    m_Shuttle->setValue(50);

    m_txtShuttle = new QLabel(tr("[Display]"));

    // Layouts section

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_PlaybackSlider);

    QHBoxLayout* bottomLeftLayout = new QHBoxLayout();
    bottomLeftLayout->addWidget(m_btnFirstFrame);
    bottomLeftLayout->addWidget(m_btnFastRewind);
    bottomLeftLayout->addWidget(m_btnPreviousFrame);
    bottomLeftLayout->addWidget(m_btnPlay);
    bottomLeftLayout->addWidget(m_btnNextFrame);
    bottomLeftLayout->addWidget(m_btnFastForward);
    bottomLeftLayout->addWidget(m_btnLastFrame);

    QHBoxLayout* bottomRightLayout = new QHBoxLayout();
    bottomRightLayout->addWidget(m_Shuttle);
    bottomRightLayout->addWidget(m_txtShuttle);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addLayout(bottomLeftLayout);
    bottomLayout->addLayout(bottomRightLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    // In this section the private slots connections are setup
    connect(m_btnFirstFrame, SIGNAL(clicked()),
            this, SLOT(firstFrameClicked()));
    connect(m_btnFastRewind, SIGNAL(clicked()),
            this, SLOT(fastRewindClicked()));
    connect(m_btnPreviousFrame, SIGNAL(clicked()),
            this, SLOT(fastRewindClicked()));
    connect(m_btnPlay, SIGNAL(clicked()),
            this, SLOT(playClicked()));
    connect(m_btnNextFrame, SIGNAL(clicked()),
            this, SLOT(nextFrameClicked()));
    connect(m_btnFastForward, SIGNAL(clicked()),
            this, SLOT(fastForwardClicked()));
    connect(m_btnLastFrame, SIGNAL(clicked()),
            this, SLOT(lastFrameClicked()));
}


// This section contains the implementation for the class' private slots

void PlaybackControl::firstFrameClicked()
{
    emit playbackFirstFrame();
}

void PlaybackControl::fastRewindClicked()
{
    emit playbackFastRewind();
}

void PlaybackControl::previousFrameClicked()
{
    emit playbackPreviousFrame();
}

void PlaybackControl::playClicked()
{
    emit playbackPlay();
}

void PlaybackControl::nextFrameClicked()
{
    emit playbackNextFrame();
}

void PlaybackControl::fastForwardClicked()
{
    emit playbackFastForward();
}

void PlaybackControl::lastFrameClicked()
{
    emit playbackLastFrame();
}
