//
// Copyright (c) 2025.
// Author: Joran.
//

module;
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ostream>

export module FawnAlgebra:Arithmetics;

namespace FawnAlgebra
{
// namespace
// {
//
//     template<typename T, std::size_t N>
//     constexpr auto DetermineSize() noexcept
//     {
//         return sizeof( T ) * ( N == 3 ? 4 : N );
//     }
//     template<typename T, std::size_t N>
//     constexpr auto DetermineABI() noexcept
//     {
//         constexpr size_t bytes { DetermineSize<T, N>() };
//         if constexpr ( bytes == sizeof( T ) )
//         {
//             return static_cast<T*>( nullptr );
//         }
//         if constexpr ( bytes == sizeof( uint16_t ) )
//         {
//             return static_cast<uint16_t*>( nullptr );
//         }
//         if constexpr ( bytes == sizeof( uint32_t ) )
//         {
//             return static_cast<uint32_t*>( nullptr );
//         }
//         if constexpr ( bytes == sizeof( uint64_t ) )
//         {
//             return static_cast<uint64_t*>( nullptr );
//         }
//         return static_cast<simd<T, N>*>( nullptr );
//     }
// }// namespace
//
// template<typename T, std::size_t N>
// struct DefaultStorage
// {
//     using type = std::remove_pointer_t<decltype( DetermineABI<T, N>() )>;
// };

// Based on: https://kiorisyshen.github.io/2018/08/27/Vector%20Swizzling%20and%20Parameter%20Pack%20in%20C++/
template <class C, typename T, uint32_t... Indices>
class Swizzle
{
  public:
    static constexpr uint32_t i[] = {Indices...};
    static constexpr size_t   N   = sizeof...(Indices);

    constexpr auto& operator=(const C& rhs) noexcept
    {
        for (size_t n = 0; n < N; n++)
        {
            a[i[n]] = rhs[n];
        }
        return *this;
    }

    explicit constexpr operator C() const noexcept
    {
        return C(a[Indices]...);
    }

