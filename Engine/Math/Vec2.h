#pragma once

/**
 * @struct Vec2
 * @brief Represents a 2-dimensional vector with float components
 */
struct Vec2
{
    float x, y; ///< The x and y components of the vector.

    /// Default constructor. Initializes all components to zero.
    Vec2() : x(0.0f), y(0.0f) {}

    /// Constructs a vector with the given x and y components.
    /// @param x The x component.
    /// @param y The y component.
    Vec2(float x, float y) : x(x), y(y) {}

    /// Adds this vector to another vector.
    /// @param other The vector to add.
    /// @return The result of vector addition.
    Vec2 operator+(const Vec2 &other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    /// Subtracts another vector from this vector.
    /// @param other The vector to subtract.
    /// @return The result of vector subtraction.
    Vec2 operator-(const Vec2 &other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    /// Multiplies this vector by a scalar.
    /// @param scalar The scalar value.
    /// @return The result of scalar multiplication.
    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }
};
