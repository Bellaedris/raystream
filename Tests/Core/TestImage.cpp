//
// Created by Bellaedris on 05/10/2025.
//

#include "TestImage.h"

namespace ray::test::core
{
    TEST_F(TestImage, TestWriteDefaultImage)
    {
        ASSERT_EQ(m_testImage.Write("test"), true);
    }

    TEST_F(TestImage, TestDefaultColor)
    {
        ASSERT_EQ(glm::all(glm::equal(m_testImage.GetPixel(0, 0), glm::vec<3, unsigned char>(.0f, .0f, .0f))), true);
    }
}