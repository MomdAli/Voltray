#pragma once

#include <glad/gl.h>

namespace Voltray::Editor::Components
{
    /**
     * @brief Manages OpenGL framebuffer operations for viewport rendering
     */
    class ViewportFramebuffer
    {
    public:
        ViewportFramebuffer();
        ~ViewportFramebuffer();

        // Disable copying
        ViewportFramebuffer(const ViewportFramebuffer &) = delete;
        ViewportFramebuffer &operator=(const ViewportFramebuffer &) = delete;

        /**
         * @brief Initialize the framebuffer
         */
        void Initialize();

        /**
         * @brief Resize the framebuffer to new dimensions
         * @param width New width
         * @param height New height
         */
        void Resize(int width, int height);

        /**
         * @brief Bind the framebuffer for rendering
         */
        void Bind();

        /**
         * @brief Unbind the framebuffer
         */
        void Unbind();

        /**
         * @brief Clear the framebuffer
         * @param r Red component
         * @param g Green component
         * @param b Blue component
         * @param a Alpha component
         */
        void Clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);

        /**
         * @brief Get the color texture ID for ImGui rendering
         * @return OpenGL texture ID
         */
        GLuint GetColorTexture() const { return m_ColorTex; }

        /**
         * @brief Get current framebuffer width
         * @return Width in pixels
         */
        int GetWidth() const { return m_Width; }

        /**
         * @brief Get current framebuffer height
         * @return Height in pixels
         */
        int GetHeight() const { return m_Height; } /**
                                                    * @brief Check if framebuffer is valid
                                                    * @return True if framebuffer is complete
                                                    */
        bool IsValid() const;

        /**
         * @brief Check if framebuffer objects are created
         * @return True if OpenGL objects are created
         */
        bool IsCreated() const;

    private:
        void createFramebuffer();
        void cleanup();

        GLuint m_FBO = 0;
        GLuint m_ColorTex = 0;
        GLuint m_DepthRbo = 0;
        int m_Width = 0;
        int m_Height = 0;
    };
}
