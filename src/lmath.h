
/*
BSD 3-Clause License

Copyright (c) 2022, alinivar
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __lmath_h__
#define __lmath_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif

#include <math.h>

/*! @brief 3 component vector.
 *
 *  3 component vector.
 */
typedef struct {
    float X, Y, Z;
} vec3;

/*! @brief 4x4 Matrix.
 *
 *  4x4 Matrix.
 */
typedef float mat4[16];

/*! @brief Convert degrees into radians
 *
 *  This function convert an angle in degrees to radians.
 * 
 *  @param[in] degrees The angle in degrees.
 *  @return The angle in radians.
 */
static inline float Radians(float degrees) {
    return degrees * (float)M_PI / 180.0f;
}

/*! @brief Construct a new 3 component vector from 3 values.
 *
 *  This function constructs a new 3 component vector from 3 values.
 * 
 *  @param[in] x The X value of the vector.
 *  @param[in] y The Y value of the vector.
 *  @param[in] z The Z value of the vector.
 *  @return The newly constructed vector.
 */
static inline vec3 V3(float x, float y, float z) {
    return (vec3){x, y, z};
}

/*! @brief Construct a new 3 component vector from 1 value.
 *
 *  This function constructs a new 3 component vector from 1 value.
 * 
 *  @param[in] v The value of all 3 components.
 *  @return The newly constructed vector.
 */
static inline vec3 V3All(float v) {
    return V3(v, v, v);
}

/*! @brief Add two vectors.
 *
 *  This function adds two vectors.
 * 
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The result of the addition.
 */
static inline vec3 VecAdd(vec3 first, vec3 second) {
    return (vec3){
        first.X + second.X,
        first.Y + second.Y,
        first.Z + second.Z,
    };
}

/*! @brief Subtract two vectors.
 *
 *  This function subtracts two vectors.
 * 
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The result of the subtraction.
 */
static inline vec3 VecSub(vec3 first, vec3 second) {
    return (vec3){
        first.X - second.X,
        first.Y - second.Y,
        first.Z - second.Z,
    };
}

/*! @brief Multiply two vectors.
 *
 *  This function multiplies two vectors.
 * 
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The result of the multiplication.
 */
static inline vec3 VecMul(vec3 first, vec3 second) {
    return (vec3){
        first.X * second.X,
        first.Y * second.Y,
        first.Z * second.Z,
    };
}

/*! @brief Divide two vectors.
 *
 *  This function divides two vectors.
 *  
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The result of the division.
 */
static inline vec3 VecDiv(vec3 first, vec3 second) {
    return (vec3){
        first.X / second.X,
        first.Y / second.Y,
        first.Z / second.Z,
    };
}

/*! @brief Calculate the dot product of two vectors.
 *
 *  This function calculates the dot product of two vectors.
 * 
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The dot product of two vectors.
 */
static inline float VecDot(vec3 first, vec3 second) {
    return  first.X*second.X + 
            first.Y*second.Y + 
            first.Z*second.Z;
}

/*! @brief Calculate the cross product of two vectors.
 *
 *  This function calculates the cross product of two vectors.
 * 
 *  @param[in] first The first vector.
 *  @param[in] second The second vector.
 *  @return The cross product of two vectors.
 */
static inline vec3 VecCross(vec3 first, vec3 second) {
    return (vec3){
        first.Y * second.Z - second.Y * first.Z,
        first.Z * second.X - second.Z * first.X,
        first.X * second.Y - second.X * first.Y,
    };
}

/*! @brief Calculate the length of a vector.
 *
 *  This function calculates the length of a vector.
 * 
 *  @param[in] v The vector to calculate.
 *  @return The length of the vector.
 */
static inline float VecLength(vec3 v) {
    return sqrtf(VecDot(v, v));
}

/*! @brief Normalize a vector.
 *
 *  This function normalizes a vector.
 *  
 *  @param[in] v The vector to normalize.
 *  @return The normalized vector.
 */
static inline vec3 VecNormalize(vec3 v) {
    const float len = VecLength(v);
    return (vec3){
        v.X / len,
        v.Y / len,
        v.Z / len,
    };
}

/*! @brief Load an identity matrix into a matrix.
 *
 *  This function loads an identity matrix into a matrix.
 * 
 *  @param[in] m The matrix whose data will be replaced with an identity matrix.
 */
static inline void MatIdentity(mat4 m) {
    static const mat4 iden = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };

    for (int i = 0; i < 16; i++) {
        m[i] = iden[i];
    }
}

/*! @brief Multiply a matrix with a matrix.
 *
 *  This function multiplies the first matrix with the second matrix,
 *  the result will be stored in the first matrix.
 * 
 *  @param[in] first The first matrix.
 *  @param[in] second The second matrix.
 */
static inline void MatMulMat(mat4 first, mat4 second) {
    for (int i = 0; i < 4; i++) {
        const float a0 = first[i], a1 = first[i+4], a2 = first[i+8], a3 = first[i + 12];
        first[i]    = a0 * second[0]    + a1 * second[1]    + a2 * second[2]    + a3 * second[3];
        first[i+4]  = a0 * second[4]    + a1 * second[5]    + a2 * second[6]    + a3 * second[7];
        first[i+8]  = a0 * second[8]    + a1 * second[9]    + a2 * second[10]   + a3 * second[11];
        first[i+12] = a0 * second[12]   + a1 * second[13]   + a2 * second[14]   + a3 * second[15];
    }
}

