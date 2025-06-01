#include "PerspectiveCamera.h"
#include "../../Math/MathUtil.h"
#include "../../Math/Vec4.h"
#include <cmath>

PerspectiveCamera::PerspectiveCamera(const std::string &name, float fovDeg, float aspect, float nearPlane, float farPlane)
    : BaseCamera(name, aspect, nearPlane, farPlane), m_Fov(fovDeg)
{
}

void PerspectiveCamera::SetFieldOfView(float fovDeg)
{
    m_Fov = MathUtil::Clamp(fovDeg, 1.0f, 179.0f);
}

Mat4 PerspectiveCamera::GetProjectionMatrix() const
{
    return CreatePerspectiveMatrix(m_Fov, m_Aspect, m_Near, m_Far);
}

Ray PerspectiveCamera::ScreenToWorldRay(float screenX, float screenY) const
{
    // Ensure viewport dimensions are valid
    if (m_ViewportWidth <= 0.0f || m_ViewportHeight <= 0.0f)
    {
        // Fallback ray pointing forward
        return Ray(m_Position, GetForward());
    }

    // Convert screen coordinates to normalized device coordinates (-1 to 1)
    // screenX and screenY are expected to be in viewport-relative coordinates (0 to viewport dimensions)
    float ndcX = (2.0f * screenX / m_ViewportWidth) - 1.0f;
    float ndcY = 1.0f - (2.0f * screenY / m_ViewportHeight);

    // Convert to view space
    float tanHalfFov = tan(m_Fov * 0.5f * M_PI / 180.0f);
    float viewX = ndcX * tanHalfFov * m_Aspect;
    float viewY = ndcY * tanHalfFov;

    // Ray direction in view space (camera looks down -Z)
    Vec3 rayDirView = Vec3(viewX, viewY, -1.0f).Normalize();

    // Transform ray direction to world space
    Mat4 viewMatrix = GetViewMatrix();
    Mat4 invViewMatrix = viewMatrix.Inverse();

    Vec3 rayDirWorld = invViewMatrix.MultiplyVec4(Vec4(rayDirView.x, rayDirView.y, rayDirView.z, 0.0f)).xyz().Normalize();

    return Ray(m_Position, rayDirWorld);
}

Mat4 PerspectiveCamera::CreatePerspectiveMatrix(float fov, float aspect, float nearPlane, float farPlane) const
{
    // Convert FOV from degrees to radians
    float fovRadians = fov * DEG2RAD;
    return Mat4::Perspective(fovRadians, aspect, nearPlane, farPlane);
}

Mat4 PerspectiveCamera::ApplyPerspectiveCorrection(const Mat4 &baseProjection) const
{
    // Default implementation returns the base projection unchanged
    // Derived classes can override this to apply specific corrections
    return baseProjection;
}
