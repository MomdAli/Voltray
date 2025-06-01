#pragma once

#include "BaseCamera.h"

/**
 * @class OrthographicCamera
 * @brief Orthographic projection camera for parallel projections
 *
 * Orthographic projection preserves parallel lines and doesn't apply perspective distortion.
 * Commonly used in technical drawings, architectural plans, and 2D-style games.
 */
class OrthographicCamera : public BaseCamera
{
public:
    /**
     * @brief Constructor for orthographic camera
     * @param name Camera name
     * @param aspect Aspect ratio
     * @param nearPlane Near clipping plane
     * @param farPlane Far clipping plane
     * @param size Orthographic size (default: 10)
     */
    OrthographicCamera(const std::string &name, float aspect, float nearPlane, float farPlane, float size = 10.0f);

    // Camera type identification
    virtual CameraType GetType() const override { return CameraType::ORTHOGRAPHIC; }
    virtual std::string GetTypeName() const override { return "Orthographic"; }

    // Orthographic projection
    virtual Mat4 GetProjectionMatrix() const override;
    virtual Ray ScreenToWorldRay(float screenX, float screenY) const override;

    // Orthographic-specific operations
    virtual void SetOrthographicSize(float size);
    virtual float GetOrthographicSize() const { return m_Size; }

protected:
    float m_Size; // Half the height of the orthographic view volume
};
