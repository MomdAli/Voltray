#pragma once

#include "SceneObject.h"
#include "PrimitiveGenerator.h"
#include <memory>
#include <string>

namespace Voltray::Engine
{

    /**
     * @class SceneObjectFactory
     * @brief Factory class for creating common scene objects with primitive meshes.
     *
     * SceneObjectFactory provides convenient methods to create scene objects
     * with pre-generated primitive meshes, making it easy to add common shapes
     * to the scene without manually creating meshes and objects.
     */
    class SceneObjectFactory
    {
    public:
        /**
         * @brief Creates a cube scene object.
         * @param name Name of the object.
         * @param size Size of the cube.
         * @return Shared pointer to the created scene object.
         */
        static std::shared_ptr<SceneObject> CreateCube(const std::string &name = "Cube", float size = 1.0f);

        /**
         * @brief Creates a sphere scene object.
         * @param name Name of the object.
         * @param radius Radius of the sphere.
         * @param widthSegments Number of horizontal segments.
         * @param heightSegments Number of vertical segments.
         * @return Shared pointer to the created scene object.
         */
        static std::shared_ptr<SceneObject> CreateSphere(const std::string &name = "Sphere", float radius = 0.5f,
                                                         int widthSegments = 32, int heightSegments = 16);

        /**
         * @brief Creates a plane scene object.
         * @param name Name of the object.
         * @param width Width of the plane.
         * @param height Height of the plane.
         * @param widthSegments Number of width segments.
         * @param heightSegments Number of height segments.
         * @return Shared pointer to the created scene object.
         */
        static std::shared_ptr<SceneObject> CreatePlane(const std::string &name = "Plane", float width = 1.0f,
                                                        float height = 1.0f, int widthSegments = 1, int heightSegments = 1);

        /**
         * @brief Creates a cylinder scene object.
         * @param name Name of the object.
         * @param radiusTop Radius at the top.
         * @param radiusBottom Radius at the bottom.
         * @param height Height of the cylinder.
         * @param radialSegments Number of radial segments.
         * @param heightSegments Number of height segments.
         * @return Shared pointer to the created scene object.
         */
        static std::shared_ptr<SceneObject> CreateCylinder(const std::string &name = "Cylinder", float radiusTop = 0.5f,
                                                           float radiusBottom = 0.5f, float height = 1.0f,
                                                           int radialSegments = 32, int heightSegments = 1); /**
                                                                                                              * @brief Creates a triangle scene object.
                                                                                                              * @param name Name of the object.
                                                                                                              * @param size Size of the triangle.
                                                                                                              * @return Shared pointer to the created scene object.
                                                                                                              */
        static std::shared_ptr<SceneObject> CreateTriangle(const std::string &name = "Triangle", float size = 1.0f);

        /**
         * @brief Load a scene object from an external mesh file (OBJ, FBX, etc.).
         * @param filepath Path to the mesh file.
         * @param name Name of the object (defaults to filename if empty).
         * @return Shared pointer to the created scene object, or nullptr if failed.
         */
        static std::shared_ptr<SceneObject> LoadFromFile(const std::string &filepath, const std::string &name = "");

        /**
         * @brief Load all meshes from a file as separate scene objects.
         * @param filepath Path to the mesh file.
         * @return Vector of shared pointers to all created scene objects.
         */
        static std::vector<std::shared_ptr<SceneObject>> LoadAllFromFile(const std::string &filepath);

        /**
         * @brief Create a scene object from an existing mesh.
         * @param mesh Shared pointer to the mesh.
         * @param name Name of the object.
         * @return Shared pointer to the created scene object.     */
        static std::shared_ptr<SceneObject> CreateFromMesh(std::shared_ptr<Mesh> mesh, const std::string &name = "CustomMesh");
    };

}
