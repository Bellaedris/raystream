//
// Created by Bellaedris on 10/11/2025.
//

#include "Triangle.h"
#include "../Core/VectorUtils.h"

namespace ray::shape
{
ray::shape::Triangle::Triangle(
        const glm::vec3 &a,
        const glm::vec3 &b,
        const glm::vec3 &c,
        const std::shared_ptr<core::Material> &mat
)
    : m_a(a)
    , m_b(b)
    , m_c(c)
    , m_material(mat)
{
    glm::vec3 cross = glm::cross(b - a, c - a);
    m_normal = glm::normalize(cross);
    m_area = glm::length(cross) * .5f;
}

std::optional<core::RayHit> ray::shape::Triangle::Intersect(const ray::core::Ray &ray, float tMin, float tMax) const
{
    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = m_b - m_a;
    edge2 = m_c - m_a;
    h = glm::cross(ray.m_direction, edge2);
    a = glm::dot(edge1, h);
    if (a > -core::VectorUtils::M_EPSILON && a < core::VectorUtils::M_EPSILON)
        return {};

    f = 1.f / a;
    s = ray.m_origin - m_a;
    u = f * (glm::dot(s, h));
    if (u < 0.0 || u > 1.0)
        return {};
    q = glm::cross(s, edge1);
    v = f * glm::dot(ray.m_direction, q);
    if (v < 0.0 || u + v > 1.0)
        return {};

    float t = f * glm::dot(edge2, q);
    if (t > tMin && t < tMax)
    {
        bool isFrontFace = glm::dot(ray.m_direction, m_normal) < .0f;
        return core::RayHit
        {
            .m_distance = t,
            .m_point = ray.m_origin + ray.m_direction * t,
            .m_normal = isFrontFace ? m_normal : -m_normal,
            .m_IsFrontFace = isFrontFace,
            .m_material = m_material
        };
    }
    else
        return {};
}
}