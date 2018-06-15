#include "types.hpp"
#include "mask_generator.hpp"

#include <OpenImageIO/imageio.h>
#include <boost/filesystem/path.hpp>
#include <random>

void write_png(const image_type& image, boost::filesystem::path path)
{
    OIIO::ImageSpec spec(image.columns(), image.rows(), 1, OIIO::TypeDesc::UINT8);
    auto writer = OIIO::ImageOutput::create(path.native());
    writer->open(path.native(), spec);
    writer->write_image(OIIO::TypeDesc::UINT8, image.data(), 1, image.spacing());
    writer->close();
}

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

int main(int argc, char* argv[])
{
    std::vector<mask_generator> masks {
        mask_generator(shape::square()).scale(25).rotate(M_PI/16.0f).translate(-65, 120),
        mask_generator(shape::circle()).xscale(25).yscale(10).rotate(-M_PI/6.0f).translate(100, -60),
    };

    mask_image image(512, 449);
    image.accumulate_masks(masks);
    write_png(image, "masks.png");
}

