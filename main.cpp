#include <blaze/math/StaticMatrix.h>
#include <OpenImageIO/imageio.h>
#include <boost/filesystem/path.hpp>
#include <random>

using homogeneous_transform = blaze::StaticMatrix<float, 3UL, 3UL>;
using image_type = blaze::DynamicMatrix<std::uint8_t, blaze::rowMajor>;
using homogeneous_coordinate = blaze::StaticVector<float, 3UL>;

class mask_generator {
public:

    template <typename Shape>
    mask_generator(const Shape& shape) : m_is_inside(shape)
    {
        m_inverse = blaze::inv(m_forward);
    }

    mask_generator& scale(float s)
    {
        homogeneous_transform A {
            { s, 0, 0 },
            { 0, s, 0 },
            { 0, 0, 1 }
        };
        return chain_transform(A);
    }

    mask_generator& yscale(float s)
    {
        homogeneous_transform A {
            { 1, 0, 0 },
            { 0, s, 0 },
            { 0, 0, 1 }
        };
        return chain_transform(A);
    }

    mask_generator& xscale(float s)
    {
        homogeneous_transform A {
            { s, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 }
        };
        return chain_transform(A);
    }

    mask_generator& rotate(float radians)
    {
        homogeneous_transform A {
            {  cos(radians), sin(radians), 0 },
            { -sin(radians), cos(radians), 0 },
            { 0, 0, 1 }
        };
        return chain_transform(A);
    }

    mask_generator& translate(float x, float y)
    {
        homogeneous_transform A {
            { 1, 0, x },
            { 0, 1, y },
            { 0, 0, 1 }
        };
        return chain_transform(A);
    }

    // Implement a functor to check if a mask contains a particular pixel.
    // This is used as a predicate in STL algorithms such as std::any_of(...).
    struct contains {
        const homogeneous_coordinate pixel;

        bool operator()(const mask_generator& mask) const
        {
            return mask.m_is_inside(mask.m_inverse*pixel);
        }
    };

private:

    homogeneous_transform m_forward {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 }
    };
    homogeneous_transform m_inverse;

    // m_is_inside() is a predicate that tests containment of a pixel inside a
    // normalized shape prototoype, such as the unit circle or unit square.
    std::function<bool (homogeneous_coordinate)> m_is_inside;

    mask_generator& chain_transform(const homogeneous_transform& A)
    {
        m_forward = A*m_forward;
        m_inverse = blaze::inv(m_forward);

        return *this;
    }
};

namespace shape {

     struct circle {
         bool operator()(homogeneous_coordinate loc) { return (loc[0]*loc[0] + loc[1]*loc[1] < 1); }
     };

     struct square {
         bool operator()(homogeneous_coordinate loc) { return fabs(loc[0]) <= 1 and fabs(loc[1]) <= 1; }
     };

} // namespace shape

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

