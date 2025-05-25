/**
 * @file Viewport.h
 * @brief Defines the Viewport component which renders a 3D scene to an offscreen framebuffer
 *
 * The Viewport component creates a dockable panel that can display a 3D scene within the editor.
 * It handles framebuffer initialization, resizing, and rendering the scene with OpenGL.
 */

#pragma once

#include <glad/gl.h>
#include <memory>

#include "../UI/Panel.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

namespace Editor::Components
{
    /**
     * @class Viewport
     * @brief A dockable panel that renders the 3D scene into an offscreen framebuffer
     *
     * The Viewport provides a window within the editor UI that displays a 3D scene.
     * It manages an OpenGL framebuffer and the rendering pipeline for the scene.
     */
    class Viewport : public Panel
    {
    public:
        /**
         * @brief Constructs a new Viewport instance
         *
         * Initializes the viewport panel, sets up OpenGL resources, and prepares the scene for rendering.
         */
        Viewport();

        /**
         * @brief Destroys the Viewport instance
         *
         * Cleans up OpenGL resources and deallocates memory.
         */
        ~Viewport();

        /**
         * @brief Renders the viewport panel and its contents
         *
         * Called each frame to update and display the viewport. Handles input, renders
         * the scene to the framebuffer, and displays the rendered image within the panel.
         */
        void Draw() override;

    private:
        void initScene();
        void initFramebuffer();
        void resizeFramebuffer(int width, int height);
        void renderScene(int width, int height);

        // OpenGL framebuffer objects
        GLuint m_FBO = 0;
        GLuint m_ColorTex = 0;
        GLuint m_DepthRbo = 0;
        int m_FBOWidth = 0;
        int m_FBOHeight = 0;

        // Scene objects
        std::unique_ptr<Renderer> m_Renderer;
        std::unique_ptr<Mesh> m_Mesh;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Shader> m_SkyboxShader;
        std::unique_ptr<Camera> m_Camera;
    };
}
