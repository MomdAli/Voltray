#pragma once

#include <GLFW/glfw3.h>

#include "Math/vec3.h"
#include "Math/Mat4.h"

/**
 * @class Camera
 * @brief Represents a 3D camera with perspective projection.
 *
 * The Camera class encapsulates the parameters and operations for a perspective camera,
 * including position, target, field of view, aspect ratio, and near/far clipping planes.
 * Provides methods to set the camera's position and target, and to retrieve view,
 * projection, and combined view-projection matrices.
 */
class Camera
{
public:
    /**
     * @brief Constructs a Camera object with the specified parameters.
     *
     * @param fovDeg Field of view in degrees.
     * @param aspect Aspect ratio (width divided by height).
     * @param nearPlane Distance to the near clipping plane.
     * @param farPlane Distance to the far clipping plane.
     */
    Camera(float fovDeg, float aspect, float nearPlane, float farPlane);

    void SetPosition(const Vec3 &position);
    void SetTarget(const Vec3 &target);

    Mat4 GetViewMatrix() const;
    Mat4 GetProjectionMatrix() const;
    Mat4 GetViewProjectionMatrix() const;

    void Update();
    void ProcessMouseInput();
    void ProcessScrollInput();
    void Zoom(float delta);

private:
    void UpdatePositionFromAngles();
    void UpdateAnglesFromPosition();

    Vec3 m_Position = Vec3(0.0f, 0.0f, 3.0f);
    Vec3 m_Target = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 m_Up = Vec3(0.0f, 1.0f, 0.0f);

    float m_Fov;
    float m_Aspect;
    float m_Near;
    float m_Far;
    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_MouseSensitivity = 0.1f;
    float m_Distance = 5.0f;
};
