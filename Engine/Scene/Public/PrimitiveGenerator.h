#pragma once

#include "Mesh.h"
#include <memory>

namespace Voltray::Engine
{

    /**
     * @class PrimitiveGenerator
     * @brief Static utility class for generating primitive mesh shapes.
     *
     * PrimitiveGenerator provides methods to create common 3D primitive shapes
     * such as cubes, spheres, planes, cylinders, etc. Each method returns a
     * shared pointer to a Mesh object that can be used with scene objects.
     */
    class PrimitiveGenerator
    {
    public:
        /**
         * @brief Creates a cube mesh.
         * @param size The size of the cube (default: 1.0).
         * @return Shared pointer to the cube mesh.
         */
        static std::shared_ptr<Mesh> CreateCube(float size = 1.0f);

        /**
         * @brief Creates a plane mesh.
         * @param width The width of the plane (default: 1.0).
         * @param height The height of the plane (default: 1.0).
         * @param widthSegments Number of width segments (default: 1).
         * @param heightSegments Number of height segments (default: 1).
         * @return Shared pointer to the plane mesh.
         */
        static std::shared_ptr<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f,
                                                 int widthSegments = 1, int heightSegments = 1);

        /**
         * @brief Creates a sphere mesh.
         * @param radius The radius of the sphere (default: 0.5).
         * @param widthSegments Number of horizontal segments (default: 32).
         * @param heightSegments Number of vertical segments (default: 16).
         * @return Shared pointer to the sphere mesh.
         */
        static std::shared_ptr<Mesh> CreateSphere(float radius = 0.5f, int widthSegments = 32, int heightSegments = 16);

        /**
         * @brief Creates a cylinder mesh.
         * @param radiusTop The radius at the top (default: 0.5).
         * @param radiusBottom The radius at the bottom (default: 0.5).
         * @param height The height of the cylinder (default: 1.0).
         * @param radialSegments Number of radial segments (default: 32).
         * @param heightSegments Number of height segments (default: 1).
         * @return Shared pointer to the cylinder mesh.
         */
        static std::shared_ptr<Mesh> CreateCylinder(float radiusTop = 0.5f, float radiusBottom = 0.5f,
                                                    float height = 1.0f, int radialSegments = 32, int heightSegments = 1);

        /**
         * @brief Creates a triangle mesh.
         * @param size The size of the triangle (default: 1.0).
         * @return Shared pointer to the triangle mesh.
         */
        static std::shared_ptr<Mesh> CreateTriangle(float size = 1.0f);

    private: // Helper methods
        static void AddVertex(std::vector<float> &vertices, float x, float y, float z, float nx, float ny, float nz, float u, float v);
        static void AddTriangle(std::vector<unsigned int> &indices, unsigned int a, unsigned int b, unsigned int c);
    };

} // namespace Voltray::Engine
