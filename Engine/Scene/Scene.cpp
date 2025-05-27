#include "Scene.h"
#include <algorithm>

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

void Scene::Render(Renderer &renderer, const Camera &camera, Shader &shader)
{
    for (auto &object : m_Objects)
    {
        if (object && object->IsVisible() && object->GetMesh())
        {
            // Call the object's pre-render hook
            object->OnRender();

            // Get the model matrix and render the object
            Mat4 modelMatrix = object->GetModelMatrix();
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
