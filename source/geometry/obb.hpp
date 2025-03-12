#pragma once

#pragma once
#include <cstdint>
#include <type_traits>

import FawnAlgebra;
using namespace FawnAlgebra;

namespace DeerGeometry
{
template <typename Type, std::uint8_t Dimension>
    requires(std::is_arithmetic_v<Type> && Dimension != 0)
struct obb
{
    Vec<Type, Dimension> origine {};
    Vec<Type, Dimension> extend {};
};

} // namespace DeerGeometry
