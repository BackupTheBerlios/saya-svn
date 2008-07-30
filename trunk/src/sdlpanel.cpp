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

#include "sdlpanel.h"
#include <SDL/SDL.h>

IMPLEMENT_CLASS(SDLPanel, wxPanel)

BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT(SDLPanel::OnPaint)
    EVT_ERASE_BACKGROUND(SDLPanel::OnEraseBackground)
    EVT_SIZE(SDLPanel::OnSize)
    EVT_IDLE(SDLPanel::OnIdle)
END_EVENT_TABLE()

SDLPanel::SDLPanel(wxWindow *parent) : wxPanel(parent), m_Screen(NULL),m_SizeChanged(false) {
    m_SizeChanged = false;
}

SDLPanel::~SDLPanel() {
    if(m_Screen) {
        SDL_FreeSurface(m_Screen);
        m_Screen = NULL;
    }
}

void SDLPanel::OnPaint(wxPaintEvent& event) {
    // can't draw if the screen doesn't exist yet
    if (!m_Screen) {
        return;
    }

    // lock the surface if necessary
    if (SDL_MUSTLOCK(m_Screen)) {
        if (SDL_LockSurface(m_Screen) < 0) {
            return;
        }
    }

    // create a bitmap from our pixel data
    wxBitmap bmp(wxImage(m_Screen->w, m_Screen->h,
                    static_cast<unsigned char *>(m_Screen->pixels), true));

    // unlock the screen
    if (SDL_MUSTLOCK(m_Screen)) {
        SDL_UnlockSurface(m_Screen);
    }

    // paint the screen
    wxBufferedPaintDC dc(this, bmp);
}

void SDLPanel::OnEraseBackground(wxEraseEvent& event) {
    // Do Nothing
}

void SDLPanel::OnSize(wxSizeEvent& event) {
    m_SizeChanged = true;
}

void SDLPanel::OnIdle(wxIdleEvent& event) {
    // create the SDL_Surface
    CreateScreen();

    // Lock surface if needed
    if (SDL_MUSTLOCK(m_Screen)) {
        if (SDL_LockSurface(m_Screen) < 0) {
            return;
        }
    }

    // Ask SDL for the time in milliseconds
    int tick = SDL_GetTicks();

    for (int y = 0; y < m_Screen->h; y++) {
        for (int x = 0; x < m_Screen->w; x++) {
            wxUint32 color = (y * y) + (x * x) + tick;
            wxUint8 *pixels = static_cast<wxUint8 *>(m_Screen->pixels) +
                              (y * m_Screen->pitch) +
                              (x * m_Screen->format->BytesPerPixel);

            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixels[0] = color & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = (color >> 16) & 0xFF;
            #else
                pixels[0] = (color >> 16) & 0xFF;
                pixels[1] = (color >> 8) & 0xFF;
                pixels[2] = color & 0xFF;
            #endif
        }
    }

    // Unlock if needed
    if (SDL_MUSTLOCK(m_Screen)) {
        SDL_UnlockSurface(m_Screen);
    }

    // refresh the panel
    Refresh(false);

    // throttle to keep from flooding the event queue
    wxMilliSleep(33);
}

void SDLPanel::CreateScreen() {
    if(m_SizeChanged && m_Screen) {
        SDL_FreeSurface(m_Screen);
        m_Screen = NULL;
    }
    if (!m_Screen) {
        int width, height;
        GetSize(&width, &height);

        m_Screen = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,
                                      24, 0, 0, 0, 0);
    }
}
