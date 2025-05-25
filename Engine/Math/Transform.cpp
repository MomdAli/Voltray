#include "Transform.h"
#include "MathUtil.h"
#include <cmath>

#define DEG2RAD 0.01745329251994329576923690768489f // pi / 180.0f

Transform::Transform()
    : m_Position(0.0f, 0.0f, 0.0f), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(const Vec3 &position)
    : m_Position(position), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(const Vec3 &position, const Vec3 &rotation, const Vec3 &scale)
    : m_Position(position), m_Rotation(rotation), m_Scale(scale)
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
    // For a TRS matrix, the inverse is: S^-1 * R^-1 * T^-1
    Mat4 invScale = Mat4::Scale(Vec3(1.0f / m_Scale.x, 1.0f / m_Scale.y, 1.0f / m_Scale.z));
    Mat4 invRotation = Mat4::RotateZ(-m_Rotation.z * DEG2RAD); // Simple Z rotation for now
    Mat4 invTranslation = Mat4::Translate(Vec3(-m_Position.x, -m_Position.y, -m_Position.z));

    return invScale * invRotation * invTranslation;
}

void Transform::Reset()
{
    m_Position = Vec3(0.0f, 0.0f, 0.0f);
    m_Rotation = Vec3(0.0f, 0.0f, 0.0f);
    m_Scale = Vec3(1.0f, 1.0f, 1.0f);
    InvalidateMatrix();
}

void Transform::UpdateMatrix() const
{
    // Create TRS matrix: Translation * Rotation * Scale
    Mat4 translation = Mat4::Translate(m_Position);
    Mat4 rotation = Mat4::RotateZ(m_Rotation.z * DEG2RAD); // For now, just Z rotation
    Mat4 scale = Mat4::Scale(m_Scale);

    m_CachedMatrix = translation * rotation * scale;
}
