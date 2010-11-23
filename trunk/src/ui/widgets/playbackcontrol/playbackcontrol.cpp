/**************************************************************************************
 * Name:      playbackcontrol.cpp
 * Purpose:   Implementation of class PlaybackControl
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL license version 3 or later, with linking exception.
 **************************************************************************************/
#include <QtGui>
#include "playbackcontrol.h"
#include "../jog_ctrl/jog_ctrl.h"
#include "../tubeslider/tubeslider.h"

// ---------------------------
// Begin PlaybackControl::Data
// ---------------------------

class PlaybackControl::Data : public QObject {
    Q_OBJECT
    public:
        Data(PlaybackControl* parent);
        virtual ~Data();
        static const char* s_icon_firstframe;
        static const char* s_icon_fastrewind;
        static const char* s_icon_prevframe;
        static const char* s_icon_play;
        static const char* s_icon_nextframe;
        static const char* s_icon_fastforward;
        static const char* s_icon_lastframe;
        static const char* s_icon_mute1;
        static const char* s_icon_mute2;

        PlaybackControl* m_Parent;
        double m_StartTime, m_EndTime;

        QPixmap* m_pixVolumeNormal;
        QPixmap* m_pixVolumeMuted;
        QPushButton* m_btnMute;

        QPushButton* m_btnFirstFrame;
		QPushButton* m_btnFastRewind;
		QPushButton* m_btnPreviousFrame;
		QPushButton* m_btnPlay;
		QPushButton* m_btnNextFrame;
		QPushButton* m_btnFastForward;
		QPushButton* m_btnLastFrame;
		TubeSlider* m_PlaybackSlider;
		QSlider* m_VolumeSlider;
		QSlider* m_Shuttle;
		JogControl* m_Jog;
		QVBoxLayout* m_VBoxLayout;

    signals:
        void playbackSetVolume(unsigned int percentage); // from 0 to 100.
        void playbackAtSpeed(int percentage);

    public slots:
        void PlaybackSliderMoved(double time);
        void volumeSliderPressed();
        void volumeSliderMoved(int value);
        void volumeSliderReleased();
        void muteButtonToggled(bool checked);

        void shuttleSliderPressed();
        void shuttleSliderMoved(int value);
        void shuttleSliderReleased();

    protected:
        void showSeekTooltip();
        void MovePlaybackSlider();
        bool eventFilter(QObject *obj, QEvent *event);
};