  private:
    T a[sizeof...(Indices)];
};

#define SWIZZLE_2D(Type, ABI)            \
    Swizzle<Vec<Type, 2>, ABI, 0, 0> xx; \
    Swizzle<Vec<Type, 2>, ABI, 0, 1> xy; \
    Swizzle<Vec<Type, 2>, ABI, 1, 0> yx; \
    Swizzle<Vec<Type, 2>, ABI, 1, 1> yy;

#define SWIZZLE_3D(Type2, ABI2, Type3, ABI3)   \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 0>    xx;  \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 1>    xy;  \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 2>    xz;  \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 0>    yx;  \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 1>    yy;  \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 2>    yz;  \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 0>    zx;  \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 1>    zy;  \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 2>    zz;  \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 0> xxx; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 1> xxy; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 2> xxz; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 0> xyx; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 1> xyy; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 2> xyz; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 0> xzx; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 1> xzy; \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 2> xzz; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 0> yxx; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 1> yxy; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 2> yxz; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 0> yyx; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 1> yyy; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 2> yyz; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 0> yzx; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 1> yzy; \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 2> yzz; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 0> zxx; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 1> zxy; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 2> zxz; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 0> zyx; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 1> zyy; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 2> zyz; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 0> zzx; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 1> zzy; \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 2> zzz;

#define SWIZZLE_4D(Type2, ABI2, Type3, ABI3, Type4, ABI4) \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 0>       xx;          \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 1>       xy;          \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 2>       xz;          \
    Swizzle<Vec<Type2, 2>, ABI2, 0, 3>       xw;          \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 0>       yx;          \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 1>       yy;          \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 2>       yz;          \
    Swizzle<Vec<Type2, 2>, ABI2, 1, 3>       yw;          \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 0>       zx;          \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 1>       zy;          \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 2>       zz;          \
    Swizzle<Vec<Type2, 2>, ABI2, 2, 3>       zw;          \
    Swizzle<Vec<Type2, 2>, ABI2, 3, 0>       wx;          \
    Swizzle<Vec<Type2, 2>, ABI2, 3, 1>       wy;          \
    Swizzle<Vec<Type2, 2>, ABI2, 3, 2>       wz;          \
    Swizzle<Vec<Type2, 2>, ABI2, 3, 3>       ww;          \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 0>    xxx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 1>    xxy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 2>    xxz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 0, 3>    xxw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 0>    xyx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 1>    xyy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 2>    xyz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 1, 3>    xyw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 0>    xzx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 1>    xzy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 2>    xzz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 2, 3>    xzw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 3, 0>    xwx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 3, 1>    xwy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 3, 2>    xwz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 0, 3, 3>    xww;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 0>    yxx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 1>    yxy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 2>    yxz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 0, 3>    yxw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 0>    yyx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 1>    yyy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 2>    yyz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 1, 3>    yyw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 0>    yzx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 1>    yzy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 2>    yzz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 2, 3>    yzw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 3, 0>    ywx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 3, 1>    ywy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 3, 2>    ywz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 1, 3, 3>    yww;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 0>    zxx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 1>    zxy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 2>    zxz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 0, 3>    zxw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 0>    zyx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 1>    zyy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 2>    zyz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 1, 3>    zyw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 0>    zzx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 1>    zzy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 2>    zzz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 2, 3>    zzw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 3, 0>    zwx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 3, 1>    zwy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 3, 2>    zwz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 2, 3, 3>    zww;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 0, 0>    wxx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 0, 1>    wxy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 0, 2>    wxz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 0, 3>    wxw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 1, 0>    wyx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 1, 1>    wyy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 1, 2>    wyz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 1, 3>    wyw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 2, 0>    wzx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 2, 1>    wzy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 2, 2>    wzz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 2, 3>    wzw;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 3, 0>    wwx;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 3, 1>    wwy;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 3, 2>    wwz;         \
    Swizzle<Vec<Type3, 3>, ABI3, 3, 3, 3>    www;         \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 0, 0> xxxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 0, 1> xxxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 0, 2> xxxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 0, 3> xxxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 1, 0> xxyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 1, 1> xxyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 1, 2> xxyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 1, 3> xxyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 2, 0> xxzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 2, 1> xxzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 2, 2> xxzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 2, 3> xxzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 3, 0> xxwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 3, 1> xxwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 3, 2> xxwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 0, 3, 3> xxww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 0, 0> xyxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 0, 1> xyxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 0, 2> xyxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 0, 3> xyxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 1, 0> xyyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 1, 1> xyyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 1, 2> xyyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 1, 3> xyyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 2, 0> xyzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 2, 1> xyzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 2, 2> xyzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 2, 3> xyzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 3, 0> xywx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 3, 1> xywy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 3, 2> xywz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 1, 3, 3> xyww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 0, 0> xzxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 0, 1> xzxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 0, 2> xzxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 0, 3> xzxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 1, 0> xzyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 1, 1> xzyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 1, 2> xzyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 1, 3> xzyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 2, 0> xzzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 2, 1> xzzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 2, 2> xzzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 2, 3> xzzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 3, 0> xzwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 3, 1> xzwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 3, 2> xzwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 2, 3, 3> xzww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 0, 0> xwxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 0, 1> xwxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 0, 2> xwxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 0, 3> xwxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 1, 0> xwyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 1, 1> xwyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 1, 2> xwyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 1, 3> xwyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 2, 0> xwzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 2, 1> xwzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 2, 2> xwzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 2, 3> xwzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 3, 0> xwwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 3, 1> xwwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 3, 2> xwwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 0, 3, 3, 3> xwww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 0, 0> yxxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 0, 1> yxxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 0, 2> yxxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 0, 3> yxxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 1, 0> yxyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 1, 1> yxyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 1, 2> yxyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 1, 3> yxyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 2, 0> yxzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 2, 1> yxzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 2, 2> yxzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 2, 3> yxzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 3, 0> yxwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 3, 1> yxwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 3, 2> yxwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 0, 3, 3> yxww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 0, 0> yyxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 0, 1> yyxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 0, 2> yyxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 0, 3> yyxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 1, 0> yyyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 1, 1> yyyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 1, 2> yyyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 1, 3> yyyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 2, 0> yyzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 2, 1> yyzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 2, 2> yyzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 2, 3> yyzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 3, 0> yywx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 3, 1> yywy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 3, 2> yywz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 1, 3, 3> yyww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 0, 0> yzxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 0, 1> yzxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 0, 2> yzxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 0, 3> yzxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 1, 0> yzyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 1, 1> yzyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 1, 2> yzyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 1, 3> yzyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 2, 0> yzzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 2, 1> yzzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 2, 2> yzzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 2, 3> yzzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 3, 0> yzwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 3, 1> yzwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 3, 2> yzwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 2, 3, 3> yzww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 0, 0> ywxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 0, 1> ywxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 0, 2> ywxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 0, 3> ywxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 1, 0> ywyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 1, 1> ywyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 1, 2> ywyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 1, 3> ywyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 2, 0> ywzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 2, 1> ywzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 2, 2> ywzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 2, 3> ywzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 3, 0> ywwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 3, 1> ywwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 3, 2> ywwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 1, 3, 3, 3> ywww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 0, 0> zxxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 0, 1> zxxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 0, 2> zxxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 0, 3> zxxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 1, 0> zxyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 1, 1> zxyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 1, 2> zxyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 1, 3> zxyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 2, 0> zxzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 2, 1> zxzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 2, 2> zxzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 2, 3> zxzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 3, 0> zxwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 3, 1> zxwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 3, 2> zxwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 0, 3, 3> zxww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 0, 0> zyxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 0, 1> zyxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 0, 2> zyxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 0, 3> zyxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 1, 0> zyyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 1, 1> zyyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 1, 2> zyyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 1, 3> zyyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 2, 0> zyzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 2, 1> zyzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 2, 2> zyzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 2, 3> zyzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 3, 0> zywx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 3, 1> zywy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 3, 2> zywz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 1, 3, 3> zyww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 0, 0> zzxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 0, 1> zzxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 0, 2> zzxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 0, 3> zzxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 1, 0> zzyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 1, 1> zzyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 1, 2> zzyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 1, 3> zzyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 2, 0> zzzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 2, 1> zzzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 2, 2> zzzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 2, 3> zzzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 3, 0> zzwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 3, 1> zzwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 3, 2> zzwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 2, 3, 3> zzww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 0, 0> zwxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 0, 1> zwxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 0, 2> zwxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 0, 3> zwxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 1, 0> zwyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 1, 1> zwyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 1, 2> zwyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 1, 3> zwyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 2, 0> zwzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 2, 1> zwzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 2, 2> zwzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 2, 3> zwzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 3, 0> zwwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 3, 1> zwwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 3, 2> zwwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 2, 3, 3, 3> zwww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 0, 0> wxxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 0, 1> wxxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 0, 2> wxxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 0, 3> wxxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 1, 0> wxyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 1, 1> wxyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 1, 2> wxyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 1, 3> wxyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 2, 0> wxzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 2, 1> wxzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 2, 2> wxzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 2, 3> wxzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 3, 0> wxwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 3, 1> wxwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 3, 2> wxwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 0, 3, 3> wxww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 0, 0> wyxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 0, 1> wyxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 0, 2> wyxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 0, 3> wyxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 1, 0> wyyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 1, 1> wyyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 1, 2> wyyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 1, 3> wyyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 2, 0> wyzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 2, 1> wyzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 2, 2> wyzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 2, 3> wyzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 3, 0> wywx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 3, 1> wywy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 3, 2> wywz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 1, 3, 3> wyww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 0, 0> wzxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 0, 1> wzxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 0, 2> wzxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 0, 3> wzxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 1, 0> wzyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 1, 1> wzyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 1, 2> wzyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 1, 3> wzyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 2, 0> wzzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 2, 1> wzzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 2, 2> wzzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 2, 3> wzzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 3, 0> wzwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 3, 1> wzwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 3, 2> wzwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 2, 3, 3> wzww;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 0, 0> wwxx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 0, 1> wwxy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 0, 2> wwxz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 0, 3> wwxw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 1, 0> wwyx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 1, 1> wwyy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 1, 2> wwyz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 1, 3> wwyw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 2, 0> wwzx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 2, 1> wwzy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 2, 2> wwzz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 2, 3> wwzw;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 3, 0> wwwx;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 3, 1> wwwy;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 3, 2> wwwz;        \
    Swizzle<Vec<Type4, 4>, ABI4, 3, 3, 3, 3> wwww;

export template <typename T, std::size_t N>
class Vec
{
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;
    //     using abi_type       = typename DefaultStorage<T, N>::type;

