#include "types.hpp"
#include "mask_generator.hpp"
#include "mask_image.hpp"

#include <boost/filesystem/path.hpp>

extern void write_png(const image_type&, boost::filesystem::path);

int main(int argc, char* argv[])
{
    std::vector<mask_generator> masks {
        mask_generator(shape::square()).scale(25).rotate(M_PI/16.0f).translate(-65, 120),
        mask_generator(shape::circle()).xscale(25).yscale(10).rotate(-M_PI/6.0f).translate(100, -60),
    };

    mask_image image(512, 449);
    image.accumulate_noise();
    image.accumulate_masks(masks);
    write_png(image, "masks.png");
}

