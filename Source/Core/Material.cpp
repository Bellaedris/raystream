//
// Created by Bellaedris on 17/10/2025.
//

#include "Material.h"
#include "Ray.h"
#include "RandomUtils.h"

#include <glm/gtc/random.hpp>

namespace ray::core
{
    // std::optional<RayAttenuation> Material::ScatterDiffuse(const Ray& in, const RayHit& hit)
    // {
    //     // return a random direction in a hemisphere around normal.
    //     // we could decide that our ray has a probability to scatter and attenuate based on this too,
    //     // but this is simpler
    //     glm::vec3 randDir = RNG::Vector3(-1.f, 1.f);
    //     if(glm::dot(randDir, hit.m_normal) < 0)
    //         randDir = -randDir;
    //     return RayAttenuation({hit.m_point, randDir}, m_albedo);
    // }

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

        // schlick fresnel
        float cosTheta = glm::dot(-in.m_direction, hit.m_normal);
        float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);
        float reflectance = SchlickFresnel(std::max(.0f, cosTheta));

        float refractionCoefficient = hit.m_IsFrontFace ? (1.f / m_refractiveIndex) : m_refractiveIndex;

        // if we look back at snell descartes formula, it is possible that we find no solution for our equation (if sintheta1 is greater than 1)
        // in that case, we reflect instead of refracting
        if(RNG::Float() < reflectance || refractionCoefficient * sinTheta > 1.f)
        {
            // reflect
            return RayAttenuation({hit.m_point, glm::reflect(in.m_direction, hit.m_normal)}, m_albedo);
        }
        else
        {
            // refract
            glm::vec3 rparallel = refractionCoefficient * (in.m_direction + cosTheta * hit.m_normal);
            float sin2theta2 = refractionCoefficient * refractionCoefficient * (1.f - cosTheta * cosTheta);
            glm::vec3 rortho = -std::sqrt(1.f - sin2theta2) * hit.m_normal;
            return RayAttenuation({hit.m_point, rortho + rparallel}, m_albedo);
        }
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

    Material Material::CreateDielectric(const Color& albedo, float refractiveIndex)
    {
        Material dielectric
        {
            .m_albedo = albedo,
            .m_refractiveIndex = refractiveIndex,
            .m_type = MATERIAL_DIELECTRIC
        };
        return dielectric;
    }

    float Material::SchlickFresnel(float cosTheta)
    {
        float r0 = (1.f - m_refractiveIndex) / (1.f + m_refractiveIndex);
        r0 = r0 * r0;

        return r0 + (1.f - r0) * std::pow((1.f - cosTheta), 5.f);
    }
}