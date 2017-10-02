# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from vec cimport Vector, Vec2, Vec3, Vec4

# ==============================================================================
# ~ [ quaternion math ]
# ==============================================================================

cdef extern from "ae_math.h":
    int quat_fix_denormals_ex(float q[4], float epsilon)
    int quat_fix_denormals(float q[4]) # floor tiny vals

    void quat_lerp(float out[4], const float a[4], const float b[4], const float t)

    float quat_mag(const float q[4])
    float quat_mag2(const float q[4])

    void quat_copy(float dst[4], const float src[4])
    int quat_cmp(const float a[4], const float b[4])
    int quat_cmp_e(const float a[4], const float b[4], const float epsilon)

    void quat_zero(float dst[4])
    int quat_is_zero(const float q[4])
    int quat_is_zero_e(const float q[4], const float epsilon)

    void quat_identity(float q[4])
    int quat_is_identity(const float q[4])
    int quat_is_identity_e(const float q[4], const float epsilon)

    void quat_conjugated(float out[4], const float q[4])
    void quat_conjugate(float q[4]) # negate quat x, y, z

    void quat_normalize(float out[4], const float q[4]) # make mag = 1.0
    void quat_mul_quat(float out[4], const float a[4], const float b[4])

    void quat_mul_point3(float out[3], const float q[4], const float p[3])
    void quat_mul_point3_safe(float out[3], const float q[4], const float p[3])

    void quat_mul_vector3(float out[3], const float q[4], const float p[3])
    void quat_mul_vector3_safe(float out[3], const float q[4], const float p[3])

    void quat_to_row_mat3x3(float m[ 9], const float q[4])
    void quat_to_col_mat3x3(float m[ 9], const float q[4])

    void quat_to_row_mat4x4(float m[16], const float q[4])
    void quat_to_col_mat4x4(float m[16], const float q[4])

    # ===== [ construction & conversion (angles in radians) ] ==================

    void quat_new_rotate_axis(float rq[4], const float angle, const float axis[3])
    void quat_new_rotate_euler(float q[4], float heading, float attitude, float bank)

    void quat_to_angle_axis(float* angle, float axis[3], const float q[4])
    void quat_to_euler(float* heading, float* attitude, float* bank, float q[4])

# ===== [ quaternion class ] ===================================================

