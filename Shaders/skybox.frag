#version 460 core
out vec4 FragColor;

in vec3 vWorldDir;

void main() {
    // vWorldDir is the direction from the camera through the pixel in world space
    float t = 0.5 * (vWorldDir.y + 1.0); // y: -1 (down) to +1 (up)
    vec3 topColor = vec3(0.36, 0.65, 0.98);    // Soft blue (clear sky)
    vec3 bottomColor = vec3(0.87, 0.93, 0.99); // Light blue (near horizon)
    FragColor = vec4(mix(bottomColor, topColor, t), 1.0);
}