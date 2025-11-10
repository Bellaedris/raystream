//
// Created by Bellaedris on 10/11/2025.
//

#pragma once

#include <gtest/gtest.h>
#include "../../Source/Shapes/Triangle.h"
#include "../../Source/Core/Ray.h"

// read this https://google.github.io/googletest/primer.html

namespace ray::test::core
{
    class TestTriangle : public testing::Test
    {
    protected:
        shape::Triangle m_testTriangle;

        TestTriangle()
                : m_testTriangle({.0f, .0f, .0f}, {1.f, .0f, .0f}, {1.f, 1.f, .0f}, nullptr)
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