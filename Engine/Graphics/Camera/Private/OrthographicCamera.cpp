#include "OrthographicCamera.h"
#include "MathUtil.h"
#include "Vec4.h"
#include <cmath>

using namespace Voltray::Math;

namespace Voltray::Engine
{

    OrthographicCamera::OrthographicCamera(const std::string &name, float aspect, float nearPlane, float farPlane, float size)
        : BaseCamera(name.empty() ? "Orthographic Camera" : name, aspect, nearPlane, farPlane), m_Size(size)
    {
    }

    Mat4 OrthographicCamera::GetProjectionMatrix() const
    {
        float halfWidth = m_Size * m_Aspect;
        float halfHeight = m_Size;

        // Create orthographic projection matrix using the static method
        // left = -halfWidth, right = halfWidth, bottom = -halfHeight, top = halfHeight
        return Mat4::Orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, m_Near, m_Far);
    }

    Ray OrthographicCamera::ScreenToWorldRay(float screenX, float screenY) const
    {
        // Ensure viewport dimensions are valid
        if (m_ViewportWidth <= 0.0f || m_ViewportHeight <= 0.0f)
        {
            // Fallback ray pointing forward from camera position
            return Ray(m_Position, GetForward());
        }

        // Convert screen coordinates to normalized device coordinates (-1 to 1)
        // screenX and screenY are expected to be in viewport-relative coordinates (0 to viewport dimensions)
        float ndcX = (2.0f * screenX / m_ViewportWidth) - 1.0f;
        float ndcY = 1.0f - (2.0f * screenY / m_ViewportHeight);

        // For orthographic projection, the ray direction is always the same (camera forward)
        Vec3 direction = GetForward();

        // Calculate the world position on the near plane
        float halfWidth = m_Size * m_Aspect;
        float halfHeight = m_Size;

        Vec3 right = GetRight();
        Vec3 up = GetUp();

        // Position on the near plane
        Vec3 nearPlaneCenter = m_Position + direction * m_Near;
        Vec3 worldPosition = nearPlaneCenter + right * (ndcX * halfWidth) + up * (ndcY * halfHeight);

        return Ray(worldPosition, direction);
    }

    void OrthographicCamera::SetOrthographicSize(float size)
    {
        m_Size = std::max(size, 0.1f); // Prevent zero or negative size
    }

}
