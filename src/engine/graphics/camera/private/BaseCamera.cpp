#include "BaseCamera.h"
#include "CameraAnimator.h"
#include "MathUtil.h"
#include "Input.h"
#include "EngineSettings.h"
#include <GLFW/glfw3.h>
#include <cmath>

// Forward declarations for camera types
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

using namespace Voltray::Math;
using EngineSettings = Voltray::Engine::EngineSettings;
using Input = Voltray::Engine::Input;

namespace Voltray::Engine
{

    BaseCamera::BaseCamera(const std::string &name, float aspect, float nearPlane, float farPlane)
        : m_Name(name), m_Aspect(aspect), m_Near(nearPlane), m_Far(farPlane)
    {
        m_Target = Voltray::Math::Vec3(0.0f, 0.0f, 0.0f);
        m_Distance = 3.0f;
        m_Yaw = -90.0f;
        m_Pitch = 0.0f;
        UpdatePositionFromAngles();
        UpdateViewMatrix();

        // Initialize the animator
        m_Animator = std::make_unique<CameraAnimator>(*this);
    }

    BaseCamera::~BaseCamera() = default;

    void BaseCamera::SetMouseMovementActive(bool active)
    {
        m_isMouseMovementActive = active;
    }

    Mat4 BaseCamera::GetViewMatrix() const
    {
        if (m_ViewMatrixDirty)
        {
            UpdateViewMatrix();
        }
        return m_ViewMatrix;
    }

    Mat4 BaseCamera::GetViewProjectionMatrix() const
    {
        return GetViewMatrix() * GetProjectionMatrix();
    }

    void BaseCamera::SetPosition(const Vec3 &position)
    {
        m_Position = position;
        m_Distance = (m_Position - m_Target).Length();
        UpdateAnglesFromPosition();
        m_ViewMatrixDirty = true;
    }

    void BaseCamera::SetTarget(const Vec3 &target)
    {
        m_Target = target;
        m_Distance = (m_Position - m_Target).Length();
        UpdateAnglesFromPosition();
        m_ViewMatrixDirty = true;
    }

    void BaseCamera::SetUp(const Vec3 &up)
    {
        m_Up = up;
        m_ViewMatrixDirty = true;
    }

    void BaseCamera::LookAt(const Vec3 &position, const Vec3 &target, const Vec3 &up)
    {
        m_Position = position;
        m_Target = target;
        m_Up = up;
        m_ViewMatrixDirty = true;
    }

    void BaseCamera::SetAspectRatio(float aspect)
    {
        m_Aspect = aspect;
    }

    void BaseCamera::SetClippingPlanes(float nearPlane, float farPlane)
    {
        m_Near = nearPlane;
        m_Far = farPlane;
    }

    void BaseCamera::SetName(const std::string &name)
    {
        m_Name = name;
    }

    void BaseCamera::Update()
    {
        // Update camera animation first
        if (m_Animator)
        {
            m_Animator->Update(0.016f); // Assuming 60 FPS, you might want to pass actual deltaTime
        }

        if (m_InputEnabled)
        {
            ProcessInput();
            UpdatePositionFromAngles();
        }
    }

    void BaseCamera::ProcessInput()
    {
        ProcessMouseInput();
        ProcessKeyboardInput();
        ProcessScrollInput();
    }

    void BaseCamera::SetInputEnabled(bool enabled)
    {
        m_InputEnabled = enabled;
    }

    void BaseCamera::SetViewportBounds(float x, float y, float width, float height)
    {
        m_ViewportX = x;
        m_ViewportY = y;
        m_ViewportWidth = width;
        m_ViewportHeight = height;
    }

    bool BaseCamera::IsMouseInViewport() const
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

    void BaseCamera::SetTransform(const Mat4 &transform)
    {
        // Extract position from transform matrix
        m_Position = Vec3(transform.data[12], transform.data[13], transform.data[14]);

        // Extract forward direction and calculate target
        Vec3 forward = Vec3(-transform.data[8], -transform.data[9], -transform.data[10]).Normalize();
        m_Target = m_Position + forward;

        // Extract up vector
        m_Up = Vec3(transform.data[4], transform.data[5], transform.data[6]).Normalize();

        m_ViewMatrixDirty = true;
    }

    Mat4 BaseCamera::GetTransform() const
    {
        Vec3 forward = GetForward();
        Vec3 right = GetRight();
        Vec3 up = m_Up;

        Mat4 transform;
        // Right vector
        transform.data[0] = right.x;
        transform.data[1] = right.y;
        transform.data[2] = right.z;
        transform.data[3] = 0.0f;

        // Up vector
        transform.data[4] = up.x;
        transform.data[5] = up.y;
        transform.data[6] = up.z;
        transform.data[7] = 0.0f;

        // Forward vector (negated for OpenGL convention)
        transform.data[8] = -forward.x;
        transform.data[9] = -forward.y;
        transform.data[10] = -forward.z;
        transform.data[11] = 0.0f;

        // Position
        transform.data[12] = m_Position.x;
        transform.data[13] = m_Position.y;
        transform.data[14] = m_Position.z;
        transform.data[15] = 1.0f;

        return transform;
    }