    std::array<T, N> data{};
    //        abi_type simdData;

    Vec() = default;
    explicit Vec(T v) noexcept
    {
        std::fill(data.cbegin(), data.cend(), v);
    }
    template <typename TT>
        requires std::is_convertible_v<TT, T>
    constexpr explicit Vec(TT v) noexcept
    {
        std::for_each(data.begin(), data.end(), [v](T& d) { d = v; });
    }
    constexpr Vec(std::initializer_list<T> initializers) noexcept
    {
        std::size_t count{};
        for (auto& i{initializers.begin()}; i < initializers.end(); ++i)
        {
            data[count++] = initializers[i];
            if (count == N)
            {
                break;
            }
        }
    }
    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        assert(idx < N);
        return data[idx];
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        assert(idx < N);
        return data[idx];
    }
    constexpr auto operator-() const
    {
        Vec temp{*this};
        for (auto& val : temp.data)
        {
            val = -val;
        }
        return temp;
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] += other.data[i];
        }
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        *this += -other;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<T>
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] <<= other.data[i];
        }
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<T>
    {
        for (int i = 0; i < N; ++i)
        {
            data[i] <<= other.data[i];
        }
        return *this;
    }
};

template <typename T>
class Vec<T, 2>
{
  private:
    static constexpr auto Size{2ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;
    //    using abi_type       = typename DefaultStorage<T, Size>::type;
    struct Coord
    {
        T x;
        T y;
    };

    union
    {
        Coord coord;
        //        abi_type simdData;
        SWIZZLE_2D(value_type, value_type)
    };

    Vec() = default;
    constexpr explicit Vec(T v) noexcept : coord{v, v} {}
    template <typename TT>
        requires std::is_convertible_v<TT, value_type>
    constexpr explicit Vec(TT v) noexcept : coord{v, v}
    {
    }
    constexpr Vec(std::initializer_list<value_type> initializers) noexcept : coord{*initializers.begin(), *(initializers.begin() + 1)} {}
    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : coord.y;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : coord.y;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~coord.x, ~coord.y};
    }
    constexpr auto operator-() const -> Vec
    {
        return {-coord.x, -coord.y};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        coord.x += other.coord.x;
        coord.y += other.coord.y;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        coord.x -= other.coord.x;
        coord.y -= other.coord.y;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        coord.x *= other.coord.x;
        coord.y *= other.coord.y;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        coord.x /= other.coord.x;
        coord.y /= other.coord.y;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x <<= other.coord.x;
        coord.y <<= other.coord.y;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x >>= other.coord.x;
        coord.y >>= other.coord.y;
        return *this;
    }
    // Inner Product
    constexpr T operator|(const Vec& other) noexcept
    {
        return coord.x * other.coord.x + coord.y * other.coord.y;
    }

    // Outer Product
    constexpr T operator^(const Vec& other) noexcept
    {
        return coord.x * other.coord.y - coord.y * other.coord.x;
    }

    constexpr Vec operator&=(const Vec& other) noexcept
    {
        *this = (*this | other) * other;
        return *this;
    }
    // Reject Product
    constexpr Vec& operator%=(const Vec& other) noexcept
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

