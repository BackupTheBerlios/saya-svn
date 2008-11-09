/***************************************************************
 * Name:      avtypes.h
 * Purpose:   Declaration for the common Audio Video types
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Created:   2008-10-05
 * Copyright: Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 **************************************************************/

#ifndef sy_avtypes_h
#define sy_avtypes_h

/** @brief Defines the type avtime_t for use with the AVDevice class.
 *  An avtime_t unit represents the minimum video/audio time unit.
 *  Currently it is a nanosecond (specified in AVTIME_T_SCALE).
 */
typedef unsigned long long avtime_t;

const avtime_t AVTIME_T_SCALE = 1000000000; // One avtime_t = 1 nanosecond

inline unsigned long GetMilliSecondsFromAVTimeT(avtime_t time) {
    time /= (AVTIME_T_SCALE / 1000);
    if(!time) {
        time = 1;
    }
    return (unsigned long)time;
}

#endif
