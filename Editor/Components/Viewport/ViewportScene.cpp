#include "ViewportScene.h"
#include "Console.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/SceneObject.h"
#include "../../Engine/Scene/SceneObjectFactory.h"
#include "../../Engine/Graphics/Camera.h"
#include "../../Engine/Graphics/Renderer.h"
#include "../../Utils/ResourceManager.h"
#include "../../Math/Vec3.h"

namespace Editor::Components
{
    ViewportScene::ViewportScene()
    {
        // Constructor - initialization done in Initialize()
    }

    ViewportScene::~ViewportScene()
    {
        // Destructor - unique_ptrs will automatically clean up
    }

    void ViewportScene::Initialize()
    {
        // Initialize renderer and camera
        m_Renderer = std::make_unique<::Renderer>();
        m_Camera = std::make_unique<::Camera>(60.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        m_Camera->SetPosition({0, 0, 5});
        m_Camera->SetTarget({0, 0, 0});

        // Create scene and add demo objects
        m_Scene = std::make_unique<::Scene>();
        createDemoScene();
        loadDemoModel();
    }

    bool ViewportScene::IsInitialized() const
    {
        return m_Scene && m_Camera && m_Renderer;
    }

    void ViewportScene::UpdateCameraAspect(int width, int height)
    {
        if (m_Camera && width > 0 && height > 0)
        {
            float aspect = static_cast<float>(width) / static_cast<float>(height);
            m_Camera->SetAspectRatio(aspect);
        }
    }

    void ViewportScene::SetCameraViewportBounds(float x, float y, float width, float height)
    {
        if (m_Camera)
        {
            m_Camera->SetViewportBounds(x, y, width, height);
        }
    }

    void ViewportScene::createDemoScene()
    {
        try
        {
            // Add a cube at the origin
            auto cube = ::SceneObjectFactory::CreateCube("DemoCube", 1.0f);
            cube->GetTransform().SetPosition(::Vec3(0.0f, 0.0f, 0.0f));
            m_Scene->AddObject(cube);

            // Add a sphere to the right
            auto sphere = ::SceneObjectFactory::CreateSphere("DemoSphere", 0.8f);
            sphere->GetTransform().SetPosition(::Vec3(3.0f, 0.0f, 0.0f));
            m_Scene->AddObject(sphere);

            // Add a plane as ground
            auto ground = ::SceneObjectFactory::CreatePlane("Ground", 10.0f, 10.0f);
            ground->GetTransform().SetPosition(::Vec3(0.0f, -2.0f, 0.0f));
            m_Scene->AddObject(ground);
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Failed to create scene objects: " + std::string(e.what()));
        }
    }

    void ViewportScene::loadDemoModel()
    {
        // Try to load pyramid model using ResourceManager
        std::string modelPath = ::ResourceManager::GetResourcePath("Models/pyramid.obj");

        if (!modelPath.empty())
        {
            try
            {
                auto pyramid = ::SceneObjectFactory::LoadFromFile(modelPath, "TestPyramid");
                if (pyramid)
                {
                    pyramid->GetTransform().SetPosition(::Vec3(-3.0f, 1.0f, 0.0f));
                    m_Scene->AddObject(pyramid);
                }
            }
            catch (const std::exception &e)
            {
                Console::PrintWarning("Exception loading pyramid: " + std::string(e.what()));
            }
        }
    }
}