template <typename T>
class Vec<T, 3>
{
  private:
    static constexpr auto Size{3ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;
    //    using abi_type       = typename DefaultStorage<T, Size>::type;
    struct Coord
    {
        T x;
        T y;
        T z;
    };

    union
    {
        Coord coord;
        //        abi_type simdData;
        SWIZZLE_3D(value_type, value_type, value_type, value_type)
    };

    Vec() = default;
    constexpr explicit Vec(T v) noexcept : coord{v, v, v} {}
    template <typename TT>
        requires std::is_convertible_v<TT, value_type>
    constexpr explicit Vec(TT v) noexcept : coord{v, v, v}
    {
    }
    constexpr Vec(std::initializer_list<value_type> initializers) noexcept :
        coord{*initializers.begin(), *(initializers.begin() + 1), *(initializers.begin() + 2)}
    {
    }
    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : idx == 1 ? coord.y : coord.z;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : idx == 1 ? coord.y : coord.z;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~coord.x, ~coord.y, ~coord.z};
    }
    constexpr auto operator-() const -> Vec
    {
        return Vec{-coord.x, -coord.y, -coord.z};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        coord.x += other.coord.x;
        coord.y += other.coord.y;
        coord.z += other.coord.z;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        coord.x -= other.coord.x;
        coord.y -= other.coord.y;
        coord.z -= other.coord.z;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        coord.x *= other.coord.x;
        coord.y *= other.coord.y;
        coord.z *= other.coord.z;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        coord.x /= other.coord.x;
        coord.y /= other.coord.y;
        coord.z /= other.coord.z;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x <<= other.coord.x;
        coord.y <<= other.coord.y;
        coord.z <<= other.coord.z;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x >>= other.coord.x;
        coord.y >>= other.coord.y;
        coord.z >>= other.coord.z;
        return *this;
    }
    // Outer Product
    constexpr Vec operator^=(const Vec& other) noexcept
    {
        *this = {coord.y * other.coord.z - coord.z * other.coord.y,
                 coord.z * other.coord.x - coord.x * other.coord.z,
                 coord.x * other.coord.y - coord.y * other.coord.x};
        return *this;
    }

    // Projection Product
    constexpr Vec operator&=(const Vec& other) noexcept
    {
        *this = (*this | other) * other;
        return *this;
    }
    // Reject Product
    constexpr Vec& operator%=(const Vec& other) noexcept
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

