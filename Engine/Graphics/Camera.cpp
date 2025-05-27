#include <glad/gl.h>
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Config/EngineSettings.h"
#include "Graphics/Camera.h"
#include "Input/Input.h"
#include "../Math/MathUtil.h"
#include "../Math/Ray.h"

#define DEG2RAD 0.01745329251994329576923690768489f // pi / 180.0f
#define RAD2DEG 57.295779513082320876798154814105f  // 180.0f / pi

Camera::Camera(float fovDeg, float aspect, float nearPlane, float farPlane)
    : m_Fov(fovDeg), m_Aspect(aspect), m_Near(nearPlane), m_Far(farPlane), m_CursorHidden(false)
{
    m_Target = Vec3(0.0f, 0.0f, 0.0f);
    m_Distance = 3.0f;
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;
    UpdatePositionFromAngles();
}

void Camera::SetPosition(const Vec3 &position)
{
    m_Position = position;
    m_Distance = (m_Position - m_Target).Length();
    UpdateAnglesFromPosition();
}

void Camera::SetTarget(const Vec3 &target)
{
    m_Target = target;
    m_Distance = (m_Position - m_Target).Length();
    UpdateAnglesFromPosition();
}

void Camera::SetAspectRatio(float aspect)
{
    m_Aspect = aspect;
}

void Camera::Update()
{
    ProcessMouseMiddleClick();
    ProcessScrollInput();
    UpdatePositionFromAngles();
}

void Camera::ProcessMouseMiddleClick()
{
    bool mmb = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
    static bool wasMmb = false;

    if (mmb && !wasMmb && IsMouseInViewport())
    {
        // Hide cursor and enable unlimited movement
        glfwSetInputMode(Input::s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_CursorHidden = true;
        Input::ResetMouseDelta(); // Optionally reset delta to avoid jump
    }
    else if (!mmb && wasMmb && m_CursorHidden)
    {
        // Show cursor again
        glfwSetInputMode(Input::s_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_CursorHidden = false;
        Input::ResetMouseDelta();
    }

    // Update the previous state
    wasMmb = mmb;

    // Only process camera movement if middle mouse is pressed AND mouse started in viewport
    if (!mmb)
        return;

    // Check if mouse is in viewport OR if we're already in camera control mode (cursor hidden)
    if (!m_CursorHidden && !IsMouseInViewport())
        return;
    float dx = static_cast<float>(Input::GetMouseDeltaX());
    float dy = static_cast<float>(Input::GetMouseDeltaY());
    // Clamp mouse delta to prevent excessive camera movement on fast mouse motion
    const float maxDelta = EngineSettings::MouseClampDelta;
    dx = MathUtil::Clamp(dx, -maxDelta, maxDelta);
    dy = MathUtil::Clamp(dy, -maxDelta, maxDelta);

    if (dx == 0 && dy == 0)
        return;

    bool shift = Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT);
    bool ctrl = Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Input::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL);

    if (shift)
    {
        // Pan: move target and position parallel to view plane
        Vec3 forward = (m_Target - m_Position).Normalize();
        Vec3 right = forward.Cross(m_Up).Normalize();
        Vec3 up = m_Up;
        float panSpeed = EngineSettings::CameraPanSpeed * m_Distance;
        m_Target -= right * dx * panSpeed;
        m_Target -= up * dy * panSpeed;
    }
    else if (ctrl)
    {
        // Dolly (zoom): move camera closer/farther from target
        // Make dolly less sensitive than scroll for a more natural feel
        float dollyFactor = EngineSettings::CameraZoomSpeed * 0.05f;
        Zoom(dy * dollyFactor);
    }
    else
    {
        // Orbit: rotate around target
        float orbitSpeed = EngineSettings::CameraOrbitSpeed;
        m_Yaw += dx * orbitSpeed;
        m_Pitch -= dy * orbitSpeed;
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }
}

void Camera::ProcessScrollInput()
{
    // Assume Input::GetScrollDeltaY() returns the vertical scroll offset since last frame
    float scrollY = static_cast<float>(Input::GetScrollDeltaY());
    if (scrollY != 0.0f && (IsMouseInViewport() || m_CursorHidden))
        Zoom(scrollY);
}

