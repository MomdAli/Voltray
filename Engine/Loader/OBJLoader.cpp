#include "IFormatLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include "../Math/Vec3.h"
#include "../Math/Vec2.h"

using namespace Engine::Loader;

// ===== OBJLoader Implementation =====

bool OBJLoader::CanLoad(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".obj";
}

std::vector<MeshData> OBJLoader::LoadMeshData(const std::string &filepath)
{
    std::vector<MeshData> result;
    MeshData meshData;
    meshData.name = std::filesystem::path(filepath).stem().string();

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open OBJ file: " + filepath);
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") // Vertex position
        {
            Vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (type == "vn") // Vertex normal
        {
            Vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "vt") // Texture coordinate
        {
            Vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (type == "f") // Face
        {
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            // Parse face indices (format: v/vt/vn or v//vn or v/vt or v)
            auto parseVertex = [&](const std::string &vertexStr) -> unsigned int
            {
                std::istringstream viss(vertexStr);
                std::string posIdx, texIdx, normalIdx;

                std::getline(viss, posIdx, '/');
                std::getline(viss, texIdx, '/');
                std::getline(viss, normalIdx, '/');

                int pIdx = std::stoi(posIdx) - 1; // OBJ indices are 1-based
                int tIdx = texIdx.empty() ? -1 : std::stoi(texIdx) - 1;
                int nIdx = normalIdx.empty() ? -1 : std::stoi(normalIdx) - 1;

                // Add vertex data: position(3) + normal(3) + texcoord(2)
                if (pIdx >= 0 && pIdx < positions.size())
                {
                    Vec3 pos = positions[pIdx];
                    meshData.vertices.push_back(pos.x);
                    meshData.vertices.push_back(pos.y);
                    meshData.vertices.push_back(pos.z);
                }

                // Normal (default to up if not available)
                if (nIdx >= 0 && nIdx < normals.size())
                {
                    Vec3 normal = normals[nIdx];
                    meshData.vertices.push_back(normal.x);
                    meshData.vertices.push_back(normal.y);
                    meshData.vertices.push_back(normal.z);
                }
                else
                {
                    meshData.vertices.push_back(0.0f);
                    meshData.vertices.push_back(1.0f);
                    meshData.vertices.push_back(0.0f);
                }

                // Texture coordinate (default to 0,0 if not available)
                if (tIdx >= 0 && tIdx < texCoords.size())
                {
                    Vec2 texCoord = texCoords[tIdx];
                    meshData.vertices.push_back(texCoord.x);
                    meshData.vertices.push_back(texCoord.y);
                }
                else
                {
                    meshData.vertices.push_back(0.0f);
                    meshData.vertices.push_back(0.0f);
                }

                return static_cast<unsigned int>((meshData.vertices.size() / 8) - 1); // 8 floats per vertex
            };

            unsigned int idx1 = parseVertex(vertex1);
            unsigned int idx2 = parseVertex(vertex2);
            unsigned int idx3 = parseVertex(vertex3);

            meshData.indices.push_back(idx1);
            meshData.indices.push_back(idx2);
            meshData.indices.push_back(idx3);
        }
    }

    file.close();

    if (!meshData.vertices.empty())
    {
        result.push_back(std::move(meshData));
        std::cout << "Successfully loaded OBJ file: " << filepath << std::endl;
        std::cout << "Vertices: " << meshData.vertices.size() / 8 << ", Faces: " << meshData.indices.size() / 3 << std::endl;
    }

    return result;
}
