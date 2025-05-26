#include "MeshLoader.h"
#include "IFormatLoader.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

using namespace Engine::Loader;

std::shared_ptr<Mesh> MeshLoader::LoadMesh(const std::string &filepath)
{
    auto meshes = LoadMeshes(filepath);
    if (meshes.empty())
    {
        std::cerr << "Failed to load any mesh from: " << filepath << std::endl;
        return nullptr;
    }

    return meshes[0]; // Return first mesh
}

std::vector<std::shared_ptr<Mesh>> MeshLoader::LoadMeshes(const std::string &filepath)
{
    std::vector<std::shared_ptr<Mesh>> meshes;

    if (!std::filesystem::exists(filepath))
    {
        std::cerr << "File does not exist: " << filepath << std::endl;
        return meshes;
    }

    if (!IsFormatSupported(filepath))
    {
        std::cerr << "Unsupported file format: " << filepath << std::endl;
        return meshes;
    }

    auto meshData = LoadMeshData(filepath);

    for (const auto &data : meshData)
    {
        if (!data.vertices.empty() && !data.indices.empty())
        {
            auto mesh = std::make_shared<Mesh>(data.vertices, data.indices);
            meshes.push_back(mesh);
        }
    }

    return meshes;
}

std::vector<MeshData> MeshLoader::LoadMeshData(const std::string &filepath)
{
    try
    {
        std::string extension = GetFileExtension(filepath);
        auto loader = GetLoaderForExtension(extension);

        if (!loader)
        {
            std::cerr << "No loader available for extension: " << extension << std::endl;
            return {};
        }

        std::cout << "Using " << loader->GetLoaderName() << " for file: " << filepath << std::endl;
        return loader->LoadMeshData(filepath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading mesh: " << e.what() << std::endl;
        return {};
    }
}

bool MeshLoader::IsFormatSupported(const std::string &filepath)
{
    std::string extension = GetFileExtension(filepath);
    return GetLoaderForExtension(extension) != nullptr;
}

std::vector<std::string> MeshLoader::GetSupportedFormats()
{
    std::vector<std::string> allFormats;
    auto loaders = GetAllLoaders();

    for (auto &loader : loaders)
    {
        // Test common extensions to see what each loader supports
        std::vector<std::string> testExtensions = {
            ".obj", ".fbx", ".dae", ".gltf", ".glb", ".3ds", ".ply", ".stl",
            ".blend", ".x", ".ac", ".ase", ".b3d", ".bvh", ".csm", ".hmp",
            ".ifc", ".irrmesh", ".lwo", ".lws", ".md2", ".md3", ".md5mesh",
            ".mdc", ".mdl", ".nff", ".ndo", ".off", ".ogex", ".raw", ".smd",
            ".ter", ".x3d", ".xgl", ".zgl", ".step", ".stp", ".iges", ".igs",
            ".ms3d", ".cob", ".scn", ".mesh.xml", ".irr", ".pmx", ".q3o", ".q3s"};

        for (const auto &ext : testExtensions)
        {
            if (loader->CanLoad(ext))
            {
                if (std::find(allFormats.begin(), allFormats.end(), ext) == allFormats.end())
                {
                    allFormats.push_back(ext);
                }
            }
        }
    }

    std::sort(allFormats.begin(), allFormats.end());
    return allFormats;
}

std::vector<std::pair<std::string, std::vector<std::string>>> MeshLoader::GetLoaderInfo()
{
    std::vector<std::pair<std::string, std::vector<std::string>>> info;
    auto loaders = GetAllLoaders();

    for (auto &loader : loaders)
    {
        std::vector<std::string> supportedFormats;

        // Test extensions for this specific loader
        std::vector<std::string> testExtensions = {
            ".obj", ".fbx", ".dae", ".gltf", ".glb", ".3ds", ".ply", ".stl",
            ".blend", ".x", ".ac", ".ase", ".b3d", ".bvh", ".csm", ".hmp",
            ".ifc", ".irrmesh", ".lwo", ".lws", ".md2", ".md3", ".md5mesh",
            ".mdc", ".mdl", ".nff", ".ndo", ".off", ".ogex", ".raw", ".smd",
            ".ter", ".x3d", ".xgl", ".zgl", ".step", ".stp", ".iges", ".igs",
            ".ms3d", ".cob", ".scn", ".mesh.xml", ".irr", ".pmx", ".q3o", ".q3s"};

        for (const auto &ext : testExtensions)
        {
            if (loader->CanLoad(ext))
            {
                supportedFormats.push_back(ext);
            }
        }

        info.emplace_back(loader->GetLoaderName(), std::move(supportedFormats));
    }

    return info;
}

std::shared_ptr<IFormatLoader> MeshLoader::GetLoaderForExtension(const std::string &extension)
{
    auto loaders = GetAllLoaders();

    for (auto &loader : loaders)
    {
        if (loader->CanLoad(extension))
        {
            return loader;
        }
    }

    return nullptr;
}

std::string MeshLoader::GetFileExtension(const std::string &filepath)
{
    size_t lastDot = filepath.find_last_of('.');
    if (lastDot == std::string::npos)
        return "";

    std::string extension = filepath.substr(lastDot);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return extension;
}

std::vector<std::shared_ptr<IFormatLoader>> MeshLoader::GetAllLoaders()
{
    static std::vector<std::shared_ptr<IFormatLoader>> loaders;

    // Initialize loaders only once
    if (loaders.empty())
    {
        // Add OBJ loader (lightweight, specific implementation)
        loaders.push_back(std::make_shared<OBJLoader>());

        // Add Assimp loader (comprehensive, handles most formats)
        loaders.push_back(std::make_shared<AssimpLoader>());
    }

    return loaders;
}
