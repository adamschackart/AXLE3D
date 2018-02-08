# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from mem cimport *
from utl cimport *

cdef extern from "ae_random.h":
    void ae_random_gen_seed(void* buffer, size_t size)
    u32  ae_random_gen_seed_u32()

    # ===== [ generators ] =====================================================

    u32 ae_random_u32()

    void ae_random_xorshift8_seed(const u8 seed)
    u8   ae_random_xorshift8_ex(u8* state)
    u8   ae_random_xorshift8()

    void ae_random_xorshift16_seed(const u16 seed)
    u16  ae_random_xorshift16_ex(u16* state)
    u16  ae_random_xorshift16()

    void ae_random_xorshift32_seed(const u32 seed)
    u32  ae_random_xorshift32()
    u32  ae_random_xorshift32_ex(u32* state)

    void ae_random_xorshift64_seed(const u32 L, const u32 R)
    u64  ae_random_xorshift64_ex(u64* state)
    u64  ae_random_xorshift64()

    void ae_random_xorshift128_seed(const u32 x, const u32 y,
                                    const u32 z, const u32 w)
    u32  ae_random_xorshift128()
    u32  ae_random_xorshift128_ex(u32 state[4])

    # ===== [ integer distributions ] ==========================================

    u32 ae_random_bounded_u32(u32 bound)
    s32 ae_random_bounded_s32(s32 min_v, s32 max_v)

    # ===== [ array shuffling (permutation) ] ==================================

    # NOTE: this is wrapped in `mem.pyx` instead, for arrays
    # void memshuffle(void* base, size_t count, size_t size)

    # ===== [ lower-level random float api ] ===================================

    float  ae_random_flt_cc()
    double ae_random_dbl_cc()

    float  ae_random_flt_co()
    double ae_random_dbl_co()

    float  ae_random_flt_oc()
    double ae_random_dbl_oc()

    float  ae_random_flt_oo()
    double ae_random_dbl_oo()

    # ===== [ higher-level random float api ] ==================================

    float  ae_random_flt()
    double ae_random_dbl()

    float  ae_random_bi_flt()
    double ae_random_bi_dbl()

    float  ae_random_bounded_flt(const float  a, const float  b)
    double ae_random_bounded_dbl(const double a, const double b)

    # ===== [ random booleans ] ================================================

    int ae_random_bool()

    float  ae_random_flt_sign()
    double ae_random_dbl_sign()

    # ===== [ float distributions ] ============================================

    float  ae_random_uniform_flt(const float  a, const float  b)
    double ae_random_uniform_dbl(const double a, const double b)

    float  ae_random_exponential_flt(const float  mean)
    double ae_random_exponential_dbl(const double mean)

    float  ae_random_erlang_flt(int order, float  mean)
    double ae_random_erlang_dbl(int order, double mean)

    float  ae_random_gauss_flt_ex(float  mu, float  sigma, float * next)
    double ae_random_gauss_dbl_ex(double mu, double sigma, double* next)

    float  ae_random_gauss_flt(const float  mu, const float  sigma)
    double ae_random_gauss_dbl(const double mu, const double sigma)

    # ===== [ random angles ] ==================================================

    float  ae_random_bi_radian_flt()
    double ae_random_bi_radian_dbl()

    float  ae_random_radian_flt()
    double ae_random_radian_dbl()

    float  ae_random_bi_degree_flt()
    double ae_random_bi_degree_dbl()

    float  ae_random_degree_flt()
    double ae_random_degree_dbl()

    # ===== [ random vectors ] =================================================

    void ae_random_unit_vec(float* v, size_t n)

    void ae_random_unit_vec2(float v[2])
    void ae_random_unit_vec3(float v[3])
    void ae_random_unit_vec4(float v[4])

    # ===== [ point on line segment ] ==========================================

    void ae_random_seg_point(float* d, const float* a, const float* b, size_t n)

    void ae_random_seg2_point(float out[2], const float a[2], const float b[2])
    void ae_random_seg3_point(float out[3], const float a[3], const float b[3])

    # ===== [ point on triangle edge ] =========================================

    void ae_random_tri_edge_index(float* out, const float* v0, const float* v1,
                            const float* v2, const size_t idx, const size_t n)

    void ae_random_tri2_edge_index(  float pt[2], const float v0[2],
            const float v1[2], const float v2[2], const size_t idx)

    void ae_random_tri3_edge_index(  float pt[3], const float v0[3],
            const float v1[3], const float v2[3], const size_t idx)

    void ae_random_tri_edge(float * out, const float * v0, const float * v1,
                                                const float * v2, size_t n)

    void ae_random_tri2_edge(float out[2], const float v0[2],
                        const float v1[2], const float v2[2])

    void ae_random_tri3_edge(float out[3], const float v0[3],
                        const float v1[3], const float v2[3])

    # ===== [ point on triangle face ] =========================================

    void ae_random_tri_point(float * pt, const float * v0, const float * v1,
                                                const float * v2, size_t n)

    void ae_random_tri2_point(float pt[2], const float v0[2],
                        const float v1[2], const float v2[2])

    void ae_random_tri3_point(float pt[3], const float v0[3],
                        const float v1[3], const float v2[3])

