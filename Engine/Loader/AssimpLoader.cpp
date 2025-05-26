#include "IFormatLoader.h"
#include <iostream>
#include <algorithm>

// Include Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Engine::Loader;

// ===== AssimpLoader Implementation =====

bool AssimpLoader::CanLoad(const std::string &extension) const
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    // Comprehensive list of formats supported by Assimp
    static const std::vector<std::string> supportedFormats = {
        // Common 3D formats
        ".fbx",   // Autodesk FBX
        ".dae",   // COLLADA
        ".gltf",  // glTF 2.0
        ".glb",   // Binary glTF
        ".blend", // Blender

        // Legacy formats
        ".3ds",     // 3D Studio Max
        ".max",     // 3D Studio Max
        ".obj",     // Wavefront OBJ (backup if OBJLoader fails)
        ".ply",     // Stanford PLY
        ".stl",     // Stereolithography
        ".x",       // DirectX X
        ".ac",      // AC3D
        ".ase",     // 3D Studio Max ASE
        ".assbin",  // Assimp Binary
        ".b3d",     // Blitz3D
        ".bvh",     // Biovision Hierarchy
        ".csm",     // CharacterStudio Motion
        ".hmp",     // 3D GameStudio
        ".ifc",     // Industry Foundation Classes
        ".irrmesh", // Irrlicht Mesh
        ".lwo",     // LightWave Object
        ".lws",     // LightWave Scene
        ".md2",     // Quake II
        ".md3",     // Quake III
        ".md5mesh", // Doom 3
        ".mdc",     // Return to Castle Wolfenstein
        ".mdl",     // Quake
        ".nff",     // Neutral File Format
        ".ndo",     // Nendo
        ".off",     // Object File Format
        ".ogex",    // Open Game Engine Exchange
        ".raw",     // Raw Triangles
        ".smd",     // Valve SMD
        ".ter",     // Terragen Terrain
        ".x3d",     // Extensible 3D
        ".xgl",     // XGL
        ".zgl",     // ZGL

        // CAD formats
        ".step", // STEP
        ".stp",  // STEP
        ".iges", // IGES
        ".igs",  // IGES

        // Game engine formats
        ".ms3d",     // MilkShape 3D
        ".cob",      // TrueSpace
        ".scn",      // TrueSpace
        ".mesh.xml", // Ogre3D
        ".irr",      // Irrlicht Scene
        ".pmx",      // MikuMikuDance
        ".q3o",      // Quick3D Object
        ".q3s"       // Quick3D Scene
    };

    return std::find(supportedFormats.begin(), supportedFormats.end(), ext) != supportedFormats.end();
}

std::vector<MeshData> AssimpLoader::LoadMeshData(const std::string &filepath)
{
    std::vector<MeshData> result;

    Assimp::Importer importer;

    // Configure import settings for optimal processing
    unsigned int flags =
        aiProcess_Triangulate |              // Convert all faces to triangles
        aiProcess_FlipUVs |                  // Flip UV coordinates (OpenGL convention)
        aiProcess_GenSmoothNormals |         // Generate smooth normals if missing
        aiProcess_CalcTangentSpace |         // Calculate tangent space for normal mapping
        aiProcess_JoinIdenticalVertices |    // Remove duplicate vertices
        aiProcess_SortByPType |              // Sort primitives by type
        aiProcess_OptimizeMeshes |           // Optimize mesh structure
        aiProcess_OptimizeGraph |            // Optimize scene graph
        aiProcess_ValidateDataStructure |    // Validate data structure
        aiProcess_ImproveCacheLocality |     // Improve cache locality
        aiProcess_RemoveRedundantMaterials | // Remove redundant materials
        aiProcess_FixInfacingNormals |       // Fix inward-facing normals
        aiProcess_FindDegenerates |          // Find degenerate triangles
        aiProcess_FindInvalidData |          // Find invalid data
        aiProcess_GenBoundingBoxes;          // Generate bounding boxes

    const aiScene *scene = importer.ReadFile(filepath, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error("Assimp error loading " + filepath + ": " + std::string(importer.GetErrorString()));
    }

    std::cout << "Loading with Assimp: " << filepath << std::endl;
    std::cout << "Scene contains " << scene->mNumMeshes << " meshes" << std::endl;

    // Process all meshes in the scene
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[i];
        MeshData data = ProcessMesh(mesh, const_cast<aiScene *>(scene));

        if (!data.vertices.empty())
        {
            // Set mesh name
            data.name = mesh->mName.C_Str();
            if (data.name.empty())
            {
                data.name = "Mesh_" + std::to_string(i);
            }

            result.push_back(std::move(data));
        }
    }

    if (!result.empty())
    {
        std::cout << "Successfully loaded " << result.size() << " meshes from: " << filepath << std::endl;
        size_t totalVertices = 0, totalFaces = 0;
        for (const auto &mesh : result)
        {
            totalVertices += mesh.vertices.size() / 8;
            totalFaces += mesh.indices.size() / 3;
        }
        std::cout << "Total vertices: " << totalVertices << ", Total faces: " << totalFaces << std::endl;
    }

    return result;
}

MeshData AssimpLoader::ProcessMesh(void *meshPtr, void *scenePtr)
{
    aiMesh *mesh = static_cast<aiMesh *>(meshPtr);
    const aiScene *scene = static_cast<const aiScene *>(scenePtr);

    MeshData data;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Position (required)
        data.vertices.push_back(mesh->mVertices[i].x);
        data.vertices.push_back(mesh->mVertices[i].y);
        data.vertices.push_back(mesh->mVertices[i].z);

        // Normal (default to up if not present)
        if (mesh->HasNormals())
        {
            data.vertices.push_back(mesh->mNormals[i].x);
            data.vertices.push_back(mesh->mNormals[i].y);
            data.vertices.push_back(mesh->mNormals[i].z);
        }
        else
        {
            data.vertices.push_back(0.0f);
            data.vertices.push_back(1.0f);
            data.vertices.push_back(0.0f);
        }

        // Texture coordinates (use first UV channel, default to 0,0 if not present)
        if (mesh->mTextureCoords[0])
        {
            data.vertices.push_back(mesh->mTextureCoords[0][i].x);
            data.vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            data.vertices.push_back(0.0f);
            data.vertices.push_back(0.0f);
        }
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            data.indices.push_back(face.mIndices[j]);
        }
    }

    // Store material information if available
    if (mesh->mMaterialIndex >= 0 && scene->mMaterials)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiString materialName;
        if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS)
        {
            data.materialName = materialName.C_Str();
        }
    }

    return data;
}
