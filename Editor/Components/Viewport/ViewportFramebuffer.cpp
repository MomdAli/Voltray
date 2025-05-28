#include "ViewportFramebuffer.h"
#include "Console.h"
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

namespace Editor::Components
{
    ViewportFramebuffer::ViewportFramebuffer()
    {
        // Constructor - initialization done in Initialize()
    }

    ViewportFramebuffer::~ViewportFramebuffer()
    {
        cleanup();
    }

    void ViewportFramebuffer::Initialize()
    {
        createFramebuffer();
    }

    void ViewportFramebuffer::Resize(int width, int height)
    {
        if (width <= 0 || height <= 0)
            return;

        m_Width = width;
        m_Height = height;

        // Bind and configure framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        // Configure color texture
        glBindTexture(GL_TEXTURE_2D, m_ColorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0);

        // Configure depth renderbuffer
        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRbo);

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            Console::PrintError("Framebuffer not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ViewportFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, m_Width, m_Height);
        glEnable(GL_DEPTH_TEST);
    }

    void ViewportFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void ViewportFramebuffer::Clear(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    bool ViewportFramebuffer::IsValid() const
    {
        if (m_FBO == 0)
            return false;

        GLint oldFBO;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        bool isComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
        return isComplete;
    }

    bool ViewportFramebuffer::IsCreated() const
    {
        return m_FBO != 0 && m_ColorTex != 0 && m_DepthRbo != 0;
    }
    void ViewportFramebuffer::createFramebuffer()
    {
        // Check if OpenGL context is available by testing a simple OpenGL call
        GLint maxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        if (glGetError() != GL_NO_ERROR)
        {
            Console::PrintError("OpenGL context not available for framebuffer creation");
            return;
        }

        Console::Print("Creating OpenGL framebuffer objects...");

        glGenFramebuffers(1, &m_FBO);
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Console::PrintError("Failed to generate framebuffer: OpenGL error " + std::to_string(error));
            return;
        }

        glGenTextures(1, &m_ColorTex);
        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Console::PrintError("Failed to generate color texture: OpenGL error " + std::to_string(error));
            return;
        }

        glGenRenderbuffers(1, &m_DepthRbo);
        error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Console::PrintError("Failed to generate depth renderbuffer: OpenGL error " + std::to_string(error));
            return;
        }

        m_Width = m_Height = 0;
        Console::Print("Framebuffer objects created successfully");
    }

    void ViewportFramebuffer::cleanup()
    {
        if (m_FBO)
            glDeleteFramebuffers(1, &m_FBO);
        if (m_ColorTex)
            glDeleteTextures(1, &m_ColorTex);
        if (m_DepthRbo)
            glDeleteRenderbuffers(1, &m_DepthRbo);

        m_FBO = 0;
        m_ColorTex = 0;
        m_DepthRbo = 0;
    }
}
