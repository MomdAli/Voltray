#include "Graphics/Mesh.h"

Mesh::Mesh(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iCount)
    : m_VBO(vertices, vSize), m_IBO(indices, iCount)
{
    m_VAO.Bind();
    m_VBO.Bind();
    m_IBO.Bind();
    m_VAO.AddVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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