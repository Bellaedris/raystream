//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <gtest/gtest.h>
#include "../../Source/Core/Image.h"

// read this https://google.github.io/googletest/primer.html

namespace ray::test::core
{
    class TestImage : public testing::Test
    {
    protected:
        ray::core::Image m_testImage;

        TestImage()
            : m_testImage(800, 600)
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