cdef class Quat:
    cdef float q[4]

    def __init__(self, float x=0, float y=0, float z=0, float w=1):
        self.q[0] = x; self.q[1] = y; self.q[2] = z; self.q[3] = w

    def __repr__(self):
        return "{}{}".format(self.__class__.__name__, tuple(self))

    def __richcmp__(self, Quat other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Quat')

    def __copy__(self):
        cdef Quat q = Quat() # __class__?
        quat_copy(q.q, self.q); return q

    copy = __copy__

    def __abs__(self):
        return quat_mag(self.q)

    def __neg__(self):
        cdef Quat q = Quat() # XXX: __class__?
        quat_conjugated(q.q, self.q); return q

    def __nonzero__(self):
        return not quat_is_zero(self.q)

    property x:
        def __get__(self): return self.q[0]
        def __set__(self, float value): self.q[0] = value

    property y:
        def __get__(self): return self.q[1]
        def __set__(self, float value): self.q[1] = value

    property z:
        def __get__(self): return self.q[2]
        def __set__(self, float value): self.q[2] = value

    property w:
        def __get__(self): return self.q[3]
        def __set__(self, float value): self.q[3] = value

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.q[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 3: raise IndexError
        self.q[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[4])
        cdef char* p = <char*>b

        quat_copy(<float*>p, self.q)
        return (Quat, (), b)

    def __setstate__(self, bytes state):
        cdef char * p = <char *>state
        quat_copy(self.q, <float *>p)

    def address(self):
        return <size_t>self.q

    def cmp(self, Quat other):
        return quat_cmp(self.q, other.q)

    def cmp_e(self, Quat other, float epsilon):
        return quat_cmp_e(self.q, other.q, epsilon)

    def zero(self):
        quat_zero(self.q); return self

    def is_zero(self):
        return quat_is_zero(self.q)

    def is_zero_e(self, float epsilon):
        return quat_is_zero_e(self.q, epsilon)

    def identity(self):
        quat_identity(self.q); return self

    def is_identity(self):
        return quat_is_identity(self.q)

    def is_identity_e(self, float epsilon):
        return quat_is_identity_e(self.q, epsilon)

    def fix_denormals_ex(self, float denormal):
        return quat_fix_denormals_ex(self.q, denormal)

    def fix_denormals(self):
        return quat_fix_denormals(self.q)

    def neg(self):
        quat_conjugate(self.q); return self

    conjugated = __neg__
    conjugate = neg

    def mag2(self):
        return quat_mag2(self.q)

    magnitude = mag = __abs__
    magnitude_squared = mag2

    def to_row_mat3x3(self):
        cdef Mat3x3 m = Mat3x3() # row-major mat3x3
        quat_to_row_mat3x3(m.m, self.q); return m

    def to_col_mat3x3(self):
        cdef Mat3x3 m = Mat3x3() # col-major mat3x3
        quat_to_col_mat3x3(m.m, self.q); return m

    def to_row_mat4x4(self):
        cdef Mat4x4 m = Mat4x4() # row-major mat4x4
        quat_to_row_mat4x4(m.m, self.q); return m

    def to_col_mat4x4(self):
        cdef Mat4x4 m = Mat4x4() # col-major mat4x4
        quat_to_col_mat4x4(m.m, self.q); return m

    def normalize(self):
        quat_normalize(self.q, self.q); return self

    def normalized(self):
        cdef Quat q = Quat()
        quat_normalize(q.q, self.q); return q

    def lerp(self, Quat other, float t):
        quat_lerp(self.q, self.q, other.q, t); return self

    def lerped(self, Quat other, float t):
        cdef Quat q = Quat()
        quat_lerp(q.q, self.q, other.q, t); return q

    def imul_quat(self, Quat other):
        quat_mul_quat(self.q, self.q, other.q); return self

    def mul_quat(self, Quat other):
        cdef Quat q = Quat()
        quat_mul_quat(q.q, self.q, other.q); return q

    def mul_point3(self, Vec3 p):
        cdef Vec3 out = Vec3()
        quat_mul_point3(out.v, self.q, p.v); return out

    def mul_vector3(self, Vec3 v):
        cdef Vec3 out = Vec3()
        quat_mul_vector3(out.v, self.q, v.v); return out

    # ===== [ construction & conversion (angles in radians) ] ==================

    def rotate_axis(self, float angle, Vec3 axis):
        quat_new_rotate_axis(self.q, angle, axis.v); return self

    def rotate_euler(self, float heading, float attitude, float bank):
        quat_new_rotate_euler(self.q, heading, attitude, bank); return self

    def to_angle_axis(self):
        # get angle in radians and axis of rotation
        cdef float angle
        cdef Vec3 axis = Vec3()

        quat_to_angle_axis(&angle, axis.v, self.q)
        return (angle, axis)

    def to_euler(self):
        cdef float heading
        cdef float attitude
        cdef float bank

        quat_to_euler(&heading, &attitude, &bank, self.q)
        return (heading, attitude, bank)

# ==============================================================================
# ~ [ n-dimensional matrix math ]
# ==============================================================================

cdef extern from "ae_math.h":
    float* ae_matrix_alloc(const size_t rows, const size_t cols) # allocation
    void ae_matrix_free(float* m, const size_t rows, const size_t cols)

    void mat_copy(float* dst, const float* const src,
                const size_t rows, const size_t cols)

    int mat_cmp(const float* const a, const float* const b,
                    const size_t rows, const size_t cols )

    int mat_cmp_e(const float* const a, const float* const b, # epsilon cmp
                const float epsilon, const size_t rows, const size_t cols)

    void mat_zero(float* m, const size_t rows, const size_t cols) # clear to 0.0
    int mat_is_zero(const float* const m, const size_t rows, const size_t cols)

    int mat_is_zero_e(const float * const m, const float epsilon,
                            const size_t rows, const size_t cols)

    float mat_sum(const float* const m, const size_t rows, const size_t cols)

    void mat_identity_ex(float* m, const size_t n)
    void mat_identity(float* m, const size_t n)

    int mat_is_identity(const float* const m, const size_t n) # is default square mat
    int mat_is_identity_e(const float* const m, const float epsilon, const size_t n)

    int mat_is_symmetric(const float* const m, const size_t n) # transpose of M is M
    int mat_is_symmetric_e(const float* const m, const float epsilon, const size_t n)

    void mat_transposed(float* out, const float* const m,
                    const size_t rows, const size_t cols)

    void mat_transpose(float* m, const size_t n)
    float mat_trace(const float* const m, const size_t n)

    void mat_get_diag(float* out, const float* const m, const size_t n)
    void mat_set_diag(float* mat, const float* const v, const size_t n)

    void mat_get_row(float* out, const float* const m, # copy mat row into vec
                    const size_t row, const size_t n_rows, const size_t n_cols)

    void mat_get_col(float* out, const float* const m, # copy mat col into vec
                    const size_t col, const size_t n_rows, const size_t n_cols)

    void mat_set_row(float* m, const float* const vec, # copy vec into mat row
                    const size_t row, const size_t n_rows, const size_t n_cols)

    void mat_set_col(float* m, const float* const vec, # copy vec into mat col
                    const size_t col, const size_t n_rows, const size_t n_cols)

    void mat_swap_rows(float * mat, const size_t r1, const size_t r2,
                            const size_t n_rows, const size_t n_cols)

    void mat_swap_cols(float * mat, const size_t c1, const size_t c2,
                            const size_t n_rows, const size_t n_cols)

    void mat_get_mat(float* dst, const size_t dst_rows, const size_t dst_cols,
        const float * const src, const size_t src_rows, const size_t src_cols,
        const size_t src_row_offset, const size_t src_col_offset)

    void mat_set_mat(float* dst, const size_t dst_rows, const size_t dst_cols,
        const float * const src, const size_t src_rows, const size_t src_cols,
        const size_t dst_row_offset, const size_t dst_col_offset)

    int mat_inverse(float* out, const float* const m, const size_t n)
    int mat_invert(float* m, const size_t n)

    float mat_determinant(const float* const m, const size_t n)

    void mat_mul_mat(float* out, const float* const a, const float* const b,
                    const size_t rows, const size_t cols) # unsafe multiply

    void mat_mul_mat_safe(float* out, const float* const a, # allow aliasing
                const float* const b, const size_t rows, const size_t cols)

    void mat_mul_flt(float* out, const float* const m, const float f,
                                const size_t rows, const size_t cols)

# ===== [ matrix class ] =======================================================

cdef class Matrix:
    cdef float* m

    cdef readonly size_t rows
    cdef readonly size_t cols

    def __init__(self, size_t rows, size_t cols):
        self.rows = rows
        self.cols = cols

        self.m = ae_matrix_alloc(rows, cols)

        # only square matrices have id (zero otherwise)
        if rows == cols: mat_identity_ex(self.m, rows)

    def __dealloc__(self):
        ae_matrix_free(self.m, self.rows, self.cols)

    def __repr__(self):
        return "Matrix({}x{})".format(self.rows, self.cols)

    def __richcmp__(self, Matrix other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Matrix')

    def __copy__(self):
        cdef Matrix m = Matrix(self.rows, self.cols)
        mat_copy( m.m, self.m, self.rows, self.cols)

        return m

    copy = __copy__

    def __nonzero__(self):
        return not mat_is_zero(self.m, self.rows, self.cols)

    def __len__(self):
        return self.rows * self.cols

    def __getitem__(self, int key):
        if key < 0 or key >= <int>(self.rows * self.cols): raise IndexError
        return self.m[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key >= <int>(self.rows * self.cols): raise IndexError
        self.m[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * (self.rows * self.cols * sizeof(float))
        cdef char* p = <char *>b

        mat_copy(<float*>p, self.m, self.rows, self.cols)
        return (Matrix, (self.rows, self.cols), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        mat_copy(self.m, <float*>p, self.rows, self.cols)

    def address(self):
        return <size_t>self.m

    def cmp(self, Matrix other):
        assert (self.rows == other.rows and # do matrix dimensions match?
                self.cols == other.cols, "{} {}".format(self, other))

        return mat_cmp(self.m, other.m, self.rows, self.cols)

    def cmp_e(self, Matrix other, float epsilon):
        assert (self.rows == other.rows and # do matrix dimensions match?
                self.cols == other.cols, "{} {}".format(self, other))

        return mat_cmp_e(self.m, other.m, epsilon, self.rows, self.cols)

    def zero(self):
        mat_zero(self.m, self.rows, self.cols); return self

    def is_zero(self):
        return mat_is_zero(self.m, self.rows, self.cols)

    def is_zero_e(self, float epsilon):
        return mat_is_zero_e(self.m, epsilon, self.rows, self.cols)

    def sum(self):
        return mat_sum(self.m, self.rows, self.cols)

    def is_square(self):
        return self.rows == self.cols

    def identity(self):
        if self.rows == self.cols: mat_identity(self.m, self.rows)
        else: raise RuntimeError("non-square id: {}".format(self))

        return self

    def is_identity(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square id: {}".format(self))
        else:
            return mat_is_identity(self.m, self.rows)

    def is_identity_e(self, float epsilon):
        if self.rows != self.cols:
            raise RuntimeError("non-square id: {}".format(self))
        else:
            return mat_is_identity_e(self.m, epsilon, self.rows)

    def is_symmetric(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            return mat_is_symmetric(self.m, self.rows)

    def is_symmetric_e(self, float epsilon):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            return mat_is_symmetric_e(self.m, epsilon, self.rows)

    def fix_denormals_ex(self, float denormal):
        raise NotImplementedError("TODO")

    def fix_denormals(self):
        raise NotImplementedError("TODO")

    def transpose(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            mat_transpose(self.m, self.rows) # in-place swapping

        return self

    def transposed(self):
        assert self.is_square(), "FIXME transpose is broken " \
                        "for non-square matrix {}".format(self)

        cdef Matrix m = Matrix(self.cols, self.rows)
        mat_transposed(m.m, self.m, m.rows, m.cols); return m

    def trace(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            return mat_trace(self.m, self.rows) # sum of diagonal

    def get_diag(self):
        cdef Vector v = Vector(self.rows)

        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            mat_get_diag(v.v, self.m, self.rows); return v

    def set_diag(self, Vector val):
        assert len(val) == self.rows, "{} {}".format(self, val)

        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            mat_set_diag(self.m, val.v, self.rows); return self

    def get_row(self, size_t row):
        cdef Vector v = Vector(self.cols)
        mat_get_row(v.v, self.m, row, self.rows, self.cols); return v

    def get_col(self, size_t col):
        cdef Vector v = Vector(self.rows)
        mat_get_col(v.v, self.m, col, self.rows, self.cols); return v

    def set_row(self, size_t row, Vector val):
        assert len(val) == self.cols, "{} {}".format(self, val)
        mat_set_row(self.m, val.v, row, self.rows, self.cols); return self

    def set_col(self, size_t col, Vector val):
        assert len(val) == self.rows, "{} {}".format(self, val)
        mat_set_col(self.m, val.v, col, self.rows, self.cols); return self

    def swap_rows(self, size_t r1, size_t r2):
        mat_swap_rows(self.m, r1, r2, self.rows, self.cols); return self

    def swap_cols(self, size_t c1, size_t c2):
        mat_swap_cols(self.m, c1, c2, self.rows, self.cols); return self

    def get_matrix(self, size_t row, size_t col, size_t n_rows, size_t n_cols):
        cdef Matrix m = Matrix(n_rows, n_cols)
        mat_get_mat(m.m, n_rows, n_cols, self.m, self.rows, self.cols, row, col); return m

    def set_matrix(self, size_t row, size_t col, Matrix m):
        mat_set_mat(self.m, self.rows, self.cols, m.m, m.rows, m.cols, row, col); return self

    def get_mat4x4(self, size_t row, size_t col):
        cdef Mat4x4 m = Mat4x4()
        mat_get_mat(m.m, 4, 4, self.m, self.rows, self.cols, row, col); return m

    def set_mat4x4(self, size_t row, size_t col, Mat4x4 m):
        mat_set_mat(self.m, self.rows, self.cols, m.m, 4, 4, row, col); return self

    def get_mat3x3(self, size_t row, size_t col):
        cdef Mat3x3 m = Mat3x3()
        mat_get_mat(m.m, 3, 3, self.m, self.rows, self.cols, row, col); return m

    def set_mat3x3(self, size_t row, size_t col, Mat3x3 m):
        mat_set_mat(self.m, self.rows, self.cols, m.m, 3, 3, row, col); return self

    def get_mat2x2(self, size_t row, size_t col):
        cdef Mat2x2 m = Mat2x2()
        mat_get_mat(m.m, 2, 2, self.m, self.rows, self.cols, row, col); return m

    def set_mat2x2(self, size_t row, size_t col, Mat2x2 m):
        mat_set_mat(self.m, self.rows, self.cols, m.m, 2, 2, row, col); return self

    def set_v(self, *vals):
        """Set every value in the matrix in row order. Note that CPython
        has a built-in limit of 255 function arguments (16 * 16 - 1)."""

        cdef double val
        cdef int idx

        assert len(self) == len(vals), "{} {}".format(self, len(vals))

        # if len(self) != len(vals):
        #     raise TypeError("set_v() takes exactly {} positional " \
        #         "arguments ({} given)".format(len(self), len(vals)))

        for idx, val in enumerate(vals): self.m[idx] = <float>val
        return self

    def invert(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            if mat_invert(self.m, self.rows):
                return self
            else:
                raise RuntimeError("{} is singular".format(self))

    def inverse(self):
        cdef Matrix m = Matrix(self.rows, self.cols)

        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            if mat_inverse(m.m, self.m, self.rows):
                return m
            else:
                raise RuntimeError("{} is singular".format(self))

    def determinant(self):
        if self.rows != self.cols:
            raise RuntimeError("non-square mat: {}".format(self))
        else:
            return mat_determinant(self.m, self.rows)

    def mul_mat(self, Matrix other):
        assert (self.rows == other.rows and # do matrix dimensions match?
                self.cols == other.cols, "{} {}".format(self, other))

        cdef Matrix m = Matrix(self.rows, self.cols)
        mat_mul_mat(m.m, self.m, other.m, self.rows, self.cols); return m

    def imul_mat(self, Matrix other):
        assert (self.rows == other.rows and # do matrix dimensions match?
                self.cols == other.cols, "{} {}".format(self, other))

        mat_mul_mat_safe(self.m, self.m, other.m, self.rows, self.cols)
        return self

    def mul_flt(self, float val):
        cdef Matrix m = Matrix(self.rows, self.cols) # scale all vals in mat
        mat_mul_flt(m.m, self.m, val, self.rows, self.cols); return m

    def imul_flt(self, float val):
        mat_mul_flt(self.m, self.m, val, self.rows, self.cols); return self

# ==============================================================================
# ~ [ fixed-size matrix math ]
# ==============================================================================

cdef extern from "ae_math.h":
    void mat2x2copy(float dst[ 4], const float src[ 4])
    void mat3x3copy(float dst[ 9], const float src[ 9])
    void mat4x4copy(float dst[16], const float src[16])

    int mat2x2cmp(const float a[ 4], const float b[ 4])
    int mat3x3cmp(const float a[ 9], const float b[ 9])
    int mat4x4cmp(const float a[16], const float b[16])

    int mat2x2cmp_e(const float a[ 4], const float b[ 4], const float epsilon)
    int mat3x3cmp_e(const float a[ 9], const float b[ 9], const float epsilon)
    int mat4x4cmp_e(const float a[16], const float b[16], const float epsilon)

    void mat2x2zero(float m[ 4])
    void mat3x3zero(float m[ 9])
    void mat4x4zero(float m[16])

    int mat2x2is_zero(const float m[ 4])
    int mat3x3is_zero(const float m[ 9])
    int mat4x4is_zero(const float m[16])

    int mat2x2is_zero_e(const float m[ 4], const float epsilon)
    int mat3x3is_zero_e(const float m[ 9], const float epsilon)
    int mat4x4is_zero_e(const float m[16], const float epsilon)

    float mat2x2sum(const float v[ 4])
    float mat3x3sum(const float v[ 9])
    float mat4x4sum(const float v[16])

    void mat2x2identity_ex(float m[ 4])
    void mat3x3identity_ex(float m[ 9])
    void mat4x4identity_ex(float m[16])

    void mat2x2identity(float m[ 4])
    void mat3x3identity(float m[ 9])
    void mat4x4identity(float m[16])

    int mat2x2is_identity(const float m[ 4])
    int mat3x3is_identity(const float m[ 9])
    int mat4x4is_identity(const float m[16])

    int mat2x2is_identity_e(const float m[ 4], const float epsilon)
    int mat3x3is_identity_e(const float m[ 9], const float epsilon)
    int mat4x4is_identity_e(const float m[16], const float epsilon)

    int mat2x2is_symmetric(const float m[ 4])
    int mat3x3is_symmetric(const float m[ 9])
    int mat4x4is_symmetric(const float m[16])

    int mat2x2is_symmetric_e(const float m[ 4], const float epsilon)
    int mat3x3is_symmetric_e(const float m[ 9], const float epsilon)
    int mat4x4is_symmetric_e(const float m[16], const float epsilon)

    void mat2x2transposed(float out[ 4], const float m[ 4])
    void mat3x3transposed(float out[ 9], const float m[ 9])
    void mat4x4transposed(float out[16], const float m[16])

    void mat2x2transpose(float m[ 4])
    void mat3x3transpose(float m[ 9])
    void mat4x4transpose(float m[16])

    float mat2x2trace(const float m[ 4])
    float mat3x3trace(const float m[ 9])
    float mat4x4trace(const float m[16])

    void mat2x2get_diag(float v[2], const float m[ 4])
    void mat3x3get_diag(float v[3], const float m[ 9])
    void mat4x4get_diag(float v[4], const float m[16])

    void mat2x2set_diag(float m[ 4], const float v[2])
    void mat3x3set_diag(float m[ 9], const float v[3])
    void mat4x4set_diag(float m[16], const float v[4])

    void mat2x2get_row(float vec[2], const float m[ 4], const size_t row)
    void mat3x3get_row(float vec[3], const float m[ 9], const size_t row)
    void mat4x4get_row(float vec[4], const float m[16], const size_t row)

    void mat2x2get_col(float vec[2], const float m[ 4], const size_t col)
    void mat3x3get_col(float vec[3], const float m[ 9], const size_t col)
    void mat4x4get_col(float vec[4], const float m[16], const size_t col)

    void mat2x2set_row(float mat[ 4], const float v[2], const size_t row)
    void mat3x3set_row(float mat[ 9], const float v[3], const size_t row)
    void mat4x4set_row(float mat[16], const float v[4], const size_t row)

    void mat2x2set_col(float mat[ 4], const float v[2], const size_t col)
    void mat3x3set_col(float mat[ 9], const float v[3], const size_t col)
    void mat4x4set_col(float mat[16], const float v[4], const size_t col)

    void mat2x2swap_rows(float m[ 4], const size_t r1, const size_t r2)
    void mat3x3swap_rows(float m[ 9], const size_t r1, const size_t r2)
    void mat4x4swap_rows(float m[16], const size_t r1, const size_t r2)

    void mat2x2swap_cols(float m[ 4], const size_t c1, const size_t c2)
    void mat3x3swap_cols(float m[ 9], const size_t c1, const size_t c2)
    void mat4x4swap_cols(float m[16], const size_t c1, const size_t c2)

    int mat2x2inverse(float out[ 4], const float m[ 4])
    int mat3x3inverse(float out[ 9], const float m[ 9])
    int mat4x4inverse(float out[16], const float m[16])

    int mat2x2invert(float m[ 4])
    int mat3x3invert(float m[ 9])
    int mat4x4invert(float m[16])

    float mat2x2determinant(const float m[ 4])
    float mat3x3determinant(const float m[ 9])
    float mat4x4determinant(const float m[16])

    void mat2x2mul_mat(float out[ 4], const float a[ 4], const float b[ 4])
    void mat3x3mul_mat(float out[ 9], const float a[ 9], const float b[ 9])
    void mat4x4mul_mat(float out[16], const float a[16], const float b[16])

    void mat2x2mul_mat_safe(float out[ 4], const float a[ 4], const float b[ 4])
    void mat3x3mul_mat_safe(float out[ 9], const float a[ 9], const float b[ 9])
    void mat4x4mul_mat_safe(float out[16], const float a[16], const float b[16])

    void mat2x2mul_flt(float out[ 4], const float mat[ 4], const float val)
    void mat3x3mul_flt(float out[ 9], const float mat[ 9], const float val)
    void mat4x4mul_flt(float out[16], const float mat[16], const float val)

    void mat4x4row_mul_point3(float outp[3], const float mat[16], const float p[3])
    void mat4x4col_mul_point3(float out[3], const float mat[16], const float p[3])

    void mat4x4row_mul_point3_safe(float out[3], const float mat[16], const float p[3])
    void mat4x4col_mul_point3_safe(float out[3], const float mat[16], const float p[3])

    void mat4x4row_mul_vector3(float out[3], const float mat[16], const float v[3])
    void mat4x4col_mul_vector3(float out[3], const float mat[16], const float v[3])

    void mat4x4row_mul_vector3_safe(float out[3], const float mat[16], const float v[3])
    void mat4x4col_mul_vector3_safe(float out[3], const float mat[16], const float v[3])

    void mat4x4row_to_quat(float q[4], const float m[16])
    void mat4x4col_to_quat(float q[4], const float m[16])

    # ===== [ construction (angles in radians) ] ===============================

    void mat4x4row_translate(float out[16], const float xyz[3])
    void mat4x4col_translate(float out[16], const float xyz[3])

    void mat4x4row_scale(float out[16], const float xyz[3])
    void mat4x4col_scale(float out[16], const float xyz[3])

    void mat4x4row_rotate_x(float out[16], const float angle)
    void mat4x4col_rotate_x(float out[16], const float angle)

    void mat4x4row_rotate_y(float out[16], const float angle)
    void mat4x4col_rotate_y(float out[16], const float angle)

    void mat4x4row_rotate_z(float out[16], const float angle)
    void mat4x4col_rotate_z(float out[16], const float angle)

    void mat4x4row_rotate_axis(float out[16], const float angle, const float axis[3])
    void mat4x4col_rotate_axis(float out[16], const float angle, const float axis[3])

    void mat4x4row_rotate_euler(float out[16], float heading, float attitude, float bank)
    void mat4x4col_rotate_euler(float out[16], float heading, float attitude, float bank)

    void mat4x4row_rotate_triple_axis(float out[16], float x[3], float y[3], float z[3])
    void mat4x4col_rotate_triple_axis(float out[16], float x[3], float y[3], float z[3])

    void mat4x4row_perspective(float outm[16], const float fovy,
        const float aspect, const float znear, const float zfar)

    void mat4x4col_perspective(float outm[16], const float fovy,
        const float aspect, const float znear, const float zfar)

# ===== [ matrix classes ] =====================================================

cdef class Mat2x2:
    cdef float m[4]

    # since the matrix memory is already zero-initialized, just fill in the ones
    def __init__(self): mat2x2identity_ex(self.m)

    def __repr__(self):
        return ('Mat2x2([% 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f])') % tuple(self)

    def __richcmp__(self, Mat2x2 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Mat2x2')

    def __copy__(self):
        cdef Mat2x2 m = Mat2x2()
        mat2x2copy(m.m, self.m); return m

    copy = __copy__

    def __nonzero__(self):
        return not mat2x2is_zero(self.m)

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.m[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 3: raise IndexError
        self.m[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[4])
        cdef float * p = <float *> (<char *> b)

        mat2x2copy(p, self.m)
        return(Mat2x2, (), b)

    def __setstate__(self, bytes state):
        mat2x2copy(self.m, <float*>(<char*>state))

    def address(self):
        return <size_t>self.m

    def cmp(self, Mat2x2 other):
        return mat2x2cmp(self.m, other.m)

    def cmp_e(self, Mat2x2 other, float epsilon):
        return mat2x2cmp_e(self.m, other.m, epsilon)

    def zero(self):
        mat2x2zero(self.m); return self

    def is_zero(self):
        return mat2x2is_zero(self.m)

    def is_zero_e(self, float epsilon):
        return mat2x2is_zero_e(self.m, epsilon)

    def sum(self):
        return mat2x2sum(self.m)

    def is_square(self):
        return True # num rows = num cols

    def identity(self):
        mat2x2identity(self.m); return self

    def is_identity(self):
        return mat2x2is_identity(self.m)

    def is_identity_e(self, float epsilon):
        return mat2x2is_identity_e(self.m, epsilon)

    def is_symmetric(self):
        return mat2x2is_symmetric(self.m)

    def is_symmetric_e(self, float epsilon):
        return mat2x2is_symmetric_e(self.m, epsilon)

    def fix_denormals_ex(self, float denormal):
        raise NotImplementedError("TODO")

    def fix_denormals(self):
        raise NotImplementedError("TODO")

    def transpose(self):
        mat2x2transpose(self.m); return self

    def transposed(self):
        cdef Mat2x2 m = Mat2x2() # out-of-place
        mat2x2transposed(m.m, self.m); return m

    def trace(self):
        return mat2x2trace(self.m)

    def get_diag(self):
        cdef Vec2 v = Vec2()
        mat2x2get_diag(v.v, self.m); return v

    def set_diag(self, Vec2 v):
        mat2x2set_diag(self.m, v.v); return self

    def get_row(self, size_t row):
        cdef Vec2 v = Vec2()
        mat2x2get_row(v.v, self.m, row); return v

    def get_col(self, size_t col):
        cdef Vec2 v = Vec2()
        mat2x2get_col(v.v, self.m, col); return v

    def set_row(self, size_t row, Vec2 val):
        mat2x2set_row(self.m, val.v, row); return self

    def set_col(self, size_t col, Vec2 val):
        mat2x2set_col(self.m, val.v, col); return self

    def swap_rows(self, size_t r1, size_t r2):
        mat2x2swap_rows(self.m, r1, r2); return self

    def swap_cols(self, size_t c1, size_t c2):
        mat2x2swap_cols(self.m, c1, c2); return self

    def set_v(self, float a, float b, float c, float d):
        self.m[0] = a; self.m[1] = b;
        self.m[2] = c; self.m[3] = d; return self

    def invert(self):
        if mat2x2invert(self.m): return self # for vec transform
        else: raise RuntimeError("{} is singular".format(self))

    def inverse(self):
        cdef Mat2x2 m = Mat2x2()

        if mat2x2inverse(m.m, self.m): return m # for vec xforms
        else: raise RuntimeError("{} is singular".format(self))

    def determinant(self):
        return mat2x2determinant(self.m)

    def mul_mat(self, Mat2x2 other):
        cdef Mat2x2 m = Mat2x2() # s * m out of place
        mat2x2mul_mat(m.m, self.m, other.m); return m

    def imul_mat(self, Mat2x2 other):
        mat2x2mul_mat_safe(self.m, self.m, other.m); return self

    def mul_flt(self, float val):
        cdef Mat2x2 m = Mat2x2() # scale all matrix vals
        mat2x2mul_flt(m.m, self.m, val); return m

    def imul_flt(self, float val):
        mat2x2mul_flt(self.m, self.m, val); return self

cdef class Mat3x3:
    cdef float m[9]

    # since the matrix memory is already zero-initialized, just fill in the ones
    def __init__(self): mat3x3identity_ex(self.m)

    def __repr__(self):
        return ('Mat3x3([% 8.2f % 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f % 8.2f])') % tuple(self)

    def __richcmp__(self, Mat3x3 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Mat3x3')

    def __copy__(self):
        cdef Mat3x3 m = Mat3x3()
        mat3x3copy(m.m, self.m); return m

    copy = __copy__

    def __nonzero__(self):
        return not mat3x3is_zero(self.m)

    def __len__(self): return 9

    def __getitem__(self, int key):
        if key < 0 or key > 8: raise IndexError
        return self.m[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 8: raise IndexError
        self.m[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[9])
        cdef float * p = <float *> (<char *> b)

        mat3x3copy(p, self.m)
        return(Mat3x3, (), b)

    def __setstate__(self, bytes state):
        mat3x3copy(self.m, <float*>(<char*>state))

    def address(self):
        return <size_t>self.m

    def cmp(self, Mat3x3 other):
        return mat3x3cmp(self.m, other.m)

    def cmp_e(self, Mat3x3 other, float epsilon):
        return mat3x3cmp_e(self.m, other.m, epsilon)

    def zero(self):
        mat3x3zero(self.m); return self

    def is_zero(self):
        return mat3x3is_zero(self.m)

    def is_zero_e(self, float epsilon):
        return mat3x3is_zero_e(self.m, epsilon)

    def sum(self):
        return mat3x3sum(self.m)

    def is_square(self):
        return True # num rows = num cols

    def identity(self):
        mat3x3identity(self.m); return self

    def is_identity(self):
        return mat3x3is_identity(self.m)

    def is_identity_e(self, float epsilon):
        return mat3x3is_identity_e(self.m, epsilon)

    def is_symmetric(self):
        return mat3x3is_symmetric(self.m)

    def is_symmetric_e(self, float epsilon):
        return mat3x3is_symmetric_e(self.m, epsilon)

    def fix_denormals_ex(self, float denormal):
        raise NotImplementedError("TODO")

    def fix_denormals(self):
        raise NotImplementedError("TODO")

    def transpose(self):
        mat3x3transpose(self.m); return self

    def transposed(self):
        cdef Mat3x3 m = Mat3x3() # out-of-place
        mat3x3transposed(m.m, self.m); return m

    def trace(self):
        return mat3x3trace(self.m)

    def get_diag(self):
        cdef Vec3 v = Vec3()
        mat3x3get_diag(v.v, self.m); return v

    def set_diag(self, Vec3 v):
        mat3x3set_diag(self.m, v.v); return self

    def get_row(self, size_t row):
        cdef Vec3 v = Vec3()
        mat3x3get_row(v.v, self.m, row); return v

    def get_col(self, size_t col):
        cdef Vec3 v = Vec3()
        mat3x3get_col(v.v, self.m, col); return v

    def set_row(self, size_t row, Vec3 val):
        mat3x3set_row(self.m, val.v, row); return self

    def set_col(self, size_t col, Vec3 val):
        mat3x3set_col(self.m, val.v, col); return self

    def swap_rows(self, size_t r1, size_t r2):
        mat3x3swap_rows(self.m, r1, r2); return self

    def swap_cols(self, size_t c1, size_t c2):
        mat3x3swap_cols(self.m, c1, c2); return self

    def get_mat2x2(self, size_t row, size_t col):
        cdef Mat2x2 m = Mat2x2()
        mat_get_mat(m.m, 2, 2, self.m, 3, 3, row, col); return m

    def set_mat2x2(self, size_t row, size_t col, Mat2x2 m):
        mat_set_mat(self.m, 3, 3, m.m, 2, 2, row, col); return self

    def set_v(self, float a, float b, float c,
                    float d, float e, float f,
                    float g, float h, float i):
        self.m[0] = a; self.m[1] = b; self.m[2] = c
        self.m[3] = d; self.m[4] = e; self.m[5] = f
        self.m[6] = g; self.m[7] = h; self.m[8] = i

        return self

    def invert(self):
        if mat3x3invert(self.m): return self # for vec transform
        else: raise RuntimeError("{} is singular".format(self))

    def inverse(self):
        cdef Mat3x3 m = Mat3x3()

        if mat3x3inverse(m.m, self.m): return m # for vec xforms
        else: raise RuntimeError("{} is singular".format(self))

    def determinant(self):
        return mat3x3determinant(self.m)

    def mul_mat(self, Mat3x3 other):
        cdef Mat3x3 m = Mat3x3() # s * m out of place
        mat3x3mul_mat(m.m, self.m, other.m); return m

    def imul_mat(self, Mat3x3 other):
        mat3x3mul_mat_safe(self.m, self.m, other.m); return self

    def mul_flt(self, float val):
        cdef Mat3x3 m = Mat3x3() # scale all matrix vals
        mat3x3mul_flt(m.m, self.m, val); return m

    def imul_flt(self, float val):
        mat3x3mul_flt(self.m, self.m, val); return self

cdef class Mat4x4:
    cdef float m[16]

    # since the matrix memory is already zero-initialized, just fill in the ones
    def __init__(self): mat4x4identity_ex(self.m)

    def __repr__(self):
        return ('Mat4x4([% 8.2f % 8.2f % 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f % 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f % 8.2f % 8.2f\n'\
                '        % 8.2f % 8.2f % 8.2f % 8.2f])') % tuple(self)

    def __richcmp__(self, Mat4x4 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Mat4x4')

    def __copy__(self):
        cdef Mat4x4 m = Mat4x4()
        mat4x4copy(m.m, self.m); return m

    copy = __copy__

    def __nonzero__(self):
        return not mat4x4is_zero(self.m)

    def __len__(self): return 16

    def __getitem__(self, int key):
        if key < 0 or key > 15: raise IndexError
        return self.m[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 15: raise IndexError
        self.m[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[16])
        cdef float * p = <float *> ( <char *>b )

        mat4x4copy(p, self.m)
        return(Mat4x4, (), b)

    def __setstate__(self, bytes state):
        mat4x4copy(self.m, <float*>(<char*>state))

    def address(self):
        return <size_t>self.m

    def cmp(self, Mat4x4 other):
        return mat4x4cmp(self.m, other.m)

    def cmp_e(self, Mat4x4 other, float epsilon):
        return mat4x4cmp_e(self.m, other.m, epsilon)

    def zero(self):
        mat4x4zero(self.m); return self

    def is_zero(self):
        return mat4x4is_zero(self.m)

    def is_zero_e(self, float epsilon):
        return mat4x4is_zero_e(self.m, epsilon)

    def sum(self):
        return mat4x4sum(self.m)

    def is_square(self):
        return True # num rows = num cols

    def identity(self):
        mat4x4identity(self.m); return self

    def is_identity(self):
        return mat4x4is_identity(self.m)

    def is_identity_e(self, float epsilon):
        return mat4x4is_identity_e(self.m, epsilon)

    def is_symmetric(self):
        return mat4x4is_symmetric(self.m)

    def is_symmetric_e(self, float epsilon):
        return mat4x4is_symmetric_e(self.m, epsilon)

    def fix_denormals_ex(self, float denormal):
        raise NotImplementedError("TODO")

    def fix_denormals(self):
        raise NotImplementedError("TODO")

    def transpose(self):
        mat4x4transpose(self.m); return self

    def transposed(self):
        cdef Mat4x4 m = Mat4x4() # out-of-place
        mat4x4transposed(m.m, self.m); return m

    def trace(self):
        return mat4x4trace(self.m)

    def get_diag(self):
        cdef Vec4 v = Vec4()
        mat4x4get_diag(v.v, self.m); return v

    def set_diag(self, Vec4 v):
        mat4x4set_diag(self.m, v.v); return self

    def get_row(self, size_t row):
        cdef Vec4 v = Vec4()
        mat4x4get_row(v.v, self.m, row); return v

    def get_col(self, size_t col):
        cdef Vec4 v = Vec4()
        mat4x4get_col(v.v, self.m, col); return v

    def set_row(self, size_t row, Vec4 val):
        mat4x4set_row(self.m, val.v, row); return self

    def set_col(self, size_t col, Vec4 val):
        mat4x4set_col(self.m, val.v, col); return self

    def swap_rows(self, size_t r1, size_t r2):
        mat4x4swap_rows(self.m, r1, r2); return self

    def swap_cols(self, size_t c1, size_t c2):
        mat4x4swap_cols(self.m, c1, c2); return self

    def get_mat3x3(self, size_t row, size_t col):
        cdef Mat3x3 m = Mat3x3()
        mat_get_mat(m.m, 3, 3, self.m, 4, 4, row, col); return m

    def set_mat3x3(self, size_t row, size_t col, Mat3x3 m):
        mat_set_mat(self.m, 4, 4, m.m, 3, 3, row, col); return self

    def get_mat2x2(self, size_t row, size_t col):
        cdef Mat2x2 m = Mat2x2()
        mat_get_mat(m.m, 2, 2, self.m, 4, 4, row, col); return m

    def set_mat2x2(self, size_t row, size_t col, Mat2x2 m):
        mat_set_mat(self.m, 4, 4, m.m, 2, 2, row, col); return self

    def set_v(self, float a, float b, float c, float d,
                    float e, float f, float g, float h,
                    float i, float j, float k, float l,
                    float m, float n, float o, float p):
        self.m[ 0] = a; self.m[ 1] = b; self.m[ 2] = c; self.m[ 3] = d
        self.m[ 4] = e; self.m[ 5] = f; self.m[ 6] = g; self.m[ 7] = h
        self.m[ 8] = i; self.m[ 9] = j; self.m[10] = k; self.m[11] = l
        self.m[12] = m; self.m[13] = n; self.m[14] = o; self.m[15] = p

        return self

    def invert(self):
        if mat4x4invert(self.m): return self # for vec transform
        else: raise RuntimeError("{} is singular".format(self))

    def inverse(self):
        cdef Mat4x4 m = Mat4x4()

        if mat4x4inverse(m.m, self.m): return m # for vec xforms
        else: raise RuntimeError("{} is singular".format(self))

    def determinant(self):
        return mat4x4determinant(self.m)

    def row_to_quat(self):
        cdef Quat q = Quat() # row matrix to quat
        mat4x4row_to_quat(q.q, self.m); return q

    def col_to_quat(self):
        cdef Quat q = Quat() # col matrix to quat
        mat4x4col_to_quat(q.q, self.m); return q

    def mul_mat(self, Mat4x4 other):
        cdef Mat4x4 m = Mat4x4() # s * m out of place
        mat4x4mul_mat(m.m, self.m, other.m); return m

    def imul_mat(self, Mat4x4 other):
        mat4x4mul_mat_safe(self.m, self.m, other.m); return self

    def mul_flt(self, float val):
        cdef Mat4x4 m = Mat4x4() # scale all matrix vals
        mat4x4mul_flt(m.m, self.m, val); return m

    def imul_flt(self, float val):
        mat4x4mul_flt(self.m, self.m, val); return self

    def row_mul_point3(self, Vec3 p):
        cdef Vec3 out = Vec3()
        mat4x4row_mul_point3(out.v, self.m, p.v); return out

    def col_mul_point3(self, Vec3 p):
        cdef Vec3 out = Vec3()
        mat4x4col_mul_point3(out.v, self.m, p.v); return out

    def row_mul_vector3(self, Vec3 v):
        cdef Vec3 out = Vec3()
        mat4x4row_mul_vector3(out.v, self.m, v.v); return out

    def col_mul_vector3(self, Vec3 v):
        cdef Vec3 out = Vec3()
        mat4x4col_mul_vector3(out.v, self.m, v.v); return out

    # ===== [ construction (angles in radians) ] ===============================

    def row_translate(self, Vec3 xyz):
        mat4x4row_translate(self.m, xyz.v); return self

    def col_translate(self, Vec3 xyz):
        mat4x4col_translate(self.m, xyz.v); return self

    def row_scale(self, Vec3 xyz):
        mat4x4row_scale(self.m, xyz.v); return self

    def col_scale(self, Vec3 xyz):
        mat4x4col_scale(self.m, xyz.v); return self

    def row_rotate_x(self, float angle):
        mat4x4row_rotate_x(self.m, angle); return self

    def col_rotate_x(self, float angle):
        mat4x4col_rotate_x(self.m, angle); return self

    def row_rotate_y(self, float angle):
        mat4x4row_rotate_y(self.m, angle); return self

    def col_rotate_y(self, float angle):
        mat4x4col_rotate_y(self.m, angle); return self

    def row_rotate_z(self, float angle):
        mat4x4row_rotate_z(self.m, angle); return self

    def col_rotate_z(self, float angle):
        mat4x4col_rotate_z(self.m, angle); return self

    def row_rotate_axis(self, float angle, Vec3 axis):
        mat4x4row_rotate_axis(self.m, angle, axis.v); return self

    def col_rotate_axis(self, float angle, Vec3 axis):
        mat4x4col_rotate_axis(self.m, angle, axis.v); return self

    def row_rotate_euler(self, float heading, float attitude, float bank):
        mat4x4row_rotate_euler(self.m, heading, attitude, bank); return self

    def col_rotate_euler(self, float heading, float attitude, float bank):
        mat4x4col_rotate_euler(self.m, heading, attitude, bank); return self

    def row_rotate_triple_axis(self, Vec3 x, Vec3 y, Vec3 z):
        mat4x4row_rotate_triple_axis(self.m, x.v, y.v, z.v); return self

    def col_rotate_triple_axis(self, Vec3 x, Vec3 y, Vec3 z):
        mat4x4col_rotate_triple_axis(self.m, x.v, y.v, z.v); return self

    def row_perspective(self, float fovy, float aspect, float znear, float zfar):
        mat4x4row_perspective(self.m, fovy, aspect, znear, zfar); return self

    def col_perspective(self, float fovy, float aspect, float znear, float zfar):
        mat4x4col_perspective(self.m, fovy, aspect, znear, zfar); return self
