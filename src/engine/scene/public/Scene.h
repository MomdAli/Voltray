#pragma once

#include "SceneObject.h"
#include "Renderer.h"
#include "BaseCamera.h"
#include <vector>
#include <memory>
#include <string>

namespace Voltray::Engine
{
    /**
     * @class Scene
     * @brief Manages a collection of scene objects and provides rendering functionality.
     *
     * The Scene class is responsible for managing all objects in the 3D scene,
     * providing methods to add, remove, and update objects. It also handles
     * rendering all visible objects in the scene.
     */
    class Scene
    {
    public:
        /**
         * @brief Constructor.
         */
        Scene();

        /**
         * @brief Destructor.
         */
        ~Scene();

        /**
         * @brief Adds a scene object to the scene.
         * @param object Shared pointer to the scene object.
         * @return Reference to the added object.
         */
        SceneObject &AddObject(std::shared_ptr<SceneObject> object);

        /**
         * @brief Creates and adds a scene object with a mesh.
         * @param mesh Shared pointer to the mesh.
         * @param name Name of the object.
         * @return Reference to the created object.
         */
        SceneObject &AddObject(std::shared_ptr<Mesh> mesh, const std::string &name = "Object");

        /**
         * @brief Removes a scene object by name.
         * @param name Name of the object to remove.
         * @return True if object was found and removed, false otherwise.
         */
        bool RemoveObject(const std::string &name);

        /**
         * @brief Removes a scene object by pointer.
         * @param object Shared pointer to the object to remove.
         * @return True if object was found and removed, false otherwise.
         */
        bool RemoveObject(std::shared_ptr<SceneObject> object);

        /**
         * @brief Finds a scene object by name.
         * @param name Name of the object to find.
         * @return Shared pointer to the object, or nullptr if not found.
         */
        std::shared_ptr<SceneObject> FindObject(const std::string &name) const;

        /**
         * @brief Gets all scene objects.
         * @return Vector of shared pointers to all objects.
         */
        const std::vector<std::shared_ptr<SceneObject>> &GetObjects() const { return m_Objects; }

        /**
         * @brief Clears all objects from the scene.
         */
        void Clear();

        /**
         * @brief Updates all objects in the scene.
         * @param deltaTime Time since last update in seconds.
         */
        void Update(float deltaTime);

        /**
         * @brief Renders all visible objects in the scene.
         * @param renderer Reference to the renderer.
         * @param camera Reference to the camera.
         * @param shader Reference to the shader.
         */
        void Render(Renderer &renderer, const BaseCamera &camera, Shader &shader); /**
                                                                                    * @brief Gets the number of objects in the scene.
                                                                                    * @return Number of objects.
                                                                                    */
        size_t GetObjectCount() const { return m_Objects.size(); }

        /**
         * @brief Selects an object by pointer and clears other selections.
         * @param object Shared pointer to the object to select.
         */
        void SelectObject(std::shared_ptr<SceneObject> object);

        /**
         * @brief Clears all object selections.    */
        void ClearSelection(); /**
                                * @brief Gets the currently selected object.
                                * @return Shared pointer to selected object, or nullptr if none.
                                */
        std::shared_ptr<SceneObject> GetSelectedObject() const;

        /**
         * @brief Save the scene to a file
         * @param filepath Path to save the scene file
         * @return True if successful, false otherwise
         */
        bool SaveToFile(const std::string &filepath) const;

        /**
         * @brief Load the scene from a file
         * @param filepath Path to the scene file
         * @return True if successful, false otherwise
         */
        bool LoadFromFile(const std::string &filepath);

        /**
         * @brief Casts a ray into the scene and returns the first object hit.
         * @param ray The ray to cast.
         * @return Shared pointer to the first object hit, or nullptr if no object was hit.
         */
        std::shared_ptr<SceneObject> RaycastToObject(const Ray &ray) const;

    private:
        std::vector<std::shared_ptr<SceneObject>> m_Objects;
    };
}
