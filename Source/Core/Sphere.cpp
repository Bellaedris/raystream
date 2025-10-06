//
// Created by Bellaedris on 05/10/2025.
//

#include "Sphere.h"

namespace ray::core
{
    Sphere::Sphere(const glm::vec3 &center, float radius)
        : m_center(center)
        , m_radius(radius)
    {

    }

    std::optional<ray::core::RayHit> Sphere::Intersect(const ray::core::Ray &ray, float tMin, float tMax) const
    {
        glm::vec3 oc = m_center - ray.m_origin;
        float a = glm::dot(ray.m_direction, ray.m_direction);
        float b = -2.f * glm::dot(ray.m_direction, oc);
        float c = glm::dot(oc, oc) - m_radius * m_radius;

        float delta = b * b - 4 * a * c;
        if(delta >= 0)
        {
            float dist = (-b - std::sqrtf(delta)) / (2 * a);
            if(dist < tMin || dist > tMax)
                return {};
            glm::vec3 intersect = ray.m_origin + dist * ray.m_direction;
            bool isFrontFace = glm::dot(-oc, ray.m_direction) > 0.f;
            return RayHit
            {
                .m_distance = dist,
                .m_point = ray.m_origin + dist * ray.m_direction,
                .m_normal = isFrontFace ? glm::normalize(intersect - m_center) : -glm::normalize(intersect - m_center),
                .m_IsFrontFace = isFrontFace
            };
        }
        else
        {
            return {};
        }
    }
} // ray::core