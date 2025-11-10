//
// Created by bellaedris on 27/10/25.
//

#pragma once
#include <random>
#include <glm/gtc/constants.hpp>

#include "PDF.h"
#include "../Core/VectorUtils.h"

namespace ray::mc
{
    class Uniform
    {
    private:
        glm::vec3 m_normal;
        /**
         * With uniform distribution, the PDF depends on nothing. We can precompute it.
         */
        float m_pdf;
    public:
        Uniform(const glm::vec3& normal) : m_normal(normal), m_pdf(glm::one_over_two_pi<float>()) {}

        float Value(const glm::vec3& dir) const
        {
            return m_pdf;
        }

        /**
         * Computes A random direction around the unit sphere, then transform it to world space
         * @return A random direction in the hemisphere centered at m_normal
         */
        glm::vec3 Generate() const
        {
            static std::random_device rd;
            static std::mt19937 generator(rd());
            static std::uniform_real_distribution<float> distribution(.0f, 1.f);

            float u = distribution(generator);
            float v = distribution(generator);

            float phi = 2.f * glm::pi<float>() * u;
            float theta = std::sqrt(1 - v * v);

            glm::vec3 dir = {theta * std::cos(phi), v, theta * std::sin(phi)};

            return core::VectorUtils::FrisvadBasis(m_normal) * dir;
        }
    };
}
