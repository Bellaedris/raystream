//
// Created by Bellaedris on 10/11/2025.
//

#pragma once

#include <memory>
#include <optional>

#include <glm/vec3.hpp>

#include "../Core/Material.h"

namespace ray::shape
{

class Triangle
{
private:
    glm::vec3 m_a, m_b, m_c;
    glm::vec3 m_normal;
    std::shared_ptr<core::Material> m_material;

    float m_area;
public:
    Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const std::shared_ptr<core::Material>& mat);

    /**
     * \brief Moller-trumbore ray-triangle intersection https://fr.wikipedia.org/wiki/Algorithme_d%27intersection_de_M%C3%B6ller-Trumbore
     * \param ray input ray
     * \param tMin minimal intersection distance
     * \param tMax maximal intersection distance
     * \return a struct describing the hit if there is one, nullopt otherwise
     */
    std::optional<core::RayHit> Intersect(const core::Ray& ray, float tMin, float tMax) const;

    inline glm::vec3 GetA() const { return m_a; };
    inline glm::vec3 GetB() const { return m_b; };
    inline glm::vec3 GetC() const { return m_c; };
    inline glm::vec3 GetNormal() const { return m_normal; };
    inline float GetArea() const { return m_area; };
    inline std::shared_ptr<core::Material> GetMaterial() const { return m_material; };
};
}