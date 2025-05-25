#include "SceneObjectFactory.h"

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
