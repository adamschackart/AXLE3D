# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
# NOTE: this is the "extra math" module, for miscellaneous math functionality!
# ------------------------------------------------------------------------------
import sys # version info

cdef extern from "ae_easing.h":
    float ease_linear_in_flt    (float t, float b, float c, float d)
    float ease_linear_out_flt   (float t, float b, float c, float d)
    float ease_linear_inout_flt (float t, float b, float c, float d)

    double ease_linear_in_dbl   (double t, double b, double c, double d)
    double ease_linear_out_dbl  (double t, double b, double c, double d)
    double ease_linear_inout_dbl(double t, double b, double c, double d)

    float ease_quadratic_in_flt     (float t, float b, float c, float d)
    float ease_quadratic_out_flt    (float t, float b, float c, float d)
    float ease_quadratic_inout_flt  (float t, float b, float c, float d)

    double ease_quadratic_in_dbl    (double t, double b, double c, double d)
    double ease_quadratic_out_dbl   (double t, double b, double c, double d)
    double ease_quadratic_inout_dbl (double t, double b, double c, double d)

    float ease_cubic_in_flt     (float t, float b, float c, float d)
    float ease_cubic_out_flt    (float t, float b, float c, float d)
    float ease_cubic_inout_flt  (float t, float b, float c, float d)

    double ease_cubic_in_dbl    (double t, double b, double c, double d)
    double ease_cubic_out_dbl   (double t, double b, double c, double d)
    double ease_cubic_inout_dbl (double t, double b, double c, double d)

    float ease_quartic_in_flt   (float t, float b, float c, float d)
    float ease_quartic_out_flt  (float t, float b, float c, float d)
    float ease_quartic_inout_flt(float t, float b, float c, float d)

    double ease_quartic_in_dbl      (double t, double b, double c, double d)
    double ease_quartic_out_dbl     (double t, double b, double c, double d)
    double ease_quartic_inout_dbl   (double t, double b, double c, double d)

    float ease_quintic_in_flt   (float t, float b, float c, float d)
    float ease_quintic_out_flt  (float t, float b, float c, float d)
    float ease_quintic_inout_flt(float t, float b, float c, float d)

    double ease_quintic_in_dbl      (double t, double b, double c, double d)
    double ease_quintic_out_dbl     (double t, double b, double c, double d)
    double ease_quintic_inout_dbl   (double t, double b, double c, double d)

    float ease_sine_in_flt      (float t, float b, float c, float d)
    float ease_sine_out_flt     (float t, float b, float c, float d)
    float ease_sine_inout_flt   (float t, float b, float c, float d)

    double ease_sine_in_dbl     (double t, double b, double c, double d)
    double ease_sine_out_dbl    (double t, double b, double c, double d)
    double ease_sine_inout_dbl  (double t, double b, double c, double d)

    float ease_circle_in_flt    (float t, float b, float c, float d)
    float ease_circle_out_flt   (float t, float b, float c, float d)
    float ease_circle_inout_flt (float t, float b, float c, float d)

    double ease_circle_in_dbl   (double t, double b, double c, double d)
    double ease_circle_out_dbl  (double t, double b, double c, double d)
    double ease_circle_inout_dbl(double t, double b, double c, double d)

    float ease_exponential_in_flt   (float t, float b, float c, float d)
    float ease_exponential_out_flt  (float t, float b, float c, float d)
    float ease_exponential_inout_flt(float t, float b, float c, float d)

    double ease_exponential_in_dbl      (double t, double b, double c, double d)
    double ease_exponential_out_dbl     (double t, double b, double c, double d)
    double ease_exponential_inout_dbl   (double t, double b, double c, double d)

    float ease_elastic_in_flt   (float t, float b, float c, float d)
    float ease_elastic_out_flt  (float t, float b, float c, float d)
    float ease_elastic_inout_flt(float t, float b, float c, float d)

    double ease_elastic_in_dbl      (double t, double b, double c, double d)
    double ease_elastic_out_dbl     (double t, double b, double c, double d)
    double ease_elastic_inout_dbl   (double t, double b, double c, double d)

    float ease_back_in_flt      (float t, float b, float c, float d)
    float ease_back_out_flt     (float t, float b, float c, float d)
    float ease_back_inout_flt   (float t, float b, float c, float d)

    double ease_back_in_dbl     (double t, double b, double c, double d)
    double ease_back_out_dbl    (double t, double b, double c, double d)
    double ease_back_inout_dbl  (double t, double b, double c, double d)

    float ease_bounce_out_flt   (float t, float b, float c, float d)
    float ease_bounce_in_flt    (float t, float b, float c, float d)
    float ease_bounce_inout_flt (float t, float b, float c, float d)

    double ease_bounce_out_dbl  (double t, double b, double c, double d)
    double ease_bounce_in_dbl   (double t, double b, double c, double d)
    double ease_bounce_inout_dbl(double t, double b, double c, double d)

    ctypedef enum ae_ease_mode_t:
        AE_EASE_MODE_LINEAR_IN
        AE_EASE_MODE_LINEAR_OUT
        AE_EASE_MODE_LINEAR_INOUT
        AE_EASE_MODE_QUADRATIC_IN
        AE_EASE_MODE_QUADRATIC_OUT
        AE_EASE_MODE_QUADRATIC_INOUT
        AE_EASE_MODE_CUBIC_IN
        AE_EASE_MODE_CUBIC_OUT
        AE_EASE_MODE_CUBIC_INOUT
        AE_EASE_MODE_QUARTIC_IN
        AE_EASE_MODE_QUARTIC_OUT
        AE_EASE_MODE_QUARTIC_INOUT
        AE_EASE_MODE_QUINTIC_IN
        AE_EASE_MODE_QUINTIC_OUT
        AE_EASE_MODE_QUINTIC_INOUT
        AE_EASE_MODE_SINE_IN
        AE_EASE_MODE_SINE_OUT
        AE_EASE_MODE_SINE_INOUT
        AE_EASE_MODE_CIRCLE_IN
        AE_EASE_MODE_CIRCLE_OUT
        AE_EASE_MODE_CIRCLE_INOUT
        AE_EASE_MODE_EXPONENTIAL_IN
        AE_EASE_MODE_EXPONENTIAL_OUT
        AE_EASE_MODE_EXPONENTIAL_INOUT
        AE_EASE_MODE_ELASTIC_IN
        AE_EASE_MODE_ELASTIC_OUT
        AE_EASE_MODE_ELASTIC_INOUT
        AE_EASE_MODE_BACK_IN
        AE_EASE_MODE_BACK_OUT
        AE_EASE_MODE_BACK_INOUT
        AE_EASE_MODE_BOUNCE_IN
        AE_EASE_MODE_BOUNCE_OUT
        AE_EASE_MODE_BOUNCE_INOUT
        AE_EASE_MODE_COUNT

    const char* ae_ease_mode_name[] # enum names & short mode identifier strings
    const char* ae_ease_mode_short_name[]

    ae_ease_mode_t ae_ease_mode_from_short_name(const char* name)

    float  ease_flt(ae_ease_mode_t mode, float  t, float  b, float  c, float  d)
    double ease_dbl(ae_ease_mode_t mode, double t, double b, double c, double d)

