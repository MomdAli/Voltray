#include "Transform.h"
#include "MathUtil.h"
#include <cmath>

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
    Mat4 invTranslation = Mat4::Translate(Vec3(-m_Position.x, -m_Position.y, -m_Position.z));

    Vec3 absolutePivot = m_MeshCenter + m_Pivot;
    Mat4 translateToPivot = Mat4::Translate(absolutePivot);
    Mat4 translateFromPivot = Mat4::Translate(Vec3(-absolutePivot.x, -absolutePivot.y, -absolutePivot.z));

    Mat4 invRotationX = Mat4::RotateX(-m_Rotation.x * DEG2RAD);
    Mat4 invRotationY = Mat4::RotateY(-m_Rotation.y * DEG2RAD);
    Mat4 invRotationZ = Mat4::RotateZ(-m_Rotation.z * DEG2RAD);
    Mat4 invRotation = invRotationX * invRotationY * invRotationZ;

    Mat4 invScale = Mat4::Scale(Vec3(1.0f / m_Scale.x, 1.0f / m_Scale.y, 1.0f / m_Scale.z));

    return translateToPivot * invScale * invRotation * translateFromPivot * invTranslation;
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

void Transform::UpdateMatrix() const
{
    Mat4 scale = Mat4::Scale(m_Scale);

    Mat4 rotationX = Mat4::RotateX(m_Rotation.x * DEG2RAD);
    Mat4 rotationY = Mat4::RotateY(m_Rotation.y * DEG2RAD);
    Mat4 rotationZ = Mat4::RotateZ(m_Rotation.z * DEG2RAD);
    Mat4 rotation = rotationZ * rotationY * rotationX;

    Vec3 absolutePivot = m_MeshCenter + m_Pivot;
    Mat4 translateFromPivot = Mat4::Translate(Vec3(-absolutePivot.x, -absolutePivot.y, -absolutePivot.z));
    Mat4 translateToPivot = Mat4::Translate(absolutePivot);

    Mat4 translation = Mat4::Translate(m_Position);

    m_CachedMatrix = translation * translateToPivot * rotation * scale * translateFromPivot;
}
