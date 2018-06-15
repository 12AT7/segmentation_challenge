#pragma once

#include "types.hpp"
#include <functional>

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
            {  std::cos(radians), std::sin(radians), 0 },
            { -std::sin(radians), std::cos(radians), 0 },
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


