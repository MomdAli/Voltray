#pragma once

#include "vec3.h"

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
};
