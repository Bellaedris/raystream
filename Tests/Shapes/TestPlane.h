//
// Created by Bellaedris on 15/10/2025.
//

#pragma once

#include <gtest/gtest.h>
#include "../../Source/Shapes/Plane.h"
#include "../../Source/Core/Ray.h"

// read this https://google.github.io/googletest/primer.html

namespace ray::test::core
{
    class TestPlane : public testing::Test
    {
    protected:
        shape::Plane m_testPlane;

        TestPlane()
                : m_testPlane({.0f, .0f, .0f}, {.0f, .0f, 1.f}, nullptr)
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