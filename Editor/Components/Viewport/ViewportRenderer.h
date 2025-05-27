#pragma once

#include <memory>
#include <glad/gl.h>

// Forward declarations
class Camera;
class Scene;
class Renderer;
class Shader;

namespace Editor::Components
{
    /**
     * @brief Handles scene rendering operations for the viewport
     */
    class ViewportRenderer
    {
    public:
        ViewportRenderer();
        ~ViewportRenderer(); // Moved destructor declaration to ensure proper cleanup

        /**
         * @brief Initialize the renderer with required shaders
         */
        void Initialize();

        /**
         * @brief Render the complete scene
         * @param scene Scene to render
         * @param camera Camera for rendering
         * @param renderer Renderer instance
         * @param width Viewport width
         * @param height Viewport height
         */
        void RenderScene(::Scene &scene, ::Camera &camera, ::Renderer &renderer, int width, int height);

        /**
         * @brief Check if renderer is properly initialized
         * @return True if all required shaders are loaded
         */
        bool IsInitialized() const;

    private:
        void renderSkybox(::Camera &camera);
        void renderSceneObjects(::Scene &scene, ::Camera &camera, ::Renderer &renderer);
        void renderSelectionOutlines(::Scene &scene, ::Camera &camera);

        // Shader resources
        std::unique_ptr<::Shader> m_Shader;
        std::unique_ptr<::Shader> m_SkyboxShader;
        std::unique_ptr<::Shader> m_OutlineShader;
    };
}
