#pragma once

#include <glad/gl.h>

/**
 * @class IndexBuffer
 * @brief Manages an OpenGL index buffer object (IBO) for efficient rendering of indexed geometry.
 *
 * The IndexBuffer class encapsulates the creation, binding, and management of an OpenGL index buffer.
 * It stores indices used for drawing elements and provides methods to bind and unbind the buffer.
 * It is typically used in conjunction with a Vertex Array Object (VAO) to render complex shapes
 * without duplicating vertex data.
 * @note The indices are expected to be of type unsigned int, and the count represents the number of indices.
 */
class IndexBuffer
{
public:
    IndexBuffer(unsigned int *indices, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return m_Count; }

private:
    GLuint m_ID;
    unsigned int m_Count;
};
