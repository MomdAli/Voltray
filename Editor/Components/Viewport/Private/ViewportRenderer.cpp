#include "ViewportRenderer.h"
#include "Console.h"
#include "SceneObject.h"
#include "ResourceManager.h"

using Voltray::Utils::ResourceManager;

namespace Voltray::Editor::Components
{
    ViewportRenderer::ViewportRenderer()
    {
        // Constructor - initialization done in Initialize()
    }

    ViewportRenderer::~ViewportRenderer()
    {
        // Destructor - unique_ptrs will automatically clean up
    }

    void ViewportRenderer::Initialize()
    { // Load shaders using ResourceManager
        std::string defaultVertPath = ResourceManager::GetGlobalResourcePath("Shaders/default.vert");
        std::string defaultFragPath = ResourceManager::GetGlobalResourcePath("Shaders/default.frag");
        std::string skyboxVertPath = ResourceManager::GetGlobalResourcePath("Shaders/skybox.vert");
        std::string skyboxFragPath = ResourceManager::GetGlobalResourcePath("Shaders/skybox.frag");
        std::string outlineVertPath = ResourceManager::GetGlobalResourcePath("Shaders/outline.vert");
        std::string outlineFragPath = ResourceManager::GetGlobalResourcePath("Shaders/outline.frag");

        // Load default shader
        if (!defaultVertPath.empty() && !defaultFragPath.empty())
        {
            try
            {
                m_Shader = std::make_unique<::Shader>(defaultVertPath, defaultFragPath);
            }
            catch (const std::exception &e)
            {
                Console::PrintError("Failed to create default shader: " + std::string(e.what()));
                m_Shader = nullptr;
            }
        }

        // Load skybox shader
        if (!skyboxVertPath.empty() && !skyboxFragPath.empty())
        {
            try
            {
                m_SkyboxShader = std::make_unique<::Shader>(skyboxVertPath, skyboxFragPath);
            }
            catch (const std::exception &e)
            {
                Console::PrintError("Failed to create skybox shader: " + std::string(e.what()));
                m_SkyboxShader = nullptr;
            }
        }

        // Load outline shader
        if (!outlineVertPath.empty() && !outlineFragPath.empty())
        {
            try
            {
                m_OutlineShader = std::make_unique<::Shader>(outlineVertPath, outlineFragPath);
            }
            catch (const std::exception &e)
            {
                Console::PrintError("Failed to create outline shader: " + std::string(e.what()));
                m_OutlineShader = nullptr;
            }
        }

        // Setup full-screen triangle for skybox rendering
        {
            // Triangle that covers the screen
            float skyboxVertices[] = {
                -1.0f, -1.0f,
                3.0f, -1.0f,
                -1.0f, 3.0f};
            glGenVertexArrays(1, &m_SkyboxVAO);
            glGenBuffers(1, &m_SkyboxVBO);
            glBindVertexArray(m_SkyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
            glBindVertexArray(0);
        }
    }

    void ViewportRenderer::RenderScene(::Scene &scene, ::BaseCamera &camera, ::Renderer &renderer, int width, int height)
    {
        if (width <= 0 || height <= 0)
            return;

        // Set viewport and clear color buffer to ensure skybox is visible even with no objects
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set your preferred background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera
        camera.Update();

        // Render skybox first
        renderSkybox(camera);

        // Clear depth buffer after skybox to ensure proper depth testing for scene objects
        glClear(GL_DEPTH_BUFFER_BIT);

        // Update scene
        scene.Update(0.016f); // Assume ~60 FPS

        // Render scene objects
        renderSceneObjects(scene, camera, renderer);

        // Render selection outlines
        renderSelectionOutlines(scene, camera);
    }

    bool ViewportRenderer::IsInitialized() const
    {
        return m_Shader && m_SkyboxShader && m_OutlineShader;
    }

    void ViewportRenderer::renderSkybox(::BaseCamera &camera)
    {
        if (!m_SkyboxShader)
        {
            Console::PrintError("Skybox shader not initialized");
            return;
        }

        // Draw with depth <= far plane, disable depth write
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        m_SkyboxShader->Bind();

        // Set the inverse view-projection matrix uniform
        auto invVP = camera.GetViewProjectionMatrix().Inverse();
        m_SkyboxShader->SetUniformMat4("u_InverseViewProj", invVP.data);

        // Bind full-screen triangle VAO and draw
        glBindVertexArray(m_SkyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Restore depth state
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    void ViewportRenderer::renderSceneObjects(::Scene &scene, ::BaseCamera &camera, ::Renderer &renderer)
    {
        (void)renderer; // Suppress unreferenced parameter warning
        if (!m_Shader)
            return;

        m_Shader->Bind();
        m_Shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);

        // Render each object with its material color
        auto &objects = scene.GetObjects();
        for (auto &object : objects)
        {
            if (object && object->IsVisible() && object->GetMesh())
            {
                m_Shader->SetUniformMat4("u_Model", object->GetModelMatrix().data);

                // Set the material color uniform
                Vec3 materialColor = object->GetMaterialColor();
                m_Shader->SetUniform3f("u_MaterialColor", materialColor.x, materialColor.y, materialColor.z);

                object->GetMesh()->Draw();
            }
        }

        // Unbind shader to prevent conflicts
        m_Shader->Unbind();
    }

    void ViewportRenderer::renderSelectionOutlines(::Scene &scene, ::BaseCamera &camera)
    {
        if (!m_OutlineShader)
            return;

        auto selectedObject = scene.GetSelectedObject();
        if (!selectedObject || !selectedObject->GetMesh())
            return;

        // Save OpenGL state
        GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);

        // Enable wireframe mode for outline rendering
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        // Enable line smoothing and set line width for better outline quality
        glEnable(GL_LINE_SMOOTH);
        glLineWidth(2.0f);

        m_OutlineShader->Bind();
        m_OutlineShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);
        m_OutlineShader->SetUniformMat4("u_Model", selectedObject->GetModelMatrix().data);
        m_OutlineShader->SetUniform3f("u_OutlineColor", 0.7f, 0.9f, 1.0f); // Glowing light blue closer to white

        selectedObject->GetMesh()->Draw();

        // Restore OpenGL state
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode[0]);
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);

        if (!depthTestEnabled)
            glDisable(GL_DEPTH_TEST);
        if (cullFaceEnabled)
            glEnable(GL_CULL_FACE);
    }
}
