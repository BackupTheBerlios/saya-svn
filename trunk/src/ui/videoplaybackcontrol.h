// **************************************************************************************
// * Name:      videoplaybackcontrol.cpp
// * Purpose:   Implementation of class VideoPlaybackControl
// * Authors:   Rigoberto Calleja
// * Created:   2009-02-06
// * Copyright: Rigoberto Calleja
// * License:   wxWindows license
// **************************************************************************************/

#ifndef VIDEOPLAYBACKCONTROL_H_INCLUDED
#define VIDEOPLAYBACKCONTROL_H_INCLUDED

#include "playbackcontrol.h"
#include "../saya/core/avcontroller.h"

// These are the forward declarations for the classes used
// to implement de playback control. I chose to use forward declarations
// instead of header files to make compiling somewhat faster

class VideoPlaybackControl : public PlaybackControl
{
    Q_OBJECT

    public:
        VideoPlaybackControl(QWidget *parent = 0);
        void SetAVPlayer(AVPlayer* player);
        virtual ~VideoPlaybackControl();

    protected:
        QWidget* GetVideoPanel();
        AVPlayer* m_Player;
};
#endif // VIDEOPLAYBACKCONTROL_H_INCLUDED
