#pragma once

#include "vec3.h"
#include "Mat4.h"
#include <vector>

/**
 * @struct Ray
 * @brief Represents a ray in 3D space with an origin and direction.
 *
 * A ray is defined by an origin point and a normalized direction vector.
 * Used for ray casting operations like object picking and collision detection.
 */
struct Ray
{
    Vec3 origin;    ///< The starting point of the ray
    Vec3 direction; ///< The normalized direction vector of the ray

    /**
     * @brief Default constructor - creates a ray at origin pointing in positive Z direction.
     */
    Ray();

    /**
     * @brief Constructs a ray with specified origin and direction.
     * @param origin The starting point of the ray.
     * @param direction The direction vector (will be normalized).
     */
    Ray(const Vec3 &origin, const Vec3 &direction);

    /**
     * @brief Gets a point along the ray at parameter t.
     * @param t The parameter value (distance along ray).
     * @return Point at origin + t * direction.
     */
    Vec3 GetPoint(float t) const;

    /**
     * @brief Tests intersection with a sphere.
     * @param center Center of the sphere.
     * @param radius Radius of the sphere.
     * @param t Output parameter for intersection distance.
     * @return True if intersection occurs, false otherwise.
     */
    bool IntersectSphere(const Vec3 &center, float radius, float &t) const;

    /**
     * @brief Tests intersection with an axis-aligned bounding box.
     * @param minBounds Minimum corner of the box.
     * @param maxBounds Maximum corner of the box.
     * @param t Output parameter for intersection distance.
     * @return True if intersection occurs, false otherwise.
     */
    bool IntersectAABB(const Vec3 &minBounds, const Vec3 &maxBounds, float &t) const;

    /**
     * @brief Tests intersection with a triangle using the Möller-Trumbore algorithm.
     * @param v0 First vertex of the triangle.
     * @param v1 Second vertex of the triangle.
     * @param v2 Third vertex of the triangle.
     * @param t Output parameter for intersection distance.
     * @return True if intersection occurs, false otherwise.
     */
    bool IntersectTriangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, float &t) const;

    /**
     * @brief Tests intersection with a mesh by testing all triangles.
     * @param vertices Vertex data array (position, normal, texcoord per vertex).
     * @param indices Index data array.
     * @param t Output parameter for closest intersection distance.
     * @return True if intersection occurs, false otherwise.
     */
    bool IntersectMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, float &t) const;

    /**
     * @brief Tests intersection with a mesh by testing all triangles, with transformation.
     * @param vertices Vertex data array (position, normal, texcoord per vertex).
     * @param indices Index data array.
     * @param transform Transformation matrix from object local space to world space.
     * @param t Output parameter for closest intersection distance.
     * @return True if intersection occurs, false otherwise.
     */
    bool IntersectMesh(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, const Mat4 &transform, float &t) const;

    /**
     * @brief Gets the origin of the ray.
     * @return The origin point of the ray.
     */
    Vec3 GetOrigin() const;

    /**
     * @brief Gets the direction of the ray.
     * @return The normalized direction vector of the ray.
     */
    Vec3 GetDirection() const;

    /**
     * @brief Calculate the closest point on a line to a ray.
     * @param rayOrigin Origin of the ray.
     * @param rayDirection Direction of the ray.
     * @param linePoint A point on the line.
     * @param lineDirection Direction of the line.
     * @return The closest point on the line to the ray.
     */
    static Vec3 ClosestPointOnLine(const Vec3 &rayOrigin,
                                   const Vec3 &linePoint, const Vec3 &lineDirection);

    /**
     * @brief Calculate the distance from a ray to a line segment.
     * @param rayOrigin Origin of the ray.
     * @param rayDirection Direction of the ray.
     * @param segmentStart Start point of the line segment.
     * @param segmentEnd End point of the line segment.
     * @return The shortest distance between the ray and line segment.
     */
    static float DistanceToLineSegment(const Vec3 &rayOrigin, const Vec3 &rayDirection,
                                       const Vec3 &segmentStart, const Vec3 &segmentEnd);
};
