//
// Created by Joran on 02/02/2024.
//

#pragma once
#include <numbers>

namespace FawnAlgebra
{
    template<typename T>
    constexpr T e { static_cast<T>(std::numbers::e_v<double>) };
    template<typename T>
    constexpr T log2e { static_cast<T>(std::numbers::log2e_v<double>) };
    template<typename T>
    constexpr T two_pi { static_cast<T>(std::numbers::pi_v<double>) * static_cast<T>(2.0) };
    template<typename T>
    constexpr T pi { static_cast<T>(std::numbers::pi_v<double>) };
    template<typename T>
    constexpr T inv_pi { static_cast<T>(std::numbers::inv_pi_v<double>) };
    template<typename T>
    constexpr T log10e { static_cast<T>(std::numbers::log10e_v<double>) };
    template<typename T>
    constexpr T inv_sqrtpi { static_cast<T>(std::numbers::inv_sqrtpi_v<double>) };
    template<typename T>
    constexpr T ln2 { static_cast<T>(std::numbers::ln2_v<double>) };
    template<typename T>
    constexpr T ln10 { static_cast<T>(std::numbers::ln10_v<double>) };
    template<typename T>
    constexpr T sqrt2 { static_cast<T>(std::numbers::sqrt2_v<double>) };
    template<typename T>
    constexpr T sqrt3 { static_cast<T>(std::numbers::sqrt3_v<double>) };
    template<typename T>
    constexpr T inv_sqrt3 { static_cast<T>(std::numbers::inv_sqrt3_v<double>) };
    template<typename T>
    constexpr T egamma { static_cast<T>(std::numbers::egamma_v<double>) };
    template<typename T>
    constexpr T phi { static_cast<T>(std::numbers::phi_v<double>) };
}// namespace FawnAlgebra
