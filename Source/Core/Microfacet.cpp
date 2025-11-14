//
// Created by Bellaedris on 11/11/2025.
//

#include <cmath>

#include "Microfacet.h"

namespace ray::core
{

    glm::vec3 Microfacet::SchlickFresnel(float cosTheta, glm::vec3 r0)
    {
        return r0 + (1.f - r0) * std::pow((1.f - cosTheta), 5.f);
    }

    float Microfacet::NormalDistributionGGX(const glm::vec3 &h, const glm::vec3 &n, float roughness)
    {
        float a = roughness*roughness;
        float a2 = a * a;
        float NdotH = std::fmax(glm::dot(n, h), .0f);

        float denom = NdotH * NdotH * (a2 - 1.f) + 1.f;
        denom = glm::pi<float>() * denom * denom;

        return a2 / denom;
    }

    float Microfacet::GeometricSchlickGGX(float dot, float roughness)
    {
        float r = (roughness + 1.f);
        float k = (r * r) / 8.f;

        return dot / (dot * (1.f - k) + k);
    }

    float Microfacet::GeometricSmith(float NdotV, float NdotL, const glm::vec3 &n, float alpha)
    {
        return GeometricSchlickGGX(NdotV, alpha) * GeometricSchlickGGX(NdotL, alpha);
    }


}