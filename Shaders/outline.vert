#version 460 core

layout(location = 0) in vec3 aPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    // Simple vertex transformation for wireframe outline
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    gl_Position = u_ViewProjection * worldPos;
}
