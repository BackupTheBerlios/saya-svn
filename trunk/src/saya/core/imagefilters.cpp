/**************************************************************************
 * Name:      imagefilters.cpp
 * Purpose:   Implementation of the syImageFilter and derived classes
 * Author:    Eran Yariv
 * Created:   1999-12-11
 * Copyright: For this version:
 *            Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * License:   GPL version 3 or later
 * Comments:
 *  Adapted from the article:
 *  "Two Pass Scaling using Filters", by Eran Yariv
 *  http://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=146
 *  The original article included no copyright or licensing terms,
 *  so it's assumed as public domain. We license it as GPL to avoid
 *  potential licensing conflicts with the rest of the code.
 *************************************************************************/

#include <cmath>
#include "imagefilters.h"
using std::abs;

#define FILTER_PI  double (3.1415926535897932384626433832795)
#define FILTER_2PI double (2.0 * 3.1415926535897932384626433832795)
#define FILTER_4PI double (4.0 * 3.1415926535897932384626433832795)

#define DEFINE_FILTER(classname, defaultwidth) \
class classname : public syImageFilter { \
    public: \
        classname (double dWidth = double(defaultwidth)) : syImageFilter(dWidth) {} \
        virtual ~classname() {} \
        double Filter (double t); \
}

DEFINE_FILTER(syBoxFilter, 0.5);
DEFINE_FILTER(syBilinearFilter, 1.0);
DEFINE_FILTER(syGaussianFilter, 3.0);
DEFINE_FILTER(syHammingFilter, 0.5);
DEFINE_FILTER(syBlackmanFilter, 0.5);
DEFINE_FILTER(syBellFilter, 0.5);
DEFINE_FILTER(syBSplineFilter, 2.0);
DEFINE_FILTER(syMitchellFilter, 2.0);
DEFINE_FILTER(syCatmullRomFilter, 2.0);
DEFINE_FILTER(syLanczosFilter, 3.0);
DEFINE_FILTER(syKaiserFilter, 3.0);
DEFINE_FILTER(syQuadraticInterpFilter, 1.5);
DEFINE_FILTER(syQuadraticApproxFilter, 1.5);
DEFINE_FILTER(syQuadraticMixFilter, 1.5);


// ----------------------------
// begin filters implementation
// ----------------------------

double syBoxFilter::Filter (double t) {
    return (abs(t) <= m_Width ? 1.0 : 0.0);
}

double syBilinearFilter::Filter (double t) {
    t = abs(t);
    return (t < m_Width ? m_Width - t : 0.0);
}

double syGaussianFilter::Filter (double t) {
    if (abs(t) >= m_Width) {
        return 0.0;
    }
    return exp (-t * t / 2.0) / sqrt (FILTER_2PI);
}

double syHammingFilter::Filter (double t) {
    if (abs (t) >= m_Width) {
        return 0.0;
    }
    double dWindow = 0.54 + 0.46 * cos (FILTER_2PI * t);
    double dSinc = (t == 0) ? 1.0 : sin (FILTER_PI * t) / (FILTER_PI * t);
    return dWindow * dSinc;
}

double syBlackmanFilter::Filter (double t) {
    if (abs (t) >= m_Width) {
        return 0.0;
    }
    double dN = 2.0 * m_Width + 1.0;
    return 0.42 + 0.5 * cos (FILTER_2PI * t / ( dN - 1.0 )) +
           0.08 * cos (FILTER_4PI * t / ( dN - 1.0 ));
}

double syBellFilter::Filter (double t) {
    t = abs(t);
    if (t >= m_Width) {
        return 0.0;
    }

    if (t < (m_Width / 3.0)) {
        return ((m_Width / 2) - (t * t));
    }
    t = (t - m_Width);
    return (.5d * (t * t));
}

double syBSplineFilter::Filter (double t) {
    t = abs(t);
    if (t >= m_Width) {
        return 0.0;
    }

    if (2.0*t < m_Width) {
        double tt = t * t;
        return ((.5d * tt * t) - tt + (m_Width / 3.0d));
    } else {
        t = m_Width - t;
        return ((1.0f / 6.0d) * (t * t * t));
    }
}

double MitchellFilterfunc(double t, const double width, const double B, const double C) {
    t = abs(t);
    if (t >= width) {
        return 0.0;
    }
    double tt;
    tt = t * t;

    if(t < (width/2.0d)) {
        t = (((12.0d - 9.0d * B - 6.0d * C) * (t * tt))
             + ((-18.0d + 12.0d * B + 6.0d * C) * tt)
             + (6.0d - 2.0d * B));

        return (t / 6.0d);
    } else {
        t = (((-1.0d * B - 6.0d * C) * (t * tt))
             + ((6.0d * B + 30.0d * C) * tt)
             + ((-12.0d * B - 48.0d * C) * t)
             + (8.0d * B + 24.0d * C));

        return (t / 6.0d);
    }

    return (0.0f);
}

