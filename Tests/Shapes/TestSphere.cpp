//
// Created by Bellaedris on 05/10/2025.
//

#include "TestSphere.h"

namespace ray::test::core
{
    /**
     * \brief hitting the middle of the sphere gives correct hit, distance, normal, face information
     */
    TEST_F(TestSphere, TestIntersection)
    {
        ray::core::Ray r({.0f, .0f, -2.f}, {.0f, .0f, 1.f});
        std::optional<ray::core::RayHit> hit = m_testSphere.Intersect(r, 0.f, 1000.f);
        ASSERT_TRUE(hit.has_value());

        ASSERT_EQ(hit->m_distance, 1.f);
        ASSERT_TRUE(hit->m_normal == glm::vec3(.0f, .0f, -1.f));
        ASSERT_TRUE(hit->m_IsFrontFace);
    }

    /**
     * \brief hitting on the tangent hits correctly
     */
    TEST_F(TestSphere, TestIntersectionTangent)
    {
        ray::core::Ray r({.0f, .0f, -2.f}, {.0f, 1.f, 2.f});
        std::optional<ray::core::RayHit> hit = m_testSphere.Intersect(r, 0.f, 1000.f);
        ASSERT_TRUE(hit.has_value());
    }

    /**
     * \brief Sending a ray in the opposite direction of the sphere yields no hit
     */
    TEST_F(TestSphere, TestNoIntersect)
    {
        ray::core::Ray r({.0f, .0f, -2.f}, {.0f, .0f, -1.f});
        std::optional<ray::core::RayHit> hit = m_testSphere.Intersect(r, 0.f, 1000.f);
        ASSERT_FALSE(hit.has_value());
    }

    /**
     * \brief Sending a ray through the inside of the sphere yields a correct hit
     */
    TEST_F(TestSphere, TestBackface)
    {
        ray::core::Ray r({.0f, .0f, .0f}, {.0f, .0f, 1.f});
        std::optional<ray::core::RayHit> hit = m_testSphere.Intersect(r, 0.f, 1000.f);
        ASSERT_TRUE(hit.has_value());

        ASSERT_EQ(hit->m_distance, 1.f);
        ASSERT_TRUE(hit->m_normal == glm::vec3(.0f, .0f, -1.f));
        ASSERT_FALSE(hit->m_IsFrontFace);
    }
}