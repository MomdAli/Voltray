#version 460 core

layout(location = 0) in vec3 aPos;
uniform mat4 u_ViewProjection;

void main() {
    gl_Position = u_ViewProjection * vec4(aPos, 1.0);
}
