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
#include "../../../saya/inputmonitor.h"
#include "../videopanel/videopanel.h"

// ----------------------------------------
// --- Begin VideoPlaybackControl::Data ---
// ----------------------------------------

class VideoPlaybackControl::Data : public QObject {
    Q_OBJECT
    public:
        Data(VideoPlaybackControl* parent);
        virtual ~Data();
        VideoPlaybackControl *m_Parent;
        VideoPanel* m_VideoPanel;

    public slots:

};

VideoPlaybackControl::Data::Data(VideoPlaybackControl* parent) :
m_Parent(parent),
m_VideoPanel(0)
{

}

VideoPlaybackControl::Data::~Data() {
    m_Parent->m_Data = 0;
    delete m_Parent;
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
}

VideoPlaybackControl::~VideoPlaybackControl() {
    delete m_Data;
    m_Data = 0;
}

void VideoPlaybackControl::SetAVPlayer(AVPlayer* player) {
    m_Player = player;
    if(m_Player) {
        #warning TODO: Implement a Video Panel class and give it to m_Player
        // m_Player->SetVideoOut(static_cast<VideoPanel*>(m_Data->m_VideoPanel)->GetVideo());
        m_Player->Init();
    }
}

QWidget* VideoPlaybackControl::GetVideoPanel() {
    return m_Data->m_VideoPanel;
}

// --------------------------------
// --- End VideoPlaybackControl ---
// --------------------------------

#include "moc/videoplaybackcontrol_data.moc.h"
