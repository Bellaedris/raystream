//
// Created by Bellaedris on 07/10/2025.
//

#include "Plane.h"

namespace ray::shape
{
    ray::shape::Plane::Plane(const glm::vec3 &origin, const glm::vec3 &normal, const core::Material& mat)
        : m_origin(origin)
        , m_normal(normal)
        , m_mat(mat)
    {

    }

    std::optional<core::RayHit> ray::shape::Plane::Intersect(const ray::core::Ray &ray, float tMin, float tMax) const
    {
        float top = glm::dot(m_normal, (m_origin - ray.m_origin));
        float bottom = glm::dot(ray.m_direction, m_normal);

        if(std::abs(bottom) > 1e-6)
        {
            float t = top / bottom;

            if(t > tMin && t < tMax)
            {
                bool isFrontFace = glm::dot(ray.m_direction, m_normal) < .0f;
                return core::RayHit
                {
                    .m_distance = t,
                    .m_point = ray.m_origin + t * ray.m_direction,
                    .m_normal = isFrontFace ? m_normal : -m_normal,
                    .m_IsFrontFace = isFrontFace,
                    .m_material = m_mat
                };
            }
        }

        return {};
    }
}