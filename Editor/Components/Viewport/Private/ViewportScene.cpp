#include "ViewportScene.h"
#include "Console.h"
#include "SceneObject.h"
#include "SceneObjectFactory.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "ResourceManager.h"
#include "Vec3.h"

using Voltray::Engine::SceneObjectFactory;
using Voltray::Utils::ResourceManager;

namespace Voltray::Editor::Components
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
        // Initialize renderer
        m_Renderer = std::make_unique<::Renderer>();

        // Initialize camera - using OnePoint perspective camera as default
        m_Camera = BaseCamera::Create(CameraType::PERSPECTIVE);
        m_Camera->SetPosition({0, 0, 5});
        m_Camera->SetTarget({0, 0, 0});
        m_Camera->SetAspectRatio(16.0f / 9.0f);

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
            auto cube = SceneObjectFactory::CreateCube("DemoCube", 1.0f);
            cube->GetTransform().SetPosition(Vec3(0.0f, 0.0f, 0.0f));
            m_Scene->AddObject(cube);

            // Add a sphere to the right
            auto sphere = SceneObjectFactory::CreateSphere("DemoSphere", 0.8f);
            sphere->GetTransform().SetPosition(Vec3(3.0f, 0.0f, 0.0f));
            m_Scene->AddObject(sphere);

            // Add a plane as ground
            auto ground = SceneObjectFactory::CreatePlane("Ground", 10.0f, 10.0f);
            ground->GetTransform().SetPosition(Vec3(0.0f, -2.0f, 0.0f));
            m_Scene->AddObject(ground);

            // Select the cube by default
            m_Scene->SelectObject(cube);
            Console::Print("Demo scene created with cube, sphere, and ground plane");
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Failed to create demo scene objects: " + std::string(e.what()));
        }
    }
    void ViewportScene::loadDemoModel()
    {
        // Demo model loading removed - models will be loaded via drag and drop from workspace
        Console::Print("Demo model loading disabled - use Assets panel to drag models into the scene");
    }

    void ViewportScene::SwitchCamera(CameraType type)
    {
        if (!m_Camera || m_Camera->GetType() == type)
        {
            return; // Same camera type, no need to switch
        }

        // Store current camera properties
        Vec3 currentPosition = m_Camera->GetPosition();
        Vec3 currentTarget = m_Camera->GetTarget();
        Vec3 currentUp = m_Camera->GetUp();
        float currentAspect = m_Camera->GetAspectRatio();
        float currentNear = m_Camera->GetNearPlane();
        float currentFar = m_Camera->GetFarPlane();

        // Create new camera of the specified type
        m_Camera = BaseCamera::Create(type, "", currentAspect, currentNear, currentFar);

        // Restore camera properties
        m_Camera->SetPosition(currentPosition);
        m_Camera->SetTarget(currentTarget);
        m_Camera->SetUp(currentUp);

        Console::Print("Switched to camera: " + std::string(m_Camera->GetTypeName()));
    }

    void ViewportScene::UpdateCameraSettings(float fov, float orthoSize, float nearPlane, float farPlane)
    {
        if (!m_Camera)
        {
            return;
        }

        // Try to cast to perspective camera for FOV setting
        if (auto *perspectiveCamera = dynamic_cast<PerspectiveCamera *>(m_Camera.get()))
        {
            perspectiveCamera->SetFieldOfView(fov);
            perspectiveCamera->SetClippingPlanes(nearPlane, farPlane);
        }

        // Try to cast to orthographic camera for orthographic size setting
        if (auto *orthographicCamera = dynamic_cast<OrthographicCamera *>(m_Camera.get()))
        {
            orthographicCamera->SetOrthographicSize(orthoSize);
            orthographicCamera->SetClippingPlanes(nearPlane, farPlane);
        }
    }
    CameraType ViewportScene::GetCurrentCameraType() const
    {
        if (m_Camera)
        {
            return m_Camera->GetType();
        }
        return CameraType::PERSPECTIVE; // Default fallback
    }

    bool ViewportScene::SaveScene(const std::string &filepath) const
    {
        if (!m_Scene)
        {
            Console::PrintError("Cannot save scene: Scene not initialized");
            return false;
        }

        return m_Scene->SaveToFile(filepath);
    }

    bool ViewportScene::LoadScene(const std::string &filepath)
    {
        if (!m_Scene)
        {
            Console::PrintError("Cannot load scene: Scene not initialized");
            return false;
        }

        return m_Scene->LoadFromFile(filepath);
    }
}
