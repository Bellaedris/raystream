//
// Created by Bellaedris on 07/10/2025.
//

#pragma once

#include <vector>
#include "../Shapes/Sphere.h"
#include "../Shapes/Plane.h"
#include "../Shapes/Triangle.h"
#include "Hittable.h"
#include "PointLight.h"

namespace ray::core
{
    class Scene
    {
    private:
        std::vector<shape::Sphere> m_spheres;
        std::vector<shape::Plane> m_planes;
        std::vector<shape::Triangle> m_triangles;
        std::vector<PointLight> m_lights;

    public:
        /**
         * \brief Just a hack to try and avoid inheritance pointer table by using templates
         * \tparam Primitive An type that defines an Intersect(const Ray& ray)
         * \param ray A ray
         * \param primitive A primitive to check intersection with
         * \return A struct filled with infos about the hit, or nothing
         */
        [[nodiscard]] std::optional<RayHit> Intersect(const Ray &ray, float tMin, float tMax) const;

        inline void AddSphere(const shape::Sphere& sphere) { m_spheres.push_back(sphere); };
        inline void AddPlane(const shape::Plane& plane) { m_planes.push_back(plane); };
        inline void AddTriangle(const shape::Triangle& triangle) { m_triangles.push_back(triangle); };
        inline void AddLight(const PointLight& light) { m_lights.push_back(light); };

        [[nodiscard]] inline const std::vector<PointLight>& GetLights() const { return m_lights; };
    };
}