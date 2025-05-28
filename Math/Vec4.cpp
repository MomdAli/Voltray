#include "Vec4.h"
#include <cmath>
#include <stdexcept>

// Constructors
Vec4::Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

Vec4::Vec4(const Vec4 &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

// Assignment operator
Vec4 &Vec4::operator=(const Vec4 &other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    return *this;
}

// Arithmetic operators
Vec4 Vec4::operator+(const Vec4 &other) const
{
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vec4 Vec4::operator-(const Vec4 &other) const
{
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vec4 Vec4::operator*(float scalar) const
{
    return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 Vec4::operator/(float scalar) const
{
    if (scalar == 0.0f)
    {
        throw std::runtime_error("Division by zero in Vec4");
    }
    return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

// Compound assignment operators
Vec4 &Vec4::operator+=(const Vec4 &other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vec4 &Vec4::operator-=(const Vec4 &other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vec4 &Vec4::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Vec4 &Vec4::operator/=(float scalar)
{
    if (scalar == 0.0f)
    {
        throw std::runtime_error("Division by zero in Vec4");
    }
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

// Comparison operators
bool Vec4::operator==(const Vec4 &other) const
{
    const float epsilon = 1e-6f;
    return std::abs(x - other.x) < epsilon &&
           std::abs(y - other.y) < epsilon &&
           std::abs(z - other.z) < epsilon &&
           std::abs(w - other.w) < epsilon;
}

bool Vec4::operator!=(const Vec4 &other) const
{
    return !(*this == other);
}

// Utility functions
float Vec4::length() const
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::lengthSquared() const
{
    return x * x + y * y + z * z + w * w;
}

Vec4 Vec4::normalized() const
{
    float len = length();
    if (len == 0.0f)
    {
        return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    return *this / len;
}

void Vec4::normalize()
{
    float len = length();
    if (len != 0.0f)
    {
        *this /= len;
    }
}

float Vec4::dot(const Vec4 &other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

// Array access
float &Vec4::operator[](int index)
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        throw std::out_of_range("Vec4 index out of range");
    }
}

const float &Vec4::operator[](int index) const
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        throw std::out_of_range("Vec4 index out of range");
    }
}
