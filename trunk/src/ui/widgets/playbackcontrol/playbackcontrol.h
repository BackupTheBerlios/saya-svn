/**************************************************************************************
 * Name:      playbackcontrol.cpp
 * Purpose:   Implementation of class PlaybackControl
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL license version 3 or later, with linking exception
 **************************************************************************************/

#ifndef PLAYBACKCONTROL_H_INCLUDED
#define PLAYBACKCONTROL_H_INCLUDED
#include <QWidget>

// These are the forward declarations for the classes used
// to implement de playback control. I chose to use forward declarations
// instead of header files to make compiling somewhat faster

class QPushButton;
class QSlider;
class QLabel;
class JogControl;

class PlaybackControl : public QWidget
{
    Q_OBJECT
    public:
        PlaybackControl(QWidget *parent = 0);
        virtual ~PlaybackControl();

    signals:
        void playbackFirstFrame();
        void playbackFastRewind();
        void playbackPreviousFrame();
        void playbackPlay();
        void playbackStop();
        void playbackNextFrame();
        void playbackFastForward();
        void playbackLastFrame();
        void playbackSeekAndPlayFrame(double time);
        void playbackAtSpeed(int percentage); // Sets playback speed at given percentage. Can be negative,greater than 100.
        void playbackSetVolume(unsigned int percentage); // from 0 to 100.

    public slots:
        void setTimeRange(double start, double finish); // In seconds
        void setCurrentTime(double time); // Update the seek bar's current position.

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // PLAYBACKCONTROL_H_INCLUDED
