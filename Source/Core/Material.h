//
// Created by Bellaedris on 07/10/2025.
//

#pragma once
#include "ColorUtils.h"
#include "Ray.h"

#include <optional>

namespace ray::core
{
    /**
     * \brief Small wrapper that contains a ray and an attenuation value
     */
    struct RayAttenuation
    {
        Ray m_ray;
        Color m_attenuation;

        RayAttenuation(const Ray& ray, const Color& attenuation) : m_ray(ray), m_attenuation(attenuation) {}
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
            MATERIAL_EMISSIVE   = 1 << 3
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
        MaterialType m_type;
        #pragma endregion MaterialParameters

    private:
        std::optional<RayAttenuation> ScatterDiffuse(const Ray& in, const RayHit& hit);
        std::optional<RayAttenuation> ScatterMetal(const Ray& in, const RayHit& hit);
        std::optional<RayAttenuation> ScatterDielectric(const Ray& in, const RayHit& hit);
        std::optional<RayAttenuation> ScatterEmissive(const Ray& in, const RayHit& hit);

        /**
         * \brief Schlick's approximation of the fresnel term, that represents how much light will be refracted or diffracted for a dielectric material
         * \param indiceOfRefraction indice of refraction of the medium. If going from the medium to air, use 1/indiceOfRefraction as parameter
         * \param cosTheta Half of the normal, reflect direction. In fact, it's just the angle between the incoming ray and the normal
         * \return The fresnel term
         */
        [[nodiscard]] float SchlickFresnel(float cosTheta);
    public:
        std::optional<RayAttenuation> Scatter(const Ray& in, const RayHit& hit);

        #pragma region ConstructionStatics
        static Material CreateDiffuse(const Color& albedo);
        static Material CreateMetallic(const Color& albedo, float fuzz);
        static Material CreateDielectric(const Color& albedo, float refractiveIndex);
        static Material CreateEmissive(const Color& emissive);
        #pragma endregion ConstructionStatics
    };
}
