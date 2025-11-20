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
        std::vector<shape::Triangle> m_lights;

    public:
        /**
         * \brief Wrapper to a random point on one of the scene's light and the PDF associated with this point,
         * which is 1.f / (numberOfLights * AreaOfTheChosenLight)
         */
        struct SampledLightInfo
        {
            glm::vec3 sampledPoint;
            glm::vec3 sampledNormal;
            float pdf;
            std::shared_ptr<Material> mat;
        };

        /**
         * \param primitive A primitive to check intersection with
         * \param tMin minimal intersection distance
         * \param tMax maximal intersection distance
         * \return A struct filled with infos about the hit, or nothing
         */
        [[nodiscard]] std::optional<RayHit> Intersect(const Ray &ray, float tMin, float tMax) const;

        inline void AddSphere(const shape::Sphere& sphere) { m_spheres.push_back(sphere); };
        inline void AddPlane(const shape::Plane& plane) { m_planes.push_back(plane); };

        /**
         * \brief Adds a triangle to the scene. If the triangle is emissive, also add it to the list of lights.
         * TODO eliminate duplication of triangle (have a vector of shared_ptr, for instance)
         * \param triangle a triangle
         */
        void AddTriangle(const shape::Triangle& triangle);
        inline void AddLight(const shape::Triangle& light) { m_lights.push_back(light); };

        [[nodiscard]] SampledLightInfo GetSampleOnRandomLight() const;

        [[nodiscard]] inline const std::vector<shape::Triangle>& GetLights() const { return m_lights; };
    };
}