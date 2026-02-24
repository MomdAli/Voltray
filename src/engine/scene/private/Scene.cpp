#include "Scene.h"
#include "SceneObjectFactory.h"
#include "Console.h"
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <limits>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using Voltray::Editor::Components::Console;

namespace Voltray::Engine
{

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        Clear();
    }

    SceneObject &Scene::AddObject(std::shared_ptr<SceneObject> object)
    {
        m_Objects.push_back(object);
        return *object;
    }

    SceneObject &Scene::AddObject(std::shared_ptr<Mesh> mesh, const std::string &name)
    {
        auto object = std::make_shared<SceneObject>(mesh, name);
        m_Objects.push_back(object);
        return *object;
    }

    bool Scene::RemoveObject(const std::string &name)
    {
        auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                               [&name](const std::shared_ptr<SceneObject> &obj)
                               {
                                   return obj->GetName() == name;
                               });

        if (it != m_Objects.end())
        {
            m_Objects.erase(it);
            return true;
        }
        return false;
    }

    bool Scene::RemoveObject(std::shared_ptr<SceneObject> object)
    {
        auto it = std::find(m_Objects.begin(), m_Objects.end(), object);
        if (it != m_Objects.end())
        {
            m_Objects.erase(it);
            return true;
        }
        return false;
    }

    std::shared_ptr<SceneObject> Scene::FindObject(const std::string &name) const
    {
        auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                               [&name](const std::shared_ptr<SceneObject> &obj)
                               {
                                   return obj->GetName() == name;
                               });

        return (it != m_Objects.end()) ? *it : nullptr;
    }

    void Scene::Clear()
    {
        m_Objects.clear();
    }

    void Scene::Update(float deltaTime)
    {
        for (auto &object : m_Objects)
        {
            if (object)
            {
                object->Update(deltaTime);
            }
        }
    }

    void Scene::Render(Renderer &renderer, const BaseCamera &camera, Shader &shader)
    {
        // Set the camera view-projection matrix for all objects
        shader.Bind();
        shader.SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix().data);

        for (auto &object : m_Objects)
        {
            if (object && object->IsVisible() && object->GetMesh())
            {
                // Call the object's pre-render hook
                object->OnRender();

                // Get the model matrix and render the object
                Mat4 modelMatrix = object->GetModelMatrix();
                shader.SetUniformMat4("u_Model", modelMatrix.data);

                renderer.Draw(*object->GetMesh(), shader, modelMatrix);
            }
        }
    }

    void Scene::SelectObject(std::shared_ptr<SceneObject> object)
    {
        // Clear all selections first
        ClearSelection();

        // Select the specified object if it exists in the scene
        if (object && std::find(m_Objects.begin(), m_Objects.end(), object) != m_Objects.end())
        {
            object->SetSelected(true);
        }
    }

    void Scene::ClearSelection()
    {
        for (auto &obj : m_Objects)
        {
            if (obj)
            {
                obj->SetSelected(false);
            }
        }
    }
    std::shared_ptr<SceneObject> Scene::GetSelectedObject() const
    {
        for (auto &obj : m_Objects)
        {
            if (obj && obj->IsSelected())
            {
                return obj;
            }
        }
        return nullptr;
    }
    bool Scene::SaveToFile(const std::string &filepath) const
    {
        try
        {
            json root;
            root["version"] = "1.0";
            root["objects"] = json::array();

            // Serialize each object
            for (const auto &obj : m_Objects)
            {
                if (!obj)
                    continue;

                json objJson;
                objJson["name"] = obj->GetName();
                objJson["visible"] = obj->IsVisible();
                objJson["selected"] = obj->IsSelected(); // Transform data
                const auto &transform = obj->GetTransform();
                json transformJson;

                const auto &pos = transform.GetPosition();
                transformJson["position"]["x"] = pos.x;
                transformJson["position"]["y"] = pos.y;
                transformJson["position"]["z"] = pos.z;
                const auto &rot = transform.GetRotation();
                transformJson["rotation"]["x"] = rot.x;
                transformJson["rotation"]["y"] = rot.y;
                transformJson["rotation"]["z"] = rot.z;

                const auto &scale = transform.GetScale();
                transformJson["scale"]["x"] = scale.x;
                transformJson["scale"]["y"] = scale.y;
                transformJson["scale"]["z"] = scale.z;

                objJson["transform"] = transformJson; // Material color
                const auto &color = obj->GetMaterialColor();
                objJson["materialColor"]["r"] = color.x;
                objJson["materialColor"]["g"] = color.y;
                objJson["materialColor"]["b"] = color.z; // Store mesh file path if it was loaded from a file
                objJson["meshFilePath"] = obj->GetMeshFilePath();

                root["objects"].push_back(objJson);
            }

            // Create directory if it doesn't exist
            std::filesystem::path scenePath(filepath);
            std::filesystem::create_directories(scenePath.parent_path()); // Write to file
            std::ofstream file(filepath);
            if (!file.is_open())
            {
                Console::PrintError("Failed to open scene file for writing: " + filepath);
                return false;
            }

            file << root.dump(2); // Pretty print with 2 space indentation
            file.close();

            Console::Print("Scene saved to: " + filepath);
            return true;
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Error saving scene: " + std::string(e.what()));
            return false;
        }
    }

    bool Scene::LoadFromFile(const std::string &filepath)
    {
        try
        {
            if (!std::filesystem::exists(filepath))
            {
                Console::PrintError("Scene file not found: " + filepath);
                return false;
            }
            std::ifstream file(filepath);
            if (!file.is_open())
            {
                Console::PrintError("Failed to open scene file for reading: " + filepath);
                return false;
            }

            json root;
            try
            {
                file >> root;
            }
            catch (const json::parse_error &e)
            {
                Console::PrintError("Failed to parse scene file: " + std::string(e.what()));
                return false;
            }

            // Clear current scene
            Clear();

            // Load objects
            if (root.contains("objects") && root["objects"].is_array())
            {
                for (const auto &objJson : root["objects"])
                {
                    if (!objJson.contains("name"))
                        continue;

                    std::string objName = objJson["name"].get<std::string>();
                    std::shared_ptr<SceneObject> sceneObject;

                    // Try to load from original mesh file if available
                    if (objJson.contains("meshFilePath") && !objJson["meshFilePath"].get<std::string>().empty())
                    {
                        std::string meshFilePath = objJson["meshFilePath"].get<std::string>();

                        // Check if the mesh file still exists
                        if (std::filesystem::exists(meshFilePath))
                        {
                            Console::Print("Loading mesh from file: " + meshFilePath);
                            sceneObject = SceneObjectFactory::LoadFromFile(meshFilePath, objName);
                        }
                        else
                        {
                            Console::PrintWarning("Mesh file not found: " + meshFilePath + ", creating placeholder cube");
                            sceneObject = SceneObjectFactory::CreateCube(objName, 1.0f);
                        }
                    }
                    else
                    {
                        // No mesh file path, create a basic cube
                        Console::Print("No mesh file path found for object: " + objName + ", creating cube");
                        sceneObject = SceneObjectFactory::CreateCube(objName, 1.0f);
                    }

                    if (sceneObject)
                    {
                        // Apply transform
                        if (objJson.contains("transform"))
                        {
                            const auto &transformJson = objJson["transform"];
                            auto &transform = sceneObject->GetTransform();

                            if (transformJson.contains("position"))
                            {
                                const auto &pos = transformJson["position"];
                                transform.SetPosition(Vec3(
                                    pos["x"].get<float>(),
                                    pos["y"].get<float>(),
                                    pos["z"].get<float>()));
                            }
                            if (transformJson.contains("rotation"))
                            {
                                const auto &rot = transformJson["rotation"];
                                transform.SetRotation(Vec3(
                                    rot["x"].get<float>(),
                                    rot["y"].get<float>(),
                                    rot["z"].get<float>()));
                            }

                            if (transformJson.contains("scale"))
                            {
                                const auto &scale = transformJson["scale"];
                                transform.SetScale(Vec3(
                                    scale["x"].get<float>(),
                                    scale["y"].get<float>(),
                                    scale["z"].get<float>()));
                            }
                        }

                        // Apply material color
                        if (objJson.contains("materialColor"))
                        {
                            const auto &color = objJson["materialColor"];
                            sceneObject->SetMaterialColor(Vec3(
                                color["r"].get<float>(),
                                color["g"].get<float>(),
                                color["b"].get<float>()));
                        }

                        // Apply visibility
                        if (objJson.contains("visible"))
                        {
                            sceneObject->SetVisible(objJson["visible"].get<bool>());
                        }

                        // Add to scene
                        AddObject(sceneObject);

                        // Apply selection
                        if (objJson.contains("selected") && objJson["selected"].get<bool>())
                        {
                            SelectObject(sceneObject);
                        }
                    }
                }
            }

            Console::Print("Scene loaded from: " + filepath);
            return true;
        }
        catch (const std::exception &e)
        {
            Console::PrintError("Error loading scene: " + std::string(e.what()));
            return false;
        }
    }

    std::shared_ptr<SceneObject> Scene::RaycastToObject(const Ray &ray) const
    {
        std::shared_ptr<SceneObject> closestObject = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        // Iterate through all objects in the scene
        for (const auto& obj : m_Objects)
        {
            if (!obj)
                continue;

            // First, perform AABB intersection test for early rejection
            Vec3 minBounds, maxBounds;
            obj->GetWorldBounds(minBounds, maxBounds);

            float aabbDistance;
            if (!ray.IntersectAABB(minBounds, maxBounds, aabbDistance))
            {
                // Ray doesn't intersect the AABB, skip expensive mesh intersection
                continue;
            }

            // AABB test passed, now do detailed mesh intersection
            float intersectionDistance = 0.0f;
            auto mesh = obj->GetMesh();
            if (mesh)
            {
                // Get the object's model matrix (local to world transformation)
                Mat4 modelMatrix = obj->GetModelMatrix();
                if (ray.IntersectMesh(mesh->GetVertices(), mesh->GetIndices(), modelMatrix, intersectionDistance))
                {
                    // Check if this is the closest intersection so far
                    if (intersectionDistance < closestDistance)
                    {
                        closestDistance = intersectionDistance;
                        closestObject = obj;
                    }
                }
            }
            else
            {
                // No mesh available, fall back to AABB intersection distance
                if (aabbDistance < closestDistance)
                {
                    closestDistance = aabbDistance;
                    closestObject = obj;
                }
            }
        }

        return closestObject;
    }
}