try:
    import sys, random

    # these methods are mostly pure python; replace them with faster versions
    if getattr(sys, "no_patching_random_module", False): raise ImportError()

    def _seed(*a, **k):
        raise NotImplementedError("TODO")

    def _random():
        return ae_random_dbl_co()

    def _uniform(double a, double b):
        return ae_random_uniform_dbl(a, b)

    def _triangular(*a, **k):
        raise NotImplementedError("TODO")

    def _randint(int a, int b):
        return ae_random_bounded_s32(a, b + 1)

    def _choice(object sequence):
        if not sequence:
            raise IndexError('Cannot choose from an empty sequence')

        return sequence[ae_random_bounded_u32(len(sequence))]

    def _randrange(*a, **k):
        raise NotImplementedError("TODO")

    def _sample(*a, **k):
        raise NotImplementedError("TODO")

    def _shuffle(*a, **k):
        raise NotImplementedError("TODO")

    def _choices(*a, **k):
        raise NotImplementedError("TODO")

    def _normalvariate(*a, **k):
        raise NotImplementedError("TODO")

    def _lognormvariate(*a, **k):
        raise NotImplementedError("TODO")

    def _expovariate(double lambd):
        return ae_random_exponential_dbl(1.0 / lambd)

    def _vonmisesvariate(*a, **k):
        raise NotImplementedError("TODO")

    def _gammavariate(*a, **k):
        raise NotImplementedError("TODO")

    def _gauss(double mu, double sigma):
        return ae_random_gauss_dbl(mu, sigma)

    def _betavariate(*a, **k):
        raise NotImplementedError("TODO")

    def _paretovariate(*a, **k):
        raise NotImplementedError("TODO")

    def _weibullvariate(*a, **k):
        raise NotImplementedError("TODO")

    def _getstate(*a, **k):
        raise NotImplementedError("TODO")

    def _setstate(*a, **k):
        raise NotImplementedError("TODO")

    def _getrandbits(*a, **k):
        raise NotImplementedError("TODO")

    random.seed = _seed
    random.random = _random
    random.uniform = _uniform
    random.triangular = _triangular
    random.randint = _randint
    random.choice = _choice
    random.randrange = _randrange
    random.sample = _sample
    random.shuffle = _shuffle
    random.choices = _choices
    random.normalvariate = _normalvariate
    random.lognormvariate = _lognormvariate
    random.expovariate = _expovariate
    random.vonmisesvariate = _vonmisesvariate
    random.gammavariate = _gammavariate
    random.gauss = _gauss
    random.betavariate = _betavariate
    random.paretovariate = _paretovariate
    random.weibullvariate = _weibullvariate
    random.getstate = _getstate
    random.setstate = _setstate
    random.getrandbits = _getrandbits

except ImportError:
    ae_log_str("PYTHON", "failed to patch the python random number generator")

def vec2random_cc():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_flt_cc()
    v.v[1] = ae_random_flt_cc()

    return v

def vec3random_cc():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_flt_cc()
    v.v[1] = ae_random_flt_cc()
    v.v[2] = ae_random_flt_cc()

    return v

def vec4random_cc():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_flt_cc()
    v.v[1] = ae_random_flt_cc()
    v.v[2] = ae_random_flt_cc()
    v.v[3] = ae_random_flt_cc()

    return v

