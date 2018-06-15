#pragma once

#include "types.hpp"
#include "mask_generator.hpp"
#include <random>

struct mask_image : image_type
{
    mask_image(size_t rows, size_t columns, std::uint8_t noise_amplitude=120)
        : image_type(rows, columns), m_noise(noise_amplitude)
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<std::uint8_t> noise(0, noise_amplitude);
       for (size_t y = 0; y < rows; ++y)
            std::generate(begin(y), end(y), [&]() { return noise(generator); });
    }

    void accumulate_masks(const std::vector<mask_generator>& masks)
    {
        for (size_t y = 0; y < rows(); ++y)
            for (size_t x = 0; x < columns(); ++x)
            {
                homogeneous_coordinate pixel {
                    static_cast<float>(std::round(x)) - rows()/2.0f,
                    static_cast<float>(std::round(y)) - columns()/2.0f,
                    1.0f
                };
                if (std::any_of(masks.begin(), masks.end(), mask_generator::contains{ pixel }))
                    (*this)(y,x) += 255 - m_noise;
            }
    }

    std::uint8_t m_noise;
};


