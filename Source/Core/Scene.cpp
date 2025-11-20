//
// Created by Bellaedris on 07/10/2025.
//

#include <random>
#include "Scene.h"

namespace ray::core
{
    std::optional<RayHit> ray::core::Scene::Intersect(const ray::core::Ray &ray, float tMin, float tMax) const
    {
        std::optional<RayHit> hit;
        float closest = tMax;
        for (const shape::Sphere &sphere: m_spheres)
        {
            std::optional<RayHit> currentHit = sphere.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        for (const shape::Plane &plane: m_planes)
        {
            std::optional<RayHit> currentHit = plane.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        for (const shape::Triangle &triangle: m_triangles)
        {
            std::optional<RayHit> currentHit = triangle.Intersect(ray, tMin, closest);
            if (currentHit.has_value())
            {
                closest = currentHit->m_distance;
                hit = currentHit;
            }
        }

        return hit;
    }

    void Scene::AddTriangle(const shape::Triangle &triangle)
    {
        m_triangles.push_back(triangle);
        // add any emissive triangle to the list of lamps
        if(glm::any(glm::greaterThan(triangle.GetMaterial()->m_emission, glm::vec3(.0f))))
        {
            m_lights.push_back(triangle);
        }
    }

    Scene::SampledLightInfo Scene::GetSampleOnRandomLight() const
    {
        thread_local std::mt19937 generator(std::random_device{}());
        thread_local std::uniform_int_distribution<size_t> distribution(0, m_lights.size() - 1);

        thread_local std::uniform_real_distribution<float> floatDistrib(.0f, 1.f);

        shape::Triangle light = m_lights[distribution(generator)];

        // random point in triangle, thanks https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/html/group__mcdirect.html
        float u1 = floatDistrib(generator);
        float u2 = floatDistrib(generator);

        float b0 = u1 * .5f;
        float b1 = u2 * .5f;
        float offset = b1 - b0;
        if(offset > 0)
            b1= b1 + offset;
        else
            b0= b0 - offset;
        float b2 = 1.f - b0 - b1;

        return {
            .sampledPoint = light.GetA() * b0 + light.GetB() * b1 + light.GetC() * b2,
            .sampledNormal = light.GetNormal(),
            .pdf = 1.f / (static_cast<float>(m_lights.size()) * light.GetArea()),
            .mat = light.GetMaterial()
        };
    }
}