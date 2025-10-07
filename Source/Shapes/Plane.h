//
// Created by Bellaedris on 07/10/2025.
//

#pragma once

#include <optional>
#include <glm/glm.hpp>
#include "../Core/Ray.h"
#include "../Core/Material.h"

namespace ray::shape
{
    class Plane
    {
    private:
        glm::vec3 m_origin;
        glm::vec3 m_normal;
        core::Material m_mat;

    public:
        Plane(const glm::vec3& origin, const glm::vec3& normal, const core::Material& mat);

        std::optional<core::RayHit> Intersect(const core::Ray& ray, float tMin, float tMax) const;

        inline core::Material GetMaterial() const { return m_mat; };
    };
}