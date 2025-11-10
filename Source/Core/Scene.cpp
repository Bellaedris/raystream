//
// Created by Bellaedris on 07/10/2025.
//

#include "Scene.h"

namespace ray::core
{
    std::optional<RayHit> ray::core::Scene::Intersect(const ray::core::Ray &ray, float tMin, float tMax) const
    {
        std::optional<RayHit> hit;
        float closest = tMax;
        for (const shape::Sphere &sphere: m_spheres)
        {
            std::optional<RayHit> currentHit = sphere.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        for (const shape::Plane &plane: m_planes)
        {
            std::optional<RayHit> currentHit = plane.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        for (const shape::Triangle &triangle: m_triangles)
        {
            std::optional<RayHit> currentHit = triangle.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        return hit;
    }
}