template <typename T>
class Vec<T, 4>
{
  private:
    static constexpr auto Size{4ULL};

  public:
    using value_type     = T;
    using pointer_type   = T*;
    using ref_type       = T&;
    using const_ref_type = const T&;
    //    using abi_type       = typename DefaultStorage<T, Size>::type;
    struct Coord
    {
        T x;
        T y;
        T z;
        T w;
    };

    union
    {
        Coord coord;
        //        abi_type simdData;
        SWIZZLE_4D(value_type, value_type, value_type, value_type, value_type, value_type)
    };

    Vec() = default;
    constexpr explicit Vec(T v) noexcept : coord{v, v, v, v} {}
    template <typename TT>
        requires std::is_convertible_v<TT, value_type>
    constexpr explicit Vec(TT v) noexcept : coord{v, v, v, v}
    {
    }
    constexpr Vec(std::initializer_list<value_type> initializers) noexcept :
        coord{*initializers.begin(), *(initializers.begin() + 1), *(initializers.begin() + 2), *(initializers.begin() + 3)}
    {
    }
    constexpr auto operator[](const std::size_t idx) const noexcept -> const T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : idx == 1 ? coord.y : idx == 2 ? coord.z : coord.w;
    }
    constexpr auto operator[](const std::size_t idx) noexcept -> T&
    {
        assert(idx < Size);
        return idx == 0 ? coord.x : idx == 1 ? coord.y : idx == 2 ? coord.z : coord.w;
    }
    constexpr T operator~() noexcept
        requires std::is_integral_v<value_type>
    {
        return {~coord.x, ~coord.y, ~coord.z};
    }
    constexpr auto operator-() const -> Vec
    {
        return Vec{-coord.x, -coord.y, -coord.z};
    }
    constexpr auto operator+=(const Vec& other) noexcept
    {
        coord.x += other.coord.x;
        coord.y += other.coord.y;
        coord.z += other.coord.z;
        coord.w += other.coord.w;
        return *this;
    }

    constexpr auto operator-=(const Vec& other) noexcept
    {
        coord.x -= other.coord.x;
        coord.y -= other.coord.y;
        coord.z -= other.coord.z;
        coord.w -= other.coord.w;
        return *this;
    }
    constexpr auto operator*=(const Vec& other) noexcept
    {
        coord.x *= other.coord.x;
        coord.y *= other.coord.y;
        coord.z *= other.coord.z;
        coord.w *= other.coord.w;
        return *this;
    }
    constexpr auto operator/=(const Vec& other) noexcept
    {
        coord.x /= other.coord.x;
        coord.y /= other.coord.y;
        coord.z /= other.coord.z;
        coord.w /= other.coord.w;
        return *this;
    }

    constexpr auto operator<<=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x <<= other.coord.x;
        coord.y <<= other.coord.y;
        coord.z <<= other.coord.z;
        coord.w <<= other.coord.w;
        return *this;
    }
    constexpr auto operator>>=(const Vec& other) noexcept
        requires std::is_integral_v<value_type>
    {
        coord.x >>= other.coord.x;
        coord.y >>= other.coord.y;
        coord.z >>= other.coord.z;
        coord.w >>= other.coord.w;
        return *this;
    }
    // Outer Product
    constexpr Vec operator^=(const Vec& other) noexcept
    {
        return {coord.y * other.coord.z - coord.z * other.coord.y,
                coord.z * other.coord.x - other.coord.z * coord.x,
                coord.x * other.coord.y - coord.y * other.coord.x};
    }
    constexpr auto operator&=(const Vec& other) noexcept -> Vec&
    {
        *this = (*this | other) * other;
        return *this;
    }
    constexpr auto operator%=(const Vec& other) noexcept -> Vec&
    {
        *this = *this - (((*this | other) / (other | other)) * other);
        return *this;
    }
};

