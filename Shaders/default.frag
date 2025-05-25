#version 460 core

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPos;

out vec4 FragColor;

void main() {
    // Simple lighting calculation
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    vec3 normal = normalize(v_Normal);
    float diff = max(dot(normal, lightDir), 0.0);

    // Base color with simple diffuse lighting
    vec3 baseColor = vec3(0.8, 0.6, 0.4);
    vec3 color = baseColor * (0.3 + 0.7 * diff); // ambient + diffuse

    FragColor = vec4(color, 1.0);
}