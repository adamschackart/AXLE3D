/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------

    This is a port of Robert Penner's easing functions, originally found at:
    http://robertpenner.com/easing/

    t = current time (in arbitrary units, same unit measure as duration)
    b = starting value to interpolate
    c = the total change in value of b that must occur (end - start)
    d = the total duration (time it takes to complete)

----------------------------------------------------------------------------- */
#ifndef __AE_EASING_H__
#define __AE_EASING_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_MATH_H__
#include <ae_math.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_easing_init(int argc, char** argv);
void ae_easing_quit(void); // private init

/*
================================================================================
 * ~~ [ linear easing (p ^ 1) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_linear_in_flt    (float t, float b, float c, float d)
{
    return c * t / d + b;
}

static c_inline float ease_linear_out_flt   (float t, float b, float c, float d)
{
    return c * t / d + b;
}

static c_inline float ease_linear_inout_flt (float t, float b, float c, float d)
{
    return c * t / d + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_linear_in_dbl   (double t, double b, double c, double d)
{
    return c * t / d + b;
}

static c_inline double ease_linear_out_dbl  (double t, double b, double c, double d)
{
    return c * t / d + b;
}

static c_inline double ease_linear_inout_dbl(double t, double b, double c, double d)
{
    return c * t / d + b;
}

/*
================================================================================
 * ~~ [ quadratic easing (p ^ 2) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_quadratic_in_flt     (float t, float b, float c, float d)
{
    t /= d;
    return c * t * t + b;
}

static c_inline float ease_quadratic_out_flt    (float t, float b, float c, float d)
{
    t /= d;
    return -c * t * (t - 2) + b;
}

static c_inline float ease_quadratic_inout_flt  (float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) { return c / 2 * t * t + b; }

    t--;
    return -c / 2 * (t * (t - 2) - 1) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_quadratic_in_dbl    (double t, double b, double c, double d)
{
    t /= d;
    return c * t * t + b;
}

static c_inline double ease_quadratic_out_dbl   (double t, double b, double c, double d)
{
    t /= d;
    return -c * t * (t - 2) + b;
}

static c_inline double ease_quadratic_inout_dbl (double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) { return c / 2 * t * t + b; }

    t--;
    return -c / 2 * (t * (t - 2) - 1) + b;
}

/*
================================================================================
 * ~~ [ cubic easing (p ^ 3) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_cubic_in_flt     (float t, float b, float c, float d)
{
    t /= d;
    return c * t * t * t + b;
}

static c_inline float ease_cubic_out_flt    (float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t * t * t + 1) + b;
}

static c_inline float ease_cubic_inout_flt  (float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t + b;

    t -= 2;
    return c / 2 * (t * t * t + 2) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_cubic_in_dbl    (double t, double b, double c, double d)
{
    t /= d;
    return c * t * t * t + b;
}

static c_inline double ease_cubic_out_dbl   (double t, double b, double c, double d)
{
    t /= d;
    t--;
    return c * (t * t * t + 1) + b;
}

static c_inline double ease_cubic_inout_dbl (double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t + b;

    t -= 2;
    return c / 2 * (t * t * t + 2) + b;
}

/*
================================================================================
 * ~~ [ quartic easing (p ^ 4) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_quartic_in_flt   (float t, float b, float c, float d)
{
    t /= d;
    return c * t * t * t * t + b;
}

static c_inline float ease_quartic_out_flt  (float t, float b, float c, float d)
{
    t /= d;
    t--;
    return -c * (t * t * t * t - 1) + b;
}

static c_inline float ease_quartic_inout_flt(float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t * t + b;

    t -= 2;
    return -c / 2 * (t * t * t * t - 2) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_quartic_in_dbl      (double t, double b, double c, double d)
{
    t /= d;
    return c * t * t * t * t + b;
}

static c_inline double ease_quartic_out_dbl     (double t, double b, double c, double d)
{
    t /= d;
    t--;
    return -c * (t * t * t * t - 1) + b;
}

static c_inline double ease_quartic_inout_dbl   (double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t * t + b;

    t -= 2;
    return -c / 2 * (t * t * t * t - 2) + b;
}

/*
================================================================================
 * ~~ [ quintic easing (p ^ 5) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_quintic_in_flt   (float t, float b, float c, float d)
{
    t /= d;
    return c * t * t * t * t * t + b;
}

static c_inline float ease_quintic_out_flt  (float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t * t * t * t * t + 1) + b;
}

static c_inline float ease_quintic_inout_flt(float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t * t * t + b;

    t -= 2;
    return c / 2 * (t * t * t * t * t + 2) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_quintic_in_dbl      (double t, double b, double c, double d)
{
    t /= d;
    return c * t * t * t * t * t + b;
}

static c_inline double ease_quintic_out_dbl     (double t, double b, double c, double d)
{
    t /= d;
    t--;
    return c * (t * t * t * t * t + 1) + b;
}

static c_inline double ease_quintic_inout_dbl   (double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * t * t * t * t * t + b;

    t -= 2;
    return c / 2 * (t * t * t * t * t + 2) + b;
}

/*
================================================================================
 * ~~ [ sine wave easing (sin(p * pi/2)) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_sine_in_flt      (float t, float b, float c, float d)
{
    return -c * ae_cosf(t / d * (M_PI / 2)) + c + b;
}

static c_inline float ease_sine_out_flt     (float t, float b, float c, float d)
{
    return c * ae_sinf(t / d * (M_PI / 2)) + b;
}

static c_inline float ease_sine_inout_flt   (float t, float b, float c, float d)
{
    return -c / 2 * (ae_cosf(M_PI * t / d) - 1) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_sine_in_dbl     (double t, double b, double c, double d)
{
    return -c * cos(t / d * (M_PI / 2)) + c + b;
}

static c_inline double ease_sine_out_dbl    (double t, double b, double c, double d)
{
    return c * sin(t / d * (M_PI / 2)) + b;
}

static c_inline double ease_sine_inout_dbl  (double t, double b, double c, double d)
{
    return -c / 2 * (cos(M_PI * t / d) - 1) + b;
}

/*
================================================================================
 * ~~ [ circular easing (sqrt(1 - p ^ 2)) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_circle_in_flt    (float t, float b, float c, float d)
{
    t /= d;
    return -c * (ae_sqrtf(1 - t * t) - 1) + b;
}

static c_inline float ease_circle_out_flt   (float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * ae_sqrtf(1 - t * t) + b;
}

static c_inline float ease_circle_inout_flt (float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) return -c / 2 * (ae_sqrtf(1 - t * t) - 1) + b;

    t -= 2;
    return c / 2 * (ae_sqrtf(1 - t * t) + 1) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_circle_in_dbl   (double t, double b, double c, double d)
{
    t /= d;
    return -c * (sqrt(1 - t * t) - 1) + b;
}

static c_inline double ease_circle_out_dbl  (double t, double b, double c, double d)
{
    t /= d;
    t--;
    return c * sqrt(1 - t * t) + b;
}

static c_inline double ease_circle_inout_dbl(double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;

    t -= 2;
    return c / 2 * (sqrt(1 - t * t) + 1) + b;
}

/*
================================================================================
 * ~~ [ exponential easing (base 2) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_exponential_in_flt   (float t, float b, float c, float d)
{
    return c * ae_powf(2, 10 * (t / d - 1)) + b;
}

static c_inline float ease_exponential_out_flt  (float t, float b, float c, float d)
{
    return c * (-ae_powf(2, -10 * t / d) + 1) + b;
}

static c_inline float ease_exponential_inout_flt(float t, float b, float c, float d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * ae_powf(2, 10 * (t - 1)) + b;

    t--;
    return c / 2 * (-ae_powf(2, -10 * t) + 2) + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_exponential_in_dbl      (double t, double b, double c, double d)
{
    return c * pow(2, 10 * (t / d - 1)) + b;
}

static c_inline double ease_exponential_out_dbl     (double t, double b, double c, double d)
{
    return c * (-pow(2, -10 * t / d) + 1) + b;
}

static c_inline double ease_exponential_inout_dbl   (double t, double b, double c, double d)
{
    t /= d / 2;
    if (t < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;

    t--;
    return c / 2 * (-pow(2, -10 * t) + 2) + b;
}

/*
================================================================================
 * ~~ [ exponentially-damped sine wave easing (elastic) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_elastic_in_flt   (float t, float b, float c, float d)
{
    float p, a, s, postfix;
    if (t == 0) return b;

    t /= d;
    if (t == 1) return b + c;

    p = d * 0.3;
    a = c;
    s = p / 4;
    postfix = a * ae_powf(2, 10 * (t -= 1));

    return -(postfix * ae_sinf((t * d - s) * (2 * M_PI) / p)) + b;
}

static c_inline float ease_elastic_out_flt  (float t, float b, float c, float d)
{
    float p, a, s;
    if (t == 0) return b;

    t /= d;
    if (t == 1) return b + c;

    p = d * 0.3;
    a = c;
    s = p / 4;

    return a * ae_powf(2, -10 * t) * ae_sinf((t * d - s) * (2 * M_PI) / p) + c + b;
}

static c_inline float ease_elastic_inout_flt(float t, float b, float c, float d)
{
    float p, a, s, postfix;
    if (t == 0) return b;

    t /= d / 2;
    if (t == 2) return b + c;

    p = d * 0.3 * 1.5;
    a = c;
    s = p / 4;

    if (t < 1)
    {
        postfix = a * ae_powf(2, 10 * (t -= 1));
        return -0.5 * (postfix * ae_sinf((t * d - s) * (2 * M_PI) / p)) + b;
    }

    postfix = a * ae_powf(2, -10 * (t -= 1));
    return postfix * ae_sinf((t * d - s) * (2 * M_PI) / p) * 0.5 + c + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_elastic_in_dbl      (double t, double b, double c, double d)
{
    double p, a, s, postfix;
    if (t == 0) return b;

    t /= d;
    if (t == 1) return b + c;

    p = d * 0.3;
    a = c;
    s = p / 4;
    postfix = a * pow(2, 10 * (t -= 1));

    return -(postfix * sin((t * d - s) * (2 * M_PI) / p)) + b;
}

static c_inline double ease_elastic_out_dbl     (double t, double b, double c, double d)
{
    double p, a, s;
    if (t == 0) return b;

    t /= d;
    if (t == 1) return b + c;

    p = d * 0.3;
    a = c;
    s = p / 4;

    return a * pow(2, -10 * t) * sin((t * d - s) * (2 * M_PI) / p) + c + b;
}

static c_inline double ease_elastic_inout_dbl   (double t, double b, double c, double d)
{
    double p, a, s, postfix;
    if (t == 0) return b;

    t /= d / 2;
    if (t == 2) return b + c;

    p = d * 0.3 * 1.5;
    a = c;
    s = p / 4;

    if (t < 1)
    {
        postfix = a * pow(2, 10 * (t -= 1));
        return -0.5 * (postfix * sin((t * d - s) * (2 * M_PI) / p)) + b;
    }

    postfix = a * pow(2, -10 * (t -= 1));
    return postfix * sin((t * d - s) * (2 * M_PI) / p) * 0.5 + c + b;
}

/*
================================================================================
 * ~~ [ overshooting cubic easing (back) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_back_in_flt      (float t, float b, float c, float d)
{
    const float s = 1.70158f; // 10% overshoot
    t /= d;
    return c * t * t * ((s + 1) * t - s) + b;
}

static c_inline float ease_back_out_flt     (float t, float b, float c, float d)
{
    const float s = 1.70158f;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

static c_inline float ease_back_inout_flt   (float t, float b, float c, float d)
{
    const float s = 1.70158f * 1.525f;

    if ((t /= d / 2) < 1)
    {
        return c / 2 * (t * t * ((s + 1) * t - s) + 0) + b;
    }
    else
    {
        t -= 2;
        return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
    }
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_back_in_dbl     (double t, double b, double c, double d)
{
    const double s = 1.70158; // 10% overshoot
    t /= d;
    return c * t * t * ((s + 1) * t - s) + b;
}

static c_inline double ease_back_out_dbl    (double t, double b, double c, double d)
{
    const double s = 1.70158;
    t = t / d - 1;
    return c * (t * t * ((s + 1) * t + s) + 1) + b;
}

static c_inline double ease_back_inout_dbl  (double t, double b, double c, double d)
{
    const double s = 1.70158 * 1.525;

    if ((t /= d / 2) < 1)
    {
        return c / 2 * (t * t * ((s + 1) * t - s) + 0) + b;
    }
    else
    {
        t -= 2;
        return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
    }
}

/*
================================================================================
 * ~~ [ exponentially-decaying bounce easing (bounce) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ease_bounce_out_flt   (float t, float b, float c, float d)
{
    if ((t /= d) < (1 / 2.75f))
    {
        return c * (7.5625f * t * t) + b;
    }
    else if (t < (2 / 2.75f))
    {
        t -= 1.5f / 2.75f;
        return c * (7.5625f * t * t + 0.75f) + b;
    }
    else if (t < (2.5 / 2.75))
    {
        t -= 2.25f / 2.75f;
        return c * (7.5625f * t * t + 0.9375f) + b;
    }
    else
    {
        t -= 2.625f / 2.75f;
        return c * (7.5625f * t * t + 0.984375f) + b;
    }
}

static c_inline float ease_bounce_in_flt    (float t, float b, float c, float d)
{
    return c - ease_bounce_out_flt(d - t, 0, c, d) + b;
}

static c_inline float ease_bounce_inout_flt (float t, float b, float c, float d)
{
    if (t < d / 2) return ease_bounce_in_flt(t * 2, 0, c, d)  * 0.5f + b;
    return ease_bounce_out_flt(t * 2 - d, 0, c, d) * 0.5f + c * 0.5f + b;
}

/* ===== [ double ] ========================================================= */

