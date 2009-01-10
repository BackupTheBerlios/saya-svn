///****************************************************************************
// * Name:      wxvideopanel.h
// * Purpose:   Declaration for class wxVideoPanel
// * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
// * Created:   2008-07-30
// * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
// * License:   GPL version 3 or later
// ***************************************************************************/
//#ifndef wxvideopanel_h
//#define wxvideopanel_h
//
//#include <wx/panel.h>
//#include "../saya/core/videooutputdevice.h"
//
//class wxVideoPanel : public wxPanel, public syBitmapSink {
//    DECLARE_CLASS(wxVideoPanel)
//    private:
//        /**
//         * Called to paint the panel.
//         */
//        void OnPaint(wxPaintEvent &event);
//
//        /**
//         * Called to erase the background.
//         */
//        void OnEraseBackground(wxEraseEvent &event);
//
//        /**
//         * Called to update the panel in idle time.
//         */
//        void OnIdle(wxIdleEvent &event);
//
//        /**
//         *   Called whenever the size changes.
//         */
//        void OnSize(wxSizeEvent& event);
//
//        /** @brief Loads data from m_Video->RenderData().
//         *
//         *  @see wxVideoOutputDevice::RenderData
//         */
//        virtual void LoadData(const syBitmap* bitmap);
//
//        /** @brief Returns the Width. */
//        virtual unsigned int GetWidth() const;
//
//        /** @brief Returns the Height. */
//        virtual unsigned int GetHeight() const;
//
//        /** @brief Returns the distance from the screen top. */
//        virtual int GetTop() const;
//
//        /** @brief Returns the distance from the screen top. */
//        virtual int GetLeft() const;
//
//        /** @brief returns the current color format. */
//        virtual VideoColorFormat GetColorFormat() const;
//
//        VideoOutputDevice* m_Video;
//
//        bool m_IsPlaying;
//        bool m_SizeChanging;
//        bool m_BufferChanged;
//        bool m_PaintingDemo;
//
//        /** We need a bitmap besides the one in wxVideoOutputDevice to hold the video data temporarily.
//         *  Otherwise we can get into a lot of awful situations that are nearly impossible to debug.
//         */
//        syBitmap* m_Bitmap;
//
//        VideoColorFormat m_NativeFormat;
//
//    public:
//
//        /**
//         * Creates a new SDLPanel.
//         *
//         * @param parent The wxWindow parent.
//         */
//        wxVideoPanel(wxWindow *parent);
//
//        /**
//         * Gets the currently assigned VideoOutputDevice
//         */
//        VideoOutputDevice* GetVideo();
//
//        /**
//         * Standard Destructor
//         */
//        ~wxVideoPanel();
//
//        /**
//         * Tells the Panel that the buffer has changed
//         */
//        void FlagForRepaint();
//
//        DECLARE_EVENT_TABLE()
//};
//
//#endif
