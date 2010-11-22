/**************************************************************************************
 * Name:      videoplaybackcontrol.h
 * Purpose:   declaration of class VideoPlaybackControl
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3 or later
 **************************************************************************************/

#ifndef VIDEOPLAYBACKCONTROL_H_INCLUDED
#define VIDEOPLAYBACKCONTROL_H_INCLUDED

#include "../playbackcontrol/playbackcontrol.h"
class AVPlayer;

class VideoPlaybackControl : public PlaybackControl
{
    public:
        VideoPlaybackControl(QWidget *parent = 0);
        virtual ~VideoPlaybackControl();
        void SetAVPlayer(AVPlayer* player);
    private:
        class Data;
        friend class Data;
        Data *m_Data;
    protected:
        QWidget* GetVideoPanel();
        AVPlayer* m_Player;
};
#endif // VIDEOPLAYBACKCONTROL_H_INCLUDED
