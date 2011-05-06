/**************************************************************************
 * Name:      imagefilters.h
 * Purpose:   Declaration for the syImageFilter and derived classes
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

#ifndef imagefilters_h
#define imagefilters_h

/** @class syImageFilter
 *  @brief The base for all the separable image filters.
 *  You initialize the filter with the desired filter width.
 *  The function Filter returns the contribution of a given pixel,
 *  given its relative position to the center pixel.
 */

enum syFilterType {
    filter_none               =  0,
    filter_box                =  1,
    filter_bilinear           =  2,
    filter_gaussian           =  3,
    filter_hamming            =  4,
    filter_blackman           =  5,
    filter_bell               =  7,
    filter_bspline            =  8,
    filter_mitchell           =  9,
    filter_lanczos3           = 10,
    filter_lanczos4           = 12,
    filter_lanczos6           = 13,
    filter_lanczos12          = 14,
    filter_kaiser             = 15,
    filter_catmullrom         = 16,
    filter_quadratic_interp   = 17,
    filter_quadratic_approx   = 18,
    filter_quadratic_mix      = 19
};

class syImageFilter {
    public:

        syImageFilter (double width) : m_Width (width) {}
        static syImageFilter* Create(syFilterType filtertype);
        virtual ~syImageFilter() {}

        double GetWidth() {
            return m_Width;
        }

        void   SetWidth (double width) {
            m_Width = width;
        }

        virtual double Filter (double t) = 0;

    protected:
        double  m_Width;
};

#endif