static c_inline double ease_bounce_out_dbl  (double t, double b, double c, double d)
{
    if ((t /= d) < (1 / 2.75))
    {
        return c * (7.5625 * t * t) + b;
    }
    else if (t < (2 / 2.75))
    {
        t -= 1.5 / 2.75;
        return c * (7.5625 * t * t + 0.75) + b;
    }
    else if (t < (2.5 / 2.75))
    {
        t -= 2.25 / 2.75;
        return c * (7.5625 * t * t + 0.9375) + b;
    }
    else
    {
        t -= 2.625 / 2.75;
        return c * (7.5625 * t * t + 0.984375) + b;
    }
}

static c_inline double ease_bounce_in_dbl   (double t, double b, double c, double d)
{
    return c - ease_bounce_out_dbl(d - t, 0, c, d) + b;
}

static c_inline double ease_bounce_inout_dbl(double t, double b, double c, double d)
{
    if (t < d / 2) return ease_bounce_in_dbl(t * 2, 0, c, d) * 0.5 + b;
    return ease_bounce_out_dbl(t * 2 - d, 0, c, d) * 0.5 + c * 0.5 + b;
}

/*
================================================================================
 * ~~ [ ease by mode ] ~~ *
--------------------------------------------------------------------------------
TODO: square/snap easing mode - inout mode would consist of (t < d / 2) ? b : c
TODO: define a templated easing class in C++ builds with static specializations
--------------------------------------------------------------------------------
*/

