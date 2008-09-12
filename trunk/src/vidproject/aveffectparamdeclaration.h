/***************************************************************
 * Name:      aveffectparamdeclaration.h
 * Purpose:   Declaration for the AVEffectParamDeclaration class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef aveffectparamdeclaration_h
#define aveffectparamdeclaration_h

#include "serializable.h"

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
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();
};

#endif
