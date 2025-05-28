#include <limits>
#include <algorithm>

#include "SceneObject.h"
#include "../Math/Vec3.h"

SceneObject::SceneObject(const std::string &name)
    : m_Name(name), m_Transform(), m_Mesh(nullptr), m_Visible(true)
{
}

SceneObject::SceneObject(std::shared_ptr<Mesh> mesh, const std::string &name)
    : m_Name(name), m_Transform(), m_Mesh(mesh), m_Visible(true)
{
    UpdatePivotFromMesh();
}

void SceneObject::GetWorldBounds(Vec3 &minBounds, Vec3 &maxBounds) const
{
    if (!m_Mesh)
    {
        // If no mesh, create a small bounding box around the object position
        Vec3 position = m_Transform.GetPosition();
        minBounds = position - Vec3(0.1f, 0.1f, 0.1f);
        maxBounds = position + Vec3(0.1f, 0.1f, 0.1f);
        return;
    }

    // Get mesh bounds and transform them to world space
    Vec3 meshMin, meshMax;
    m_Mesh->GetBounds(meshMin, meshMax);

    // Transform the bounding box corners to world space
    Mat4 modelMatrix = GetModelMatrix();

    // Transform all 8 corners of the bounding box
    Vec3 corners[8] = {
        Vec3(meshMin.x, meshMin.y, meshMin.z),
        Vec3(meshMax.x, meshMin.y, meshMin.z),
        Vec3(meshMin.x, meshMax.y, meshMin.z),
        Vec3(meshMax.x, meshMax.y, meshMin.z),
        Vec3(meshMin.x, meshMin.y, meshMax.z),
        Vec3(meshMax.x, meshMin.y, meshMax.z),
        Vec3(meshMin.x, meshMax.y, meshMax.z),
        Vec3(meshMax.x, meshMax.y, meshMax.z)};

    // Transform corners and find min/max
    minBounds = Vec3(
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max());
    maxBounds = Vec3(
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest(),
        std::numeric_limits<float>::lowest());

    for (int i = 0; i < 8; ++i)
    {
        Vec3 transformedCorner = modelMatrix.MultiplyVec3(corners[i]);

        minBounds.x = std::min(minBounds.x, transformedCorner.x);
        minBounds.y = std::min(minBounds.y, transformedCorner.y);
        minBounds.z = std::min(minBounds.z, transformedCorner.z);

        maxBounds.x = std::max(maxBounds.x, transformedCorner.x);
        maxBounds.y = std::max(maxBounds.y, transformedCorner.y);
        maxBounds.z = std::max(maxBounds.z, transformedCorner.z);
    }
}

void SceneObject::UpdatePivotFromMesh()
{
    if (m_Mesh)
    {
        Vec3 meshCenter = m_Mesh->GetCenter();
        m_Transform.SetRelativePivot(meshCenter, m_RelativePivot);
    }
}

void SceneObject::SetRelativePivot(const Vec3 &relativePivot)
{
    m_RelativePivot = relativePivot;
    UpdatePivotFromMesh();
}

Vec3 SceneObject::GetRelativePivot() const
{
    return m_RelativePivot;
}
