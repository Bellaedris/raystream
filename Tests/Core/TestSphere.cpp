//
// Created by Bellaedris on 05/10/2025.
//

#include "TestSphere.h"

namespace ray::test::core
{
    TEST_F(TestSphere, TestIntersection)
    {
        ray::core::Ray r({.0f, .0f, -2.f}, {.0f, .0f, 1.f});
        std::optional<ray::core::RayHit> hit = m_testSphere.Intersect(r);
        ASSERT_TRUE(hit.has_value());

        ASSERT_EQ(hit->m_distance, 1.f);
    }
}