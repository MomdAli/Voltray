#pragma once

#include "vec3.h"
#include "vec4.h"

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

    /**
     * @brief Creates a translation matrix.
     * @param translation The translation vector.
     * @return Translation matrix.
     */
    static Mat4 Translate(const Vec3 &translation);

    /**
     * @brief Creates a scaling matrix.
     * @param scale The scaling vector.
     * @return Scaling matrix.
     */
    static Mat4 Scale(const Vec3 &scale); /**
                                           * @brief Creates a rotation matrix around the X axis.
                                           * @param angleRad Rotation angle in radians.
                                           * @return Rotation matrix.
                                           */
    static Mat4 RotateX(float angleRad);

    /**
     * @brief Creates a rotation matrix around the Y axis.
     * @param angleRad Rotation angle in radians.
     * @return Rotation matrix.
     */
    static Mat4 RotateY(float angleRad);

    /**
     * @brief Creates a rotation matrix around the Z axis.
     * @param angleRad Rotation angle in radians.
     * @return Rotation matrix.
     */
    static Mat4 RotateZ(float angleRad);

    /**
     * @brief Creates a view matrix using the LookAt method.
     * @param eye Camera position.
     * @param center Target position.
     * @param up Up direction vector.
     * @return View matrix.
     */
    static Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

    /**
     * @brief Creates a perspective projection matrix.
     * @param fovRadians Field of view in radians.
     * @param aspect Aspect ratio (width/height).
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     * @return Perspective projection matrix.
     */
    static Mat4 Perspective(float fovRadians, float aspect, float near, float far);

    /**
     * @brief Creates an orthographic projection matrix.
     * @param left Left clipping plane.
     * @param right Right clipping plane.
     * @param bottom Bottom clipping plane.
     * @param top Top clipping plane.
     * @param near Near clipping plane.
     * @param far Far clipping plane.
     * @return Orthographic projection matrix.
     */
    static Mat4 Orthographic(float left, float right, float bottom, float top, float near, float far);

    /**
     * @brief Returns the inverse of this matrix.
     * @return Inverse matrix.
     */
    Mat4 Inverse() const;

    /**
     * @brief Prints the matrix to the standard output.
     */
    void Print() const;
};
