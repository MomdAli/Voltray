#version 460 core

uniform vec3 u_OutlineColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(u_OutlineColor, 1.0);
}
