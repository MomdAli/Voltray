#pragma once
#include "Panel.h"
#include "BaseCamera.h"
#include "BaseCamera.h"

using Voltray::Engine::CameraType;

/**
 * @file Settings.h
 * @brief Defines the Settings panel component for the Voltray editor.
 */

namespace Voltray::Editor::Components
{
    /**
     * @class Settings
     * @brief A panel component that provides settings interface in the editor.
     * @extends Panel
     */
    class Settings : public Panel
    {
    public:
        /**
         * @brief Renders the settings panel interface.
         * @override Implements the abstract method from the Panel base class.
         */
        void Draw() override;

    private:
        // Camera settings
        CameraType m_selectedCameraType = CameraType::PERSPECTIVE;
        float m_perspectiveFOV = 60.0f;
        float m_orthographicSize = 5.0f;
        float m_cameraNearPlane = 0.1f;
        float m_cameraFarPlane = 100.0f;

        // Helper methods
        void DrawCameraSettings();
        void DrawCameraSelector();
        void DrawCameraSpecificSettings();
        const char *GetCameraTypeName(CameraType type) const;
        const char *GetCameraCategory(CameraType type) const;
    };
}
