#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_OutlineThickness;

void main() {
    // Expand vertices along their normals to create outline effect
    vec3 expandedPos = aPos + aNormal * u_OutlineThickness;
    vec4 worldPos = u_Model * vec4(expandedPos, 1.0);

    gl_Position = u_ViewProjection * worldPos;
}
