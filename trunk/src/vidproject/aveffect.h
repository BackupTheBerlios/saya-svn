#ifndef AVEFFECT_H
#define AVEFFECT_H

/***************************************************************
 * Name:      aveffect.h
 * Purpose:   Definitions for Video/Audio effect classes
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-05-16
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "avcommon.h"
#include <map>

class FXParamKeyFrame {
    wxString datum; // The data for the current parameter at this point in time. Untyped
    // (to manage the data we need to access the FX's parameter declaration, which has yet to be designed)
    signed char inSlope;
    signed char outSlope;
    // Slope of the curve before arriving to the inpoint (or after leaving the outpoint).
    // -128 = 0 degrees. 127 = MAX (nearly 90 degrees). Default = 0.

    // NOTE:
    // If we have keyframes A and B, and A has parameter "x" 0, and B has parameter "x" 100, if the slope for both
    // keyframes is 127, the curve to both points would be a straight line, meaning that between those two keyframes,
    // parameter "x" would be 50.

    //  outSlope(A) = 0; inSlope(B) = 0
    //       B
    //      /
    //     /
    //   A/

    //  outSlope(A) = 127; inSlope(B) = -128
    //      ____ B
    //     /
    //   A |


    //  outSlope(A) = -128; inSlope(B) = 127
    //           B
    //           |
    //   A ______/

    //  outSlope(A) = 127; inSlope(B) = 127. Note that in the middle point, the curve becomes a straight line.
    //        |B
    //        |
    //       /
    //      /
    //     /
    //    |
    //   A|

    //  outSlope(A) = -128; inSlope(B) = -128. Note that in the middle point, the curve becomes a straight line.
    //          ___B
    //         /
    //        /
    //   A___/

    // IMPORTANT NOTE!!! The algorithm for calculating the actual points has yet to be designed!!!

};

typedef std::map<unsigned int,FXParamKeyFrame> FXParamTimeline;
// Parameters have a timeline.
// The key is a frame index, where the first point is the clip's first frame (not the resource clip, the timeline-clip).
// IMPORTANT NOTE! If a clip is reversed, the reversal will be interpreted as the first "effect" (before transitions
// are applied). If you want to reverse a final processed clip, you'd have to add a "reverse" effect at the end.

// We use strings to simplify data handling. On loading the clip params, the effect plugin will use its own data
// format for the parameters. This is only for information exchange and storage.

typedef std::map<wxString,FXParamTimeline> FXParameterList; // parameter list for each Effect. Each parameter
// has its own timeline, so we'll be able to play with the curves and add/modify keyframes as we please.

class AVEffectParamDeclaration {
    wxString ParamName;
    // Parameters can use the suffixes ".x" and ".y" to choose a point on the clip's area.
    wxString Description; // The parameter's description as it will appear in the effects stack.
    wxString LongDescription; // The parameter's Long Description as it will appear in the help dialog.
    wxString Tooltip; // Tooltip help for the parameter
    wxString ParamType; // A C++ basic data type name, except for "string" which will be interpreted as wxString.
    wxString MinValue;
    wxString MaxValue;
    wxString DefaultValue;
    wxString WidgetType;
    // Widget Types are the widgets used to describe parameters. The default is the slider.
    // Clickable parameters will be grouped.
    // The widget types can be one of the following:
    // "slider" (default), "logslider" (for logarithmic input),
    // "knob", "textbox" (single-line input box), "textarea" (for strings),
    // "angle" (for rotation effects)
};

class AVEffectDeclaration {
    public:
        AVType EffectType;
        wxString EffectId; // Effect's ID. Must contain:
        // 1) a namespace for the effect provider (alphanumeric and underscore; first character must be alphabetic)
        // The namespaces "saya" and "openvip" are reserved for this program's default FX.
        // 2) a colon
        // 3) the effect's name (alphanumeric and underscore; first character must be alphabetic)
        wxString DisplayName; // Displayable effect name
        wxString ShortDescription; // for tooltip
        wxString LongDescription; // For documentation and help
        std::map<wxString,AVEffectParamDeclaration> ParametersInfo;
};

class AVEffect {
    public:
        AVType m_EffectType;
        wxString EffectId; // Effect's ID
        FXParameterList m_Parameters;
        bool m_Enabled;
        unsigned int m_StartFrame; // Beginning clip frame for the effect (this is independent from the timeline)
        unsigned int m_EndFrame; // Ending clip frame for the effect (independent from the timeline)
        AVEffect();
        virtual ~AVEffect();
};


#endif