PlaybackControl::Data::Data(PlaybackControl* parent) :
m_Parent(parent),
m_StartTime(0),
m_EndTime(10),
m_pixVolumeNormal(0),
m_pixVolumeMuted(0),
m_VBoxLayout(0)
{

    m_pixVolumeNormal = new QPixmap(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_mute1))));
    m_pixVolumeMuted = new QPixmap(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_mute2))));

    m_PlaybackSlider = new TubeSlider();
    m_PlaybackSlider->setFocusPolicy(Qt::NoFocus);

    m_btnMute = new QPushButton();
    m_btnMute->setCheckable(true);
    m_btnMute->setChecked(false);
    m_btnMute->setFocusPolicy(Qt::NoFocus);
    m_btnMute->setIcon(*m_pixVolumeNormal);

    m_VolumeSlider = new QSlider(Qt::Horizontal);
    m_VolumeSlider->setFocusPolicy(Qt::NoFocus);
    m_VolumeSlider->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_VolumeSlider->setMaximumWidth(72);
    m_VolumeSlider->setMinimumWidth(72);
    m_VolumeSlider->setMinimum(0);
    m_VolumeSlider->setMaximum(100);
    m_VolumeSlider->setValue(100);
    m_VolumeSlider->setTickInterval(10);
    m_VolumeSlider->setTickPosition(QSlider::TicksBelow);

    m_btnFirstFrame = new QPushButton();m_btnFirstFrame->setCursor(Qt::PointingHandCursor);
    m_btnFastRewind = new QPushButton();m_btnFastRewind->setCursor(Qt::PointingHandCursor);
    m_btnPreviousFrame = new QPushButton();m_btnPreviousFrame->setCursor(Qt::PointingHandCursor);
    m_btnPlay = new QPushButton();m_btnPlay->setCursor(Qt::PointingHandCursor);
    m_btnNextFrame = new QPushButton();m_btnNextFrame->setCursor(Qt::PointingHandCursor);
    m_btnFastForward = new QPushButton();m_btnFastForward->setCursor(Qt::PointingHandCursor);
    m_btnLastFrame = new QPushButton();m_btnLastFrame->setCursor(Qt::PointingHandCursor);
    m_Jog = new JogControl();
    m_Jog->setFocusPolicy(Qt::NoFocus);
    m_Jog->setMinimumSize(40, 40);
    m_Jog->setMaximumSize(40, 40);

    m_btnMute->setMaximumSize(QSize(20, 20));
    m_btnMute->setMinimumSize(QSize(20, 20));
    m_btnMute->setFocusPolicy(Qt::NoFocus);
    m_btnMute->setCursor(Qt::PointingHandCursor);

    m_btnFirstFrame->setMaximumSize(QSize(24, 24));
    m_btnFirstFrame->setFocusPolicy(Qt::NoFocus);
    m_btnFastRewind->setMaximumSize(QSize(24, 24));
    m_btnFastRewind->setFocusPolicy(Qt::NoFocus);
    m_btnPreviousFrame->setMaximumSize(QSize(24, 24));
    m_btnPreviousFrame->setFocusPolicy(Qt::NoFocus);
    m_btnPlay->setMaximumSize(QSize(40, 36));
    m_btnPlay->setMinimumSize(QSize(40, 36));
    m_btnPlay->setFocusPolicy(Qt::NoFocus);
    m_btnNextFrame->setMaximumSize(QSize(24, 24));
    m_btnNextFrame->setFocusPolicy(Qt::NoFocus);
    m_btnFastForward->setMaximumSize(QSize(24, 24));
    m_btnFastForward->setFocusPolicy(Qt::NoFocus);
    m_btnFastForward->setIconSize(QSize(24,24));
    m_btnFastForward->setFocusPolicy(Qt::NoFocus);
    m_btnLastFrame->setMaximumSize(QSize(24, 24));
    m_btnLastFrame->setFocusPolicy(Qt::NoFocus);

    m_btnFirstFrame->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_firstframe))));
    m_btnFastRewind->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_fastrewind))));
    m_btnPreviousFrame->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_prevframe))));
    m_btnPlay->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_play))));
    m_btnNextFrame->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_nextframe))));
    m_btnFastForward->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_fastforward))));
    m_btnLastFrame->setIcon(QPixmap::fromImage(QImage::fromData(QByteArray::fromBase64(Data::s_icon_lastframe))));

    m_Shuttle = new QSlider(Qt::Horizontal);
    m_Shuttle->setMinimumWidth(84);
    m_Shuttle->setMaximumWidth(84);
    m_Shuttle->setFocusPolicy(Qt::NoFocus);
    m_Shuttle->setMinimum(-400);
    m_Shuttle->setMaximum(400);
    m_Shuttle->setValue(0);

    // Layouts section

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_PlaybackSlider);
    QFrame* vline1 = new QFrame();
    vline1->setFrameStyle(QFrame::VLine | QFrame::Sunken);
    topLayout->addWidget(vline1);
    topLayout->addWidget(m_btnMute);
    topLayout->addWidget(m_VolumeSlider);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(7);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_btnFirstFrame);
    bottomLayout->addWidget(m_btnFastRewind);
    bottomLayout->addWidget(m_btnPreviousFrame);
    bottomLayout->addWidget(m_btnPlay);
    bottomLayout->addWidget(m_btnNextFrame);
    bottomLayout->addWidget(m_btnFastForward);
    bottomLayout->addWidget(m_btnLastFrame);
    bottomLayout->addWidget(m_Shuttle);
    bottomLayout->addWidget(m_Jog);
    bottomLayout->addStretch();

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    m_VBoxLayout = mainLayout;
    m_Parent->setLayout(mainLayout);
    m_Parent->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Fixed);

    m_Parent->setStyleSheet(
        "QWidget { background-color: #ccc; }"
        "QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #474747, stop: 1 #999999);"
        "  border:2px outset #666666; border-radius: 3px; }"
        "QPushButton:pressed { margin-top:2px;margin-left:2px; border-right:none;border-bottom:none;border-top-right-radius:0;border-bottom-left-radius:0; }"
    );
    m_btnMute->setStyleSheet(
        "QPushButton { border:1px outset #666666; background-color: transparent; } "
        "QPushButton:checked { border:1px inset #666666; background-color: #999999; } "
    );
    // In this section the private slots connections are setup. We use Queued Connections
    // to support events from background threads.
    connect(m_btnFirstFrame,    SIGNAL(clicked()), m_Parent, SIGNAL(playbackFirstFrame()), Qt::QueuedConnection);
    connect(m_btnFastRewind,    SIGNAL(clicked()), m_Parent, SIGNAL(playbackFastRewind()), Qt::QueuedConnection);
    connect(m_btnPreviousFrame, SIGNAL(clicked()), m_Parent, SIGNAL(playbackPreviousFrame()), Qt::QueuedConnection);
    connect(m_btnPlay,          SIGNAL(clicked()), m_Parent, SIGNAL(playbackPlay()), Qt::QueuedConnection);
    connect(m_btnNextFrame,     SIGNAL(clicked()), m_Parent, SIGNAL(playbackNextFrame()), Qt::QueuedConnection);
    connect(m_btnFastForward,   SIGNAL(clicked()), m_Parent, SIGNAL(playbackFastForward()), Qt::QueuedConnection);
    connect(m_btnLastFrame,     SIGNAL(clicked()), m_Parent, SIGNAL(playbackLastFrame()), Qt::QueuedConnection);
    connect(m_Jog,              SIGNAL(JogStepUp()),m_Parent, SIGNAL(playbackNextFrame()), Qt::QueuedConnection);
    connect(m_Jog,              SIGNAL(JogStepDown()),m_Parent, SIGNAL(playbackPreviousFrame()), Qt::QueuedConnection);
    connect(m_btnFirstFrame,    SIGNAL(clicked()), m_Parent, SIGNAL(playbackFirstFrame()), Qt::QueuedConnection);

    // --- Signals and slots for the playback slider ---
    connect(m_PlaybackSlider, SIGNAL(TimeChanged(double)), this, SLOT(PlaybackSliderMoved(double)), Qt::QueuedConnection);
    // --- Signals and slots for volume handling ---
    connect(m_btnMute, SIGNAL(toggled(bool)), this, SLOT(muteButtonToggled(bool)), Qt::QueuedConnection);
    connect(m_VolumeSlider, SIGNAL(sliderPressed()), m_Parent, SIGNAL(volumeSliderPressed()), Qt::QueuedConnection);
    connect(m_VolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(volumeSliderMoved(int)), Qt::QueuedConnection);
    connect(m_VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(volumeSliderMoved(int)), Qt::QueuedConnection);
    connect(m_VolumeSlider, SIGNAL(sliderReleased()), this, SLOT(volumeSliderReleased()), Qt::QueuedConnection);
    connect(this, SIGNAL(playbackSetVolume(unsigned int)), m_Parent, SIGNAL(playbackSetVolume(unsigned int)), Qt::QueuedConnection);

    // --- Signals and slots for the shuttle control ---
    connect(m_Shuttle, SIGNAL(sliderPressed()), m_Parent, SIGNAL(shuttleSliderPressed()), Qt::QueuedConnection);
    connect(m_Shuttle, SIGNAL(sliderMoved(int)), this, SLOT(shuttleSliderMoved(int)), Qt::QueuedConnection);
    connect(m_Shuttle, SIGNAL(sliderReleased()), this, SLOT(shuttleSliderReleased()), Qt::QueuedConnection);
    connect(m_Shuttle, SIGNAL(sliderReleased()), m_Parent, SIGNAL(playbackStop()), Qt::QueuedConnection);
    connect(this, SIGNAL(playbackAtSpeed(int)), m_Parent, SIGNAL(playbackAtSpeed(int)), Qt::QueuedConnection);
    m_Shuttle->installEventFilter(this);
}

