//
// Created by Bellaedris on 07/10/2025.
//

#pragma once

#include "ColorUtils.h"
#include "Ray.h"
#include "../MonteCarlo/PDF.h"
#include "VectorUtils.h"
#include "Microfacet.h"

#include <optional>

#include <glm/gtc/constants.hpp>

namespace ray::core
{
    /**
     * \brief Small wrapper that contains a scattered ray, the surface BxDF and the associated PDF
     */
    struct ScatterInfos
    {
        Ray m_ray;
        Color m_brdf;
        float m_pdf;

        ScatterInfos(const Ray& ray, const Color& attenuation, float pdf) : m_ray(ray), m_brdf(attenuation), m_pdf(pdf) {}
    };

    class Material
    {
    public:
        #pragma region InnerStruct
        enum MaterialType
        {
            MATERIAL_DIFFUSE    = 1 << 0,
            MATERIAL_METAL      = 1 << 1,
            MATERIAL_DIELECTRIC = 1 << 2,
            MATERIAL_EMISSIVE   = 1 << 3,
            MATERIAL_PBR        = 1 << 4,
        };
        #pragma endregion InnerStruct

        #pragma region MaterialParameters
        // material parameters
        Color m_albedo {};
        Color m_emission {};
        // metallic
        float m_fuzz {};
        // dielectric
        float m_refractiveIndex {0.f};
        // PBR
        float m_roughness {.0f};
        float m_metalness {.0f};
        MaterialType m_type;
        #pragma endregion MaterialParameters

    private:
        template<mc::PDF T>
        std::optional<ScatterInfos> ScatterDiffuse(const Ray& in, const RayHit& hit, const T& distribution) const
        {
            // return a random direction in a hemisphere around normal.
            // we could decide that our ray has a probability to scatter and attenuate based on this too,
            // but this is simpler
            glm::vec3 randDir = distribution.Generate();

            float pdf = distribution.Value(randDir);
            glm::vec3 brdf = (m_albedo * glm::one_over_pi<float>()) * glm::dot(randDir, hit.m_normal);
            return ScatterInfos({hit.m_point, randDir}, brdf, pdf);
        }

        template<mc::PDF T>
        std::optional<ScatterInfos> ScatterCookTorranceDiffuse(const Ray& in, const RayHit& hit, const T& distribution, const glm::vec3& viewDir) const
        {
            // compute the Cook Torrance reflectance, which has 3 parts:
            // diffuse = albedo / PI
            // specular = DFG / (4 * NdotL * NdotV)
            // solid angle attenuation = NdotL
            // with the final equation = (1 - kS) * diffuse + kS * specular * NdotL
            // with Ks is the fresnel term, NdotL the dot between the normal and the light,
            // NdotV the dot between the normal and the view vector, and D F G the product of three functions:
            // - The Normal distribution function D (here, GGX)
            // - The Fresnel term F (here, Schlick's approximation)
            // - The Geometric term G (here, Smith's GGX)
            // return a random direction in a hemisphere around normal.
            glm::vec3 randDir = distribution.Generate();

            float alpha = m_roughness * m_roughness;
            glm::vec3 l = randDir;
            glm::vec3 h = glm::normalize(viewDir + l);

            glm::vec3 diffuse = m_albedo * glm::one_over_pi<float>();

            float NdotL = std::fmax(glm::dot(l, hit.m_normal), .0f);
            float NdotV = std::fmax(glm::dot(viewDir, hit.m_normal), .0f);
            float VdotH = std::fmax(glm::dot(h, viewDir), .0f);

            Color F0 = glm::mix(glm::vec3(.04f), m_albedo, m_metalness);
            glm::vec3 F = Microfacet::SchlickFresnel(VdotH, F0);

            glm::vec3 top = Microfacet::NormalDistributionGGX(h, hit.m_normal, alpha) * Microfacet::GeometricSmith(NdotV, NdotL, hit.m_normal, alpha) * F;
            float bottom = std::fmax(4.f * NdotL * NdotV, VectorUtils::M_EPSILON);
            glm::vec3 specular = top / bottom;

            auto kD = glm::vec3(1.f) - F;
            // note that we don't have to multiply by F before specular, since F is already in the term
            glm::vec3 brdf = (kD * diffuse + specular) * NdotL;

            return ScatterInfos({hit.m_point, randDir}, brdf, distribution.Value(randDir));
        }

        std::optional<ScatterInfos> ScatterMetal(const Ray& in, const RayHit& hit) const;
        std::optional<ScatterInfos> ScatterDielectric(const Ray& in, const RayHit& hit) const;
        std::optional<ScatterInfos> ScatterEmissive(const Ray& in, const RayHit& hit) const;
    public:
        template<mc::PDF T>
        std::optional<ScatterInfos> Scatter(const Ray& in, const RayHit& hit, const T& distribution, const glm::vec3& viewDir) const
        {
            switch (m_type)
            {
                case MATERIAL_DIFFUSE:
                    return ScatterDiffuse(in, hit, distribution);
                case MATERIAL_METAL:
                    return ScatterMetal(in, hit);
                case MATERIAL_DIELECTRIC:
                    return ScatterDielectric(in, hit);
                case MATERIAL_EMISSIVE:
                    return ScatterEmissive(in, hit);
                case MATERIAL_PBR:
                    return ScatterCookTorranceDiffuse(in, hit, distribution, viewDir);
                default:
                    return {};
            }
        }

        #pragma region ConstructionStatics
        static Material CreateDiffuse(const Color& albedo);
        static Material CreatePBRDiffuse(const Color& albedo, float roughness, float metalness);
        static Material CreateMetallic(const Color& albedo, float fuzz);
        static Material CreateDielectric(const Color& albedo, float refractiveIndex);
        static Material CreateEmissive(const Color& emissive);
        #pragma endregion ConstructionStatics
    };
}
