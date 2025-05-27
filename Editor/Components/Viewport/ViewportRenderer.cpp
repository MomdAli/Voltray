#include "ViewportRenderer.h"
#include "Console.h"
#include "../../Engine/Graphics/Shader.h"
#include "../../Engine/Graphics/Camera.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/SceneObject.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Utils/ResourceManager.h"

namespace Editor::Components
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
    {
        // Load shaders using ResourceManager
        std::string defaultVertPath = ::ResourceManager::GetResourcePath("Shaders/default.vert");
        std::string defaultFragPath = ::ResourceManager::GetResourcePath("Shaders/default.frag");
        std::string skyboxVertPath = ::ResourceManager::GetResourcePath("Shaders/skybox.vert");
        std::string skyboxFragPath = ::ResourceManager::GetResourcePath("Shaders/skybox.frag");
        std::string outlineVertPath = ::ResourceManager::GetResourcePath("Shaders/outline.vert");
        std::string outlineFragPath = ::ResourceManager::GetResourcePath("Shaders/outline.frag");

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
    }

    void ViewportRenderer::RenderScene(::Scene &scene, ::Camera &camera, ::Renderer &renderer, int width, int height)
    {
        if (width <= 0 || height <= 0)
            return;

        // Update camera
        camera.Update();

        // Render skybox first
        renderSkybox(camera);

        // Clear depth buffer after skybox to ensure proper depth testing
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

    void ViewportRenderer::renderSkybox(::Camera &camera)
    {
        if (!m_SkyboxShader)
            return;

        glDepthMask(GL_FALSE);
        m_SkyboxShader->Bind();

        auto invVP = camera.GetViewProjectionMatrix().Inverse();
        m_SkyboxShader->SetUniformMat4("u_InverseViewProj", invVP.data);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDepthMask(GL_TRUE);
    }

    void ViewportRenderer::renderSceneObjects(::Scene &scene, ::Camera &camera, ::Renderer &renderer)
    {
        if (!m_Shader)
            return;

        m_Shader->Bind();
        m_Shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);

        scene.Render(renderer, camera, *m_Shader);
    }

    void ViewportRenderer::renderSelectionOutlines(::Scene &scene, ::Camera &camera)
    {
        if (!m_OutlineShader)
            return;

        auto selectedObject = scene.GetSelectedObject();
        if (!selectedObject || !selectedObject->GetMesh())
            return;

        // Save OpenGL state
        GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLboolean cullFaceEnabled = glIsEnabled(GL_CULL_FACE);

        // First pass: Render expanded outline
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        m_OutlineShader->Bind();
        m_OutlineShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);
        m_OutlineShader->SetUniformMat4("u_Model", selectedObject->GetModelMatrix().data);
        m_OutlineShader->SetUniform3f("u_OutlineColor", 0.3f, 0.7f, 1.0f); // Soft blue color
        m_OutlineShader->SetUniform1f("u_OutlineThickness", 0.05f);        // Smaller thickness

        selectedObject->GetMesh()->Draw();

        // Second pass: Render normal object on top
        glDepthMask(GL_TRUE);
        if (cullFaceEnabled)
            glEnable(GL_CULL_FACE);

        if (m_Shader)
        {
            m_Shader->Bind();
            m_Shader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);
            m_Shader->SetUniformMat4("u_Model", selectedObject->GetModelMatrix().data);
            selectedObject->GetMesh()->Draw();
        }

        // Restore OpenGL state
        if (!depthTestEnabled)
            glDisable(GL_DEPTH_TEST);
        if (!cullFaceEnabled)
            glDisable(GL_CULL_FACE);
    }
}
