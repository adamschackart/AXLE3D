/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_easing_init(int argc, char** argv) {}
void ae_easing_quit(void) {}

float ease_flt(ae_ease_mode_t mode, float t, float b, float c, float d)
{
    ae_switch (mode, ae_ease_mode, AE_EASE_MODE, suffix)
    {
        #define ease_count_flt(t, b, c, d) 0

        #define N(cap, low) case AE_EASE_MODE_ ## cap: \
                return ease_ ## low ## _flt(t, b, c, d);

        AE_EASE_MODE_N
        #undef N
        default: break;

        #undef ease_count_flt
    }

    assert(0); return ease_linear_in_flt(t, b, c, d);
}

double ease_dbl(ae_ease_mode_t mode, double t, double b, double c, double d)
{
    ae_switch (mode, ae_ease_mode, AE_EASE_MODE, suffix)
    {
        #define ease_count_dbl(t, b, c, d) 0

        #define N(cap, low) case AE_EASE_MODE_ ## cap: \
                return ease_ ## low ## _dbl(t, b, c, d);

        AE_EASE_MODE_N
        #undef N
        default: break;

        #undef ease_count_dbl
    }

    assert(0); return ease_linear_in_dbl(t, b, c, d);
}
