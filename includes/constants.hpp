//
// Created by Joran on 02/02/2024.
//

#pragma once
#include <numbers>

namespace FawnAlgebra {
    namespace Float {
        constexpr float e{std::numbers::e_v<float>};
        constexpr float log2e{std::numbers::log2e_v<float>};
        constexpr float log10e{std::numbers::log10e_v<float>};
        constexpr float pi{std::numbers::pi_v<float>};
        constexpr float two_pi{std::numbers::pi_v<float> * 2.0f};
        constexpr float inv_pi{std::numbers::inv_pi_v<float>};
        constexpr float inv_sqrtpi{std::numbers::inv_sqrtpi_v<float>};
        constexpr float ln2{std::numbers::ln2_v<float>};
        constexpr float ln10{std::numbers::ln10_v<float>};
        constexpr float sqrt2{std::numbers::sqrt2_v<float>};
        constexpr float sqrt3{std::numbers::sqrt3_v<float>};
        constexpr float inv_sqrt3{std::numbers::inv_sqrt3_v<float>};
        constexpr float egamma{std::numbers::egamma_v<float>};
        constexpr float phi{std::numbers::phi_v<float>};
    }
    namespace Double{
        constexpr double e{std::numbers::e};
        constexpr double log2e{std::numbers::log2e};
        constexpr double log10e{std::numbers::log10e};
        constexpr double pi{std::numbers::pi};
        constexpr double two_pi{std::numbers::pi * 2.0f};
        constexpr double inv_pi{std::numbers::inv_pi};
        constexpr double inv_sqrtpi{std::numbers::inv_sqrtpi};
        constexpr double ln2{std::numbers::ln2};
        constexpr double ln10{std::numbers::ln10};
        constexpr double sqrt2{std::numbers::sqrt2};
        constexpr double sqrt3{std::numbers::sqrt3};
        constexpr double inv_sqrt3{std::numbers::inv_sqrt3};
        constexpr double egamma{std::numbers::egamma};
        constexpr double phi{std::numbers::phi};
    }
    namespace LongDouble{
        constexpr long double e{std::numbers::e_v<long double>};
        constexpr long double log2e{std::numbers::log2e_v<long double>};
        constexpr long double log10e{std::numbers::log10e_v<long double>};
        constexpr long double pi{std::numbers::pi_v<long double>};
        constexpr long double two_pi{std::numbers::pi_v<long double> * 2.0f};
        constexpr long double inv_pi{std::numbers::inv_pi_v<long double>};
        constexpr long double inv_sqrtpi{std::numbers::inv_sqrtpi_v<long double>};
        constexpr long double ln2{std::numbers::ln2_v<long double>};
        constexpr long double ln10{std::numbers::ln10_v<long double>};
        constexpr long double sqrt2{std::numbers::sqrt2_v<long double>};
        constexpr long double sqrt3{std::numbers::sqrt3_v<long double>};
        constexpr long double inv_sqrt3{std::numbers::inv_sqrt3_v<long double>};
        constexpr long double egamma{std::numbers::egamma_v<long double>};
        constexpr long double phi{std::numbers::phi_v<long double>};
    }
}
