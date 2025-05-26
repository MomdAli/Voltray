#include <glad/gl.h>
#include <imgui.h>
#include <filesystem>
#include <memory>
#include <iostream>

#include "Viewport.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/Mesh.h"
#include "../../Engine/Graphics/Shader.h"
#include "../../Engine/Graphics/Camera.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/SceneObjectFactory.h"
#include "../../Engine/Input/Input.h"

namespace Editor::Components
{
    Viewport::Viewport()
    {
        initFramebuffer();
        initScene();
    }

    Viewport::~Viewport()
    {
        // Free native OpenGL objects
        if (m_FBO)
            glDeleteFramebuffers(1, &m_FBO);
        if (m_ColorTex)
            glDeleteTextures(1, &m_ColorTex);
        if (m_DepthRbo)
            glDeleteRenderbuffers(1, &m_DepthRbo);

        // Set to zero to prevent double-deletion
        m_FBO = 0;
        m_ColorTex = 0;
        m_DepthRbo = 0; // Destroy scene objects in reverse order of creation
        // explicitly release unique_ptrs to control destruction order
        m_Scene.reset();
        m_Shader.reset();
        m_SkyboxShader.reset();
        m_Renderer.reset();
        m_Camera.reset();
    }

    void Viewport::Draw()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{0, 0, 0, 0});
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        ImVec2 size = ImGui::GetContentRegionAvail();
        int width = (int)size.x;
        int height = (int)size.y;

        if (width != m_FBOWidth || height != m_FBOHeight)
            resizeFramebuffer(width, height);

        renderScene(width, height);
        ImGui::Image((ImTextureID)(intptr_t)m_ColorTex, size, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
    }

    void Viewport::initFramebuffer()
    {
        glGenFramebuffers(1, &m_FBO);
        glGenTextures(1, &m_ColorTex);
        glGenRenderbuffers(1, &m_DepthRbo);
        m_FBOWidth = m_FBOHeight = 0;
    }

    void Viewport::resizeFramebuffer(int width, int height)
    {
        m_FBOWidth = width;
        m_FBOHeight = height;

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glBindTexture(GL_TEXTURE_2D, m_ColorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthRbo);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Viewport::initScene()
    {
        // Create scene and add some demo objects
        m_Scene = std::make_unique<Scene>();

        // Add a cube at the origin
        auto cube = SceneObjectFactory::CreateCube("DemoCube", 1.0f);
        cube->GetTransform().SetPosition(Vec3(0.0f, 0.0f, 0.0f));
        m_Scene->AddObject(cube);

        // Add a sphere to the right
        auto sphere = SceneObjectFactory::CreateSphere("DemoSphere", 0.8f);
        sphere->GetTransform().SetPosition(Vec3(3.0f, 0.0f, 0.0f));
        m_Scene->AddObject(sphere); // Add a plane as ground
        auto ground = SceneObjectFactory::CreatePlane("Ground", 10.0f, 10.0f);
        ground->GetTransform().SetPosition(Vec3(0.0f, -2.0f, 0.0f));
        m_Scene->AddObject(ground);

        // pyramid model loading
        auto root = std::filesystem::current_path();
        while (!std::filesystem::exists(root / "Models") && root.has_parent_path())
            root = root.parent_path();
        auto modelPath = root / "Models" / "pyramid.obj";

        if (std::filesystem::exists(modelPath))
        {
            auto pyramid = SceneObjectFactory::LoadFromFile(modelPath.string(), "Testpyramid");
            if (pyramid)
            {
                pyramid->GetTransform().SetPosition(Vec3(-3.0f, 1.0f, 0.0f));
                m_Scene->AddObject(pyramid);
                std::cout << "Successfully loaded pyramid.obj!" << std::endl;
            }
        }
        else
        {
            std::cout << "pyramid.obj not found at: " << modelPath << std::endl;
        }

        // Load shaders
        root = std::filesystem::current_path();
        while (!std::filesystem::exists(root / "Shaders") && root.has_parent_path())
            root = root.parent_path();
        auto sd = root / "Shaders";
        m_Shader = std::make_unique<Shader>((sd / "default.vert").string(), (sd / "default.frag").string());
        m_SkyboxShader = std::make_unique<Shader>((sd / "skybox.vert").string(), (sd / "skybox.frag").string());

        m_Renderer = std::make_unique<Renderer>();
        m_Camera = std::make_unique<Camera>(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        m_Camera->SetPosition({0, 0, 5});
        m_Camera->SetTarget({0, 0, 0});
    }

    void Viewport::renderScene(int width, int height)
    {
        if (width <= 0 || height <= 0)
            return;
        Input::Update();
        m_Camera->Update();

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox
        glDepthMask(GL_FALSE);
        m_SkyboxShader->Bind();
        auto invVP = m_Camera->GetViewProjectionMatrix().Inverse();
        m_SkyboxShader->SetUniformMat4("u_InverseViewProj", invVP.data);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT); // Update and render scene
        if (m_Scene)
        {
            m_Scene->Update(0.016f); // Assume ~60 FPS for demo
        }

        // Mesh rendering
        m_Shader->Bind();
        m_Shader->SetUniformMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix().data);

        if (m_Scene)
        {
            m_Scene->Render(*m_Renderer, *m_Camera, *m_Shader);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
