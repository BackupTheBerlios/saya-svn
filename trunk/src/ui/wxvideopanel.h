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
#include "../saya/core/videooutputdevice.h"

class VideoInputDevice;
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
    public:
        /** Initializes the Output Device based on a wxVideoPanel. */
        wxVideoOutputDevice(wxVideoPanel* panel);

        /** Standard destructor */
        virtual ~wxVideoOutputDevice();

        /** Detachs from the wxVideoPanel. */
        void Detach();

    protected:

        /** @brief Initializes the output device.
          *
          * Here we will set m_width, m_height variables according to the Panel's size.
          * m_colorformat will be set to vcfRGB24 by default, since that's the color format used by wxWidgets' wxImage.
          * @return True on success; false otherwise.
          */
        virtual bool Connect();

        /** @brief Allocates the Bitmap's memory */
        virtual bool AllocateResources();

        /** Clears the panel (i.e. sets it all to black). Called by ShutDown(). */
        virtual void Clear();

        /** Disconnects the video output device or memory resource. Called by ShutDown(). */
        virtual void FreeResources();

        /** @brief ChangeDeviceSize will be called by the Panel's Resize event.
          *
          * @param newwidth the new width to be set for playback
          * @param newheight the new height to be set for playback
          * @return true if size was changed, false otherwise.
          */
        virtual bool ChangeDeviceSize(unsigned int newwidth,unsigned int newheight);

        /** Plays the received frames by Refreshing the panel's internal buffer.
          * @note This method MUST check MustAbort() regularly and abort rendering when the result is true.
          * @note This method MUST do nothing if either m_width or m_height are set to 0.
          */
        virtual void RenderVideoData(const syBitmap* bitmap);

    private:

        /** The corresponding Panel which we will refresh and from which we will take the width and height */
        wxVideoPanel* m_Panel;
};

class wxVideoPanel : public wxPanel {
    friend class wxVideoOutputDevice;
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
         *   Called whenever the size changes.
         */
        void OnSize(wxSizeEvent& event);

        /** @brief Loads data from m_Video->RenderData().
         *
         *  @see wxVideoOutputDevice::RenderData
         */
        void LoadData(const syBitmap* bitmap);

        wxVideoOutputDevice* m_Video;

        bool m_IsPlaying;
        bool m_SizeChanging;
        bool m_BufferChanged;
        bool m_PaintingDemo;

        /** We need a bitmap besides the one in wxVideoOutputDevice to hold the video data temporarily.
         *  Otherwise we can get into a lot of awful situations that are nearly impossible to debug.
         */
        syBitmap* m_Bitmap;

        VideoColorFormat m_NativeFormat;

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