double syMitchellFilter::Filter(double t) {
    return MitchellFilterfunc(t, m_Width, 1.0d / 3.0d, 1.0d / 3.0d);
}

double syCatmullRomFilter::Filter(double t) {
    return MitchellFilterfunc(t, m_Width, 0.0d, 0.5d);
}

double cleanfilter(double t) {
    const double EPSILON = .0000125d;
    if (abs(t) < EPSILON)
        return 0.0;
    return (double)t;
}

double sinc(double x) {
    x = (x * FILTER_PI);

    if ((x < 0.01d) && (x > -0.01d))
        return 1.0d + x*x*(-1.0d/6.0d + x*x*1.0d/120.0d);
    return sin(x) / x;
}

double syLanczosFilter::Filter(double t) {
    t = abs(t);
    if (t >= m_Width) {
        return 0.0;
    }
    return cleanfilter(sinc(t) * sinc(t / m_Width));
}

static const double KAISER_ALPHA = 4.0;

static double bessel0(double x) {
    const double EPSILON_RATIO = 1E-16;
    double xh, sum, pow, ds;
    int k,i; // i is a counter for max. safety

    xh = 0.5 * x;
    sum = 1.0;
    pow = 1.0;
    ds = 1.0;
    i = 1000;
    for(k = 1,i = 1000; ds > sum * EPSILON_RATIO && i > 0;--i,++k) {
        pow *= (xh / k);
        ds = pow * pow;
        sum += ds;
    }
    return sum;
}

static double kaiserfunc(double alpha, double half_width, double x) {
    const double ratio = (x / half_width);
    return bessel0(alpha * sqrt(1 - ratio * ratio)) / bessel0(alpha);
}

double syKaiserFilter::Filter(double t) {
    t = abs(t);
    if (t >= m_Width) {
        return 0.0;
    }
    // db atten
    const double att = 40.0d;
    const double alpha = (double)(exp(log((double)0.58417 * (att - 20.96)) * 0.4) + 0.07886 * (att - 20.96));
    //const double alpha = KAISER_ALPHA;
    return (double)cleanfilter(sinc(t) * kaiserfunc(alpha, m_Width, t));
}

static double quadratic_func(double t, const double R, double width) {
    t = abs(t);
    if (t >= width) {
        return 0.0;
    }
    double tt = t * t;
    if (t <= (width/3.0d))
        return (-2.0d * R) * tt + .5d * (R + 1.0d);
    else
        return (R * tt) + (-2.0d * R - .5d) * t + (3.0d / 4.0d) * (R + 1.0d);
}

double syQuadraticInterpFilter::Filter(double t) {
    return quadratic_func(t, 1.0d, m_Width);
}

double syQuadraticApproxFilter::Filter(double t) {
    return quadratic_func(t, .5d, m_Width);
}

double syQuadraticMixFilter::Filter(double t) {
    return quadratic_func(t, .8d, m_Width);
}

syImageFilter* syImageFilter::Create(syFilterType filtertype) {
    syImageFilter* result = 0;
    switch(filtertype) {
        case filter_none:
        default:
            result = 0;
            break;
        case filter_box:
            result = new syBoxFilter(0.5);
            break;
        case filter_bilinear:
            result = new syBilinearFilter(1.0);
            break;
        case filter_gaussian:
            result = new syGaussianFilter(3.0);
            break;
        case filter_hamming:
            result = new syHammingFilter(0.5);
            break;
        case filter_blackman:
            result = new syBlackmanFilter(0.5);
            break;
        case filter_bell:
            result = new syBellFilter(1.5);
            break;
        case filter_bspline:
            result = new syBSplineFilter(2.0);
            break;
        case filter_mitchell:
            result = new syMitchellFilter(2.0);
            break;
        case filter_lanczos3:
            result = new syLanczosFilter(3.0);
            break;
        case filter_lanczos4:
            result = new syLanczosFilter(4.0);
            break;
        case filter_lanczos6:
            result = new syLanczosFilter(6.0);
            break;
        case filter_lanczos12:
            result = new syLanczosFilter(12.0);
            break;
        case filter_kaiser:
            result = new syKaiserFilter(3.0);
            break;
        case filter_catmullrom:
            result = new syCatmullRomFilter(2.0);
            break;
        case filter_quadratic_interp:
            result = new syQuadraticInterpFilter(1.5);
            break;
        case filter_quadratic_approx:
            result = new syQuadraticApproxFilter(1.5);
            break;
        case filter_quadratic_mix:
            result = new syQuadraticMixFilter(1.5);
            break;
    }
    return result;
}
