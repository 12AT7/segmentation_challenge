#pragma once

#include <blaze/math/StaticMatrix.h>
#include <blaze/math/DynamicMatrix.h>

using homogeneous_transform = blaze::StaticMatrix<float, 3UL, 3UL>;
using image_type = blaze::DynamicMatrix<std::uint8_t, blaze::rowMajor>;
using homogeneous_coordinate = blaze::StaticVector<float, 3UL>;


