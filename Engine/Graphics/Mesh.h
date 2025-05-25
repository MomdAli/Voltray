#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/**
 * @class Mesh
 * @brief Represents a 3D mesh consisting of vertex and index data for rendering.
 *
 * The Mesh class encapsulates the creation, management, and rendering of a mesh object.
 * It manages the underlying vertex array, vertex buffer, and index buffer objects.
 */
class Mesh
{
public:
    Mesh(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iCount);
    ~Mesh();

    void Draw() const;

private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;
};
