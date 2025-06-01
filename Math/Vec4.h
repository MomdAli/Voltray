#pragma once

#include "Vec2.h"
#include "Vec3.h"

struct Vec4
{
public:
    float x, y, z, w;

    // Constructors
    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec4 &other);

    // Assignment operator
    Vec4 &operator=(const Vec4 &other);

    // Arithmetic operators
    Vec4 operator+(const Vec4 &other) const;
    Vec4 operator-(const Vec4 &other) const;
    Vec4 operator*(float scalar) const;
    Vec4 operator/(float scalar) const;

    // Compound assignment operators
    Vec4 &operator+=(const Vec4 &other);
    Vec4 &operator-=(const Vec4 &other);
    Vec4 &operator*=(float scalar);
    Vec4 &operator/=(float scalar);

    // Comparison operators
    bool operator==(const Vec4 &other) const;
    bool operator!=(const Vec4 &other) const;

    // Scaler
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }
    Vec4 operator*(const Vec4 &other) const { return Vec4(x * other.x, y * other.y, z * other.z, w * other.w); }

    // Utility functions
    float length() const;
    float lengthSquared() const;
    Vec4 normalized() const;
    void normalize();
    float dot(const Vec4 &other) const;

    // Vector component accessors
    Vec2 xx() const { return Vec2(x, x); }
    Vec2 xy() const { return Vec2(x, y); }
    Vec2 xz() const { return Vec2(x, z); }
    Vec2 xw() const { return Vec2(x, w); }
    Vec2 yx() const { return Vec2(y, x); }
    Vec2 yy() const { return Vec2(y, y); }
    Vec2 yz() const { return Vec2(y, z); }
    Vec2 yw() const { return Vec2(y, w); }
    Vec2 zx() const { return Vec2(z, x); }
    Vec2 zy() const { return Vec2(z, y); }
    Vec2 zz() const { return Vec2(z, z); }
    Vec2 zw() const { return Vec2(z, w); }
    Vec2 wx() const { return Vec2(w, x); }
    Vec2 wy() const { return Vec2(w, y); }
    Vec2 wz() const { return Vec2(w, z); }
    Vec2 ww() const { return Vec2(w, w); }

    Vec3 xxx() const { return Vec3(x, x, x); }
    Vec3 xxy() const { return Vec3(x, x, y); }
    Vec3 xxz() const { return Vec3(x, x, z); }
    Vec3 xxw() const { return Vec3(x, x, w); }
    Vec3 xyx() const { return Vec3(x, y, x); }
    Vec3 xyy() const { return Vec3(x, y, y); }
    Vec3 xyz() const { return Vec3(x, y, z); }
    Vec3 xyw() const { return Vec3(x, y, w); }
    Vec3 xzx() const { return Vec3(x, z, x); }
    Vec3 xzy() const { return Vec3(x, z, y); }
    Vec3 xzz() const { return Vec3(x, z, z); }
    Vec3 xzw() const { return Vec3(x, z, w); }
    Vec3 xwx() const { return Vec3(x, w, x); }
    Vec3 xwy() const { return Vec3(x, w, y); }
    Vec3 xwz() const { return Vec3(x, w, z); }
    Vec3 xww() const { return Vec3(x, w, w); }
    Vec3 yxx() const { return Vec3(y, x, x); }
    Vec3 yxy() const { return Vec3(y, x, y); }
    Vec3 yxz() const { return Vec3(y, x, z); }
    Vec3 yxw() const { return Vec3(y, x, w); }
    Vec3 yyx() const { return Vec3(y, y, x); }
    Vec3 yyy() const { return Vec3(y, y, y); }
    Vec3 yyz() const { return Vec3(y, y, z); }
    Vec3 yyw() const { return Vec3(y, y, w); }
    Vec3 yzx() const { return Vec3(y, z, x); }
    Vec3 yzy() const { return Vec3(y, z, y); }
    Vec3 yzz() const { return Vec3(y, z, z); }
    Vec3 yzw() const { return Vec3(y, z, w); }
    Vec3 ywx() const { return Vec3(y, w, x); }
    Vec3 ywy() const { return Vec3(y, w, y); }
    Vec3 ywz() const { return Vec3(y, w, z); }
    Vec3 yww() const { return Vec3(y, w, w); }
    Vec3 zxx() const { return Vec3(z, x, x); }
    Vec3 zxy() const { return Vec3(z, x, y); }
    Vec3 zxz() const { return Vec3(z, x, z); }
    Vec3 zxw() const { return Vec3(z, x, w); }
    Vec3 zyx() const { return Vec3(z, y, x); }
    Vec3 zyy() const { return Vec3(z, y, y); }
    Vec3 zyz() const { return Vec3(z, y, z); }
    Vec3 zyw() const { return Vec3(z, y, w); }
    Vec3 zzx() const { return Vec3(z, z, x); }
    Vec3 zzy() const { return Vec3(z, z, y); }
    Vec3 zzz() const { return Vec3(z, z, z); }
    Vec3 zzw() const { return Vec3(z, z, w); }
    Vec3 zwx() const { return Vec3(z, w, x); }
    Vec3 zwy() const { return Vec3(z, w, y); }
    Vec3 zwz() const { return Vec3(z, w, z); }
    Vec3 zww() const { return Vec3(z, w, w); }
    Vec3 wxx() const { return Vec3(w, x, x); }
    Vec3 wxy() const { return Vec3(w, x, y); }
    Vec3 wxz() const { return Vec3(w, x, z); }
    Vec3 wxw() const { return Vec3(w, x, w); }
    Vec3 wyx() const { return Vec3(w, y, x); }
    Vec3 wyy() const { return Vec3(w, y, y); }
    Vec3 wyz() const { return Vec3(w, y, z); }
    Vec3 wyw() const { return Vec3(w, y, w); }
    Vec3 wzx() const { return Vec3(w, z, x); }
    Vec3 wzy() const { return Vec3(w, z, y); }
    Vec3 wzz() const { return Vec3(w, z, z); }
    Vec3 wzw() const { return Vec3(w, z, w); }
    Vec3 wwx() const { return Vec3(w, w, x); }
    Vec3 wwy() const { return Vec3(w, w, y); }
    Vec3 wwz() const { return Vec3(w, w, z); }
    Vec3 www() const { return Vec3(w, w, w); }

    // Array access
    float &operator[](int index);
    const float &operator[](int index) const;
};
