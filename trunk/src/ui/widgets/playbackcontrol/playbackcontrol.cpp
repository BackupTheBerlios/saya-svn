/**************************************************************************************
 * Name:      playbackcontrol.cpp
 * Purpose:   Implementation of class PlaybackControl
 * Authors:   Rigoberto Calleja, Ricardo Garcia
 * Created:   2009-02-06
 * Copyright: Ricardo Garcia
 * License:   LGPL license version 3 or later
 **************************************************************************************/
#include <QtGui>
#include "playbackcontrol.h"
#include "../jog_ctrl/jog_ctrl.h"

class PlaybackControl::Data : public QObject {
    public:
    class Style : public QWindowsStyle
    {
    public:
      Style()
      {}
      void drawControl ( ControlElement element, const QStyleOption * option,
        QPainter * painter, const QWidget * widget = 0 ) const
      {
        if(element == CE_PushButton) {
          const QStyleOptionButton *b = qstyleoption_cast<const QStyleOptionButton *>(option);
          QStyleOptionButton *btn = (QStyleOptionButton *)b;
          if (btn) {
            if (btn->state & State_HasFocus) {
              btn->state = btn->state ^ State_HasFocus;
            }
          }
          // QWindowsStyle::drawControl(element, btn, painter, widget);

        } else {
          // QWindowsStyle::drawControl(element, option, painter, widget);
        }}};
        Data(PlaybackControl* parent);
        virtual ~Data();
        static const char* s_icon_firstframe;
        static const char* s_icon_fastrewind;
        static const char* s_icon_prevframe;
        static const char* s_icon_play;
        static const char* s_icon_nextframe;
        static const char* s_icon_fastforward;
        static const char* s_icon_lastframe;
        PlaybackControl* m_Parent;
};

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

PlaybackControl::Data::Data(PlaybackControl* parent) :
m_Parent(parent)
{
}

PlaybackControl::Data::~Data() {
    if(m_Parent) {
        m_Parent->m_Data = 0;
        m_Parent = 0;
    }
}

PlaybackControl::PlaybackControl(QWidget* parent)
: QWidget (parent)
, m_Data(new Data(this))
{
    m_PlaybackSlider = new QSlider(Qt::Horizontal);
    m_PlaybackSlider->setFocusPolicy(Qt::NoFocus);

    m_btnFirstFrame = new QPushButton(this);
    m_btnFastRewind = new QPushButton(this);
    m_btnPreviousFrame = new QPushButton(this);
    m_btnPlay = new QPushButton(this);
    m_btnNextFrame = new QPushButton(this);
    m_btnFastForward = new QPushButton(this);
    m_btnLastFrame = new QPushButton(this);
    m_Jog = new JogControl(this);
    m_Jog->setFocusPolicy(Qt::NoFocus);

    m_btnFirstFrame->setMaximumSize(QSize(24, 24));
    m_btnFirstFrame->setFocusPolicy(Qt::NoFocus);
    m_btnFastRewind->setMaximumSize(QSize(24, 24));
    m_btnFastRewind->setFocusPolicy(Qt::NoFocus);
    m_btnPreviousFrame->setMaximumSize(QSize(24, 24));
    m_btnPreviousFrame->setFocusPolicy(Qt::NoFocus);
    m_btnPlay->setMaximumSize(QSize(24, 24));
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

    m_Shuttle = new QSlider(Qt::Horizontal, this);
    m_Shuttle->setFocusPolicy(Qt::NoFocus);
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
    bottomRightLayout->addWidget(m_Jog);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addLayout(bottomLeftLayout);
    bottomLayout->addLayout(bottomRightLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    setStyleSheet(
        "QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #474747, stop: 1 #999999);"
        "  border:2px outset #666666; border-radius: 3px; }"
        "QPushButton:pressed { margin-top:2px;margin-left:2px; border-right:none;border-bottom:none;border-top-right-radius:0;border-bottom-left-radius:0; }"
    );

    // In this section the private slots connections are setup. We use Queued Connections
    // to support events from background threads.
    connect(m_btnFirstFrame,    SIGNAL(clicked()), this, SIGNAL(playbackFirstFrame()), Qt::QueuedConnection);
    connect(m_btnFastRewind,    SIGNAL(clicked()), this, SIGNAL(playbackFastRewind()), Qt::QueuedConnection);
    connect(m_btnPreviousFrame, SIGNAL(clicked()), this, SIGNAL(playbackPreviousFrame()), Qt::QueuedConnection);
    connect(m_btnPlay,          SIGNAL(clicked()), this, SIGNAL(playbackPlay()), Qt::QueuedConnection);
    connect(m_btnNextFrame,     SIGNAL(clicked()), this, SIGNAL(playbackNextFrame()), Qt::QueuedConnection);
    connect(m_btnFastForward,   SIGNAL(clicked()), this, SIGNAL(playbackFastForward()), Qt::QueuedConnection);
    connect(m_btnLastFrame,     SIGNAL(clicked()), this, SIGNAL(playbackLastFrame()), Qt::QueuedConnection);
    connect(m_Jog,              SIGNAL(JogStepUp()),this, SIGNAL(playbackNextFrame()), Qt::QueuedConnection);
    connect(m_Jog,              SIGNAL(JogStepDown()),this, SIGNAL(playbackPreviousFrame()), Qt::QueuedConnection);
}

PlaybackControl::~PlaybackControl() {
    delete m_Data;
    m_Data = 0;
}

#ifndef Q_MOC_RUN
  #include "moc/playbackcontrol.moc.h"
#endif
#warning TODO: Move the playback control to the widgets directory and store the bitmaps as base64
#warning TODO: Fix the Q_OBJECT stuff in the playback control
