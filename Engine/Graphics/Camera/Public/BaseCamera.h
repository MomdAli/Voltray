#pragma once

#include "Vec3.h"
#include "Mat4.h"
#include "Ray.h"
#include <memory>
#include <string>

using namespace Voltray::Math;

namespace Voltray::Engine
{

    // Forward declaration
    class CameraAnimator;

    /**
     * @enum CameraType
     * @brief Enumeration of all supported camera types
     */
    enum class CameraType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    /**
     * @class BaseCamera
     * @brief Abstract base class for all camera types
     *
     * This class provides the common interface and functionality for all camera types.
     * It handles basic camera operations like positioning, input handling, and provides
     * pure virtual methods for projection-specific calculations.
     *
     * The design allows cameras to be used both as viewport cameras and potentially
     * as scene objects in the future.
     */
    class BaseCamera
    {
    public:
        /**
         * @brief Constructor with basic camera parameters
         * @param name Camera name/identifier
         * @param aspect Aspect ratio (width/height)
         * @param nearPlane Near clipping plane distance
         * @param farPlane Far clipping plane distance
         */
        BaseCamera(const std::string &name, float aspect, float nearPlane, float farPlane); /**
                                                                                             * @brief Virtual destructor
                                                                                             */
        virtual ~BaseCamera();

        // Pure virtual methods that must be implemented by derived classes
        virtual Mat4 GetProjectionMatrix() const = 0;
        virtual CameraType GetType() const = 0;
        virtual std::string GetTypeName() const = 0;
        virtual Ray ScreenToWorldRay(float screenX, float screenY) const = 0;

        // Common camera operations
        virtual Mat4 GetViewMatrix() const;
        virtual Mat4 GetViewProjectionMatrix() const;

        // Position and orientation
        virtual void SetPosition(const Vec3 &position);
        virtual void SetTarget(const Vec3 &target);
        virtual void SetUp(const Vec3 &up);
        virtual void LookAt(const Vec3 &position, const Vec3 &target, const Vec3 &up);

        // Properties
        virtual void SetAspectRatio(float aspect);
        virtual void SetClippingPlanes(float nearPlane, float farPlane);
        virtual void SetName(const std::string &name);

        // Getters
        const Vec3 &GetPosition() const { return m_Position; }
        const Vec3 &GetTarget() const { return m_Target; }
        const Vec3 &GetUp() const { return m_Up; }
        Vec3 GetForward() const { return (m_Target - m_Position).Normalize(); }
        Vec3 GetRight() const { return GetForward().Cross(m_Up).Normalize(); }
        float GetAspectRatio() const { return m_Aspect; }
        float GetNearPlane() const { return m_Near; }
        float GetFarPlane() const { return m_Far; }
        const std::string &GetName() const { return m_Name; }

        // Input handling (can be overridden by derived classes)
        virtual void Update();
        virtual void ProcessInput();
        virtual void SetInputEnabled(bool enabled);
        virtual void SetMouseMovementActive(bool active); // New method
        virtual bool IsInputEnabled() const { return m_InputEnabled; }

        // Viewport bounds for input constraint
        virtual void SetViewportBounds(float x, float y, float width, float height);
        virtual bool IsMouseInViewport() const; // Animation/interpolation support for future scene object integration
        virtual void SetTransform(const Mat4 &transform);
        virtual Mat4 GetTransform() const;

        // Camera animation methods
        virtual void FocusOnObjectSmooth(const Vec3 &objectPosition, float duration = 1.0f);
        virtual CameraAnimator &GetAnimator();
        virtual bool IsAnimating() const;

        // Factory method for creating cameras
        static std::unique_ptr<BaseCamera> Create(CameraType type, const std::string &name = "",
                                                  float aspect = 16.0f / 9.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);

    protected:
        // Core camera properties
        std::string m_Name;
        Vec3 m_Position{0.0f, 0.0f, 3.0f};
        Vec3 m_Target{0.0f, 0.0f, 0.0f};
        Vec3 m_Up{0.0f, 1.0f, 0.0f};
        float m_Aspect;
        float m_Near;
        float m_Far;

        // Input handling
        bool m_InputEnabled = true;
        bool m_isMouseMovementActive = true; // New member
        float m_MouseSensitivity = 0.1f;
        float m_ZoomSpeed = 1.0f;
        bool m_CursorHidden = false;

        // Camera orbit parameters (similar to original Camera.cpp)
        float m_Yaw = -90.0f;
        float m_Pitch = 0.0f;
        float m_Distance = 3.0f;

        // Viewport bounds for input checking
        float m_ViewportX = 0.0f;
        float m_ViewportY = 0.0f;
        float m_ViewportWidth = 0.0f;
        float m_ViewportHeight = 0.0f;

        // Helper methods for input handling
        virtual void ProcessMouseInput();
        virtual void ProcessKeyboardInput();
        virtual void ProcessScrollInput();

        // Camera orbit methods (similar to original Camera.cpp)
        virtual void UpdatePositionFromAngles();
        virtual void UpdateAnglesFromPosition();
        virtual void Zoom(float delta);

        // Focus functionality
        virtual void FocusOnObject(const Vec3 &objectPosition); // Utility methods
        void UpdateViewMatrix() const;

        // Camera animation
        std::unique_ptr<CameraAnimator> m_Animator; // Cached matrices (mutable for lazy evaluation)
        mutable Mat4 m_ViewMatrix;
        mutable bool m_ViewMatrixDirty = true;
    };

}
