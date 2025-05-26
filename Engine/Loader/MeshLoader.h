#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Graphics/Mesh.h"
#include "IFormatLoader.h"

namespace Engine::Loader
{
    /**
     * @class MeshLoader
     * @brief Main mesh loading facade that manages multiple format loaders
     *
     * This class provides a clean interface for loading 3D models from external files.
     * It automatically selects the appropriate loader based on file extension and
     * supports a wide range of formats through different loader implementations.
     */
    class MeshLoader
    {
    public:
        /**
         * @brief Load a single mesh from file (first mesh if file contains multiple)
         * @param filepath Path to the mesh file
         * @return Shared pointer to loaded mesh, or nullptr if failed
         */
        static std::shared_ptr<Mesh> LoadMesh(const std::string &filepath);

        /**
         * @brief Load all meshes from file
         * @param filepath Path to the mesh file
         * @return Vector of shared pointers to all loaded meshes
         */
        static std::vector<std::shared_ptr<Mesh>> LoadMeshes(const std::string &filepath);

        /**
         * @brief Load raw mesh data for custom processing
         * @param filepath Path to the mesh file
         * @return Vector of MeshData structures
         */
        static std::vector<MeshData> LoadMeshData(const std::string &filepath);

        /**
         * @brief Check if file format is supported
         * @param filepath Path to check
         * @return True if format is supported
         */
        static bool IsFormatSupported(const std::string &filepath);

        /**
         * @brief Get list of all supported file extensions
         * @return Vector of supported extensions (e.g., ".obj", ".fbx")
         */
        static std::vector<std::string> GetSupportedFormats();

        /**
         * @brief Get information about available loaders
         * @return Vector of loader names and their supported formats
         */
        static std::vector<std::pair<std::string, std::vector<std::string>>> GetLoaderInfo();

    private:
        /**
         * @brief Get the appropriate loader for a file extension
         * @param extension File extension
         * @return Pointer to loader, or nullptr if not supported
         */
        static std::shared_ptr<IFormatLoader> GetLoaderForExtension(const std::string &extension);

        /**
         * @brief Get file extension from filepath
         * @param filepath File path
         * @return Extension string (including dot)
         */
        static std::string GetFileExtension(const std::string &filepath);

        /**
         * @brief Initialize all available loaders
         * @return Vector of all available loaders
         */
        static std::vector<std::shared_ptr<IFormatLoader>> GetAllLoaders();
    };
};
