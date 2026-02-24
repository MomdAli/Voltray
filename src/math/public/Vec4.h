#pragma once

#include "Vec3.h"
#include "Vec2.h"

namespace Voltray::Math
{

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

        // Utility functions
        float length() const;
        float lengthSquared() const;
        Vec4 normalized() const;
        void normalize();
        float dot(const Vec4 &other) const;

        // Array access
        float &operator[](int index);
        const float &operator[](int index) const;

        // multiple swizzle accessors
        const float &r() const { return x; }
        const float &g() const { return y; }
        const float &b() const { return z; }
        const float &a() const { return w; }

        Vec4 wzyx() const
        {
            return Vec4(w, z, y, x);
        }

        Vec3 xyz() const
        {
            return Vec3(x, y, z);
        }

        Vec2 xy() const
        {
            return Vec2(x, y);
        }

        // Static utility functions
        static Vec4 Zero() { return Vec4(0.0f, 0.0f, 0.0f, 0.0f); }
        static Vec4 One() { return Vec4(1.0f, 1.0f, 1.0f, 1.0f); }
        static Vec4 FromColor(float r, float g, float b, float a)
        {
            return Vec4(r, g, b, a);
        }
    };

}
