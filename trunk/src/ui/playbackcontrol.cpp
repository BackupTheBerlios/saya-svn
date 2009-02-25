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
#include "moc/playbackcontrol.moc.h"

PlaybackControl::PlaybackControl(QWidget* parent)
    : QWidget (parent)
{
    m_PlaybackSlider = new QSlider(Qt::Horizontal);

    m_btnFirstFrame = new QPushButton(this);
    m_btnFastRewind = new QPushButton(this);
    m_btnPreviousFrame = new QPushButton(this);
    m_btnPlay = new QPushButton(this);
    m_btnNextFrame = new QPushButton(this);
    m_btnFastForward = new QPushButton(this);
    m_btnLastFrame = new QPushButton(this);

    m_btnFirstFrame->setMaximumSize(QSize(24, 24));
    m_btnFastRewind->setMaximumSize(QSize(24, 24));
    m_btnPreviousFrame->setMaximumSize(QSize(24, 24));
    m_btnPlay->setMaximumSize(QSize(24, 24));
    m_btnNextFrame->setMaximumSize(QSize(24, 24));
    m_btnFastForward->setMaximumSize(QSize(24, 24));
    m_btnLastFrame->setMaximumSize(QSize(24, 24));

    m_btnFirstFrame->setIcon(QIcon(":img/btn_firstframe.png"));
    m_btnFastRewind->setIcon(QIcon(":img/btn_fastrewind.png"));
    m_btnPreviousFrame->setIcon(QIcon(":img/btn_prevframe.png"));
    m_btnPlay->setIcon(QIcon(":img/btn_play.png"));
    m_btnNextFrame->setIcon(QIcon(":img/btn_nextframe.png"));
    m_btnFastForward->setIcon(QIcon(":img/btn_fastforward.png"));
    m_btnLastFrame->setIcon(QIcon(":img/btn_lastframe.png"));

    m_Shuttle = new QSlider(Qt::Horizontal, this);
    m_Shuttle->setMinimum(0);
    m_Shuttle->setMaximum(100);
    m_Shuttle->setValue(50);

    m_txtShuttle = new QLabel(tr("[Display]"));

    // Layouts section

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_PlaybackSlider);

    QHBoxLayout* bottomLeftLayout = new QHBoxLayout();
    bottomLeftLayout->setSpacing(7);
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

    // In this section the private slots connections are setup. We use Queued Connections
    // to support events from background threads.
    connect(m_btnFirstFrame,    SIGNAL(clicked()), this, SIGNAL(playbackFirstFrame()), Qt::QueuedConnection);
    connect(m_btnFastRewind,    SIGNAL(clicked()), this, SIGNAL(playbackFastRewind()), Qt::QueuedConnection);
    connect(m_btnPreviousFrame, SIGNAL(clicked()), this, SIGNAL(playbackPreviousFrame()), Qt::QueuedConnection);
    connect(m_btnPlay,          SIGNAL(clicked()), this, SIGNAL(playbackPlay()), Qt::QueuedConnection);
    connect(m_btnNextFrame,     SIGNAL(clicked()), this, SIGNAL(playbackNextFrame()), Qt::QueuedConnection);
    connect(m_btnFastForward,   SIGNAL(clicked()), this, SIGNAL(playbackFastForward()), Qt::QueuedConnection);
    connect(m_btnLastFrame,     SIGNAL(clicked()), this, SIGNAL(playbackLastFrame()), Qt::QueuedConnection);
}

PlaybackControl::~PlaybackControl() {

}
