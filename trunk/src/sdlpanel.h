/***************************************************************
 * Name:      sdlpanel.h
 * Purpose:   Declaration for the SDL panel
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-07-24
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Based on code from sdlpanel.cc found at
              http://code.technoplaza.net/wx-sdl/part1/
              (LGPL licensed)
 **************************************************************/

#ifndef sdlpanel_h
#define sdlpanel_h

#include <wx/panel.h>
#include <wx/dcbuffer.h>
#include <wx/image.h>

class SDL_Surface;

class SDLPanel : public wxPanel {
    DECLARE_CLASS(SDLPanel)
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

        /**
         * Creates the SDL_Surface used by this SDLPanel.
         */
        void CreateScreen();

        SDL_Surface *m_Screen;
        bool m_SizeChanged;

    public:
        /**
         * Creates a new SDLPanel.
         *
         * @param parent The wxWindow parent.
         */
        SDLPanel(wxWindow *parent);

        /**
         * Destructor for the SDLPanel.
         */
        ~SDLPanel();

        DECLARE_EVENT_TABLE()
};

#endif