/*! @brief Translate a matrix.
 *
 *  This function translates a matrix according to a vector.
 * 
 *  @param[in] m The matrix to be translated.
 *  @param[in] v The vector that contains the translation.
 */
static inline void MatTranslate(mat4 m, vec3 v) {
    m[12] = m[0] * v.X + m[4] * v.Y + m[8]  * v.Z + m[12];
    m[13] = m[1] * v.X + m[5] * v.Y + m[9]  * v.Z + m[13];
    m[14] = m[2] * v.X + m[6] * v.Y + m[10] * v.Z + m[14];
    m[15] = m[3] * v.X + m[7] * v.Y + m[11] * v.Z + m[15];
}

/*! @brief Scale a matrix.
 *
 *  This function scales a matrix according to a vector.
 * 
 *  @param[in] m The matrix to be scaled.
 *  @param[in] v The vector that contains the scale.
 */
static inline void MatScale(mat4 m, vec3 v) {
    m[0] *= v.X; m[4] *= v.Y; m[8]  *= v.Z;
    m[1] *= v.X; m[5] *= v.Y; m[9]  *= v.Z;
    m[2] *= v.X; m[6] *= v.Y; m[10] *= v.Z;
    m[3] *= v.X; m[7] *= v.Y; m[11] *= v.Z;
}

/*! @brief Rotate a matrix.
 *
 *  This function rotates a matrix by the specified angle around an axis.
 * 
 *  @param[in] m The matrix to be rotated.
 *  @param[in] d The angle in degrees.
 *  @param[in] a The axis to rotate around.
 */
static inline void MatRotate(mat4 m, float d, vec3 a) {
    const float len = VecLength(a);
    if (len > 1e-4) {

        const float x       = a.X / len;
        const float y       = a.Y / len;
        const float z       = a.Z / len;

        const float rad     = Radians(d);
        const float s       = sinf(rad);
        const float c       = cosf(rad);

        const float xx      = x * x;
        const float yy      = y * y;
        const float zz      = z * z;
        const float xy      = x * y;
        const float yz      = y * z;
        const float zx      = z * x;
        const float xs      = x * s;
        const float ys      = y * s;
        const float zs      = z * s;

        const float onec    = 1.0f - c;

        mat4 tmp;
        MatIdentity(tmp);

        tmp[0]              = (onec * xx) + c;
        tmp[4]              = (onec * xy) - zs;
        tmp[8]              = (onec * zx) + ys;

        tmp[1]              = (onec * xy) + zs;
        tmp[5]              = (onec * yy) + c;
        tmp[9]              = (onec * yz) - xs;

        tmp[2]              = (onec * zx) - ys;
        tmp[6]              = (onec * yz) + xs;
        tmp[10]             = (onec * zz) + c;

        MatMulMat(m, tmp);
    }
}

/*! @brief Create an orthographic projection matrix.
 *
 *  This function creates an orthographic projection matrix.
 * 
 *  @param[in] m The matrix to store the orthographic projection matrix.
 */
static inline void MatOrtho(mat4 m, float left, float right, float bottom, float top, float n, float f) {
    mat4 tmp = { 0 };

    tmp[0]      =  2.0f             / (right - left);
    tmp[5]      =  2.0f             / (top - bottom);
    tmp[10]     = -2.0f             / (f - n);
    tmp[12]     = -(right + left)   / (right / left);
    tmp[13]     = -(top + bottom)   / (top - bottom);
    tmp[14]     = -(f + n)          / (f - n);
    tmp[15]     =  1.0f;

    MatMulMat(m, tmp);
}

/*! @brief Create a perspective projection matrix.
 *
 *  This function creates a perspective projection matrix.
 * 
 *  @param[in] m The matrix to store the perspective projection matrix.
 */
static inline void MatFrustum(mat4 m, float left, float right, float bottom, float top, float n, float f) {
    mat4 tmp = { 0 };

    float x, y, a, b, c, d;

    x =  (2.0f * n)         / (right - left);
    y =  (2.0f * n)         / (top - bottom);

    a =  (right + left)     / (right - left);
    b =  (top + bottom)     / (top - bottom);
    c = -(f + n)            / (f - n);
    d = -(2.0f * f * n)     / (f - n);

    tmp[0]      =  x;
    tmp[5]      =  y;
    tmp[8]      =  a;
    tmp[9]      =  b;
    tmp[10]     =  c;
    tmp[11]     = -1.0f;
    tmp[14]     =  d;
    tmp[15]     =  0.0f;

    MatMulMat(m, tmp);
}

/*! @brief Creates a "look-at" view matrix.
 *
 *  This function creates a "look-at" view matrix.
 * 
 *  @param[in] m The matrix to store the "look-at" view matrix.
 *  @param[in] pos The position of the camera.
 *  @param[in] target The target of the camera.
 *  @param[in] up The up direction of the camera.
 */
static inline void MatLookAt(mat4 m, vec3 pos, vec3 target, vec3 up) {
    vec3 zaxis = VecNormalize(VecSub(pos, target));
    vec3 xaxis = VecNormalize(VecCross(up, zaxis));
    vec3 yaxis = VecCross(zaxis, xaxis);

    mat4 tmp = {
         xaxis.X,               yaxis.X,             zaxis.X,               0,
         xaxis.Y,               yaxis.Y,             zaxis.Y,               0,
         xaxis.Z,               yaxis.Z,             zaxis.Z,               0,
        -VecDot(xaxis, pos),   -VecDot(yaxis, pos), -VecDot(zaxis, pos),    1,
    };

    MatMulMat(m, tmp);
}

#ifdef __cplusplus
}
#endif

#endif
