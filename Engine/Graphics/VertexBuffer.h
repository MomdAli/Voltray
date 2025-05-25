#pragma once

#include <glad/gl.h>

/**
 * @class VertexBuffer
 * @brief Manages an OpenGL Vertex Buffer Object (VBO).
 *
 * This class encapsulates the creation, binding, and deletion of an OpenGL vertex buffer,
 * which stores vertex data on the GPU for rendering operations.
 */
class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

private:
    GLuint m_ID;
};