#define AE_EASE_MODE_N                          \
                                                \
    N(LINEAR_IN,            linear_in)          \
    N(LINEAR_OUT,           linear_out)         \
    N(LINEAR_INOUT,         linear_inout)       \
                                                \
    N(QUADRATIC_IN,         quadratic_in)       \
    N(QUADRATIC_OUT,        quadratic_out)      \
    N(QUADRATIC_INOUT,      quadratic_inout)    \
                                                \
    N(CUBIC_IN,             cubic_in)           \
    N(CUBIC_OUT,            cubic_out)          \
    N(CUBIC_INOUT,          cubic_inout)        \
                                                \
    N(QUARTIC_IN,           quartic_in)         \
    N(QUARTIC_OUT,          quartic_out)        \
    N(QUARTIC_INOUT,        quartic_inout)      \
                                                \
    N(QUINTIC_IN,           quintic_in)         \
    N(QUINTIC_OUT,          quintic_out)        \
    N(QUINTIC_INOUT,        quintic_inout)      \
                                                \
    N(SINE_IN,              sine_in)            \
    N(SINE_OUT,             sine_out)           \
    N(SINE_INOUT,           sine_inout)         \
                                                \
    N(CIRCLE_IN,            circle_in)          \
    N(CIRCLE_OUT,           circle_out)         \
    N(CIRCLE_INOUT,         circle_inout)       \
                                                \
    N(EXPONENTIAL_IN,       exponential_in)     \
    N(EXPONENTIAL_OUT,      exponential_out)    \
    N(EXPONENTIAL_INOUT,    exponential_inout)  \
                                                \
    N(ELASTIC_IN,           elastic_in)         \
    N(ELASTIC_OUT,          elastic_out)        \
    N(ELASTIC_INOUT,        elastic_inout)      \
                                                \
    N(BACK_IN,              back_in)            \
    N(BACK_OUT,             back_out)           \
    N(BACK_INOUT,           back_inout)         \
                                                \
    N(BOUNCE_IN,            bounce_out)         \
    N(BOUNCE_OUT,           bounce_in)          \
    N(BOUNCE_INOUT,         bounce_inout)       \
                                                \
    N(COUNT,                count)              \

