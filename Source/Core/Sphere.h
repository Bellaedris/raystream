//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/vec3.hpp>
#include <optional>
#include "Ray.h"

namespace ray::core
{
    class Sphere
    {
    private:
        glm::vec3 m_center;
        float m_radius;

    public:
        Sphere(const glm::vec3& center, float radius);
        [[nodiscard]] std::optional<RayHit> Intersect(const Ray& ray) const;
    };
} // ray::core