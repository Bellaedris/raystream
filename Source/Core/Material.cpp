//
// Created by Bellaedris on 17/10/2025.
//

#include "Material.h"
#include "Ray.h"
#include "RandomUtils.h"

#include <glm/gtc/random.hpp>

namespace ray::core
{
    std::optional<RayAttenuation> Material::Scatter(const Ray& in, const RayHit& hit)
    {
        switch (m_type)
        {
            case MATERIAL_DIFFUSE:
                return ScatterDiffuse(in, hit);
            case MATERIAL_METAL:
                return ScatterMetal(in, hit);
            case MATERIAL_DIELECTRIC:
                return ScatterDielectric(in, hit);
            case MATERIAL_EMISSIVE:
                return ScatterEmissive(in, hit);
            default:
                return {};
        }
    }

    std::optional<RayAttenuation> Material::ScatterDiffuse(const Ray& in, const RayHit& hit)
    {
        // return a random direction in a hemisphere around normal.
        // we could decide that our ray has a probability to scatter and attenuate based on this too,
        // but this is simpler
        glm::vec3 randDir = glm::sphericalRand(1.f);
        if(glm::dot(randDir, hit.m_normal) < 0)
            randDir = -randDir;
        return RayAttenuation({hit.m_point, randDir}, m_albedo);
    }

    std::optional<RayAttenuation> Material::ScatterMetal(const Ray& in, const RayHit& hit)
    {
        // reflect + fuzz.
        glm::vec3 newDir = glm::reflect(in.m_direction, hit.m_normal);
        // add fuzzing, which can cause the ray to end up pointing below the surface, in which case we will terminate the ray
        glm::vec3 fuzzedReflect = newDir + RNG::Vector3(-1.f, 1.f) * m_fuzz;
        if(glm::dot(fuzzedReflect, hit.m_normal) < 0)
            return {};
        else
            return RayAttenuation({hit.m_point, fuzzedReflect}, m_albedo);
    }

    std::optional<RayAttenuation> Material::ScatterDielectric(const Ray& in, const RayHit& hit)
    {
        // diffract with all the complicated things and all
        return std::optional<RayAttenuation>();
    }

    std::optional<RayAttenuation> Material::ScatterEmissive(const Ray& in, const RayHit& hit)
    {
        // should terminate the ray
        return {};
    }

    Material Material::CreateDiffuse(const Color &albedo)
    {
        Material diffuse
        {
            .m_albedo = {albedo},
            .m_type = MATERIAL_DIFFUSE
        };
        return diffuse;
    }

    Material Material::CreateMetallic(const Color &albedo, float fuzz)
    {
        Material metallic
        {
            .m_albedo = {albedo},
            .m_fuzz = fuzz,
            .m_type = MATERIAL_METAL
        };
        return metallic;
    }

    Material Material::CreateEmissive(const Color &emissive)
    {
        Material emissiveMat
        {
            .m_albedo = {.0f, .0f, .0f},
            .m_emission = emissive,
            .m_type = MATERIAL_EMISSIVE
        };
        return emissiveMat;
    }
}