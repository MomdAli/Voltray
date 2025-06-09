#include "Ray.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>
#include <iostream>

namespace Voltray::Math
{

    Ray::Ray() : origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f)
    {
    }

    Ray::Ray(const Vec3 &origin, const Vec3 &direction)
        : origin(origin), direction(direction.Normalize())
    {
    }

    Vec3 Ray::GetOrigin() const
    {
        return origin;
    }

    Vec3 Ray::GetDirection() const
    {
        return direction;
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

    // Möller-Trumbore algorithm for triangle intersection
    bool Ray::IntersectTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, float &t) const
    {
        const float EPSILON = 1e-6f; // More reasonable epsilon for ray-triangle intersection

        Vec3 edge1 = v1 - v0;
        Vec3 edge2 = v2 - v0;
        Vec3 h = direction.Cross(edge2);
        float a = edge1.Dot(h);

        if (std::abs(a) < EPSILON)
            return false; // Ray is parallel to triangle

        float f = 1.0f / a;
        Vec3 s = origin - v0;
        float u = f * s.Dot(h);

        // Allow small tolerance for edge cases
        if (u < -EPSILON || u > 1.0f + EPSILON)
            return false;

        Vec3 q = s.Cross(edge1);
        float v = f * direction.Dot(q);

        // Allow small tolerance for edge cases
        if (v < -EPSILON || u + v > 1.0f + EPSILON)
            return false;

        t = f * edge2.Dot(q);
        return t > EPSILON; // Ensure intersection is in front of ray origin
    }

    bool Ray::IntersectMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, float &t) const
    {
        bool hit = false;
        float closestT = std::numeric_limits<float>::max();

        // Determine vertex stride - check if we have 8 floats per vertex (pos + normal + texcoord)
        // or just 3 floats per vertex (position only)
        int stride = 8; // Default to full vertex layout
        if (vertices.size() % 8 != 0 && vertices.size() % 3 == 0)
        {
            stride = 3; // Simple position-only vertices
        }

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            Vec3 v0(vertices[indices[i] * stride], vertices[indices[i] * stride + 1], vertices[indices[i] * stride + 2]);
            Vec3 v1(vertices[indices[i + 1] * stride], vertices[indices[i + 1] * stride + 1], vertices[indices[i + 1] * stride + 2]);
            Vec3 v2(vertices[indices[i + 2] * stride], vertices[indices[i + 2] * stride + 1], vertices[indices[i + 2] * stride + 2]);

            float intersectionT;
            if (IntersectTriangle(v0, v1, v2, intersectionT))
            {
                if (intersectionT < closestT)
                {
                    closestT = intersectionT;
                    hit = true;
                }
            }
        }
        if (hit)
        {
            t = closestT;
        }
        return hit;
    }

    bool Ray::IntersectMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const Mat4 &transform, float &t) const
    {
        // Transform ray from world space to object local space
        Mat4 inverseTransform = transform.Inverse();

        // Transform ray origin to local space (applies full transformation)
        Vec3 localOrigin = inverseTransform.MultiplyVec3(origin);

        // Transform ray direction to local space (only rotation/scale, no translation)
        // We use Vec4 with w=0 to apply only the rotation/scale part
        Vec3 localDirection = Vec3(
            direction.x * inverseTransform.data[0] + direction.y * inverseTransform.data[4] + direction.z * inverseTransform.data[8],
            direction.x * inverseTransform.data[1] + direction.y * inverseTransform.data[5] + direction.z * inverseTransform.data[9],
            direction.x * inverseTransform.data[2] + direction.y * inverseTransform.data[6] + direction.z * inverseTransform.data[10]);

        // Store the original length of the transformed direction for distance correction
        float directionScale = localDirection.Length();
        localDirection = localDirection.Normalize();

        // Create local space ray
        Ray localRay(localOrigin, localDirection);

        // Perform intersection in local space
        float localT;
        bool hit = localRay.IntersectMesh(vertices, indices, localT);

        if (hit)
        {
            // Transform the intersection distance back to world space
            // We need to account for the scaling applied to the direction vector
            Vec3 localIntersectionPoint = localRay.GetPoint(localT);
            Vec3 worldIntersectionPoint = transform.MultiplyVec3(localIntersectionPoint);
            Vec3 worldRayToIntersection = worldIntersectionPoint - origin;
            t = worldRayToIntersection.Length();
        }

        return hit;
    }

    static Vec3 ClosestPointOnLine(const Vec3 &rayOrigin,
                                   const Vec3 &linePoint, const Vec3 &lineDirection)
    {
        Vec3 lineToRay = rayOrigin - linePoint;
        float t = lineToRay.Dot(lineDirection) / lineDirection.Dot(lineDirection);
        return linePoint + lineDirection * t;
    }

    static float DistanceToLineSegment(const Vec3 &rayOrigin, const Vec3 &rayDirection,
                                       const Vec3 &segmentStart, const Vec3 &segmentEnd)
    {
        Vec3 segmentDirection = segmentEnd - segmentStart;
        Vec3 closestPoint = ClosestPointOnLine(rayOrigin, segmentStart, segmentDirection);
        Vec3 toClosest = closestPoint - rayOrigin;

        // Check if closest point is within the segment bounds
        float t = toClosest.Dot(segmentDirection) / segmentDirection.Dot(segmentDirection);
        if (t < 0.0f || t > 1.0f)
        {
            // Closest point is outside the segment, use endpoints
            float distToStart = (rayOrigin - segmentStart).Length();
            float distToEnd = (rayOrigin - segmentEnd).Length();
            return std::min(distToStart, distToEnd);
        }
        return toClosest.Length();
    }

}
