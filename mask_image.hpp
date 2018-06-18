#pragma once

#include "types.hpp"
#include "mask_generator.hpp"
#include <random>

struct mask_image : image_type
{
    using image_type::image_type;

    // Sample the image from a set of mask descriptions.  This implementation
    // does not support being called twice on the same image, because handling
    // overlapping shapes from a previous call in the presence of noise is not
    // straightforward.
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
                    (*this)(y,x) += std::numeric_limits<ElementType>::max() - m_noise;
            }
    }

    // Add some uniform noise to the image.  Call this before
    // accumulate_masks(); the current implementation is too dumb to handle
    // overflows correctly.
    void accumulate_noise(std::uint8_t noise_amplitude = 120)
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<std::uint8_t> noise(0, noise_amplitude);
       for (size_t y = 0; y < rows(); ++y)
            std::transform(begin(y), end(y), begin(y), [&](auto pix) { return pix + noise(generator); });
       m_noise = noise_amplitude;
    }

    std::uint8_t m_noise { 0 };
};


