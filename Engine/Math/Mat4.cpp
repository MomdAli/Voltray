#include "Mat4.h"
#include <cmath>
#include <iostream>
#include <cstring>
#include "Mat4.h"

Mat4::Mat4()
{
    std::memset(data, 0, sizeof(data));
    data[0] = 1.0f;
    data[5] = 1.0f;
    data[10] = 1.0f;
    data[15] = 1.0f;
}

Mat4 Mat4::Identity()
{
    return Mat4();
}

Mat4 Mat4::operator*(const Mat4 &other) const
{
    Mat4 result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result.data[col + row * 4] =
                data[0 + row * 4] * other.data[col + 0 * 4] +
                data[1 + row * 4] * other.data[col + 1 * 4] +
                data[2 + row * 4] * other.data[col + 2 * 4] +
                data[3 + row * 4] * other.data[col + 3 * 4];
        }
    }
    return result;
}

Vec3 Mat4::MultiplyVec3(const Vec3 &v) const
{
    float x = v.x * data[0] + v.y * data[4] + v.z * data[8] + data[12];
    float y = v.x * data[1] + v.y * data[5] + v.z * data[9] + data[13];
    float z = v.x * data[2] + v.y * data[6] + v.z * data[10] + data[14];
    return Vec3(x, y, z);
}

Mat4 Mat4::Translate(const Vec3 &t)
{
    Mat4 result = Identity();
    result.data[12] = t.x;
    result.data[13] = t.y;
    result.data[14] = t.z;
    return result;
}

Mat4 Mat4::Scale(const Vec3 &s)
{
    Mat4 result = Identity();
    result.data[0] = s.x;
    result.data[5] = s.y;
    result.data[10] = s.z;
    return result;
}

Mat4 Mat4::RotateZ(float angleRad)
{
    Mat4 result = Identity();
    float cosAngle = std::cos(angleRad);
    float sinAngle = std::sin(angleRad);

    result.data[0] = cosAngle;
    result.data[1] = -sinAngle;
    result.data[4] = sinAngle;
    result.data[5] = cosAngle;

    return result;
}

Mat4 Mat4::LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    Vec3 f = (center - eye).Normalize();
    Vec3 s = f.Cross(up).Normalize();
    Vec3 u = s.Cross(f);

    Mat4 result = Identity();
    result.data[0] = s.x;
    result.data[1] = u.x;
    result.data[2] = -f.x;

    result.data[4] = s.y;
    result.data[5] = u.y;
    result.data[6] = -f.y;

    result.data[8] = s.z;
    result.data[9] = u.z;
    result.data[10] = -f.z;

    result.data[12] = -s.Dot(eye);
    result.data[13] = -u.Dot(eye);
    result.data[14] = f.Dot(eye);

    return result;
}

Mat4 Mat4::Perspective(float fovRadians, float aspect, float near, float far)
{
    Mat4 result;
    std::memset(result.data, 0, sizeof(result.data));

    float tanHalfFov = std::tan(fovRadians / 2.0f);

    result.data[0] = 1.0f / (aspect * tanHalfFov);
    result.data[5] = 1.0f / tanHalfFov;
    result.data[10] = -(far + near) / (far - near);
    result.data[11] = -1.0f;
    result.data[14] = -(2.0f * far * near) / (far - near);

    return result;
}

