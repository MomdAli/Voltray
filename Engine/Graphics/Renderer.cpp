#include "Graphics/Renderer.h"

void Renderer::Draw(const Mesh &mesh, const Shader &shader) const
{
    shader.Bind();
    mesh.Draw();
}

void Renderer::Draw(const Mesh &mesh, const Shader &shader, const Mat4 &modelMatrix) const
{
    shader.Bind();
    shader.SetUniformMat4("u_Model", modelMatrix.data);
    mesh.Draw();
}
