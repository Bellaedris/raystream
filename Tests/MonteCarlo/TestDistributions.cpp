//
// Created by Bellaedris on 10/11/2025.
//

#include "TestDistributions.h"

namespace ray::test::mc
{
    /**
     * \brief Uniform distribution should always give directions in the top hemisphere with a norm of 1
     */
    TEST_F(TestDistributions, TestUniformDirection)
    {
        float sampleAmount = 10000.f;
        float dotSum = .0f;
        for(int i = 0; i < sampleAmount; i++)
        {
            glm::vec3 dir = m_uniform.Generate();
            float dot = glm::dot(dir, m_normal);
            dotSum += dot;
            ASSERT_TRUE(glm::epsilonEqual(glm::length2(dir), 1.f, core::VectorUtils::M_EPSILON));
            ASSERT_TRUE(glm::dot(dir, m_normal) > .0f);
        }
        std::cerr << "Uniform average randomDir dot normal: " << dotSum / sampleAmount << std::endl;
        ASSERT_TRUE(glm::epsilonEqual(dotSum / sampleAmount, .5f, .01f));
    }

    /**
     * \brief Uniform distribution should always give directions in the top hemisphere with a norm of 1.
     * On average, we should have more directions closer to the normal, which translates to an average dot product > .5
     */
    TEST_F(TestDistributions, TestCosWeightedDirection)
    {
        float sampleAmount = 10000.f;
        float dotSum = .0f;
        for(int i = 0; i < sampleAmount; i++)
        {
            glm::vec3 dir = m_cosineWeighted.Generate();
            float dot = glm::dot(dir, m_normal);
            dotSum += dot;
            ASSERT_TRUE(glm::epsilonEqual(glm::length2(dir), 1.f, core::VectorUtils::M_EPSILON));
            ASSERT_TRUE(dot > .0f);
        }
        std::cerr << "Cosine Weighted average randomDir dot normal: " << dotSum / sampleAmount << std::endl;
        ASSERT_TRUE(dotSum / sampleAmount > .5);
    }
}