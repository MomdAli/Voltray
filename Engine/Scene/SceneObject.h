#pragma once

#include "../Math/Transform.h"
#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"
#include <memory>
#include <string>

/**
 * @class SceneObject
 * @brief Base class for objects in the 3D scene.
 *
 * SceneObject represents any object that can be placed in the 3D scene.
 * It contains a transform for positioning, rotation, and scaling, as well as
 * a mesh for rendering. This serves as the base class for all scene objects.
 */
class SceneObject
{
public:
    /**
     * @brief Constructor with name.
     * @param name The name of the scene object.
     */
    explicit SceneObject(const std::string &name = "SceneObject");

    /**
     * @brief Constructor with mesh.
     * @param mesh Shared pointer to the mesh.
     * @param name The name of the scene object.
     */
    SceneObject(std::shared_ptr<Mesh> mesh, const std::string &name = "SceneObject");

    /**
     * @brief Virtual destructor.
     */
    virtual ~SceneObject() = default;

    // Transform operations
    Transform &GetTransform() { return m_Transform; }
    const Transform &GetTransform() const { return m_Transform; }

    // Mesh operations
    void SetMesh(std::shared_ptr<Mesh> mesh) { m_Mesh = mesh; }
    std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

    // Properties
    const std::string &GetName() const { return m_Name; }
    void SetName(const std::string &name) { m_Name = name; }
    bool IsVisible() const { return m_Visible; }
    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsSelected() const { return m_Selected; }
    void SetSelected(bool selected) { m_Selected = selected; }

    /**
     * @brief Gets the model matrix for rendering.
     * @return The transformation matrix.
     */
    Mat4 GetModelMatrix() const { return m_Transform.GetMatrix(); }

    /**
     * @brief Gets the axis-aligned bounding box in world space.
     * @param minBounds Output minimum bounds of the bounding box.
     * @param maxBounds Output maximum bounds of the bounding box.
     */
    void GetWorldBounds(Vec3 &minBounds, Vec3 &maxBounds) const;

    /**
     * @brief Virtual update method - can be overridden for custom behavior.
     * @param deltaTime Time since last update in seconds.
     */
    virtual void Update(float deltaTime) {}

    /**
     * @brief Virtual method called before rendering - can be overridden.
     */
    virtual void OnRender() {}

protected:
    std::string m_Name;
    Transform m_Transform;
    std::shared_ptr<Mesh> m_Mesh;
    bool m_Visible = true;
    bool m_Selected = false;
};