typedef enum ae_ease_mode_t
{
    #define N(cap, low) AE_EASE_MODE_ ## cap,
    AE_EASE_MODE_N
    #undef N
} \
    ae_ease_mode_t;

static const char* ae_ease_mode_name[] =
{
    #define N(cap, low) "AE_EASE_MODE_" #cap,
    AE_EASE_MODE_N
    #undef N
};

static const char* ae_ease_mode_short_name[] =
{
    #define N(cap, low) #low,
    AE_EASE_MODE_N
    #undef N
};

static c_inline ae_ease_mode_t // find ease mode by string name
                ae_ease_mode_from_short_name(const char* name)
{
    size_t i = 0; // TODO: move to the C file, hashtable speedup
    for (; i < AE_EASE_MODE_COUNT; i++)
    {
        if (strcmp(ae_ease_mode_short_name[i], name) == 0)
        {
            return (ae_ease_mode_t)i;
        }
    }

    ae_assert(0, "invalid ease mode name string \"%s\"", name);
    return AE_EASE_MODE_LINEAR_IN;
}

AE_DECL float AE_CALL ease_flt( ae_ease_mode_t mode,
                float t, float b, float c, float d);

AE_DECL double AE_CALL ease_dbl(ae_ease_mode_t mode,
            double t, double b, double c, double d);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_EASING_H__ */
