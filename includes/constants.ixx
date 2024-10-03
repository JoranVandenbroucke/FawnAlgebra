//
// Created by Joran on 02/02/2024.
//

module;
#include <numbers>

export module FawnAlgebra.Constants;

namespace FawnAlgebra
{
    export template<typename T>
    constexpr T e{static_cast<T>(std::numbers::e_v<long double>)};
    export template<typename T>
    constexpr T log2e{static_cast<T>(std::numbers::log2e_v<long double>)};
    export template<typename T>
    constexpr T two_pi{static_cast<T>(std::numbers::pi_v<long double>) * static_cast<T>(2.0)};
    export template<typename T>
    constexpr T pi{static_cast<T>(std::numbers::pi_v<long double>)};
    export template<typename T>
    constexpr T inv_pi{static_cast<T>(std::numbers::inv_pi_v<long double>)};
    export template<typename T>
    constexpr T log10e{static_cast<T>(std::numbers::log10e_v<long double>)};
    export template<typename T>
    constexpr T inv_sqrtpi{static_cast<T>(std::numbers::inv_sqrtpi_v<long double>)};
    export template<typename T>
    constexpr T ln2{static_cast<T>(std::numbers::ln2_v<long double>)};
    export template<typename T>
    constexpr T ln10{static_cast<T>(std::numbers::ln10_v<long double>)};
    export template<typename T>
    constexpr T sqrt2{static_cast<T>(std::numbers::sqrt2_v<long double>)};
    export template<typename T>
    constexpr T sqrt3{static_cast<T>(std::numbers::sqrt3_v<long double>)};
    export template<typename T>
    constexpr T inv_sqrt3{static_cast<T>(std::numbers::inv_sqrt3_v<long double>)};
    export template<typename T>
    constexpr T egamma{static_cast<T>(std::numbers::egamma_v<long double>)};
    export template<typename T>
    constexpr T phi{static_cast<T>(std::numbers::phi_v<long double>)};
}// namespace FawnAlgebra
