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
class QVBoxLayout;

#include <saya/core/sigslot.h>
#include <ui/widgets/generic/widget.h>
using namespace sigslot;

class PlaybackControl : public syWidget
{
    public:
        PlaybackControl(QWidget *parent = 0);
        virtual ~PlaybackControl();

        signal0 playbackFirstFrame;
        signal0 playbackFastRewind;
        signal0 playbackPreviousFrame;
        signal0 playbackPlay;
        signal0 playbackStop;
        signal0 playbackNextFrame;
        signal0 playbackFastForward;
        signal0 playbackLastFrame;
        signal1<double> playbackSeekAndPlayFrame; /** Seeks frame at given time (in seconds) and plays it, then pauses. */
        signal1<int> playbackAtSpeed; /** Sets playback speed at given percentage. Can be negative,greater than 100. */
        signal1<unsigned int> playbackSetVolume; /** Sets playback volume. From 0 to 100. */

    public: // slots
        void setTimeRange(double start, double finish); // In seconds
        void setCurrentTime(double time); // Update the seek bar's current position.

    protected:
        QVBoxLayout* GetVBoxLayout(); // For VideoPlaybackControl
    private:
        class Data;
        friend class Data;
        Data* m_Data;
};
#endif // PLAYBACKCONTROL_H_INCLUDED
