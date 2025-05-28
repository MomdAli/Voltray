#pragma once

#include <memory>
#include "../../../Engine/Graphics/Camera/BaseCamera.h"

// Forward declarations
class Scene;
class BaseCamera;
class Renderer;

namespace Editor::Components
{
    /**
     * @brief Manages scene initialization and lifecycle for the viewport
     */
    class ViewportScene
    {
    public:
        ViewportScene();
        ~ViewportScene(); // Moved destructor declaration to ensure proper cleanup

        /**
         * @brief Initialize the scene with demo objects
         */
        void Initialize();

        /**
         * @brief Get the scene instance
         * @return Reference to the scene
         */
        ::Scene &GetScene() { return *m_Scene; }

        /**
         * @brief Get the camera instance
         * @return Reference to the camera
         */
        ::BaseCamera &GetCamera() { return *m_Camera; }

        /**
         * @brief Get the renderer instance
         * @return Reference to the renderer
         */
        ::Renderer &GetRenderer() { return *m_Renderer; }

        /**
         * @brief Check if scene is properly initialized
         * @return True if all components are created
         */
        bool IsInitialized() const;

        /**
         * @brief Update camera aspect ratio
         * @param width Viewport width
         * @param height Viewport height
         */
        void UpdateCameraAspect(int width, int height);

        /**
         * @brief Set camera viewport bounds for input handling
         * @param x X position
         * @param y Y position
         * @param width Width
         * @param height Height
         */
        void SetCameraViewportBounds(float x, float y, float width, float height);

        /**
         * @brief Switch to a different camera type
         * @param type The camera type to switch to
         */
        void SwitchCamera(CameraType type);

        /**
         * @brief Update camera settings
         * @param fov Field of view for perspective cameras
         * @param orthoSize Orthographic size for parallel cameras
         */
        void UpdateCameraSettings(float fov, float orthoSize);

        /**
         * @brief Get current camera type
         * @return Current camera type
         */
        CameraType GetCurrentCameraType() const;

    private:
        void createDemoScene();
        void loadDemoModel();

        std::unique_ptr<::Scene> m_Scene;
        std::unique_ptr<::BaseCamera> m_Camera;
        std::unique_ptr<::Renderer> m_Renderer;
    };
}