def vec2random_co():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_flt_co()
    v.v[1] = ae_random_flt_co()

    return v

def vec3random_co():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_flt_co()
    v.v[1] = ae_random_flt_co()
    v.v[2] = ae_random_flt_co()

    return v

def vec4random_co():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_flt_co()
    v.v[1] = ae_random_flt_co()
    v.v[2] = ae_random_flt_co()
    v.v[3] = ae_random_flt_co()

    return v

def vec2random_oc():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_flt_oc()
    v.v[1] = ae_random_flt_oc()

    return v

def vec3random_oc():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_flt_oc()
    v.v[1] = ae_random_flt_oc()
    v.v[2] = ae_random_flt_oc()

    return v

def vec4random_oc():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_flt_oc()
    v.v[1] = ae_random_flt_oc()
    v.v[2] = ae_random_flt_oc()
    v.v[3] = ae_random_flt_oc()

    return v

def vec2random_oo():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_flt_oo()
    v.v[1] = ae_random_flt_oo()

    return v

def vec3random_oo():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_flt_oo()
    v.v[1] = ae_random_flt_oo()
    v.v[2] = ae_random_flt_oo()

    return v

def vec4random_oo():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_flt_oo()
    v.v[1] = ae_random_flt_oo()
    v.v[2] = ae_random_flt_oo()
    v.v[3] = ae_random_flt_oo()

    return v

vec2random = vec2random_cc
vec3random = vec3random_cc
vec4random = vec4random_cc

def vec2bi_random():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_bi_flt()
    v.v[1] = ae_random_bi_flt()

    return v

def vec3bi_random():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_bi_flt()
    v.v[1] = ae_random_bi_flt()
    v.v[2] = ae_random_bi_flt()

    return v

def vec4bi_random():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_bi_flt()
    v.v[1] = ae_random_bi_flt()
    v.v[2] = ae_random_bi_flt()
    v.v[3] = ae_random_bi_flt()

    return v

def vec2sign():
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_flt_sign()
    v.v[1] = ae_random_flt_sign()

    return v

def vec3sign():
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_flt_sign()
    v.v[1] = ae_random_flt_sign()
    v.v[2] = ae_random_flt_sign()

    return v

def vec4sign():
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_flt_sign()
    v.v[1] = ae_random_flt_sign()
    v.v[2] = ae_random_flt_sign()
    v.v[3] = ae_random_flt_sign()

    return v

def vec2uniform(Vec2 min_v, Vec2 max_v):
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_uniform_flt(min_v.v[0], max_v.v[0])
    v.v[1] = ae_random_uniform_flt(min_v.v[1], max_v.v[1])

    return v

def vec3uniform(Vec3 min_v, Vec3 max_v):
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_uniform_flt(min_v.v[0], max_v.v[0])
    v.v[1] = ae_random_uniform_flt(min_v.v[1], max_v.v[1])
    v.v[2] = ae_random_uniform_flt(min_v.v[2], max_v.v[2])

    return v

def vec4uniform(Vec4 min_v, Vec4 max_v):
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_uniform_flt(min_v.v[0], max_v.v[0])
    v.v[1] = ae_random_uniform_flt(min_v.v[1], max_v.v[1])
    v.v[2] = ae_random_uniform_flt(min_v.v[2], max_v.v[2])
    v.v[3] = ae_random_uniform_flt(min_v.v[3], max_v.v[3])

    return v

def vec2exponential(Vec2 mean):
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_exponential_flt(mean.v[0])
    v.v[1] = ae_random_exponential_flt(mean.v[1])

    return v

def vec3exponential(Vec3 mean):
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_exponential_flt(mean.v[0])
    v.v[1] = ae_random_exponential_flt(mean.v[1])
    v.v[2] = ae_random_exponential_flt(mean.v[2])

    return v

def vec4exponential(Vec4 mean):
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_exponential_flt(mean.v[0])
    v.v[1] = ae_random_exponential_flt(mean.v[1])
    v.v[2] = ae_random_exponential_flt(mean.v[2])
    v.v[3] = ae_random_exponential_flt(mean.v[3])

    return v