PlaybackControl::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
    delete m_pixVolumeMuted;
    m_pixVolumeMuted = 0;
    delete m_pixVolumeNormal;
    m_pixVolumeNormal = 0;

}

// --- Event filtering ---
bool PlaybackControl::Data::eventFilter(QObject *obj, QEvent *event) {
    if((obj == m_Shuttle) && event->type() == QEvent::Wheel) {
        return true; // Swallow the wheel events for the Shuttle controller
    }
    return QObject::eventFilter(obj, event);
}

// --- Signal handling for the Playback slider ---

void PlaybackControl::Data::PlaybackSliderMoved(double time) {
    emit m_Parent->playbackSeekAndPlayFrame(time);
}

// --- Signal handling for the Mute button ---

void PlaybackControl::Data::muteButtonToggled(bool muted) {
    if(muted) {
        m_btnMute->setIcon(*m_pixVolumeMuted);
        emit playbackSetVolume(0);
    } else {
        m_btnMute->setIcon(*m_pixVolumeNormal);
        emit playbackSetVolume(m_VolumeSlider->value());
    }
    m_Parent->update();
}

// --- Signal handling for the Volume Slider ---

void PlaybackControl::Data::volumeSliderPressed() {
    QString s = QString("Volume: %1\%").arg(m_VolumeSlider->value());
    if(m_btnMute->isChecked()) {
        s += " (muted)";
    }
    QPoint tmppos = QPoint(QCursor::pos().x(),m_VolumeSlider->mapToGlobal(QPoint(0,16)).y());
    QToolTip::showText (tmppos, s, m_Parent);
}

