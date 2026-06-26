#pragma once

import FawnAlgebra;
import std;
using namespace fawn_algebra;

namespace deer_geometry
{
template <typename Type, std::uint8_t Dimension>
    requires(std::is_arithmetic_v<Type> && Dimension != 0)
struct obb
{
    Vec<Type, Dimension> origine{};
    Vec<Type, Dimension> extend{};
};

} // namespace deer_geometry
