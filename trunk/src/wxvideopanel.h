/****************************************************************************
 * Name:      wxvideopanel.h
 * Purpose:   Declaration for classes wxVideoPanel and wxVideoOutputDevice
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-07-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code from sdlpanel.cc found at
 *            http://code.technoplaza.net/wx-sdl/part1/
 *            (LGPL licensed)
 ***************************************************************************/
#ifndef wxvideopanel_h
#define wxvideopanel_h

#include <wx/panel.h>
#include "iomgr/videooutputdevice.h"

class wxVideoOutputDevice;
class wxVideoPanel;
class syMutex;

/** @brief wxWidgets-specific implementation of the VideoOutputDevice class.
 *
 *  This wxVideoOutputDevice class will be tied to a wxVideoPanel and refresh its internal video buffer
 *  whenever necessary.
 *
 *  @see VideoOutputDevice
 */
class wxVideoOutputDevice : public VideoOutputDevice {
    friend class syVODBitmap;
    public:
        /** Initializes the Output Device based on a wxVideoPanel. */
        wxVideoOutputDevice(wxVideoPanel* panel);

        /** Standard destructor */
        virtual ~wxVideoOutputDevice();

        /** Gets the Video Bitmap Buffer
          * @return a pointer to the Bitmap
          */
        syVODBitmap* GetBitmap();

    protected:

        /** @brief Initializes the output device.
          *
          * Here we will set m_width, m_height variables according to the Panel's size.
          * m_colorformat will be set to vcfRGB24 by default, since that's the color format used by wxWidgets' wxImage.
          * @return True on success; false otherwise.
          */
        virtual bool InitializeOutput();

        /** Clears the panel (i.e. sets it all to black). Called by ShutDown(). */
        virtual void Clear();

        /** Disconnects the video output device or memory resource. Called by ShutDown(). */
        virtual void DisconnectOutput();

        /** @brief ChangeDeviceSize will be called by the Panel's Resize event.
          *
          * @param newwidth the new width to be set for playback
          * @param newheight the new height to be set for playback
          * @return true if size was changed, false otherwise.
          */
        virtual bool ChangeDeviceSize(unsigned int newwidth,unsigned int newheight);

        /** @brief Loads video data from an external buffer.
         *
         *  @param bitmap Buffer containing the data to be processed.
         *  @note  This method MUST do nothing if either m_width or m_height are set to 0.
         *  @note  When the data is finally converted, RenderData MUST be called.
         *  @note  This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
         */
        virtual void LoadDeviceVideoData(syBitmap* bitmap);


        /** Plays the received frames by Refreshing the panel's internal buffer.
          * @note This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
          * @note This method MUST do nothing if either m_width or m_height are set to 0.
          */
        virtual void RenderData();

    private:

        /** Holds a temporary buffer for the video data */
        syVODBitmap* m_Bitmap;

        /** The corresponding Panel which we will refresh and from which we will take the width and height */
        wxVideoPanel* m_Panel;
};

class wxVideoPanel : public wxPanel {
    DECLARE_CLASS(wxVideoPanel)
    private:
        /**
         * Called to paint the panel.
         */
        void OnPaint(wxPaintEvent &event);

        /**
         * Called to erase the background.
         */
        void OnEraseBackground(wxEraseEvent &event);

        /**
         * Called to update the panel in idle time.
         */
        void OnIdle(wxIdleEvent &event);

        /**
         * Called whenever the size changes
         */
        void OnSize(wxSizeEvent& event);

        wxVideoOutputDevice* m_Video;

        bool m_IsPlaying;
        bool m_SizeChanging;
        bool m_BufferChanged;

    public:

        /**
         * Creates a new SDLPanel.
         *
         * @param parent The wxWindow parent.
         */
        wxVideoPanel(wxWindow *parent);

        /**
         * Gets the currently assigned VideoOutputDevice
         */
        wxVideoOutputDevice* GetVideo();

        /**
         * Standard Destructor
         */
        ~wxVideoPanel();

        /**
         * Tells the Panel that the buffer has changed
         */
        void FlagForRepaint();

        DECLARE_EVENT_TABLE()
};

#endif
