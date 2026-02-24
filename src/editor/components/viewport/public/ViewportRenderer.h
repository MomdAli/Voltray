#pragma once

#include "Shader.h"
#include "Renderer.h"
#include "Scene.h"
#include "BaseCamera.h"
#include <memory>
#include <glad/gl.h>

// Engine components
using namespace Voltray::Engine;

namespace Voltray::Editor::Components
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
        void RenderScene(::Scene &scene, ::BaseCamera &camera, ::Renderer &renderer, int width, int height);

        /**
         * @brief Check if renderer is properly initialized
         * @return True if all required shaders are loaded
         */
        bool IsInitialized() const;

    private:
        void renderSkybox(::BaseCamera &camera);
        void renderSceneObjects(::Scene &scene, ::BaseCamera &camera, ::Renderer &renderer);
        void renderSelectionOutlines(::Scene &scene, ::BaseCamera &camera);

        // Shader resources
        std::unique_ptr<::Shader> m_Shader;
        std::unique_ptr<::Shader> m_SkyboxShader;
        std::unique_ptr<::Shader> m_OutlineShader;

        // Full-screen triangle for skybox
        GLuint m_SkyboxVAO;
        GLuint m_SkyboxVBO;
    };
}
