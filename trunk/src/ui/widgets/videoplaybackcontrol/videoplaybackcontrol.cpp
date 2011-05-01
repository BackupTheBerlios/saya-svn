/**************************************************************************************
 * Name:      videoplaybackcontrol.cpp
 * Purpose:   Implementation of class VideoPlaybackControl
 * Authors:   Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3 or later
 * Comments:  This widget depends both on the Qt library and the Saya library, as it
 *            serves as an interface between the UI and our application.
 **************************************************************************************/

#include "videoplaybackcontrol.h"
#include <saya/inputmonitor.h>
#include <ui/widgets/videopanel/videopanel.h>

#include <QVBoxLayout>
// ----------------------------------------
// --- Begin VideoPlaybackControl::Data ---
// ----------------------------------------

class VideoPlaybackControl::Data : public has_slots {
    public:
        Data(VideoPlaybackControl* parent);
        virtual ~Data();
        VideoPlaybackControl *m_Parent;
        VideoPanel* m_VideoPanel;
};

VideoPlaybackControl::Data::Data(VideoPlaybackControl* parent) :
m_Parent(parent),
m_VideoPanel(new VideoPanel(0))
{

}

VideoPlaybackControl::Data::~Data() {
    m_Parent->m_Data = 0;
    m_Parent = 0;
}

// --------------------------------------
// --- End VideoPlaybackControl::Data ---
// --------------------------------------



// ----------------------------------
// --- Begin VideoPlaybackControl ---
// ----------------------------------

VideoPlaybackControl::VideoPlaybackControl(QWidget *parent)
: PlaybackControl(parent),
m_Data(new Data(this)),
m_Player(0)
{
    m_Data->m_VideoPanel->setMinimumSize(178,100);
    m_Data->m_VideoPanel->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    QVBoxLayout* vboxlayout = GetVBoxLayout();
    vboxlayout->insertWidget(0,m_Data->m_VideoPanel);
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
}

VideoPlaybackControl::~VideoPlaybackControl() {
    delete m_Data;
    m_Data = 0;
}

void VideoPlaybackControl::SetAVPlayer(AVPlayer* player) {
    m_Player = player;
    if(m_Player) {
        m_Player->SetVideoOut(m_Data->m_VideoPanel->GetVideo());
        m_Player->Init();
    }
}

QWidget* VideoPlaybackControl::GetVideoPanel() {
    return m_Data->m_VideoPanel;
}

// --------------------------------
// --- End VideoPlaybackControl ---
// --------------------------------
