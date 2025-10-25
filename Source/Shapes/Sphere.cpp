//
// Created by Bellaedris on 05/10/2025.
//

#include "Sphere.h"

namespace ray::shape
{
    Sphere::Sphere(const glm::vec3 &center, float radius, const std::shared_ptr<core::Material>& mat)
        : m_center(center)
        , m_radius(radius)
        , m_mat(mat)
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
            float dist = (-b - std::sqrt(delta)) / (2.f * a);
            if(dist < tMin || dist > tMax)
            {
                //handle backface hit
                dist = (-b + std::sqrt(delta)) / (2.f * a);
                if (dist < tMin || dist > tMax)
                    return {};
            }
            glm::vec3 intersect = ray.m_origin + dist * ray.m_direction;
            glm::vec3 normal = (intersect - m_center) / m_radius;
            bool isFrontFace = glm::dot(normal, ray.m_direction) < 0.f;
            return core::RayHit
            {
                .m_distance = dist,
                .m_point = ray.m_origin + dist * ray.m_direction,
                .m_normal = isFrontFace ? normal : -normal,
                .m_IsFrontFace = isFrontFace,
                .m_material = m_mat
            };
        }
        else
        {
            return {};
        }
    }
} // ray::shape