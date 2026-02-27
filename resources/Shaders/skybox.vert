#version 460 core
// Fullscreen triangle (no vertex buffer needed)
const vec2 verts[3] = vec2[3](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

out vec3 vWorldDir;

uniform mat4 u_InverseViewProj;

void main() {
    gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
    // Reconstruct NDC
    vec2 ndc = verts[gl_VertexID];
    vec4 clip = vec4(ndc, 1.0, 1.0);
    // Unproject to world space
    vec4 world = u_InverseViewProj * clip;
    vWorldDir = normalize(world.xyz / world.w);
}