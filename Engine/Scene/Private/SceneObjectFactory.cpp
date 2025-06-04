#include "SceneObjectFactory.h"
#include "MeshLoader.h"
#include <filesystem>
#include <iostream>

namespace Voltray::Engine
{

    std::shared_ptr<SceneObject> SceneObjectFactory::CreateCube(const std::string &name, float size)
    {
        auto mesh = PrimitiveGenerator::CreateCube(size);
        return std::make_shared<SceneObject>(mesh, name);
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::CreateSphere(const std::string &name, float radius,
                                                                  int widthSegments, int heightSegments)
    {
        auto mesh = PrimitiveGenerator::CreateSphere(radius, widthSegments, heightSegments);
        return std::make_shared<SceneObject>(mesh, name);
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::CreatePlane(const std::string &name, float width, float height,
                                                                 int widthSegments, int heightSegments)
    {
        auto mesh = PrimitiveGenerator::CreatePlane(width, height, widthSegments, heightSegments);
        return std::make_shared<SceneObject>(mesh, name);
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::CreateCylinder(const std::string &name, float radiusTop,
                                                                    float radiusBottom, float height,
                                                                    int radialSegments, int heightSegments)
    {
        auto mesh = PrimitiveGenerator::CreateCylinder(radiusTop, radiusBottom, height, radialSegments, heightSegments);
        return std::make_shared<SceneObject>(mesh, name);
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::CreateTriangle(const std::string &name, float size)
    {
        auto mesh = PrimitiveGenerator::CreateTriangle(size);
        return std::make_shared<SceneObject>(mesh, name);
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::LoadFromFile(const std::string &filepath, const std::string &name)
    {
        if (!std::filesystem::exists(filepath))
        {
            std::cerr << "File not found: " << filepath << std::endl;
            return nullptr;
        }

        auto mesh = Engine::MeshLoader::LoadMesh(filepath);
        if (!mesh)
        {
            std::cerr << "Failed to load mesh from: " << filepath << std::endl;
            return nullptr;
        }

        std::string objectName = name.empty() ? std::filesystem::path(filepath).stem().string() : name;

        return CreateFromMesh(mesh, objectName);
    }

    std::vector<std::shared_ptr<SceneObject>> SceneObjectFactory::LoadAllFromFile(const std::string &filepath)
    {
        std::vector<std::shared_ptr<SceneObject>> objects;

        if (!std::filesystem::exists(filepath))
        {
            std::cerr << "File not found: " << filepath << std::endl;
            return objects;
        }

        auto meshes = Engine::MeshLoader::LoadMeshes(filepath);

        for (size_t i = 0; i < meshes.size(); i++)
        {
            std::string objectName = std::filesystem::path(filepath).stem().string();
            if (meshes.size() > 1)
            {
                objectName += "_" + std::to_string(i);
            }

            auto object = CreateFromMesh(meshes[i], objectName);
            if (object)
            {
                objects.push_back(object);
            }
        }

        return objects;
    }

    std::shared_ptr<SceneObject> SceneObjectFactory::CreateFromMesh(std::shared_ptr<Mesh> mesh, const std::string &name)
    {
        if (!mesh)
        {
            std::cerr << "Cannot create scene object from null mesh" << std::endl;
            return nullptr;
        }

        return std::make_shared<SceneObject>(mesh, name);
    }

} // namespace Voltray::Engine
