//
// Created by Bellaedris on 05/10/2025.
//

#include "Image.h"
#include <stb_image_write.h>

namespace ray::core
{
    Image::Image(int width, int height)
        : m_pixels(width * height, glm::vec3(0.f, 0.f, 0.f))
          , m_width(width)
          , m_height(height)
    {

    }

    bool Image::Write(const std::string& name) const
    {
        return stbi_write_png(name.c_str(), m_width, m_height, 3, m_pixels.data(), 3 * m_width) != 0;
    }

    void Image::SetPixel(int x, int y, ColorRGB color)
    {
        m_pixels[Index(x, y)] = color;
    }

    ColorRGB Image::GetPixel(int x, int y) const
    {
        return m_pixels.at(Index(x, y));
    }

    int Image::Index(int x, int y) const
    {
        return m_width * y + x;
    }
} // ray