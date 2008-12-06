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
#include "core/systring.h"

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
        syString ParamName;

        /** The parameter's description as it will appear in the effects window. */
        syString Description;

        /** The parameter's description as it will appear in the help dialog. */
        syString LongDescription; // The parameter's Long Description as it will appear in the help dialog.

        /** Defines a tooltip for the parameter. */
        syString Tooltip;

        /** @brief Defines the parameter type.
          *
          * It must be a C++ basic data type, except for "string" which will be interpreted as syString.
          */
        syString ParamType;

        /** Defines the minimum value for the parameter. */
        syString MinValue;

        /** Defines the maximum value for the parameter. */
        syString MaxValue;

        /** Defines the default value for the parameter. */
        syString DefaultValue;

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
        syString WidgetType;

        /** @see serializable::unserialize */
        virtual bool unserialize(const char* src);

        /** @see serializable::serialize */
        virtual void serialize(serialized& dest) const;
};

#endif
