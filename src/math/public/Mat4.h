#pragma once

#include "Vec3.h"
#include "Vec4.h"

namespace Voltray::Math {

/**
 * @brief 4x4 Matrix structure for 3D transformations and projections.
 *
 * Provides basic matrix operations and utilities for 3D graphics, including
 * identity, multiplication, translation, scaling, rotation (Z axis), view (LookAt),
 * and perspective projection matrices.
 *
 * Data is stored in a 16-element float array in column-major order.
 */
struct Mat4
{
    /**
     * @brief Matrix data in column-major order.
     */
    float data[16];

    /**
     * @brief Constructs an identity matrix.
     */
    Mat4();

    /**
     * @brief Returns an identity matrix.
     * @return Mat4 Identity matrix.
     */
    static Mat4 Identity();

    /**
     * @brief Multiplies this matrix by another matrix.
     * @param other The matrix to multiply with.
     * @return Resulting matrix after multiplication.
     */
    Mat4 operator*(const Mat4 &other) const;

    /**
     * @brief Transforms a Vec3 by this matrix (assumes w=1).
     * @param v The vector to transform.
     * @return Transformed Vec3.
     */
    Vec3 MultiplyVec3(const Vec3 &v) const;

    /**
     * @brief Transforms a Vec4 by this matrix.
     * @param v The vector to transform.
     * @return Transformed Vec4.
     */
    Vec4 MultiplyVec4(const Vec4 &v) const;
    static Mat4 Translate(const Vec3 &translation);
    static Mat4 Scale(const Vec3 &scale);    static Mat4 RotateX(float angleRad);
    static Mat4 RotateY(float angleRad);
    static Mat4 RotateZ(float angleRad);
    static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
    static Mat4 Perspective(float fovRadians, float aspect, float near, float far);
    static Mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);
    Mat4 Inverse() const;
};

}
