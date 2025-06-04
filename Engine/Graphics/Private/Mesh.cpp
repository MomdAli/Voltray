#include "Mesh.h"
#include <algorithm>
#include <limits>

namespace Voltray::Engine
{

    Mesh::Mesh(float *vertices, unsigned int vSize, unsigned int *indices, unsigned int iCount)
        : m_VBO(vertices, vSize), m_IBO(indices, iCount)
    {
        // Store vertex data
        m_Vertices.assign(vertices, vertices + (vSize / sizeof(float)));

        // Store index data
        m_Indices.assign(indices, indices + iCount);

        m_VAO.Bind();
        m_VBO.Bind();
        m_IBO.Bind();
        m_VAO.AddVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    }

    Mesh::Mesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices)
        : m_VBO(const_cast<float *>(vertices.data()), vertices.size() * sizeof(float)),
          m_IBO(const_cast<unsigned int *>(indices.data()), indices.size()),
          m_Vertices(vertices), m_Indices(indices)
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

    void Mesh::GetBounds(Voltray::Math::Vec3 &minBounds, Voltray::Math::Vec3 &maxBounds) const
    {
        if (m_BoundsCalculated)
        {
            minBounds = m_MinBounds;
            maxBounds = m_MaxBounds;
            return;
        }

        if (m_Vertices.empty())
        {
            minBounds = Voltray::Math::Vec3(-0.5f, -0.5f, -0.5f);
            maxBounds = Voltray::Math::Vec3(0.5f, 0.5f, 0.5f);
            return;
        }

        // Initialize bounds to first vertex position
        m_MinBounds = Voltray::Math::Vec3(m_Vertices[0], m_Vertices[1], m_Vertices[2]);
        m_MaxBounds = m_MinBounds;

        // Determine vertex stride - check if we have 8 floats per vertex (pos + normal + texcoord)
        // or just 3 floats per vertex (position only)
        int stride = 8; // Default to full vertex layout
        if (m_Vertices.size() % 8 != 0 && m_Vertices.size() % 3 == 0)
        {
            stride = 3; // Simple position-only vertices
        }

        // Iterate through all vertex positions
        for (size_t i = 0; i < m_Vertices.size(); i += stride)
        {
            if (i + 2 < m_Vertices.size()) // Ensure we have at least x, y, z
            {
                Voltray::Math::Vec3 position(m_Vertices[i], m_Vertices[i + 1], m_Vertices[i + 2]);

                m_MinBounds.x = std::min(m_MinBounds.x, position.x);
                m_MinBounds.y = std::min(m_MinBounds.y, position.y);
                m_MinBounds.z = std::min(m_MinBounds.z, position.z);

                m_MaxBounds.x = std::max(m_MaxBounds.x, position.x);
                m_MaxBounds.y = std::max(m_MaxBounds.y, position.y);
                m_MaxBounds.z = std::max(m_MaxBounds.z, position.z);
            }
        }

        m_BoundsCalculated = true;
        minBounds = m_MinBounds;
        maxBounds = m_MaxBounds;
    }

    Voltray::Math::Vec3 Mesh::GetCenter() const
    {
        Voltray::Math::Vec3 minBounds, maxBounds;
        GetBounds(minBounds, maxBounds);
        return (minBounds + maxBounds) * 0.5f;
    }

}