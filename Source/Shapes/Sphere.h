//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/vec3.hpp>
#include <optional>
#include "../Core/Ray.h"
#include "../Core/Material.h"

namespace ray::shape
{
    class Sphere
    {
    private:
        glm::vec3 m_center;
        float m_radius;
        std::shared_ptr<core::Material> m_mat;

    public:
        Sphere(const glm::vec3& center, float radius, const std::shared_ptr<core::Material>& mat);
        [[nodiscard]] std::optional<core::RayHit> Intersect(const core::Ray& ray, float tMin, float tMax) const;

        inline std::shared_ptr<core::Material> GetMaterial() const { return m_mat; };
    };
} // ray::shape