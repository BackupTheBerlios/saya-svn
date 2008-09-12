/***************************************************************
 * Name:      fxparameterlist.h
 * Purpose:   Declaration for the FXParameterList class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-09-11
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef fxparameterlist_h
#define fxparameterlist_h

#include "serializable.h"
#include "propertymap.h"

/** @brief Defines a parameter list for effects.
  *
  * Each parameter has its own timeline, so we'll be able to play with the curves and modify keyframes as
  * we please.
  */
class FXParameterList : public serializable {
    public:
        /** Standard constructor. */
        FXParameterList();

        /** Standard destructor. */
        virtual ~FXParameterList();

        /** @see serializable::unserialize */
        virtual bool unserialize(const std::string& src);

        /** @see serializable::serialize */
        virtual std::string serialize();

        std::map<std::string,SerializableUIntMap> data;
};

#endif
