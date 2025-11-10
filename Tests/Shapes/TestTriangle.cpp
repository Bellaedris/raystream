//
// Created by Bellaedris on 10/11/2025.
//

#include "TestTriangle.h"

namespace ray::test::core
{
    /**
     * \brief hitting the middle of the sphere gives correct hit, distance, normal, face information
     */
    TEST_F(TestTriangle, TestIntersection)
    {
        ray::core::Ray r({.5f, .5f, 1.f}, {.0f, .0f, -1.f});
        std::optional<ray::core::RayHit> hit = m_testTriangle.Intersect(r, 0.f, 1000.f);
        ASSERT_TRUE(hit.has_value());

        ASSERT_EQ(hit->m_distance, 1.f);
        ASSERT_TRUE(hit->m_normal == glm::vec3(.0f, .0f, 1.f));
        ASSERT_TRUE(hit->m_IsFrontFace);
    }

    /**
     * \brief Sending a ray in the opposite direction of the sphere yields no hit
     */
    TEST_F(TestTriangle, TestNoIntersect)
    {
        ray::core::Ray r({.5f, .5f, -2.f}, {.0f, .0f, -1.f});
        std::optional<ray::core::RayHit> hit = m_testTriangle.Intersect(r, 0.f, 1000.f);
        ASSERT_FALSE(hit.has_value());
    }

    /**
     * \brief Sending a ray through the back of the plane gives a valid hit and a reversed normal
     */
    TEST_F(TestTriangle, TestBackface)
    {
        ray::core::Ray r({.5f, .5f, -1.f}, {.0f, .0f, 1.f});
        std::optional<ray::core::RayHit> hit = m_testTriangle.Intersect(r, 0.f, 1000.f);
        ASSERT_TRUE(hit.has_value());

        ASSERT_EQ(hit->m_distance, 1.f);
        ASSERT_TRUE(hit->m_normal == glm::vec3(.0f, .0f, -1.f));
        ASSERT_FALSE(hit->m_IsFrontFace);
    }
}