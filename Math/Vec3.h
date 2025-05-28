#pragma once

// Represents a 3-dimensional vector with float components and provides common vector operations.
struct Vec3
{
    float x, y, z; ///< The x, y, and z components of the vector.

    /// Default constructor. Initializes all components to zero.
    Vec3();

    /// Constructs a vector with the same value for all components.
    /// @param value The value to set for all components.
    explicit Vec3(float value);

    /// Constructs a vector with the given x, y, and z components.
    /// @param x The x component.
    /// @param y The y component.
    /// @param z The z component.
    Vec3(float x, float y, float z);

    /// Adds this vector to another vector.
    /// @param other The vector to add.
    /// @return The result of vector addition.
    Vec3 operator+(const Vec3 &other) const;

    /// Subtracts another vector from this vector.
    /// @param other The vector to subtract.
    /// @return The result of vector subtraction.
    Vec3 operator-(const Vec3 &other) const;

    /// Multiplies this vector by a scalar.
    /// @param scalar The scalar value to multiply by.
    /// @return The result of scalar multiplication.
    Vec3 operator*(float scalar) const;

    /// Divides this vector by a scalar.
    /// @param scalar The scalar value to divide by.
    /// @return The result of scalar division.
    Vec3 operator/(float scalar) const;

    /// Calculates the length (magnitude) of the vector.
    /// @return The length of the vector.
    float Length() const;

    /// Returns a normalized (unit length) version of this vector.
    /// @return The normalized vector.
    Vec3 Normalize() const;

    /// Computes the dot product of this vector and another vector.
    /// @param other The other vector.
    /// @return The dot product.
    float Dot(const Vec3 &other) const;

    /// Computes the cross product of this vector and another vector.
    /// @param other The other vector.
    /// @return The cross product vector.
    Vec3 Cross(const Vec3 &other) const;

    /// Prints the vector components to the standard output.
    void Print() const;

    /// Compound addition assignment.
    Vec3 &operator+=(const Vec3 &other);
    /// Compound subtraction assignment.
    Vec3 &operator-=(const Vec3 &other);
    /// Compound multiplication assignment.
    Vec3 &operator*=(float scalar);
};
