#include "types.hpp"
#include "mask_generator.hpp"
#include "mask_image.hpp"

#include <OpenImageIO/imageio.h>
#include <boost/filesystem/path.hpp>

void write_png(const image_type& image, boost::filesystem::path path)
{
    OIIO::ImageSpec spec(image.columns(), image.rows(), 1, OIIO::TypeDesc::UINT8);
    auto writer = OIIO::ImageOutput::create(path.native());
    writer->open(path.native(), spec);
    writer->write_image(OIIO::TypeDesc::UINT8, image.data(), 1, image.spacing());
    writer->close();
}

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

