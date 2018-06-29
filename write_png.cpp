#include "types.hpp"
#include <png.h>
#include <boost/filesystem/path.hpp>

void write_png(const image_type& image, boost::filesystem::path path)
{
    FILE* file = fopen(path.native().c_str(), "wb");

    if (!file)
        throw std::runtime_error("PNG could not open " + path.native());

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        throw std::runtime_error("cannot allocate PNG context");

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        throw std::runtime_error("cannot allocate PNG info structure");
    }

    png_init_io(png, file);
    png_set_IHDR(
            png,
            info,
            image.columns(), image.rows(),
            8, // bit depth
            PNG_COLOR_TYPE_GRAY,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    // Blaze (or any other matrix library) probably aligned the rows for
    // performance.  Odd image sizes then might have padding at the end of each
    // row so the next line starts at an aligned location.  We don't want those
    // padding bytes in the PNG though, nor do we want to copy the image to
    // contiguous memory.  The best option (supported by libpng) is to write
    // each row separately.
    std::vector<png_bytep> row_pointers;

    // Nasty old fashioned C library libpng is not const-aware.  This conflicts
    // with Blaze, which *is* const-aware, so we have to cast away constness.
    for (size_t row = 0; row < image.rows(); ++row)
        row_pointers.emplace_back(const_cast<png_bytep>(&image(row,0)));
    png_set_rows(png, info, row_pointers.data());

    png_write_png(png, info, PNG_TRANSFORM_SWAP_ENDIAN, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(file);
}


