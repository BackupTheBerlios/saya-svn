/***************************************************************
 * Name:      core/intl.h
 * Purpose:   Defines the operations for internationalization.
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-04-30
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#include "systring.h"

#ifndef saya_intl_h
#define saya_intl_h

#include <libintl.h>
#undef _
#define _(s) gettext(s)
#define _w(s) wxGetTranslation(_T(s))

#endif
