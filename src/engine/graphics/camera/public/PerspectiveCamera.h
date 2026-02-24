#pragma once

#include "BaseCamera.h"

namespace Voltray::Engine
{

    /**
     * @class PerspectiveCamera
     * @brief Base class for all perspective projection cameras
     *
     * This class provides common functionality for perspective cameras,
     * including field of view management and basic perspective projection.
     * Derived classes implement specific perspective projection types.
     */
    class PerspectiveCamera : public BaseCamera
    {
    public:
        /**
         * @brief Constructor for perspective camera
         * @param name Camera name
         * @param fovDeg Field of view in degrees
         * @param aspect Aspect ratio
         * @param nearPlane Near clipping plane
         * @param farPlane Far clipping plane
         */
        PerspectiveCamera(const std::string &name, float fovDeg, float aspect, float nearPlane, float farPlane);

        // Perspective-specific operations
        virtual void SetFieldOfView(float fovDeg);
        virtual float GetFieldOfView() const { return m_Fov; }

        // Basic perspective projection (can be overridden for specialized projections)
        virtual Mat4 GetProjectionMatrix() const override;                         // Ray casting for perspective cameras
        virtual Ray ScreenToWorldRay(float screenX, float screenY) const override; // Camera type identification
        virtual CameraType GetType() const override { return CameraType::PERSPECTIVE; }
        virtual std::string GetTypeName() const override { return "Perspective"; }

    protected:
        float m_Fov; // Field of view in degrees

        // Helper methods for specialized perspective projections
        virtual Mat4 CreatePerspectiveMatrix(float fov, float aspect, float nearPlane, float farPlane) const;
        virtual Mat4 ApplyPerspectiveCorrection(const Mat4 &baseProjection) const;
    };

}
