#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPos;

void main() {
    vec4 worldPos = u_Model * vec4(aPos, 1.0);
    v_WorldPos = worldPos.xyz;
    v_Normal = mat3(u_Model) * aNormal; // Simple normal transformation (not correct for non-uniform scaling)
    v_TexCoord = aTexCoord;

    gl_Position = u_ViewProjection * worldPos;
}
