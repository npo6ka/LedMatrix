#pragma once

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

    vec3(float xyz) : vec2(xyz), z(xyz)
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

    operator CRGB() const
    { return CRGB(x * 255, y * 255, z * 255); }
};

/** min */

inline float min(float a, float b)
{ return a > b ? b : a; }

inline vec2 min(vec2 a, vec2 b)
{ return { min(a.x, b.x), min(b.x, b.y) }; }

inline vec3 min(vec3 a, vec3 b)
{ return { min(a.x, b.x), min(b.x, b.y), min(b.z, b.z) }; }

/** max */

inline float max(float a, float b)
{ return a > b ? a : b; }

inline vec2 max(vec2 a, vec2 b)
{ return { max(a.x, b.x), max(b.x, b.y) }; }

inline vec3 max(vec3 a, vec3 b)
{ return { max(a.x, b.x), max(b.x, b.y), max(b.z, b.z) }; }

/** clamp */

inline float clamp(float v, float vMin, float vMax)
{ return max(min(v, vMax), vMin); }

inline vec2 clamp(vec2 v, vec2 vMin, vec2 vMax)
{ return { clamp(v.x, vMin.x, vMax.x), clamp(v.y, vMin.y, vMax.y) }; }

inline vec3 clamp(vec3 v, vec3 vMin, vec3 vMax)
{ return { clamp(v.x, vMin.x, vMax.x), clamp(v.y, vMin.y, vMax.y), clamp(v.z, vMin.z, vMax.z) }; }

/** abs */

inline float fabs(float v)
{ return v >= 0.f ? v : -v; }

inline vec2 fabs(vec2 v)
{ return { fabs(v.x), fabs(v.y) }; }

inline vec3 fabs(vec3 v)
{ return { fabs(v.x), fabs(v.y), fabs(v.z) }; }

/** dot */

inline float dot(float a, float b)
{ return a * b; }

inline float dot(vec2 a, vec2 b)
{ return a.x * b.x + a.y * b.y; }

inline vec3 dot(vec3 a, vec3 b)
{ return a.x * b.x + a.y * b.y + a.z * b.z; }

/** sin */

inline float fsin(float rad)
{ return sin16(rad * 65535 / 3.14) / 32767.0; }

inline vec2 fsin(vec2 rad)
{ return { fsin(rad.x), fsin(rad.y) }; }

inline vec3 fsin(vec3 rad)
{ return { fsin(rad.x), fsin(rad.y), fsin(rad.z) }; }

/** cos */

inline float fcos(float rad)
{ return cos16(rad * 65535 / 3.14f) / 32767.0f; }

inline vec2 fcos(vec2 rad)
{ return { fcos(rad.x), fcos(rad.y) }; }

inline vec3 fcos(vec3 rad)
{ return { fcos(rad.x), fcos(rad.y), fcos(rad.z) }; }
