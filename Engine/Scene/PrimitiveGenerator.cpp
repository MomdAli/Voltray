#include "PrimitiveGenerator.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

std::shared_ptr<Mesh> PrimitiveGenerator::CreateCube(float size)
{
    float halfSize = size * 0.5f;

    // Cube vertices: position (3) + normal (3) + texcoord (2) = 8 floats per vertex
    std::vector<float> vertices = {
        // Front face
        -halfSize, -halfSize, halfSize, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        halfSize, -halfSize, halfSize, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        halfSize, halfSize, halfSize, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -halfSize, halfSize, halfSize, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Back face
        -halfSize, -halfSize, -halfSize, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        -halfSize, halfSize, -halfSize, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        halfSize, halfSize, -halfSize, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        halfSize, -halfSize, -halfSize, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        // Left face
        -halfSize, -halfSize, -halfSize, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -halfSize, -halfSize, halfSize, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -halfSize, halfSize, halfSize, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -halfSize, halfSize, -halfSize, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Right face
        halfSize, -halfSize, -halfSize, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        halfSize, halfSize, -halfSize, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        halfSize, halfSize, halfSize, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        halfSize, -halfSize, halfSize, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        // Top face
        -halfSize, halfSize, -halfSize, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -halfSize, halfSize, halfSize, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        halfSize, halfSize, halfSize, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        halfSize, halfSize, -halfSize, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // Bottom face
        -halfSize, -halfSize, -halfSize, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        halfSize, -halfSize, -halfSize, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        halfSize, -halfSize, halfSize, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -halfSize, -halfSize, halfSize, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20};

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveGenerator::CreatePlane(float width, float height, int widthSegments, int heightSegments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    // Generate vertices
    for (int y = 0; y <= heightSegments; y++)
    {
        for (int x = 0; x <= widthSegments; x++)
        {
            float u = (float)x / widthSegments;
            float v = (float)y / heightSegments;

            float posX = (u - 0.5f) * width;
            float posZ = (v - 0.5f) * height;

            AddVertex(vertices, posX, 0.0f, posZ, 0.0f, 1.0f, 0.0f, u, v);
        }
    }

    // Generate indices
    for (int y = 0; y < heightSegments; y++)
    {
        for (int x = 0; x < widthSegments; x++)
        {
            int a = y * (widthSegments + 1) + x;
            int b = a + 1;
            int c = (y + 1) * (widthSegments + 1) + x;
            int d = c + 1;

            AddTriangle(indices, a, b, c);
            AddTriangle(indices, b, d, c);
        }
    }

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveGenerator::CreateSphere(float radius, int widthSegments, int heightSegments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices
    for (int y = 0; y <= heightSegments; y++)
    {
        float phi = M_PI * (float)y / heightSegments;

        for (int x = 0; x <= widthSegments; x++)
        {
            float theta = 2.0f * M_PI * (float)x / widthSegments;

            float cosTheta = cosf(theta);
            float sinTheta = sinf(theta);
            float cosPhi = cosf(phi);
            float sinPhi = sinf(phi);

            float posX = radius * sinPhi * cosTheta;
            float posY = radius * cosPhi;
            float posZ = radius * sinPhi * sinTheta;

            // Normal is the same as position for a unit sphere
            float nx = sinPhi * cosTheta;
            float ny = cosPhi;
            float nz = sinPhi * sinTheta;

            float u = (float)x / widthSegments;
            float v = (float)y / heightSegments;

            AddVertex(vertices, posX, posY, posZ, nx, ny, nz, u, v);
        }
    }

    // Generate indices
    for (int y = 0; y < heightSegments; y++)
    {
        for (int x = 0; x < widthSegments; x++)
        {
            int a = y * (widthSegments + 1) + x;
            int b = a + 1;
            int c = (y + 1) * (widthSegments + 1) + x;
            int d = c + 1;

            if (y != 0) // Skip top cap
                AddTriangle(indices, a, b, c);
            if (y != heightSegments - 1) // Skip bottom cap
                AddTriangle(indices, b, d, c);
        }
    }

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveGenerator::CreateCylinder(float radiusTop, float radiusBottom,
                                                         float height, int radialSegments, int heightSegments)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float halfHeight = height * 0.5f;

    // Generate side vertices
    for (int y = 0; y <= heightSegments; y++)
    {
        float v = (float)y / heightSegments;
        float posY = -halfHeight + v * height;
        float radius = radiusBottom + v * (radiusTop - radiusBottom);

        for (int x = 0; x <= radialSegments; x++)
        {
            float u = (float)x / radialSegments;
            float theta = 2.0f * M_PI * u;

            float cosTheta = cosf(theta);
            float sinTheta = sinf(theta);

            float posX = radius * cosTheta;
            float posZ = radius * sinTheta;

            // Normal calculation for cylinder
            float nx = cosTheta;
            float ny = 0.0f;
            float nz = sinTheta;

            AddVertex(vertices, posX, posY, posZ, nx, ny, nz, u, v);
        }
    }

    // Generate side indices
    for (int y = 0; y < heightSegments; y++)
    {
        for (int x = 0; x < radialSegments; x++)
        {
            int a = y * (radialSegments + 1) + x;
            int b = a + 1;
            int c = (y + 1) * (radialSegments + 1) + x;
            int d = c + 1;

            AddTriangle(indices, a, b, c);
            AddTriangle(indices, b, d, c);
        }
    }

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> PrimitiveGenerator::CreateTriangle(float size)
{
    float halfSize = size * 0.5f;

    std::vector<float> vertices = {
        // Triangle vertices: position (3) + normal (3) + texcoord (2)
        0.0f, halfSize, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f,
        -halfSize, -halfSize, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        halfSize, -halfSize, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};

    std::vector<unsigned int> indices = {0, 1, 2};

    return std::make_shared<Mesh>(vertices, indices);
}

void PrimitiveGenerator::AddVertex(std::vector<float> &vertices, float x, float y, float z,
                                   float nx, float ny, float nz, float u, float v)
{
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    vertices.push_back(nx);
    vertices.push_back(ny);
    vertices.push_back(nz);
    vertices.push_back(u);
    vertices.push_back(v);
}

void PrimitiveGenerator::AddTriangle(std::vector<unsigned int> &indices, unsigned int a, unsigned int b, unsigned int c)
{
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
}
