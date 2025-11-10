//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <gtest/gtest.h>
#include "../../Source/Shapes/Sphere.h"
#include "../../Source/Core/Ray.h"

// read this https://google.github.io/googletest/primer.html

namespace ray::test::core
{
    class TestSphere : public testing::Test
    {
    protected:
        shape::Sphere m_testSphere;

        TestSphere()
                : m_testSphere({.0f, .0f, .0f}, 1.f, nullptr)
        {
        }

        void SetUp() override
        {

        }

        void TearDown() override
        {

        }
    };
}