export template <typename T, std::size_t N>
constexpr auto operator+(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs += rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator-(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs -= rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator*(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs *= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator/(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs /= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator+(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs += Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator-(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs -= Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator*(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs *= Vec<T, N>(rhs);
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator/(Vec<T, N> lhs, const T rhs) noexcept -> Vec<T, N>
{
    lhs /= Vec<T, N>(rhs);
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator+(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp += rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator-(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp -= rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator*(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp *= rhs;
    return temp;
}
export template <typename T, std::size_t N>
constexpr auto operator/(const T lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    Vec<T, N> temp(lhs);
    temp /= rhs;
    return temp;
}

export template <typename T, std::size_t N>
constexpr auto operator<<(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
    requires std::is_integral_v<T>
{
    lhs <<= rhs;
    return lhs;
}

export template <typename T, std::size_t N>
constexpr auto operator>>(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
    requires std::is_integral_v<T>
{
    lhs >>= rhs;
    return lhs;
}
export template <typename T, std::size_t N>
constexpr auto operator==(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept -> bool
{
    return lhs.data == rhs.data;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs) noexcept -> bool
{
    return lhs.coord.x == rhs.coord.x && lhs.coord.y == rhs.coord.y;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs) noexcept -> bool
{
    return lhs.coord.x == rhs.coord.x && lhs.coord.y == rhs.coord.y && lhs.coord.z == rhs.coord.z;
}
export template <typename T>
constexpr auto operator==(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs) noexcept -> bool
{
    return lhs.coord.x == rhs.coord.x && lhs.coord.y == rhs.coord.y && lhs.coord.z == rhs.coord.z && lhs.coord.w == rhs.coord.w;
}

export template <typename T, std::size_t N>
constexpr auto operator<=>(const Vec<T, N>& lhs, const Vec<T, N>& rhs) noexcept -> bool
{
    return lhs.data <=> rhs.data;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 2>& lhs, const Vec<T, 2>& rhs) noexcept
{
    if (const auto value{lhs.coord.x <=> rhs.coord.x}; value != 0)
    {
        return value;
    }
    return lhs.coord.y <=> rhs.coord.y;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 3>& lhs, const Vec<T, 3>& rhs) noexcept
{
    if (const auto value{lhs.coord.x <=> rhs.coord.x}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.coord.y <=> rhs.coord.y}; value != 0)
    {
        return value;
    }
    return lhs.coord.z <=> rhs.coord.z;
}
export template <typename T>
constexpr auto operator<=>(const Vec<T, 4>& lhs, const Vec<T, 4>& rhs) noexcept -> bool
{
    if (const auto value{lhs.coord.x <=> rhs.coord.x}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.coord.y <=> rhs.coord.y}; value != 0)
    {
        return value;
    }
    if (const auto value{lhs.coord.z <=> rhs.coord.z}; value != 0)
    {
        return value;
    }
    return lhs.coord.w <=> rhs.coord.w;
}
// Inner Product
export template <typename T, std::size_t N>
constexpr auto operator|(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> T
{
    T dot{};
    for (int i{}; i < N; ++i)
    {
        dot += lhs.data[i] * rhs.data[i];
    }
    return dot;
}

export template <typename T>
constexpr auto operator|(Vec<T, 2> lhs, const Vec<T, 2>& rhs) noexcept -> T
{

    return lhs.coord.x * rhs.coord.x + lhs.coord.y * rhs.coord.y;
}

export template <typename T>
constexpr auto operator|(Vec<T, 3> lhs, const Vec<T, 3>& rhs) noexcept -> T
{

    return lhs.coord.x * rhs.coord.x + lhs.coord.y * rhs.coord.y + lhs.coord.z * rhs.coord.z;
}

export template <typename T>
constexpr auto operator|(Vec<T, 4> lhs, const Vec<T, 4>& rhs) noexcept -> T
{

    return lhs.coord.x * rhs.coord.x + lhs.coord.y * rhs.coord.y + lhs.coord.z * rhs.coord.z + lhs.coord.w * rhs.coord.w;
}

// Outer Product
export template <typename T, std::size_t N>
constexpr auto operator^(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs ^= rhs;
    return lhs;
}

export template <typename T>
constexpr auto operator^(Vec<T, 2> lhs, const Vec<T, 2>& rhs) noexcept -> T
{
    return lhs.coord.x * rhs.coord.y - lhs.coord.y * rhs.coord.x;
}
// todo : find a valid aternative for Vec<T, 4>
export template <typename T>
constexpr auto operator^(Vec<T, 4> lhs, const Vec<T, 4>& rhs) noexcept -> T
{
    return 0;
}

// Projection Product
export template <typename T, std::size_t N>
constexpr auto operator&(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs &= rhs;
    return lhs;
}

// Reject Product
export template <typename T, std::size_t N>
constexpr auto operator%(Vec<T, N> lhs, const Vec<T, N>& rhs) noexcept -> Vec<T, N>
{
    lhs %= rhs;
    return lhs;
}

#define OVERLOAD_FUNC_VEC(func)                  \
    export template <typename T, std::size_t N>  \
    constexpr Vec<T, N> func(const Vec<T, N>& v) \
    {                                            \
        Vec<T, N> result;                        \
        for (std::size_t i = 0; i < N; ++i)      \
        {                                        \
            result[i] = std::func(v[i]);         \
        }                                        \
        return result;                           \
    }

#define OVERLOAD_FUNC_VEC_VEC(func)                                  \
    export template <typename T, std::size_t N>                      \
    constexpr Vec<T, N> func(const Vec<T, N>& v, const Vec<T, N>& w) \
    {                                                                \
        Vec<T, N> result;                                            \
        for (std::size_t i{}; i < N; ++i)                            \
        {                                                            \
            result[i] = std::func(v[i], w[i]);                       \
        }                                                            \
        return result;                                               \
    }

OVERLOAD_FUNC_VEC(abs)
OVERLOAD_FUNC_VEC(exp)
OVERLOAD_FUNC_VEC(exp2)
OVERLOAD_FUNC_VEC(expm1)
OVERLOAD_FUNC_VEC(log)
OVERLOAD_FUNC_VEC(log10)
OVERLOAD_FUNC_VEC(log2)
OVERLOAD_FUNC_VEC(log1p)
OVERLOAD_FUNC_VEC(sqrt)
OVERLOAD_FUNC_VEC(cbrt)
OVERLOAD_FUNC_VEC(sin)
OVERLOAD_FUNC_VEC(cos)
OVERLOAD_FUNC_VEC(tan)
OVERLOAD_FUNC_VEC(asin)
OVERLOAD_FUNC_VEC(acos)
OVERLOAD_FUNC_VEC(atan)
OVERLOAD_FUNC_VEC(sinh)
OVERLOAD_FUNC_VEC(cosh)
OVERLOAD_FUNC_VEC(tanh)
OVERLOAD_FUNC_VEC(asinh)
OVERLOAD_FUNC_VEC(acosh)
OVERLOAD_FUNC_VEC(atanh)
OVERLOAD_FUNC_VEC(erf)
OVERLOAD_FUNC_VEC(erfc)
OVERLOAD_FUNC_VEC(tgamma)
OVERLOAD_FUNC_VEC(lgamma)
OVERLOAD_FUNC_VEC(ceil)
OVERLOAD_FUNC_VEC(floor)
OVERLOAD_FUNC_VEC(trunc)
OVERLOAD_FUNC_VEC(round)
OVERLOAD_FUNC_VEC(lround)
OVERLOAD_FUNC_VEC(llround)
OVERLOAD_FUNC_VEC(nearbyint)
OVERLOAD_FUNC_VEC(rint)
OVERLOAD_FUNC_VEC(lrint)
OVERLOAD_FUNC_VEC(llrint)
OVERLOAD_FUNC_VEC(frexp)
OVERLOAD_FUNC_VEC(ldexp)
OVERLOAD_FUNC_VEC(modf)
OVERLOAD_FUNC_VEC(scalbn)
OVERLOAD_FUNC_VEC(scalbln)
OVERLOAD_FUNC_VEC(ilogb)
OVERLOAD_FUNC_VEC(logb)
OVERLOAD_FUNC_VEC(nextafter)
OVERLOAD_FUNC_VEC(nexttoward)
OVERLOAD_FUNC_VEC(copysign)

OVERLOAD_FUNC_VEC_VEC(min)
OVERLOAD_FUNC_VEC_VEC(max)
OVERLOAD_FUNC_VEC_VEC(fdim)
OVERLOAD_FUNC_VEC_VEC(fmod)
OVERLOAD_FUNC_VEC_VEC(remainder)
OVERLOAD_FUNC_VEC_VEC(hypot)

export template <typename T, std::size_t N>
constexpr Vec<T, N> pow(const Vec<T, N>& v, T exponent)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::pow(v.data[i], exponent);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> fma(const Vec<T, N>& v, const Vec<T, N>& y, const Vec<T, N>& z)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::fma(v.data[i], y.data[i], z.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> nan(const Vec<T, N>& v)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::nan(v.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> remquo(const Vec<T, N>& a, const Vec<T, N>& b, Vec<int, N>& quo)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::remquo(a.data[i], b.data[i], &quo.data[i]);
    }
    return result;
}

export template <typename T, std::size_t N>
constexpr Vec<T, N> lerp(const Vec<T, N>& a, const Vec<T, N>& b, T t)
{
    Vec<T, N> result;
    for (std::size_t i{}; i < N; ++i)
    {
        result.data[i] = std::lerp(a.data[i], b.data[i], t);
    }
    return result;
}

// Aliases for different types
export using i8   = int8_t;
export using i16  = int16_t;
export using i32  = int32_t;
export using i64  = int64_t;
export using ui8  = uint8_t;
export using ui16 = uint16_t;
export using ui32 = uint32_t;
export using ui32 = uint32_t;
export using ui64 = uint64_t;
export using f32  = float;
export using f64  = double;

export using int2 = Vec<i32, 2>;
export using int3 = Vec<i32, 3>;
export using int4 = Vec<i32, 4>;

export using uint2 = Vec<ui32, 2>;
export using uint3 = Vec<ui32, 3>;
export using uint4 = Vec<ui32, 4>;

export using float2 = Vec<f32, 2>;
export using float3 = Vec<f32, 3>;
export using float4 = Vec<f32, 4>;

export using double2 = Vec<f64, 2>;
export using double3 = Vec<f64, 3>;
export using double4 = Vec<f64, 4>;

export using char2 = Vec<i8, 2>;
export using char3 = Vec<i8, 3>;
export using char4 = Vec<i8, 4>;

export using uchar2 = Vec<ui8, 2>;
export using uchar3 = Vec<ui8, 3>;
export using uchar4 = Vec<ui8, 4>;

export using short2 = Vec<i16, 2>;
export using short3 = Vec<i16, 3>;
export using short4 = Vec<i16, 4>;

export using ushort2 = Vec<ui16, 2>;
export using ushort3 = Vec<ui16, 3>;
export using ushort4 = Vec<ui16, 4>;

export using int2 = Vec<i32, 2>;
export using int3 = Vec<i32, 3>;
export using int4 = Vec<i32, 4>;

export using uint2 = Vec<ui32, 2>;
export using uint3 = Vec<ui32, 3>;
export using uint4 = Vec<ui32, 4>;

export using float2 = Vec<f32, 2>;
export using float3 = Vec<f32, 3>;
export using float4 = Vec<f32, 4>;

export using double2 = Vec<f64, 2>;
export using double3 = Vec<f64, 3>;
export using double4 = Vec<f64, 4>;
} // namespace FawnAlgebra
