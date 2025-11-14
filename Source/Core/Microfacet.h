//
// Created by Bellaedris on 11/11/2025.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ray::core
{
/**
 * \brief Helper functions for Cook-Torrance microfacets reflectance
 */
class Microfacet
{
public:
    /**
     * \brief Schlick's approximation of the fresnel term, that represents how much light will be refracted or diffracted for a dielectric material
     * \param indiceOfRefraction indice of refraction of the medium. If going from the medium to air, use 1/indiceOfRefraction as parameter
     * \param cosTheta Half of the normal, reflect direction. In fact, it's just the angle between the incoming ray and the normal
     * \return The fresnel term
     */
    [[nodiscard]] static glm::vec3 SchlickFresnel(float cosTheta, glm::vec3 r0);

    /**
     * \brief GGX Normal distribution function from https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
     * \param h half view vector
     * \param n normal
     * \param roughness remapped roughness of the material
     * \return Distribution of microfacets that are aligned with the normal
     */
    static float NormalDistributionGGX(const glm::vec3 &h, const glm::vec3 &n, float roughness);

    /**
     * \brief Schlick-GGX geometric shadowing term https://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
     * \param dot normal dot either light or view
     * \param roughness remapped roughness of the material
     * \return the amount of shadowing from neighboring microfacets from one direction (either light or view)
     */
    static float GeometricSchlickGGX(float dot, float roughness);

    /**
     * \brief Smith Geometric shadow term that combines both light and view obstruction
     * \param NdotV view dir
     * \param NdotL light dir (in our case, the incident ray)
     * \param n normal
     * \param alpha remapped roughness of the material
     * \return The amount of shadowing from neighboring microfacets
     */
    static float GeometricSmith(float NdotV, float NdotL, const glm::vec3& n, float alpha);
};

}