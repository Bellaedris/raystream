//
// Created by Bellaedris on 05/10/2025.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace ray::core
{
    class Image
    {
        using Color = glm::vec<3, unsigned char>;
    private:
        std::vector<Color> m_pixels;
        int m_height, m_width;

    public:
        Image(int width, int height);

        /**
         * \brief Write the image to a file, to png format
         * \param name name of the file to write the image to
         * \return true if the image was written correctly, false if an error occured
         */
        bool Write(const std::string& name) const;

        /**
         * \brief write into the array of data
         * \param x row
         * \param y column
         */
        void SetPixel(int x, int y, Color color);

        Color GetPixel(int x, int y) const;

        int Index(int x, int y) const;
    };
} // ray