void Camera::Zoom(float delta)
{
    // Smoothly clamp the distance using exponential smoothing
    float minDist = EngineSettings::CameraMinDistance;
    float maxDist = EngineSettings::CameraMaxDistance;
    float zoomSpeed = 1.0f + 0.1f * std::abs(delta); // More delta, more speed
    float targetDist = m_Distance - delta * zoomSpeed;
    // Clamp target distance
    if (targetDist < minDist)
        targetDist = minDist;
    if (targetDist > maxDist)
        targetDist = maxDist;
    // Smoothly interpolate (lerp) to target distance
    float smooth = 0.18f; // Lower = slower, higher = snappier
    m_Distance = m_Distance + (targetDist - m_Distance) * smooth;
}

void Camera::UpdatePositionFromAngles()
{
    float yawRad = m_Yaw * DEG2RAD;
    float pitchRad = m_Pitch * DEG2RAD;
    Vec3 offset;
    offset.x = m_Distance * cosf(yawRad) * cosf(pitchRad);
    offset.y = m_Distance * sinf(pitchRad);
    offset.z = m_Distance * sinf(yawRad) * cosf(pitchRad);
    m_Position = m_Target + offset;
}

void Camera::UpdateAnglesFromPosition()
{
    Vec3 offset = m_Position - m_Target;
    m_Distance = offset.Length();
    if (m_Distance < 1e-6f)
        return;
    m_Pitch = RAD2DEG * asinf(offset.y / m_Distance);
    m_Yaw = RAD2DEG * atan2f(offset.z, offset.x);
}

Mat4 Camera::GetViewMatrix() const
{
    return Mat4::LookAt(m_Position, m_Target, m_Up);
}

Mat4 Camera::GetProjectionMatrix() const
{
    float fovRadians = m_Fov * (3.1415926f / 180.0f);
    return Mat4::Perspective(fovRadians, m_Aspect, m_Near, m_Far);
}

Mat4 Camera::GetViewProjectionMatrix() const
{
    return GetViewMatrix() * GetProjectionMatrix();
}

void Camera::SetViewportBounds(float x, float y, float width, float height)
{
    m_ViewportX = x;
    m_ViewportY = y;
    m_ViewportWidth = width;
    m_ViewportHeight = height;
}

bool Camera::IsMouseInViewport() const
{
    if (!Input::s_Window)
        return false;

    double mouseX, mouseY;
    glfwGetCursorPos(Input::s_Window, &mouseX, &mouseY);

    // Check if mouse is within viewport bounds
    return (mouseX >= m_ViewportX &&
            mouseX <= m_ViewportX + m_ViewportWidth &&
            mouseY >= m_ViewportY &&
            mouseY <= m_ViewportY + m_ViewportHeight);
}

Ray Camera::ScreenToWorldRay(float screenX, float screenY) const
{
    // Convert screen coordinates to normalized device coordinates (NDC)
    // Screen coordinates are relative to the viewport
    float x = (2.0f * (screenX - m_ViewportX)) / m_ViewportWidth - 1.0f;
    float y = 1.0f - (2.0f * (screenY - m_ViewportY)) / m_ViewportHeight; // Flip Y axis

    // Create ray direction in normalized device coordinates
    // For perspective projection, we need to handle the perspective correctly
    Mat4 invProjection = GetProjectionMatrix().Inverse();
    Mat4 invView = GetViewMatrix().Inverse();

    // Create points in clip space (w=1 for proper perspective division)
    Vec3 rayNDC = Vec3(x, y, 1.0f); // Point on far plane in NDC

    // Transform to view space (eye space)
    // For NDC to view space, we need to multiply by inverse projection
    // The perspective projection formula is: NDC = view * projection
    // So: view = NDC * inverse(projection)

    // Convert NDC point to view space
    float tanHalfFOV = tan((m_Fov * 3.14159f / 180.0f) * 0.5f);
    float viewX = x * tanHalfFOV * m_Aspect;
    float viewY = y * tanHalfFOV;
    Vec3 rayViewSpace = Vec3(viewX, viewY, -1.0f); // -1 because we look down negative Z in view space

    // Transform from view space to world space
    Vec3 rayWorldDirection = invView.MultiplyVec3(rayViewSpace);

    // The ray direction is from camera position to the world point
    Vec3 rayDirection = (rayWorldDirection - invView.MultiplyVec3(Vec3(0, 0, 0))).Normalize();

    return Ray(m_Position, rayDirection);
}