def vec2erlang(int order, Vec2 mean):
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_erlang_flt(order, mean.v[0])
    v.v[1] = ae_random_erlang_flt(order, mean.v[1])

    return v

def vec3erlang(int order, Vec3 mean):
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_erlang_flt(order, mean.v[0])
    v.v[1] = ae_random_erlang_flt(order, mean.v[1])
    v.v[2] = ae_random_erlang_flt(order, mean.v[2])

    return v

def vec4erlang(int order, Vec4 mean):
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_erlang_flt(order, mean.v[0])
    v.v[1] = ae_random_erlang_flt(order, mean.v[1])
    v.v[2] = ae_random_erlang_flt(order, mean.v[2])
    v.v[3] = ae_random_erlang_flt(order, mean.v[3])

    return v

def vec2gauss(Vec2 mu, Vec2 sigma):
    cdef Vec2 v = Vec2()

    v.v[0] = ae_random_gauss_flt(mu.v[0], sigma.v[0])
    v.v[1] = ae_random_gauss_flt(mu.v[1], sigma.v[1])

    return v

def vec3gauss(Vec3 mu, Vec3 sigma):
    cdef Vec3 v = Vec3()

    v.v[0] = ae_random_gauss_flt(mu.v[0], sigma.v[0])
    v.v[1] = ae_random_gauss_flt(mu.v[1], sigma.v[1])
    v.v[2] = ae_random_gauss_flt(mu.v[2], sigma.v[2])

    return v

def vec4gauss(Vec4 mu, Vec4 sigma):
    cdef Vec4 v = Vec4()

    v.v[0] = ae_random_gauss_flt(mu.v[0], sigma.v[0])
    v.v[1] = ae_random_gauss_flt(mu.v[1], sigma.v[1])
    v.v[2] = ae_random_gauss_flt(mu.v[2], sigma.v[2])
    v.v[3] = ae_random_gauss_flt(mu.v[3], sigma.v[3])

    return v

def bi_radian():
    return ae_random_bi_radian_dbl()

def radian():
    return ae_random_radian_dbl()

def bi_degree():
    return ae_random_bi_degree_dbl()

def degree():
    return ae_random_degree_dbl()

def unit_vec2():
    cdef Vec2 vc = Vec2()
    ae_random_unit_vec2(vc.v); return vc

def unit_vec3():
    cdef Vec3 vc = Vec3()
    ae_random_unit_vec3(vc.v); return vc

def unit_vec4():
    cdef Vec4 vc = Vec4()
    ae_random_unit_vec4(vc.v); return vc

def seg2_point(Vec2 v0, Vec2 v1):
    cdef Vec2 pt = Vec2()
    ae_random_seg2_point(pt.v, v0.v, v1.v); return pt

def seg3_point(Vec3 v0, Vec3 v1):
    cdef Vec3 pt = Vec3()
    ae_random_seg3_point(pt.v, v0.v, v1.v); return pt

def tri2_edge_index(Vec2 v0, Vec2 v1, Vec2 v2, size_t index):
    cdef Vec2 pt = Vec2()
    ae_random_tri2_edge_index(pt.v, v0.v, v1.v, v2.v, index); return pt

def tri3_edge_index(Vec3 v0, Vec3 v1, Vec3 v2, size_t index):
    cdef Vec3 pt = Vec3()
    ae_random_tri3_edge_index(pt.v, v0.v, v1.v, v2.v, index); return pt

def tri2_edge(Vec2 v0, Vec2 v1, Vec2 v2):
    cdef Vec2 pt = Vec2()
    ae_random_tri2_edge(pt.v, v0.v, v1.v, v2.v); return pt

def tri3_edge(Vec3 v0, Vec3 v1, Vec3 v2):
    cdef Vec3 pt = Vec3()
    ae_random_tri3_edge(pt.v, v0.v, v1.v, v2.v); return pt

def tri2_point(Vec2 v0, Vec2 v1, Vec2 v2):
    cdef Vec2 pt = Vec2()
    ae_random_tri2_point(pt.v, v0.v, v1.v, v2.v); return pt

def tri3_point(Vec3 v0, Vec3 v1, Vec3 v2):
    cdef Vec3 pt = Vec3()
    ae_random_tri3_point(pt.v, v0.v, v1.v, v2.v); return pt