cdef class EaseMethods:
    def __call__(self, str mode, double t, double b, double c, double d):
        """
        Call an easing function by its short string name, ie. `exponential_inout`.
        """
        cdef bytes b_mode

        if sys.version_info.major > 2:
            b_mode = <bytes>mode.encode('utf-8')
        else:
            b_mode = <bytes>mode

        return ease_dbl(ae_ease_mode_from_short_name(<char*>b_mode), t, b, c, d)

    @staticmethod
    def linear_in(double t, double b, double c, double d):
        return ease_linear_in_dbl(t, b, c, d)

    @staticmethod
    def linear_out(double t, double b, double c, double d):
        return ease_linear_out_dbl(t, b, c, d)

    @staticmethod
    def linear_inout(double t, double b, double c, double d):
        return ease_linear_inout_dbl(t, b, c, d)

    @staticmethod
    def quadratic_in(double t, double b, double c, double d):
        return ease_quadratic_in_dbl(t, b, c, d)

    @staticmethod
    def quadratic_out(double t, double b, double c, double d):
        return ease_quadratic_out_dbl(t, b, c, d)

    @staticmethod
    def quadratic_inout(double t, double b, double c, double d):
        return ease_quadratic_inout_dbl(t, b, c, d)

    @staticmethod
    def cubic_in(double t, double b, double c, double d):
        return ease_cubic_in_dbl(t, b, c, d)

    @staticmethod
    def cubic_out(double t, double b, double c, double d):
        return ease_cubic_out_dbl(t, b, c, d)

    @staticmethod
    def cubic_inout(double t, double b, double c, double d):
        return ease_cubic_inout_dbl(t, b, c, d)

    @staticmethod
    def quartic_in(double t, double b, double c, double d):
        return ease_quartic_in_dbl(t, b, c, d)

    @staticmethod
    def quartic_out(double t, double b, double c, double d):
        return ease_quartic_out_dbl(t, b, c, d)

    @staticmethod
    def quartic_inout(double t, double b, double c, double d):
        return ease_quartic_inout_dbl(t, b, c, d)

    @staticmethod
    def quintic_in(double t, double b, double c, double d):
        return ease_quintic_in_dbl(t, b, c, d)

    @staticmethod
    def quintic_out(double t, double b, double c, double d):
        return ease_quintic_out_dbl(t, b, c, d)

    @staticmethod
    def quintic_inout(double t, double b, double c, double d):
        return ease_quintic_inout_dbl(t, b, c, d)

    @staticmethod
    def sine_in(double t, double b, double c, double d):
        return ease_sine_in_dbl(t, b, c, d)

    @staticmethod
    def sine_out(double t, double b, double c, double d):
        return ease_sine_out_dbl(t, b, c, d)

    @staticmethod
    def sine_inout(double t, double b, double c, double d):
        return ease_sine_inout_dbl(t, b, c, d)

    @staticmethod
    def circle_in(double t, double b, double c, double d):
        return ease_circle_in_dbl(t, b, c, d)

    @staticmethod
    def circle_out(double t, double b, double c, double d):
        return ease_circle_out_dbl(t, b, c, d)

    @staticmethod
    def circle_inout(double t, double b, double c, double d):
        return ease_circle_inout_dbl(t, b, c, d)

    @staticmethod
    def exponential_in(double t, double b, double c, double d):
        return ease_exponential_in_dbl(t, b, c, d)

    @staticmethod
    def exponential_out(double t, double b, double c, double d):
        return ease_exponential_out_dbl(t, b, c, d)

    @staticmethod
    def exponential_inout(double t, double b, double c, double d):
        return ease_exponential_inout_dbl(t, b, c, d)

    @staticmethod
    def elastic_in(double t, double b, double c, double d):
        return ease_elastic_in_dbl(t, b, c, d)

    @staticmethod
    def elastic_out(double t, double b, double c, double d):
        return ease_elastic_out_dbl(t, b, c, d)

    @staticmethod
    def elastic_inout(double t, double b, double c, double d):
        return ease_elastic_inout_dbl(t, b, c, d)

    @staticmethod
    def back_in(double t, double b, double c, double d):
        return ease_back_in_dbl(t, b, c, d)

    @staticmethod
    def back_out(double t, double b, double c, double d):
        return ease_back_out_dbl(t, b, c, d)

    @staticmethod
    def back_inout(double t, double b, double c, double d):
        return ease_back_inout_dbl(t, b, c, d)

    @staticmethod
    def bounce_in(double t, double b, double c, double d):
        return ease_bounce_out_dbl(t, b, c, d)

    @staticmethod
    def bounce_out(double t, double b, double c, double d):
        return ease_bounce_in_dbl(t, b, c, d)

    @staticmethod
    def bounce_inout(double t, double b, double c, double d):
        return ease_bounce_inout_dbl(t, b, c, d)

ease = EaseMethods()