void PlaybackControl::Data::volumeSliderMoved(int value) {
    QString s = QString("Volume: %1\%").arg(m_VolumeSlider->value());
    if(m_btnMute->isChecked()) {
        s += " (muted)";
    }
    QPoint tmppos = QPoint(QCursor::pos().x(),m_VolumeSlider->mapToGlobal(QPoint(0,16)).y());
    QToolTip::showText (tmppos, s, m_Parent);
    if(!m_btnMute->isChecked()) {
        emit playbackSetVolume(value);
    }
}

void PlaybackControl::Data::volumeSliderReleased() {
    QToolTip::showText (QPoint(0,0), "", m_Parent);
}

// --- Signal handling for the Shuttle Slider ---

void PlaybackControl::Data::shuttleSliderPressed() {
    double speed = (double)m_Shuttle->value() / 100;
    QToolTip::showText (QCursor::pos(), "", m_Parent);
    QPoint tmppos = QPoint(QCursor::pos().x(),m_Shuttle->mapToGlobal(QPoint(0,16)).y());
    QToolTip::showText (tmppos, QString("Playing at %1x").arg(speed, 5, 'f', 2), m_Parent);
}

void PlaybackControl::Data::shuttleSliderMoved(int value) {
    double speed = (double)m_Shuttle->value() / 100;
    QPoint tmppos = QPoint(QCursor::pos().x(),m_Shuttle->mapToGlobal(QPoint(0,16)).y());
    QToolTip::showText (tmppos, QString("Playing at %1x").arg(speed, 5, 'f', 2), m_Parent);
    emit playbackAtSpeed(value);
}

void PlaybackControl::Data::shuttleSliderReleased() {
    m_Shuttle->setValue(0);
    QToolTip::showText(QPoint(0,0), "", m_Parent);
}

// -------------------------
// End PlaybackControl::Data
// -------------------------

// ---------------------
// Begin PlaybackControl
// ---------------------

PlaybackControl::PlaybackControl(QWidget* parent)
: QWidget (parent)
, m_Data(new Data(this))
{
    setTimeRange(0,10);
}

PlaybackControl::~PlaybackControl() {
    delete m_Data;
    m_Data = 0;
}

void PlaybackControl::setTimeRange(double start, double finish) {
    m_Data->m_PlaybackSlider->setTimeRange(start, finish);
}

void PlaybackControl::setCurrentTime(double time) {
    m_Data->m_PlaybackSlider->setCurrentTime(time);
}

QVBoxLayout* PlaybackControl::GetVBoxLayout() {
    return m_Data->m_VBoxLayout;
}

// -------------------
// End PlaybackControl
// -------------------

#ifndef Q_MOC_RUN
  #include "moc/playbackcontrol_data.moc.h"
  #include "moc/playbackcontrol.moc.h"
#endif

