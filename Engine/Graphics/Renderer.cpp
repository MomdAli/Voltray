#include "Graphics/Renderer.h"

void Renderer::Draw(const Mesh &mesh, const Shader &shader) const
{
    shader.Bind();
    mesh.Draw();
}
