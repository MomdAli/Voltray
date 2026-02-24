#include "VertexArray.h"

namespace Voltray::Engine
{

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_ID);
        glBindVertexArray(m_ID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
    {
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
        glEnableVertexAttribArray(index);
    }

} // namespace Voltray::Engine
