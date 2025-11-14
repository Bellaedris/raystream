//
// Created by Bellaedris on 07/10/2025.
//

#pragma once
#include <glm/glm.hpp>

namespace ray::core
{
    using ColorRGB = glm::vec<3, unsigned char>;
    using Color = glm::vec3;

    class ColorUtils
    {
    public:
        /**
         * \brief Converts a floating point color to an RGB [0; 255] color
         * \param col A floating point color value
         * \return a representation of the color on a single byte per channel
         */
        static ColorRGB ColorToRGB(const Color& col)
        {
            return {
                static_cast<unsigned char>(col.x * 255.f)
                 , static_cast<unsigned char>(col.y * 255.f)
                 , static_cast<unsigned char>(col.z * 255.f)
            };
        }

        /**
         * \brief Convert a Color in floating point space to a color in Gamma space
         * \param col the color
         * \param gamma the factor. Defaults to 2.2 as it is a standard factor for gamma space in most devices
         * \return the color in gamma space
         */
        static Color ColorToGamma(const Color& col, float gamma)
        {
            return glm::pow(col, Color(1.f / gamma));
        }

        static Color TonemapReinhart(const Color& col)
        {
            return col / (1.f + col);
        }

        /**
         * Bell curve with peak at a certain wavelength and spread/standard deviation on the left/right
         * https://en.wikipedia.org/wiki/CIE_1931_color_space#Analytical_approximation
         * @param w Wavelength to evaluate
         * @param peak Wavelength at the peak of the curve
         * @param t1 spread/standard deviation to the left of the mean
         * @param t2 spread/standard deviation to the right of the mean
         * @return the value of the input wavelength on the bell curve
         */
        static float G(float w, float peak, float t1, float t2)
        {
            float t = w < peak ? t1 : t2;
            float ux = w - peak;
            return std::exp(-.5f * ux * ux * t * t);
        }

        /**
         * Convert a wavelength to an rgb color, going from wavelength to XYZ, then from XYZ to RGB
         * https://en.wikipedia.org/wiki/CIE_1931_color_space#Analytical_approximation
         * https://www.oceanopticsbook.info/view/photometry-and-visibility/from-xyz-to-rgb
         * @param w input wavelength
         * @return An RGB representation of the input wavelength
         */
        static Color WavelengthToRGB(float w)
        {
            float x = 1.056f * G(w, 599.8f, .0264f, .0323) +
                .362 * G(w, 442.f, .0624f, .0374f) -
                .065f * G(w, 501.1, .049f, .0382);

            float y = .821f * G(w, 568.8, .0213f, .0247f) +
                .286f * G(w, 530.9f, .0613f, .0322f);

            float z = 1.217f * G(w, 437.f, .0845f, .0278f) +
                .681f * G(w, 459.f, .0385f, .0725f);

            // this is a matrix multiplication, maybe this matrix could be made const and defined directly in the class
            return
            {
                3.2404542f * x - 1.5371385f * y - .4985314 * z,
                -.9692660f * x + 1.8760180f * y + .0415560f * z,
                .0556434f * x - .2040259f * y + 1.0572252f * z
            };
        }

        /**
         * \brief Linear interpolation between 2 floating point colors
         * \param a first color
         * \param b second color
         * \param t mixing factor
         * \return a linear interpolation between a and b
         */
        static Color ColorLerp(const Color& a, const Color& b, float t)
        {
            return a * t + b * (1.f - t);
        }
    };
}