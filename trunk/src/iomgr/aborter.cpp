/****************************************************************
 * Name:      aborter.cpp
 * Purpose:   Implementation of the syAborter class
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-07
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 ***************************************************************/

#include "aborter.h"
#include "sythread.h"

bool syAborter::MustAbort() {
    return (syThread::MustAbort() || InternalMustAbort());
}
