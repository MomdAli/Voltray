#include "SceneObject.h"

SceneObject::SceneObject(const std::string &name)
    : m_Name(name), m_Transform(), m_Mesh(nullptr), m_Visible(true)
{
}

SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, const std::string &name)
    : m_Name(name), m_Transform(), m_Mesh(mesh), m_Visible(true)
{
}
