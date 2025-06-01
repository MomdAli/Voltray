#pragma once

#include "../Math/Transform.h"
#include "../Math/Vec3.h"
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
    virtual ~SceneObject() = default; // Transform operations
    Transform &GetTransform() { return m_Transform; }
    const Transform &GetTransform() const { return m_Transform; }

    /**
     * @brief Sets the relative pivot point where (0,0,0) represents the mesh center.
     * @param relativePivot The pivot offset from mesh center.
     */
    void SetRelativePivot(const Vec3 &relativePivot);

    /**
     * @brief Gets the relative pivot point where (0,0,0) represents the mesh center.
     * @return The relative pivot offset from mesh center.
     */
    Vec3 GetRelativePivot() const; // Mesh operations
    void SetMesh(std::shared_ptr<Mesh> mesh)
    {
        m_Mesh = mesh;
        UpdatePivotFromMesh();
    }
    std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }

    // Properties
    const std::string &GetName() const { return m_Name; }
    void SetName(const std::string &name) { m_Name = name; }
    bool IsVisible() const { return m_Visible; }
    void SetVisible(bool visible) { m_Visible = visible; }
    bool IsSelected() const { return m_Selected; }
    void SetSelected(bool selected) { m_Selected = selected; }

    // Material properties
    const Vec3 &GetMaterialColor() const { return m_MaterialColor; }
    void SetMaterialColor(const Vec3 &color) { m_MaterialColor = color; }

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
    virtual void Update(float deltaTime) { (void)deltaTime; }

    /**
     * @brief Virtual method called before rendering - can be overridden.
     */
    virtual void OnRender() {}

private:
    /**
     * @brief Updates the transform pivot based on mesh center.
     */
    void UpdatePivotFromMesh();

protected:
    std::string m_Name;
    Transform m_Transform;
    std::shared_ptr<Mesh> m_Mesh;
    bool m_Visible = true;
    bool m_Selected = false;
    // Default material color is white
    Vec3 m_MaterialColor{1.0f, 1.0f, 1.0f};
    // Store relative pivot offset from mesh center (0,0,0 = center)
    Vec3 m_RelativePivot{0.0f, 0.0f, 0.0f};
};
