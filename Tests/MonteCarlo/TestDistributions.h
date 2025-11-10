//
// Created by Bellaedris on 10/11/2025.
//

#pragma once

#include <gtest/gtest.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtc/epsilon.hpp>

#include "../../Source/MonteCarlo/Uniform.h"
#include "../../Source/MonteCarlo/CosineWeighted.h"

namespace ray::test::mc
{
    class TestDistributions : public testing::Test
    {
    protected:
        ray::mc::Uniform m_uniform;
        ray::mc::CosineWeighted m_cosineWeighted;
        glm::vec3 m_normal;

        TestDistributions()
                : m_uniform({.0f, 1.f, .0f})
                , m_cosineWeighted({.0f, 1.f, .0f})
                , m_normal({.0f, 1.f, .0f})
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