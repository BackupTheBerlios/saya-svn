// **************************************************************************************
// * Name:      playbackcontrol.cpp
// * Purpose:   Implementation of class PlaybackControl
// * Authors:   Rigoberto Calleja
// * Created:   2009-02-06
// * Copyright: Rigoberto Calleja
// * License:   wxWindows license
// **************************************************************************************/

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
        void updatePositionInSeconds(double time); // Update the seeker's current position.
        void updatePositionInPercentage(double percentage); // Update the seeker's current position.

    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // PLAYBACKCONTROL_H_INCLUDED
