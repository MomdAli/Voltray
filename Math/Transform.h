#pragma once

#include "Vec3.h"
#include "Mat4.h"

/**
 * @class Transform
 * @brief Represents a 3D transformation with position, rotation, and scale.
 *
 * The Transform class encapsulates 3D transformations including translation,
 * rotation (using Euler angles), and uniform/non-uniform scaling. It provides
 * methods to compute transformation matrices and manipulate the transform.
 */
class Transform
{
public:
    /**
     * @brief Default constructor - creates identity transform.
     */
    Transform();

    /**
     * @brief Constructor with position.
     * @param position Initial position.
     */
    Transform(const Vec3 &position);

    /**
     * @brief Constructor with position, rotation, and scale.
     * @param position Initial position.
     * @param rotation Initial rotation in degrees (Euler angles).
     * @param scale Initial scale.
     */
    Transform(const Vec3 &position, const Vec3 &rotation, const Vec3 &scale);

    // Getters
    const Vec3 &GetPosition() const { return m_Position; }
    const Vec3 &GetRotation() const { return m_Rotation; }
    const Vec3 &GetScale() const { return m_Scale; }

    // Setters
    void SetPosition(const Vec3 &position);
    void SetRotation(const Vec3 &rotation);
    void SetScale(const Vec3 &scale);
    void SetScale(float uniformScale);

    // Transformation operations
    void Translate(const Vec3 &translation);
    void Rotate(const Vec3 &rotation);
    void Scale(const Vec3 &scale);
    void Scale(float uniformScale);

    /**
     * @brief Gets the transformation matrix.
     * @return The 4x4 transformation matrix (Translation * Rotation * Scale).
     */
    Mat4 GetMatrix() const;

    /**
     * @brief Gets the inverse transformation matrix.
     * @return The inverse transformation matrix.
     */
    Mat4 GetInverseMatrix() const;

    /**
     * @brief Resets transform to identity.
     */
    void Reset();

private:
    Vec3 m_Position{0.0f, 0.0f, 0.0f};
    Vec3 m_Rotation{0.0f, 0.0f, 0.0f}; // Euler angles in degrees
    Vec3 m_Scale{1.0f, 1.0f, 1.0f};

    mutable Mat4 m_CachedMatrix;
    mutable bool m_MatrixDirty = true;

    void InvalidateMatrix() const { m_MatrixDirty = true; }
    void UpdateMatrix() const;
};
