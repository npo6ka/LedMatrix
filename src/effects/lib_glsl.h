#pragma once

#include "pixeltypes.h"

namespace glsl {

#define DEF_VEC2_ARITH_OP(op1, op2)                     \
    inline vec2& operator op1 (const vec2& v)           \
    { x op1 v.x; y op1 v.y; return *this; }             \
                                                        \
    inline vec2 operator op2 (const vec2& v)            \
    { vec2 ret(*this); ret op1 v; return ret; }         \
                                                        \
    inline vec2 operator op2 (float v)                  \
    { vec2 ret(*this); ret op1 v; return ret; }

#define DEF_VEC3_ARITH_OP(op1, op2)                     \
    inline vec3& operator op1 (const vec3& v)           \
    { x op1 v.x; y op1 v.y; z op1 v.z; return *this; }  \
                                                        \
    inline vec3 operator op2 (const vec3& v)            \
    { vec3 ret(*this); ret op1 v; return ret; }         \
                                                        \
    inline vec3 operator op2 (float v)                  \
    { vec3 ret(*this); ret op1 v; return ret; }


#define DEF_VEC2_OF(v1, v2) \
    vec2 v1##v2() const     \
    { return { v1, v2 }; }  \

struct vec2 {
    float x;
    float y;

    inline vec2() : x(0.f), y(0.f)
    {}

    inline vec2(float x, float y) : x(x), y(y)
    {}

    inline vec2(float xy) : vec2(xy, xy)
    {}

    DEF_VEC2_OF(x, x)
    DEF_VEC2_OF(y, y)
    DEF_VEC2_OF(x, y)
    DEF_VEC2_OF(y, x)

    DEF_VEC2_ARITH_OP(+=, +)
    DEF_VEC2_ARITH_OP(-=, -)
    DEF_VEC2_ARITH_OP(*=, *)
    DEF_VEC2_ARITH_OP(/=, /)
};

struct vec3 : public vec2 {
    float z;

    inline vec3() : vec2(0.f), z(0.f)
    {}

    inline vec3(float x, float y, float z) : vec2(x, y), z(z)
    {}

    inline vec3(vec2 xy, float z) : vec2(xy), z(z)
    {}

    inline vec3(float xyz) : vec2(xyz), z(xyz)
    {}

    DEF_VEC2_OF(y, y)
    DEF_VEC2_OF(x, z)
    DEF_VEC2_OF(y, z)
    DEF_VEC2_OF(z, x)
    DEF_VEC2_OF(z, y)

    DEF_VEC3_ARITH_OP(+=, +)
    DEF_VEC3_ARITH_OP(-=, -)
    DEF_VEC3_ARITH_OP(*=, *)
    DEF_VEC3_ARITH_OP(/=, /)

    inline operator CRGB() const
    { return CRGB(x * 255, y * 255, z * 255); }
};

//
// Trigonometry
//

// sin

#define gl_sin glsl::sin

inline float sin(float rad)
{ return static_cast<float>(sin16(rad * 65535 / 3.14f)) / 32767.0f; }

inline vec2 sin(const vec2& rad)
{ return { sin(rad.x), sin(rad.y) }; }

inline vec3 sin(const vec3& rad)
{ return { sin(rad.x), sin(rad.y), sin(rad.z) }; }

// cos

#define gl_cos glsl::cos

inline float cos(float rad)
{ return static_cast<float>(cos16(rad * 65535 / 3.14f)) / 32767.0f; }

inline vec2 cos(const vec2& rad)
{ return { cos(rad.x), cos(rad.y) }; }

inline vec3 cos(const vec3& rad)
{ return { cos(rad.x), cos(rad.y), cos(rad.z) }; }

//
// Mathematics
//

// abs

//#define gl_abs glsl::abs

inline float gl_abs(float v)
{ return v >= 0 ? v : -v; }

inline vec2 gl_abs(const vec2& v)
{ return { gl_abs(v.x), gl_abs(v.y) }; }

inline vec3 abs(const vec3& v)
{ return { gl_abs(v.x), gl_abs(v.y), gl_abs(v.z) }; }

// ceil

#define gl_ceil glsl::ceil

inline float ceil(float v)
{ return std::ceil(v); }

inline vec2 ceil(const vec2& v)
{ return { ceil(v.x), ceil(v.y) }; }

inline vec3 ceil(const vec3& v)
{ return { ceil(v.x), ceil(v.y), ceil(v.z) }; }

// min

#define gl_min glsl::min

inline float min(float a, float b)
{ return a > b ? b : a; }

inline vec2 min(const vec2& a, const vec2& b)
{ return { min(a.x, b.x), min(b.x, b.y) }; }

inline vec3 min(const vec3& a, const vec3& b)
{ return { min(a.x, b.x), min(b.x, b.y), min(b.z, b.z) }; }

// max

#define gl_max glsl::max

inline float max(float a, float b)
{ return a > b ? a : b; }

inline vec2 max(const vec2& a, const vec2& b)
{ return { max(a.x, b.x), max(b.x, b.y) }; }

inline vec3 max(const vec3& a, const vec3& b)
{ return { max(a.x, b.x), max(b.x, b.y), max(b.z, b.z) }; }

// clamp

#define gl_clamp glsl::clamp

inline float clamp(float v, float vMin, float vMax)
{ return max(min(v, vMax), vMin); }

inline vec2 clamp(const vec2& v, const vec2& vMin, const vec2& vMax)
{ return { clamp(v.x, vMin.x, vMax.x), clamp(v.y, vMin.y, vMax.y) }; }

inline vec3 clamp(const vec3& v, const vec3& vMin, const vec3& vMax)
{ return { clamp(v.x, vMin.x, vMax.x), clamp(v.y, vMin.y, vMax.y), clamp(v.z, vMin.z, vMax.z) }; }

// dot

#define gl_dot glsl::dot

inline float dot(float a, float b)
{ return a * b; }

inline float dot(const vec2& a, const vec2& b)
{ return a.x * b.x + a.y * b.y; }

inline vec3 dot(const vec3& a, const vec3& b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }

}