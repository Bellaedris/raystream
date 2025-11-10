//
// Created by bellaedris on 27/10/25.
//

#pragma once
#include <random>
#include <numbers>
#include <glm/gtc/constants.hpp>

#include "PDF.h"

namespace ray::mc
{
    class CosineWeighted
    {
    private:
        glm::vec3 m_normal;
    public:
        CosineWeighted(const glm::vec3& normal) : m_normal(normal) {}

        float Value(const glm::vec3& dir) const
        {
            return std::max(glm::dot(m_normal, dir), 0.f) * glm::one_over_pi<float>();
        }

        /**
         * Computes a random, cosine-biased direction around the unit sphere, then transform it to world space
         * @return A random cosine-biased direction in the hemisphere centered at m_normal
         */
        glm::vec3 Generate() const
        {
            // TODO refactor to use pseudo random instead, for faster generation
            static std::random_device rd;
            static std::mt19937 generator(rd());
            static std::uniform_real_distribution<float> distribution(.0f, 1.f);

            float u = distribution(generator);
            float v = distribution(generator);

            float phi = 2.f * glm::pi<float>() * u;
            float theta = std::sqrt(1 - v);

            glm::vec3 dir = {theta * std::cos(phi), std::sqrt(v), theta * std::sin(phi)};

            return core::VectorUtils::FrisvadBasis(m_normal) * dir;
        }
    };
}