const char* PlaybackControl::Data::s_icon_fastforward =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A"
"/wD/oL2nkwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB9oLDxAtA/XHHIsAAAC+SURBVDjL"
"YwwND2agBDAxUAhgBvyHYlwApzzcBYUFRQz4DMElz0SMInzyKAZcunwRryHY5FEMYGFhYXjx8jlO"
"Q9DlV69cK4ZiADc3NwMvDx/D379/GSorqjAMQZZPT8tgCA0Pfkl2NAoJCWF6gYebl4GLi4uBl5eX"
"ob2jjYGBgYGRkDxGGAgJCTG0trVgaMYlj2IAPz8/Q119LVbNuORRDOif0IdTMy55RmhmgoU2I56k"
"jN1bBDQyEJKnODcCAFIyQcHsLrJyAAAAAElFTkSuQmCC";
const char* PlaybackControl::Data::s_icon_fastrewind =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A"
"/wD/oL2nkwAAAL5JREFUOMvF008OwUAYBfA3X2bVMZrMwsVU9QRYYoUEG8oKxxBxGRyCHqIZqyZt"
"fTMSXZjt773J/BXWWjQZhIaDnSBOIhsnkXNpZScOJ+MpfOWyU5NyZYIC74+bt1x3EkJ0CnxlT0gp"
"nWXOqdfvZsPBCHmeQ7faUEp9lOezhdMJAIwxv1/j5XwVu30KrTWCIEBL6Urgm1MR2qYbGGPYM/A5"
"lUOr9RJhGLJLdTnVQ8fTwblfziUX8j3juou//8Y32oBmx0sa2OIAAAAASUVORK5CYII=";
const char* PlaybackControl::Data::s_icon_firstframe =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAAAA"
"/wAAR9uPkgAAAPRJREFUOMulk01uwjAQhd/Y40XimES5GqrEMfjZlEoFWrU0dFE4RqQoh4EuW9al"
"h0hgFckhiQHxVrY879PMs0044i5xtXgY9BuoLM2pzVTVZmlObB+MRxMwM4gIq88ELnMlYW+klPjd"
"/0Ap5TSPhuN2AACUZQkp5VXmBoCIwMwNgG3efW+7AUIIKKUghGg1H/7/wMzdgC5NH59QFAVM0IPW"
"+naASzUAM0P7QW0EAPhIljDGwPd9BNq4MzifsXpM78s3xHF8OYPzAhsymz8jDEP3NXqe1xjBhqw3"
"X+4OoijqDKwNUuv35XVxMfUszcn+D3Tvdz4BzdpDLH4w+NUAAAAASUVORK5CYII=";
const char* PlaybackControl::Data::s_icon_lastframe =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAAAA"
"/wAAR9uPkgAAAPRJREFUOMulkztOxDAQhj+Px0VeJMrVEBLHAAo2iwQ0sNCwyzGQEIcBSqAG7pAH"
"VdAma0cr5a/s8e9vpHkYOmZJAY6ODzuA56cX4zP179vqvTo2hiCL84qu66jrmvXm4T8u/eH05CyY"
"DcA5x+fXB9baQVy2L1MQay1t2+6AB4C399cgxFqLqmKMCQNUlZ/fby9ERHDOISJhQJIkZOkBTdOw"
"rC72aqMwUwNAmmTEcUyWZdzdr4ZGEZI4RVWna1CWJberG3yDparTNcjznKvrS0JTOc6+A9g8roOf"
"RYQoisJtnPrcqygK/zKNF8Snarnwxs3cdf4Dq1xJE5ZkvjkAAAAASUVORK5CYII=";
const char* PlaybackControl::Data::s_icon_nextframe =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAAAA"
"/wAAR9uPkgAAAJJJREFUOMtjZPjPQBFgYqAQsGATDI0Ihrtr9Yq1jGS5oLCgCMMwkr1AjCF4Dbh0"
"+SJBQ/AawMLCwvDi5XO8huA1gJubm4GXh4/h79+/DJUVVbSJRrwG8HDzMnBxcTHw8vIytHe0MZAV"
"BkJCQgytbS040wReA/j5+Rnq6mvxJii8BvRP6COYGhmxZSZSkjLjgOdGAMS0L2/7Do6AAAAAAElF"
"TkSuQmCC";
const char* PlaybackControl::Data::s_icon_play =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAAAA"
"/wAAR9uPkgAAAPNJREFUOMulk89Kw0AQh79kN4RsugZytT6VCD5GW7GtQrWKVk/VxyiID6Me1ZOK"
"+gghJPXUpWuy4tI57R/mm99vmAlYslFIgL393QbmbnEf/P5bva1HuDoM+gcMD0eMhuNGlX5vgKuQ"
"AQgheHl9JoqiVqkuSLh+qesaIYTTbxvEUpAkiVMBwOPTQwMSenVcSr6+Py2IAcRxTHd7h7IsnYA0"
"TdGdLaqq4mh8jLeCtjCAoih4/3j7swedVKOUQmvN1fXMBvzHgpSSPM+5nF2YwfKykGUZJ6cTayq9"
"ADe388ZIWwCllFeyWSaAs/OpdzJAsOk6/wDciFEbzGf0igAAAABJRU5ErkJggg==";
const char* PlaybackControl::Data::s_icon_prevframe =
"iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAAXNSR0IArs4c6QAAAAZiS0dEAAAA"
"/wAAR9uPkgAAAJJJREFUOMtjZPjPQBFgYqAQsBBSEBoRDHfj6hVrGUlyAUxzYUER6V4gRjNOA5A1"
"X7p8kTQDkDW/ePmcgYWFhXQXVFZUMfz9+5eBl4ePgZubm7bRiNWA9o42Bl5eXgYuLi4GHm5e0gyA"
"xXVrWwuDkJAQeWEAM6SuvpaBn5+fvDCAGdI/oQ+vAYyEMhOhpMw44LkRANDbL2/n+GLWAAAAAElF"
"TkSuQmCC";

