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
    public:
    PlaybackControl(QWidget *parent = 0);
    virtual ~PlaybackControl();

    signals:
        void playbackFirstFrame();
        void playbackFastRewind();
        void playbackPreviousFrame();
        void playbackPlay();
        void playbackNextFrame();
        void playbackFastForward();
        void playbackLastFrame();

    protected:
        QPushButton* m_btnFirstFrame;
		QPushButton* m_btnFastRewind;
		QPushButton* m_btnPreviousFrame;
		QPushButton* m_btnPlay;
		QPushButton* m_btnNextFrame;
		QPushButton* m_btnFastForward;
		QPushButton* m_btnLastFrame;
		QSlider* m_PlaybackSlider;
		QSlider* m_Shuttle;
		QLabel* m_txtShuttle;
		JogControl* m_Jog;
};
#endif // PLAYBACKCONTROL_H_INCLUDED
