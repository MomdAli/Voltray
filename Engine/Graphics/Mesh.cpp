#include "Graphics/Mesh.h"

Mesh::Mesh(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iCount)
    : m_VBO(vertices, vSize), m_IBO(indices, iCount)
{
    m_VAO.Bind();
    m_VBO.Bind();
    m_IBO.Bind();
    m_VAO.AddVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
}

Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
    : m_VBO(const_cast<float *>(vertices.data()), vertices.size() * sizeof(float)),
      m_IBO(const_cast<unsigned int *>(indices.data()), indices.size())
{
    m_VAO.Bind();
    m_VBO.Bind();
    m_IBO.Bind();

    // Vertex layout: position (3) + normal (3) + texcoord (2) = 8 floats per vertex
    const unsigned int stride = 8 * sizeof(float);

    // Position attribute (location 0)
    m_VAO.AddVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);

    // Normal attribute (location 1)
    m_VAO.AddVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));

    // Texture coordinate attribute (location 2)
    m_VAO.AddVertexAttribute(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(6 * sizeof(float)));
}

Mesh::~Mesh()
{
    // VertexArray and VertexBuffer will be automatically cleaned up by their destructors
}

void Mesh::Draw() const
{
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_IBO.GetCount(), GL_UNSIGNED_INT, nullptr);
}