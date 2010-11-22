/**************************************************************************************
 * Name:      videopanel.h
 * Purpose:   declaration of class VideoPanel
 * Authors:   Ricardo Garcia
 * Created:   2009-02-06
 * Modified:  2010-11-21
 * Copyright: Ricardo Garcia
 * License:   LGPL Licence version 3 or later
 **************************************************************************************/

#ifndef VIDEOPANEL_H_INCLUDED
#define VIDEOPANEL_H_INCLUDED

#include <QWidget>

class VideoOutputDevice;
class syBitmapSink;

class VideoPanel : public QWidget {
    public:
        /**
         * Constructor.
         *
         * @param parent The parent widget.
         */
        VideoPanel(QWidget *parent);

        /**
         * Gets the currently assigned VideoOutputDevice
         */
        VideoOutputDevice* GetVideo() const;

        /**
         * Gets the corresponding Bitmap Sink
         */
        syBitmapSink* GetBitmapSink() const;

        /**
         * Standard Destructor
         */
        virtual ~VideoPanel();

    protected:
        void paintEvent ( QPaintEvent * pe );
        void resizeEvent ( QResizeEvent * event );

    private:
        class Data;
        friend class Data;
        Data *m_Data;
};

#endif