    void BaseCamera::ProcessMouseInput()
    {
        bool mmb = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE);
        static bool wasMmb = false;

        if (mmb && !wasMmb && IsMouseInViewport())
        {
            Input::HideCursor();
            m_CursorHidden = true;
        }
        else if (!mmb && wasMmb && m_CursorHidden)
        {
            Input::ShowCursor();
            m_CursorHidden = false;
        }
        wasMmb = mmb;

        // Only process camera movement if middle mouse is pressed, cursor is hidden (in camera mode),
        // AND mouse movement is currently active for the camera.
        if (!mmb || !m_CursorHidden || !m_isMouseMovementActive) // Added !m_isMouseMovementActive check
        {
            return;
        }

        // Check if mouse is in viewport OR if we're already in camera control mode (cursor hidden)
        // This check might be redundant given the m_CursorHidden check above, but kept for consistency if it serves other purposes.
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

    void BaseCamera::ProcessKeyboardInput()
    {
        // Default implementation - basic WASD movement
        // Can be overridden by derived classes

        const float speed = 5.0f;       // units per second
        const float deltaTime = 0.016f; // Assume 60 FPS for now

        Vec3 forward = GetForward();
        Vec3 right = GetRight();
        Vec3 movement(0.0f);

        if (Input::IsKeyPressed(GLFW_KEY_UP))
            movement += forward;
        if (Input::IsKeyPressed(GLFW_KEY_DOWN))
            movement -= forward;
        if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
            movement += right;
        if (Input::IsKeyPressed(GLFW_KEY_LEFT))
            movement -= right;
        if (Input::IsKeyPressed(GLFW_KEY_PAGE_UP))
            movement += m_Up;
        if (Input::IsKeyPressed(GLFW_KEY_PAGE_DOWN))
            movement -= m_Up;

        if (movement.Length() > 0.0f)
        {
            movement = movement.Normalize() * speed * deltaTime;
            m_Position += movement;
            m_Target += movement;
            m_ViewMatrixDirty = true;
        }
    }

    void BaseCamera::ProcessScrollInput()
    {
        // Assume Input::GetScrollDeltaY() returns the vertical scroll offset since last frame
        float scrollY = static_cast<float>(Input::GetScrollDeltaY());
        if (scrollY != 0.0f && (IsMouseInViewport() || m_CursorHidden))
            Zoom(scrollY);
    }

    void BaseCamera::UpdateViewMatrix() const
    {
        m_ViewMatrix = Mat4::LookAt(m_Position, m_Target, m_Up);
        m_ViewMatrixDirty = false;
    }

    void BaseCamera::Zoom(float delta)
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

    void BaseCamera::UpdatePositionFromAngles()
    {
        float yawRad = m_Yaw * DEG2RAD;
        float pitchRad = m_Pitch * DEG2RAD;
        Vec3 offset;
        offset.x = m_Distance * cosf(yawRad) * cosf(pitchRad);
        offset.y = m_Distance * sinf(pitchRad);
        offset.z = m_Distance * sinf(yawRad) * cosf(pitchRad);
        m_Position = m_Target + offset;
        m_ViewMatrixDirty = true;
    }

    void BaseCamera::UpdateAnglesFromPosition()
    {
        Vec3 offset = m_Position - m_Target;
        m_Distance = offset.Length();
        if (m_Distance < 1e-6f)
            return;
        m_Pitch = RAD2DEG * asinf(offset.y / m_Distance);
        m_Yaw = RAD2DEG * atan2f(offset.z, offset.x);
    }

    void BaseCamera::FocusOnObject(const Vec3 &objectPosition)
    {
        // Move the target to the object's position while maintaining the current viewing direction and distance
        m_Target = objectPosition;

        // Update the camera position to maintain the same relative distance and angles
        UpdatePositionFromAngles();

        // Mark the view matrix as dirty so it gets recalculated
        m_ViewMatrixDirty = true;
    }

    std::unique_ptr<BaseCamera> BaseCamera::Create(CameraType type, const std::string &name,
                                                   float aspect, float nearPlane, float farPlane)
    {
        std::string cameraName = name.empty() ? "Camera" : name;

        switch (type)
        {
        case CameraType::PERSPECTIVE:
            return std::make_unique<PerspectiveCamera>(cameraName, 45.0f, aspect, nearPlane, farPlane);
        case CameraType::ORTHOGRAPHIC:
            return std::make_unique<OrthographicCamera>(cameraName, aspect, nearPlane, farPlane);
        default:
            return std::make_unique<PerspectiveCamera>(cameraName, 45.0f, aspect, nearPlane, farPlane);
        }
    }

    void BaseCamera::FocusOnObjectSmooth(const Vec3 &objectPosition, float duration)
    {
        if (m_Animator)
        {
            m_Animator->FocusOnPosition(objectPosition, duration);
        }
    }

    CameraAnimator &BaseCamera::GetAnimator()
    {
        return *m_Animator;
    }

    bool BaseCamera::IsAnimating() const
    {
        return m_Animator && m_Animator->IsAnimating();
    }

}
