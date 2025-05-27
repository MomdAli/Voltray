#include "Ray.h"
#include <cmath>
#include <algorithm>

Ray::Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f)
{
}

Ray::Ray(const Vec3 &origin, const Vec3 &direction)
    : origin(origin), direction(direction.Normalize())
{
}

Vec3 Ray::GetPoint(float t) const
{
    return origin + direction * t;
}

bool Ray::IntersectSphere(const Vec3 &center, float radius, float &t) const
{
    Vec3 oc = origin - center;
    float a = direction.Dot(direction);
    float b = 2.0f * oc.Dot(direction);
    float c = oc.Dot(oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    float sqrt_discriminant = std::sqrt(discriminant);
    float t1 = (-b - sqrt_discriminant) / (2.0f * a);
    float t2 = (-b + sqrt_discriminant) / (2.0f * a);

    // We want the closest positive intersection
    if (t1 > 0)
        t = t1;
    else if (t2 > 0)
        t = t2;
    else
        return false; // Both intersections are behind the ray origin

    return true;
}

bool Ray::IntersectAABB(const Vec3 &minBounds, const Vec3 &maxBounds, float &t) const
{
    Vec3 invDir = Vec3(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z);

    float t1 = (minBounds.x - origin.x) * invDir.x;
    float t2 = (maxBounds.x - origin.x) * invDir.x;
    if (t1 > t2)
        std::swap(t1, t2);

    float tymin = (minBounds.y - origin.y) * invDir.y;
    float tymax = (maxBounds.y - origin.y) * invDir.y;
    if (tymin > tymax)
        std::swap(tymin, tymax);

    if (t1 > tymax || tymin > t2)
        return false;

    if (tymin > t1)
        t1 = tymin;
    if (tymax < t2)
        t2 = tymax;

    float tzmin = (minBounds.z - origin.z) * invDir.z;
    float tzmax = (maxBounds.z - origin.z) * invDir.z;
    if (tzmin > tzmax)
        std::swap(tzmin, tzmax);

    if (t1 > tzmax || tzmin > t2)
        return false;

    if (tzmin > t1)
        t1 = tzmin;

    t = (t1 > 0) ? t1 : t2;
    return t > 0;
}
