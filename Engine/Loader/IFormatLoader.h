#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Graphics/Mesh.h"

namespace Engine::Loader
{
    /**
     * @struct MeshData
     * @brief Raw mesh data loaded from files
     */
    struct MeshData
    {
        std::vector<float> vertices; // Position(3) + Normal(3) + TexCoord(2) per vertex
        std::vector<unsigned int> indices;
        std::string name;
        std::string materialName; // Optional material reference
    };

    /**
     * @class IFormatLoader
     * @brief Abstract base class for different mesh format loaders
     */
    class IFormatLoader
    {
    public:
        virtual ~IFormatLoader() = default;

        /**
         * @brief Check if this loader can handle the given file extension
         * @param extension File extension (e.g., ".obj", ".fbx")
         * @return True if format is supported
         */
        virtual bool CanLoad(const std::string &extension) const = 0;

        /**
         * @brief Load mesh data from file
         * @param filepath Path to the mesh file
         * @return Vector of MeshData structures
         */
        virtual std::vector<MeshData> LoadMeshData(const std::string &filepath) = 0;

        /**
         * @brief Get the name of this loader
         * @return Loader name for debugging
         */
        virtual std::string GetLoaderName() const = 0;
    }; /**
        * @class AssimpLoader
        * @brief Loader using Assimp library for multiple formats
        * Supports: FBX, GLTF, GLB, 3DS, DAE, PLY, STL, X3D, OBJ, and many more
        */
    class AssimpLoader : public IFormatLoader
    {
    public:
        bool CanLoad(const std::string &extension) const override;
        std::vector<MeshData> LoadMeshData(const std::string &filepath) override;
        std::string GetLoaderName() const override { return "Assimp Loader"; }

    private:
        /**
         * @brief Process an Assimp mesh node
         * @param mesh Assimp mesh pointer
         * @param scene Assimp scene pointer
         * @return Converted MeshData
         */
        MeshData ProcessMesh(void *mesh, void *scene);
    };
}
