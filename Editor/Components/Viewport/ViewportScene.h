#pragma once

#include <memory>

// Forward declarations
class Scene;
class Camera;
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
        ::Camera &GetCamera() { return *m_Camera; }

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

    private:
        void createDemoScene();
        void loadDemoModel();

        std::unique_ptr<::Scene> m_Scene;
        std::unique_ptr<::Camera> m_Camera;
        std::unique_ptr<::Renderer> m_Renderer;
    };
}
