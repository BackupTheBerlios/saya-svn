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
    public:
        /** Initializes the Output Device based on a wxVideoPanel. */
        wxVideoOutputDevice(wxVideoPanel* panel);

        /** Standard destructor */
        virtual ~wxVideoOutputDevice();

        static unsigned long CalculateBufferLength(unsigned int width,unsigned int height);

        /** Tries to Lock the Video Buffer.
          * @return true on success; false otherwise.
          */
        bool LockBuffer();

        /** Unlocks the Video Buffer, if the current thread is the owner
          * @return true if the Buffer's owner was either 0 or the current thread; false otherwise.
          */
        bool UnlockBuffer();

        /** Gets the Video Buffer's pointer
          * @return a pointer to the Video Buffer
          * @warning You MUST lock the buffer using the LockBuffer method prior to calling this function.
          */
        char* GetBuffer();

        /** Gets the Video Buffer's Size
          * @return the Video Buffer Size, in bytes.
          * @warning You MUST lock the buffer using the LockBuffer method prior to calling this function.
          */
        unsigned long GetBufferSize();

        /** Gets the Video Buffer's Actual length (which will always be less than or equal than the buffer size)
          * @return the Video Buffer Actual length, in bytes.
          * @warning You MUST lock the buffer using the LockBuffer method prior to calling this function.
          */
        unsigned long GetBufferLength();

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
         *  @param colorformat The input color format being sent.
         *  @param buf Buffer containing the data to be processed.
         *  @param buflen The length of the buffer to be processed, in bytes.
         *  @note  This method MUST do nothing if either m_width or m_height are set to 0.
         *         When the data is finally converted, RenderData MUST be called.
         *  @note  This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
         */
        virtual void LoadDeviceVideoData(VideoColorFormat colorformat, const char *buf,unsigned int buflen);


        /** Plays the received frames by Refreshing the panel's internal buffer.
          * @note This method MUST check MustAbortPlayback() regularly and abort rendering when the result is true.
          * @note This method MUST do nothing if either m_width or m_height are set to 0.
          */
        virtual void RenderData();

        /** A temporary buffer that holds the video data */
        char* m_Buffer;

        /** Specifies the allocated memory dedicated to this buffer */
        unsigned long m_BufferSize;

        /** Specifies the exact length to hold the current video frame */
        unsigned long m_BufferLength;

    private:

        /** Reallocates the current buffer to fit the new size requirements */
        void ReAllocBuffer(unsigned int newwidth, unsigned int newheight);
        /** The corresponding Panel which we will refresh and from which we will take the width and height */

        unsigned long m_BufferOwner;
        unsigned long m_BufferLockCount;
        syMutex* m_BufferMutex;
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