const char* PlaybackControl::Data::s_icon_mute1 =
"iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A"
"/wD/oL2nkwAAAR9JREFUOMvNlNtKw0AQhr9Z7BuK+AJiFRTfQywKkppSaz2gIr6b8ap7iBkvzMZA"
"UkyTXrhXy+zsx/yz/46oKttchi2vnb4X9/Z3K2lvr+8yqMIIe356GS45wh7uHsnD1zDJETafLfAh"
"dOthvTexP/VYcj3FOQeigHSrME1mqAjjo4Mqdjm5QhWc84gAKt0qNMaElbUjERCRIsZ9CBA9K/K7"
"/wtYFMUo+8wAUNXq0ZxzVY4AuokPs+yj0Z885Chayl2PbQVaaxu5PnhAEaQ82uBRrF01kr33pdwY"
"11bZrcDl/RJjDEDl3IvJOQCnJ2e16ppI2WTaRD+OD49Bfqw4TZP+fzlevEkTgvdlXwf+5Qi9XcwJ"
"LUDpO2DXjS/59xP7G8UtkUq8IPGEAAAAAElFTkSuQmCC";

const char* PlaybackControl::Data::s_icon_mute2 =
"iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAAAXNSR0IArs4c6QAAAAZiS0dEAP8A"
"/wD/oL2nkwAAAUdJREFUOMvNlDFLw0AUx/8X1E7ODh0EP0QmVwcnkWuaQgc3dbfol3B3cBY0tFSw"
"EK2Cs139Bs6CQkEa+l6eg15IzJUcmsEHB7nc5cf7v3/eUyKCOsNDzbH02w/bnVYmLbrsqz9laGDH"
"RyfVkicNJZOGkipYbzhAuum719AGzcOcTTnd1SVou9MSG+xj/OiW4TS+L0EP9g8LsGn8ACaqBnqe"
"NydivN3E2bvecIDV7a1s/z6KQTQHswMwTdNlZgIzF+SbeL0egYjBzCBmt/+QmKCgrEVnZnx1l3Lv"
"FCYGEVndXNM739mRm2QASGYzNENdcP7lIsr2zVCDiaySrcCNvW727CeiACDqXxVqut4NrS5DRArr"
"aQVi1s+zINQShFryd4JQF+4t7BSTWT7MELC5v1Cyn4iywWzQ57Pzeuahgd6Nb0tn6t9P7E/VddD0"
"nDknqQAAAABJRU5ErkJggg==";
