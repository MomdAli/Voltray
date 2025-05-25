#pragma once

#include "Shader.h"
#include "Mesh.h"

/**
 * @class Renderer
 * @brief Responsible for rendering mesh objects using specified shaders.
 *
 * The Renderer class provides functionality to draw mesh objects
 * with a given shader. It encapsulates the rendering logic and
 * serves as an interface for graphics rendering operations.
 *
 * @note This class assumes that the graphics context and resources
 *       (such as meshes and shaders) are properly initialized and managed.
 */
class Renderer
{
public:
    void Draw(const Mesh &mesh, const Shader &shader) const;
};
