/***************************************************************
 * Name:      demovideo.cpp
 * Purpose:   Implementation of a Demo VideoOutputDevice.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-09
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:  Losely based on code from sdlpanel.cc found at
 *            http://code.technoplaza.net/wx-sdl/part1/
 *            (LGPL licensed)
 **************************************************************/

#include "../saya/core/sybitmap.h"
#include "../saya/core/videoinputdevice.h"
#include "../saya/core/debuglog.h"
#include "../saya/core/systring.h"

bool DemoVideoUnitTestRan = false;

class DemoVideo1 : public VideoInputDevice {
    public:
        DemoVideo1();
        virtual ~DemoVideo1();

    protected:

        /** @brief Loads the current frame into m_Bitmap.
         *
         *  This is a stub; you need to override this function to acomplish anything.
         *  @warning You MUST NOT call Seek() from LoadCurrentFrame(), or you will trigger a mutex deadlock!!
         *  If you need to do a seeking, call InternalSeek() instead.
         */
        void LoadCurrentFrame();
    private:
        void PaintMathPattern(); // Colored circles in a math pattern.
        void PaintMovingLine(); // Paints a moving vertical line.
        void UnitTest();
};

VideoInputDevice* CreateDemoVID() {
    return new DemoVideo1;
}

namespace DummyDemoVideo1 {
    bool dummybool = VideoInputDevice::RegisterVID("VID://Demo", &CreateDemoVID);
};

DemoVideo1::DemoVideo1() {
    m_Width = 200;
    m_Height = 100;
    m_ColorFormat = vcfBGR24;
    m_VideoLength = 30000000000LL; // 30 seconds.
//    m_FramesPerSecond = 2;
//    UnitTest();
    m_Width = 200;
    m_Height = 100;
    m_ColorFormat = vcfBGR24;
    m_VideoLength = 30000000000LL; // 30 seconds.
    m_FramesPerSecond = 25;
}

DemoVideo1::~DemoVideo1() {
}

void DemoVideo1::LoadCurrentFrame() {
//    PaintMathPattern();
    PaintMovingLine();
}

void DemoVideo1::UnitTest() {
    if(!DemoVideoUnitTestRan) {
        DemoVideoUnitTestRan = true;
    } else {
        return;
    }
    avtime_t testtime;
    DebugLog("DemoVideo1 Unit Test\n");
    DebugLog("Testing avtime_t to frame conversion");
    m_FramesPerSecond = 29.997;
    syString tmps;
    for(testtime = 0; testtime <= 30000000000LL; testtime += 10000000LL) {
        tmps = "";
        tmps << "Time:" << static_cast<unsigned long int>(testtime / 1000000LL) << " ms; Frame: ";
        tmps << GetFrameIndex(testtime);
        if(GetFrameIndex(GetTimeFromFrameIndex(GetFrameIndex(testtime))) != GetFrameIndex(testtime)) {
            tmps << "\nERROR! THE NUMBERS DON'T MATCH!";
        }
        DebugLog(tmps);
    }
}

void DemoVideo1::PaintMovingLine() {
    unsigned long ticks = GetFrameIndex(m_CurrentTime);
    long x, y;

    for (x = 0; x < (int)(m_Bitmap->GetWidth()); ++x) {
        unsigned long pixel;
        if(!(x % 10)) {
            pixel = 0xFFFF0000; // red for multiples of 10
        } else if (x & 1) {
            pixel = 0xFF00FF00; // Green for odd columns
        } else {
            pixel = 0Xffffffff; // white for even columns
        }
        m_Bitmap->SetPixel(x, 0, pixel);
    }

    for (y = 1; y < (int)(m_Bitmap->GetHeight()); ++y) {
        for (x = 0; x < (int)(m_Bitmap->GetWidth()); ++x) {
            unsigned long pixel;
            if(ticks % m_Bitmap->GetWidth() == static_cast<unsigned long>(x))
            {
                pixel = 0xFF0000FF;
            } else {
                pixel = 0xFFFFFFFF;
            }
            m_Bitmap->SetPixel(x, y, pixel);
        }
    }
}

void DemoVideo1::PaintMathPattern() {
    unsigned long ticks = GetFrameIndex(m_CurrentTime) * 7;
    long x, y;

    for (y = 0; y < (int)(m_Bitmap->GetHeight()); ++y) {
        for (x = 0; x < (int)(m_Bitmap->GetWidth()); ++x) {
            unsigned long pixel = (y * y + (x * x) + ticks) & 255;
            m_Bitmap->SetPixel(x, y, pixel);
        }
    }
}
