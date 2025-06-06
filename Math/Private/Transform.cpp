#include "Transform.h"
#include "MathUtil.h"
#include <cmath>

namespace Voltray::Math
{

    Transform::Transform()
        : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f), m_Pivot(0.0f, 0.0f, 0.0f), m_MeshCenter(0.0f, 0.0f, 0.0f)
    {
    }

    Transform::Transform(const Vec3 &position)
        : m_Position(position), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f), m_Pivot(0.0f, 0.0f, 0.0f), m_MeshCenter(0.0f, 0.0f, 0.0f)
    {
    }

    Transform::Transform(const Vec3 &position, const Vec3 &rotation, const Vec3 &scale)
        : m_Position(position), m_Rotation(rotation), m_Scale(scale), m_Pivot(0.0f, 0.0f, 0.0f), m_MeshCenter(0.0f, 0.0f, 0.0f)
    {
    }

    void Transform::SetPosition(const Vec3 &position)
    {
        m_Position = position;
        InvalidateMatrix();
    }

    void Transform::SetRotation(const Vec3 &rotation)
    {
        m_Rotation = rotation;
        InvalidateMatrix();
    }

    void Transform::SetScale(const Vec3 &scale)
    {
        m_Scale = scale;
        InvalidateMatrix();
    }

    void Transform::SetScale(float uniformScale)
    {
        m_Scale = Vec3(uniformScale, uniformScale, uniformScale);
        InvalidateMatrix();
    }

    void Transform::SetPivot(const Vec3 &pivot)
    {
        m_Pivot = pivot;
        InvalidateMatrix();
    }

    void Transform::SetMeshCenter(const Vec3 &meshCenter)
    {
        m_MeshCenter = meshCenter;
        InvalidateMatrix();
    }

    void Transform::SetRelativePivot(const Vec3 &meshCenter, const Vec3 &relativePivot)
    {
        m_MeshCenter = meshCenter;
        m_Pivot = relativePivot;
        InvalidateMatrix();
    }

    void Transform::Translate(const Vec3 &translation)
    {
        m_Position = m_Position + translation;
        InvalidateMatrix();
    }

    void Transform::Rotate(const Vec3 &rotation)
    {
        m_Rotation = m_Rotation + rotation;
        InvalidateMatrix();
    }

    void Transform::Scale(const Vec3 &scale)
    {
        m_Scale.x *= scale.x;
        m_Scale.y *= scale.y;
        m_Scale.z *= scale.z;
        InvalidateMatrix();
    }

    void Transform::Scale(float uniformScale)
    {
        m_Scale.x *= uniformScale;
        m_Scale.y *= uniformScale;
        m_Scale.z *= uniformScale;
        InvalidateMatrix();
    }

    // Local space transformation methods
    void Transform::TranslateLocal(const Vec3 &localTranslation)
    {
        // Create rotation matrix to transform from local to world space
        Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
        Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
        Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
        Mat4 rotation = rotationZ * rotationY * rotationX;

        // Transform the local translation vector to world space
        Vec3 worldTranslation = rotation.MultiplyVec3(localTranslation);

        // Apply the world space translation
        Translate(worldTranslation);
    }

    void Transform::TranslateLocalX(float distance)
    {
        TranslateLocal(Vec3(distance, 0.0f, 0.0f));
    }

    void Transform::TranslateLocalY(float distance)
    {
        TranslateLocal(Vec3(0.0f, distance, 0.0f));
    }

    void Transform::TranslateLocalZ(float distance)
    {
        TranslateLocal(Vec3(0.0f, 0.0f, distance));
    }

    Vec3 Transform::GetLocalRight() const
    {
        // Local X axis is (1, 0, 0) in local space
        Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
        Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
        Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
        Mat4 rotation = rotationZ * rotationY * rotationX;

        return rotation.MultiplyVec3(Vec3(1.0f, 0.0f, 0.0f)).Normalize();
    }

    Vec3 Transform::GetLocalUp() const
    {
        // Local Y axis is (0, 1, 0) in local space
        Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
        Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
        Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
        Mat4 rotation = rotationZ * rotationY * rotationX;

        return rotation.MultiplyVec3(Vec3(0.0f, 1.0f, 0.0f)).Normalize();
    }

    Vec3 Transform::GetLocalForward() const
    {
        // Local Z axis is (0, 0, 1) in local space
        Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
        Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
        Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
        Mat4 rotation = rotationZ * rotationY * rotationX;

        return rotation.MultiplyVec3(Vec3(0.0f, 0.0f, 1.0f)).Normalize();
    }

    Mat4 Transform::GetMatrix() const
    {
        if (m_MatrixDirty)
        {
            UpdateMatrix();
            m_MatrixDirty = false;
        }
        return m_CachedMatrix;
    }

    Mat4 Transform::GetInverseMatrix() const
    {
        // Create inverse transformation matrices (reverse order of UpdateMatrix)
        Mat4 invTranslation = Mat4::Translate(-m_Position);
        Mat4 invTranslateFromPivot = Mat4::Translate(-m_Pivot);

        Mat4 invRotationX = Mat4::RotateX(-m_Rotation.x * DEG2RAD);
        Mat4 invRotationY = Mat4::RotateY(-m_Rotation.y * DEG2RAD);
        Mat4 invRotationZ = Mat4::RotateZ(-m_Rotation.z * DEG2RAD);
        Mat4 invRotation = invRotationX * invRotationY * invRotationZ;

        Mat4 invScale = Mat4::Scale(Vec3(1.0f / m_Scale.x, 1.0f / m_Scale.y, 1.0f / m_Scale.z));
        Mat4 invTranslateToPivot = Mat4::Translate(m_Pivot);

        // Inverse order: reverse of translateToPivot * scale * rotation * translateFromPivot * translation
        return invTranslation * invTranslateFromPivot * invRotation * invScale * invTranslateToPivot;
    }

    void Transform::UpdateMatrix() const
    {
        // Create transformation matrices
        Mat4 translateToPivot = Mat4::Translate(-m_Pivot);
        Mat4 scale = Mat4::Scale(m_Scale);

        Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
        Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
        Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
        Mat4 rotation = rotationZ * rotationY * rotationX;

        Mat4 translateFromPivot = Mat4::Translate(m_Pivot);
        Mat4 translation = Mat4::Translate(m_Position);

        m_CachedMatrix = translateToPivot * scale * rotation * translateFromPivot * translation;
    }

    void Transform::Reset()
    {
        m_Position = Vec3(0.0f, 0.0f, 0.0f);
        m_Rotation = Vec3(0.0f, 0.0f, 0.0f);
        m_Scale = Vec3(1.0f, 1.0f, 1.0f);
        m_Pivot = Vec3(0.0f, 0.0f, 0.0f);
        m_MeshCenter = Vec3(0.0f, 0.0f, 0.0f);
        InvalidateMatrix();
    }

}