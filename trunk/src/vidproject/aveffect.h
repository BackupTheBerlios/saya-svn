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
    public:

        /** @brief The data for the current parameter at this point in time.
         *
         *  The variable is of string type for better flexibility. This is an intermediate format;
         *  more efficient data structures will be used to pass the actual parameters to the renderer.
         */
        std::string datum;

        /** Standard constructor. */
        FXParamKeyFrame();

        /** Standard destructor. */
        virtual ~FXParamKeyFrame();
};

/** @brief Defines a timeline for effect parameters.
 *
 *  Parameters have a timeline. The index for the map is the point in time of the current clip.
 */
typedef std::map<unsigned int,FXParamKeyFrame> FXParamTimeline;

/** @brief Defines a parameter list for effects.
  *
  * Each parameter has its own timeline, so we'll be able to play with the curves and modify keyframes as
  * we please.
  */
typedef std::map<std::string,FXParamTimeline> FXParameterList;

class AVEffectParamDeclaration: public serializable {
    public:

        /** Standard constructor. */
        AVEffectParamDeclaration();

        /** Standard destructor. */
        virtual ~AVEffectParamDeclaration();

        /** @brief Sets the parameter's name.
         *
         * Parameters can use the suffixes ".x" and ".y" to choose a point in the clip's area.
         */
        std::string ParamName;

        /** The parameter's description as it will appear in the effects window. */
        std::string Description;

        /** The parameter's description as it will appear in the help dialog. */
        std::string LongDescription; // The parameter's Long Description as it will appear in the help dialog.

        /** Defines a tooltip for the parameter. */
        std::string Tooltip;

        /** @brief Defines the parameter type.
          *
          * It must be a C++ basic data type, except for "string" which will be interpreted as std::string.
          */
        std::string ParamType;

        /** Defines the minimum value for the parameter. */
        std::string MinValue;

        /** Defines the maximum value for the parameter. */
        std::string MaxValue;

        /** Defines the default value for the parameter. */
        std::string DefaultValue;

        /** @brief Specifies the widget type used to modify the parameter.
          *
          * The widget type must be one of the following:
          * "slider" (default)
          * "logslider" (for logarithmic input)
          * "knob"
          * "textbox"
          * "textarea" (for strings)
          * "angle" (for rotation effects)
          * "xyangle" (for 3D rotation effects)
          */
        std::string WidgetType;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& data);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

class AVEffectDeclaration : public serializable {
    public:

        /** Standard constructor. */
        AVEffectDeclaration();

        /** Standard destructor. */
        virtual ~AVEffectDeclaration();

        /** Specifies whether the effect is audio or video. */
        AVType EffectType;

        /** @brief Effect's unique id.
          *
          * Must be in the form: namespace:effectname, where namespace and effectname
          * must be of alphanumeric / underscore. First character must be alphabetic.
          * @note the namespaces "saya" and "openvip" are reserved.
          */
        std::string EffectId;
        /** Displayable effect name. */
        std::string DisplayName; // Displayable effect name
        /** Short description for the effect (like in a tooltip). */
        std::string ShortDescription; // for tooltip
        /** Long description for the effect for documentation / help. */
        std::string LongDescription; // For documentation and help
        /** @brief The declaration for the parameters.
          *
          * @see AVEffectParamDeclaration
          */
        std::map<std::string,AVEffectParamDeclaration> ParametersInfo;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& data);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

class AVEffect : public serializable {
    public:

        /** standard constructor */
        AVEffect();

        /** standard destructor */
        virtual ~AVEffect();

        /** Tells whether the effect is video or audio */
        AVType m_EffectType;
        /** @brief Effect's unique id.
          *
          * @see AVEffectDeclaration::EffectId
          */
        std::string EffectId; // Effect's ID
        /** @brief The effect's parameters.
          *
          * @see FXParameterList
          */
        FXParameterList m_Parameters;

        /** The enabled flag for the effect */
        bool m_Enabled;


        /** Begining time (clip-time) for the effect */
        unsigned int m_StartTime;

        /** Ending time (clip-time) for the effect */
        unsigned int m_EndTime;

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& data);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

/** An associative array of string parameters */
typedef std::map<std::string,std::string> TransitionParameters;

class AVTransition : public serializable {
    public:
        /** @brief Transition type.
         * @see AVType
         */
        AVType m_TransitionType;

        /** @brief Transition's id
          *
          * Tells which transition effect we're applying
          */
        std::string TransitionId; // Transition's ID

        /** parameters for the transition */
        TransitionParameters m_Parameters;

        /** Enables or disables the transition */
        bool m_Enabled;


        /** standard constructor */
        AVTransition();

        /** standard destructor */
        virtual ~AVTransition();

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& data);

        /** @see serializable::serialize */
        virtual std::string serialize();
};


#endif

