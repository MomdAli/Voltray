#pragma once

#include <glad/gl.h>

/**
 * @class VertexArray
 * @brief Encapsulates an OpenGL Vertex Array Object (VAO).
 *
 * The VertexArray class manages the creation, binding, and deletion of an OpenGL VAO.
 * It provides methods to bind/unbind the VAO and to add vertex attribute pointers.
 */
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

private:
    GLuint m_ID;
};