Mat4 Mat4::Inverse() const
{
    // This is a generic 4x4 matrix inverse implementation (not optimized)
    const float *m = data;
    Mat4 inv;
    float *invOut = inv.data;

    invOut[0] = m[5] * m[10] * m[15] -
                m[5] * m[11] * m[14] -
                m[9] * m[6] * m[15] +
                m[9] * m[7] * m[14] +
                m[13] * m[6] * m[11] -
                m[13] * m[7] * m[10];

    invOut[4] = -m[4] * m[10] * m[15] +
                m[4] * m[11] * m[14] +
                m[8] * m[6] * m[15] -
                m[8] * m[7] * m[14] -
                m[12] * m[6] * m[11] +
                m[12] * m[7] * m[10];

    invOut[8] = m[4] * m[9] * m[15] -
                m[4] * m[11] * m[13] -
                m[8] * m[5] * m[15] +
                m[8] * m[7] * m[13] +
                m[12] * m[5] * m[11] -
                m[12] * m[7] * m[9];

    invOut[12] = -m[4] * m[9] * m[14] +
                 m[4] * m[10] * m[13] +
                 m[8] * m[5] * m[14] -
                 m[8] * m[6] * m[13] -
                 m[12] * m[5] * m[10] +
                 m[12] * m[6] * m[9];

    invOut[1] = -m[1] * m[10] * m[15] +
                m[1] * m[11] * m[14] +
                m[9] * m[2] * m[15] -
                m[9] * m[3] * m[14] -
                m[13] * m[2] * m[11] +
                m[13] * m[3] * m[10];

    invOut[5] = m[0] * m[10] * m[15] -
                m[0] * m[11] * m[14] -
                m[8] * m[2] * m[15] +
                m[8] * m[3] * m[14] +
                m[12] * m[2] * m[11] -
                m[12] * m[3] * m[10];

    invOut[9] = -m[0] * m[9] * m[15] +
                m[0] * m[11] * m[13] +
                m[8] * m[1] * m[15] -
                m[8] * m[3] * m[13] -
                m[12] * m[1] * m[11] +
                m[12] * m[3] * m[9];

    invOut[13] = m[0] * m[9] * m[14] -
                 m[0] * m[10] * m[13] -
                 m[8] * m[1] * m[14] +
                 m[8] * m[2] * m[13] +
                 m[12] * m[1] * m[10] -
                 m[12] * m[2] * m[9];

    invOut[2] = m[1] * m[6] * m[15] -
                m[1] * m[7] * m[14] -
                m[5] * m[2] * m[15] +
                m[5] * m[3] * m[14] +
                m[13] * m[2] * m[7] -
                m[13] * m[3] * m[6];

    invOut[6] = -m[0] * m[6] * m[15] +
                m[0] * m[7] * m[14] +
                m[4] * m[2] * m[15] -
                m[4] * m[3] * m[14] -
                m[12] * m[2] * m[7] +
                m[12] * m[3] * m[6];

    invOut[10] = m[0] * m[5] * m[15] -
                 m[0] * m[7] * m[13] -
                 m[4] * m[1] * m[15] +
                 m[4] * m[3] * m[13] +
                 m[12] * m[1] * m[7] -
                 m[12] * m[3] * m[5];

    invOut[14] = -m[0] * m[5] * m[14] +
                 m[0] * m[6] * m[13] +
                 m[4] * m[1] * m[14] -
                 m[4] * m[2] * m[13] -
                 m[12] * m[1] * m[6] +
                 m[12] * m[2] * m[5];

    invOut[3] = -m[1] * m[6] * m[11] +
                m[1] * m[7] * m[10] +
                m[5] * m[2] * m[11] -
                m[5] * m[3] * m[10] -
                m[9] * m[2] * m[7] +
                m[9] * m[3] * m[6];

    invOut[7] = m[0] * m[6] * m[11] -
                m[0] * m[7] * m[10] -
                m[4] * m[2] * m[11] +
                m[4] * m[3] * m[10] +
                m[8] * m[2] * m[7] -
                m[8] * m[3] * m[6];

    invOut[11] = -m[0] * m[5] * m[11] +
                 m[0] * m[7] * m[9] +
                 m[4] * m[1] * m[11] -
                 m[4] * m[3] * m[9] -
                 m[8] * m[1] * m[7] +
                 m[8] * m[3] * m[5];

    invOut[15] = m[0] * m[5] * m[10] -
                 m[0] * m[6] * m[9] -
                 m[4] * m[1] * m[10] +
                 m[4] * m[2] * m[9] +
                 m[8] * m[1] * m[6] -
                 m[8] * m[2] * m[5];

    float det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];
    if (det == 0.0f)
        return Mat4::Identity();
    det = 1.0f / det;
    for (int i = 0; i < 16; i++)
        invOut[i] *= det;
    return inv;
}

void Mat4::Print() const
{
    for (int row = 0; row < 4; ++row)
    {
        std::cout << "| ";
        for (int col = 0; col < 4; ++col)
        {
            std::cout << data[col + row * 4] << " ";
        }
        std::cout << "|\n";
    }
}
