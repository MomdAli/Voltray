#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vec3.h"
#include <vector>

namespace Voltray::Engine
{

    /**
     * @brief Represents a 3D mesh composed of vertices and indices.
     *
     * The Mesh class encapsulates the vertex data, index data, and the
     * associated OpenGL objects (Vertex Array Object, Vertex Buffer Object,
     * and Index Buffer Object) required to render a 3D model.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructs a Mesh object from raw C-style arrays.
         * @param vertices Pointer to the array of vertex data (e.g., positions, normals, texture coordinates).
         * @param vSize Total size of the vertex data array in bytes.
         * @param indices Pointer to the array of indices that define the triangles.
         * @param iCount Total number of indices.
         */
        Mesh(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iCount);

        /**
         * @brief Constructs a Mesh object from C++ std::vector containers.
         * @param vertices A constant reference to a vector of floats representing vertex data.
         * @param indices A constant reference to a vector of unsigned integers representing index data.
         */
        Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices);

        /**
         * @brief Destroys the Mesh object.
         *
         * This destructor will handle the cleanup of OpenGL resources
         * if the underlying VertexArray, VertexBuffer, and IndexBuffer
         * classes manage their own resource deallocation.
         */
        ~Mesh();           /**
                            * @brief Renders the mesh.
                            *
                            * This method binds the associated Vertex Array Object and issues a draw call
                            * using the Index Buffer Object.
                            */
        void Draw() const; /**
                            * @brief Gets the axis-aligned bounding box of the mesh.
                            * @param minBounds Output minimum bounds of the mesh.
                            * @param maxBounds Output maximum bounds of the mesh.
                            */
        void GetBounds(Voltray::Math::Vec3 &minBounds, Voltray::Math::Vec3 &maxBounds) const;

        /**
         * @brief Gets the center point of the mesh bounds.
         * @return The center point of the mesh.
         */
        Voltray::Math::Vec3 GetCenter() const;

        /**
         * @brief Gets access to the vertex data for intersection testing.
         * @return Const reference to the vertex data vector.
         */
        const std::vector<float> &GetVertices() const { return m_Vertices; }

        /**
         * @brief Gets access to the index data for intersection testing.
         * @return Const reference to the index data vector.
         */
        const std::vector<unsigned int> &GetIndices() const { return m_Indices; }

    private:
        VertexArray m_VAO;                   ///< Vertex Array Object managing the vertex attribute configurations.
        VertexBuffer m_VBO;                  ///< Vertex Buffer Object storing the vertex data.
        IndexBuffer m_IBO;                   ///< Index Buffer Object storing the index data.    std::vector<float> m_Vertices;           ///< Copy of vertex data for bounds calculation
        std::vector<float> m_Vertices;       ///< Copy of vertex data for bounds calculation
        std::vector<unsigned int> m_Indices; ///< Copy of index data for intersection testing

        mutable Voltray::Math::Vec3 m_MinBounds, m_MaxBounds; ///< Cached bounding box
        mutable bool m_BoundsCalculated = false;              ///< Whether bounds have been calculated
    };

}