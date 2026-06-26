//
// Copyright (c) 2026.
// Author: Joran.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

import FawnAlgebra;
import std;

using namespace fawn_algebra;
using namespace fawn_algebra::simd;

// ===========================================================================
// Compile-time checks
// ===========================================================================

namespace check_types
{

static_assert(sizeof(raw_f32x4) == 16);
static_assert(sizeof(raw_f32x8) == 32);
static_assert(sizeof(raw_f64x2) == 16);
static_assert(sizeof(raw_f64x4) == 32);
static_assert(sizeof(raw_i32x4) == 16);
static_assert(sizeof(raw_i32x8) == 32);
static_assert(sizeof(raw_u8x16) == 16);
static_assert(sizeof(raw_i16x8) == 16);

static_assert(alignof(raw_f32x4) == 16);
static_assert(alignof(raw_f32x8) == 16);

// Arithmetic must genuinely operate elementwise, not scalar-decay.
// If someone simplifies raw_traits back into a single dependent alias
// template, sizeof degrades to scalar size and these fail loudly.
constexpr raw_f32x4 ra{1.0f, 2.0f, 3.0f, 4.0f};
constexpr raw_f32x4 rb{10.0f, 20.0f, 30.0f, 40.0f};
constexpr raw_f32x4 rsum = ra + rb;
static_assert(rsum[0] == 11.0f && rsum[1] == 22.0f && rsum[2] == 33.0f && rsum[3] == 44.0f);

constexpr raw_i32x4 ia{1, 2, 3, 4};
constexpr raw_i32x4 ib{10, 20, 30, 40};
constexpr raw_i32x4 isum = ia + ib;
static_assert(isum[0] == 11 && isum[3] == 44);

} // namespace check_types

namespace check_vec
{

constexpr f32x4 a{1.0f, 2.0f, 3.0f, 4.0f};
constexpr f32x4 b{10.0f, 20.0f, 30.0f, 40.0f};

// -- construction / lane access --
static_assert(a[0] == 1.0f && a[1] == 2.0f && a[2] == 3.0f && a[3] == 4.0f);

constexpr f32x4 splatted = f32x4::splat(7.0f);
static_assert(splatted[0] == 7.0f && splatted[1] == 7.0f && splatted[2] == 7.0f && splatted[3] == 7.0f);

// -- arithmetic --
constexpr f32x4 sum = a + b;
static_assert(sum[0] == 11.0f && sum[1] == 22.0f && sum[2] == 33.0f && sum[3] == 44.0f);

constexpr f32x4 diff = b - a;
static_assert(diff[0] == 9.0f && diff[1] == 18.0f && diff[2] == 27.0f && diff[3] == 36.0f);

constexpr f32x4 prod = a * b;
static_assert(prod[0] == 10.0f && prod[1] == 40.0f && prod[2] == 90.0f && prod[3] == 160.0f);

constexpr f32x4 quot = b / a;
static_assert(quot[0] == 10.0f && quot[1] == 10.0f && quot[2] == 10.0f && quot[3] == 10.0f);

constexpr f32x4 neg = -a;
static_assert(neg[0] == -1.0f && neg[3] == -4.0f);

// -- scalar broadcast forms --
constexpr f32x4 scaled     = a * 2.0f;
constexpr f32x4 scaled_lhs = 2.0f * a;
static_assert(scaled[0] == 2.0f && scaled[3] == 8.0f);
static_assert(scaled_lhs[0] == 2.0f && scaled_lhs[3] == 8.0f);

// -- compound assignment --
constexpr f32x4 compound_add = []
{
    f32x4 v{1.0f, 1.0f, 1.0f, 1.0f};
    v += f32x4{1.0f, 2.0f, 3.0f, 4.0f};
    return v;
}();
static_assert(compound_add[0] == 2.0f && compound_add[3] == 5.0f);

// -- chained expression --
constexpr f32x4 chained = a * (a + b);
static_assert(chained[0] == 1.0f * 11.0f);
static_assert(chained[3] == 4.0f * 44.0f);

// -- comparisons --
constexpr f32x4 lo{1.0f, 5.0f, 3.0f, 8.0f};
constexpr f32x4 hi{4.0f, 2.0f, 3.0f, 1.0f};
constexpr auto lt_mask = (lo < hi);
static_assert(lt_mask[0] != 0); // 1 < 4 -> true
static_assert(lt_mask[1] == 0); // 5 < 2 -> false
static_assert(lt_mask[2] == 0); // 3 < 3 -> false
static_assert(lt_mask[3] == 0); // 8 < 1 -> false

constexpr auto eq_mask = (lo == f32x4{1.0f, 5.0f, 0.0f, 8.0f});
static_assert(eq_mask[0] != 0);
static_assert(eq_mask[1] != 0);
static_assert(eq_mask[2] == 0);
static_assert(eq_mask[3] != 0);

// -- multi-width --
constexpr f32x8 wide_a{1, 2, 3, 4, 5, 6, 7, 8};
constexpr f32x8 wide_b{8, 7, 6, 5, 4, 3, 2, 1};
constexpr f32x8 wide_sum = wide_a + wide_b;
static_assert(wide_sum[0] == 9.0f && wide_sum[7] == 9.0f);

} // namespace check_vec

namespace check_ops
{

constexpr f32x4 a{1.0f, 5.0f, -3.0f, 8.0f};
constexpr f32x4 b{4.0f, 2.0f, 3.0f, 1.0f};

// -- min / max --
constexpr auto mn = fawn_algebra::simd::min(a, b);
static_assert(mn[0] == 1.0f && mn[1] == 2.0f && mn[2] == -3.0f && mn[3] == 1.0f);

constexpr auto mx = max(a, b);
static_assert(mx[0] == 4.0f && mx[1] == 5.0f && mx[2] == 3.0f && mx[3] == 8.0f);

// -- clamp --
constexpr auto cl = clamp(a, f32x4::splat(0.0f), f32x4::splat(4.0f));
static_assert(cl[0] == 1.0f && cl[1] == 4.0f && cl[2] == 0.0f && cl[3] == 4.0f);

// -- lerp --
constexpr auto lp = lerp(a, b, f32x4::splat(0.5f));
static_assert(lp[0] == 2.5f);

// -- fma --
constexpr auto fm = fma(a, b, f32x4::splat(1.0f));
static_assert(fm[0] == 1.0f * 4.0f + 1.0f);
static_assert(fm[2] == -3.0f * 3.0f + 1.0f);

// -- abs --
constexpr auto ab = abs(a);
static_assert(ab[0] == 1.0f && ab[1] == 5.0f && ab[2] == 3.0f && ab[3] == 8.0f);

constexpr f32x8 wide{-1, 2, -3, 4, -5, 6, -7, 8};
constexpr auto wide_abs = abs(wide);
static_assert(wide_abs[0] == 1.0f && wide_abs[4] == 5.0f);

// -- dot / length_squared --
constexpr float d  = dot(a, b);
constexpr float ls = length_squared(a);
static_assert(d == 1.0f * 4.0f + 5.0f * 2.0f + -3.0f * 3.0f + 8.0f * 1.0f);
static_assert(ls == 1.0f * 1.0f + 5.0f * 5.0f + -3.0f * -3.0f + 8.0f * 8.0f);

// NOTE: sqrt(), rsqrt(), length(), normalize() are NOT checked here --
// they call x86 intrinsics which are not usable in a constant expression.

} // namespace check_ops

// ===========================================================================
// Runtime checks (Catch2)
// ===========================================================================

TEST_CASE("raw vector types have correct size and alignment", "[types]")
{
    CHECK(sizeof(raw_f32x4) == 16);
    CHECK(sizeof(raw_f32x8) == 32);
    CHECK(alignof(raw_f32x4) == 16);
    CHECK(alignof(raw_f32x8) == 16);
}

TEST_CASE("raw vector arithmetic operates elementwise at runtime", "[types]")
{
    const raw_f32x4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const raw_f32x4 b{10.0f, 20.0f, 30.0f, 40.0f};
    const raw_f32x4 sum = a + b;
    CHECK(sum[0] == 11.0f);
    CHECK(sum[1] == 22.0f);
    CHECK(sum[2] == 33.0f);
    CHECK(sum[3] == 44.0f);
}

TEST_CASE("vec<float,4> construction and lane access", "[vec]")
{
    f32x4 v{1.0f, 2.0f, 3.0f, 4.0f};
    CHECK(v[0] == 1.0f);
    CHECK(v[1] == 2.0f);
    CHECK(v[2] == 3.0f);
    CHECK(v[3] == 4.0f);
}

TEST_CASE("vec<float,4> splat broadcasts to all lanes", "[vec]")
{
    f32x4 v = f32x4::splat(7.0f);
    CHECK(v[0] == 7.0f);
    CHECK(v[1] == 7.0f);
    CHECK(v[2] == 7.0f);
    CHECK(v[3] == 7.0f);
}

TEST_CASE("vec<float,4> arithmetic operators", "[vec][arithmetic]")
{
    f32x4 a{1.0f, 2.0f, 3.0f, 4.0f};
    f32x4 b{10.0f, 20.0f, 30.0f, 40.0f};

    SECTION("addition")
    {
        f32x4 r = a + b;
        CHECK(r[0] == 11.0f);
        CHECK(r[1] == 22.0f);
        CHECK(r[2] == 33.0f);
        CHECK(r[3] == 44.0f);
    }
    SECTION("subtraction")
    {
        f32x4 r = b - a;
        CHECK(r[0] == 9.0f);
        CHECK(r[3] == 36.0f);
    }
    SECTION("multiplication")
    {
        f32x4 r = a * b;
        CHECK(r[0] == 10.0f);
        CHECK(r[3] == 160.0f);
    }
    SECTION("division")
    {
        f32x4 r = b / a;
        CHECK(r[0] == Catch::Approx(10.0f));
        CHECK(r[3] == Catch::Approx(10.0f));
    }
    SECTION("unary negation")
    {
        f32x4 r = -a;
        CHECK(r[0] == -1.0f);
        CHECK(r[3] == -4.0f);
    }
    SECTION("scalar broadcast multiply, both operand orders")
    {
        CHECK((a * 2.0f)[0] == 2.0f);
        CHECK((2.0f * a)[0] == 2.0f);
    }
}

TEST_CASE("vec<float,4> compound assignment operators", "[vec][arithmetic]")
{
    f32x4 v{1.0f, 1.0f, 1.0f, 1.0f};
    v += f32x4{1.0f, 2.0f, 3.0f, 4.0f};
    CHECK(v[0] == 2.0f);
    CHECK(v[3] == 5.0f);

    v -= f32x4::splat(1.0f);
    CHECK(v[0] == 1.0f);
    CHECK(v[3] == 4.0f);

    v *= f32x4::splat(2.0f);
    CHECK(v[0] == 2.0f);
    CHECK(v[3] == 8.0f);

    v /= f32x4::splat(4.0f);
    CHECK(v[0] == Catch::Approx(0.5f));
    CHECK(v[3] == Catch::Approx(2.0f));
}

TEST_CASE("vec<float,4> chained expression a * (a + b)", "[vec][arithmetic]")
{
    const f32x4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const f32x4 b{10.0f, 20.0f, 30.0f, 40.0f};
    f32x4 r = a * (a + b);
    CHECK(r[0] == 11.0f);
    CHECK(r[1] == 44.0f);
    CHECK(r[2] == 99.0f);
    CHECK(r[3] == 176.0f);
}

TEST_CASE("vec<float,4> comparisons produce correct per-lane masks", "[vec][compare]")
{
    f32x4 lo{1.0f, 5.0f, 3.0f, 8.0f};
    f32x4 hi{4.0f, 2.0f, 3.0f, 1.0f};

    auto lt = (lo < hi);
    CHECK(lt[0] != 0);
    CHECK(lt[1] == 0);
    CHECK(lt[2] == 0);
    CHECK(lt[3] == 0);

    auto eq = (lo == f32x4{1.0f, 5.0f, 0.0f, 8.0f});
    CHECK(eq[0] != 0);
    CHECK(eq[1] != 0);
    CHECK(eq[2] == 0);
    CHECK(eq[3] != 0);
}

TEST_CASE("vec<float,8> matches vec<float,4> semantics at wider width", "[vec][f32x8]")
{
    const f32x8 a{1, 2, 3, 4, 5, 6, 7, 8};
    const f32x8 b{8, 7, 6, 5, 4, 3, 2, 1};
    f32x8 sum = a + b;
    for (int i = 0; i < 8; ++i)
        CHECK(sum[i] == 9.0f);
}

TEST_CASE("vec<int32_t,4> integer arithmetic", "[vec][int]")
{
    const i32x4 a{1, 2, 3, 4};
    const i32x4 b{10, 20, 30, 40};
    i32x4 sum = a + b;
    CHECK(sum[0] == 11);
    CHECK(sum[3] == 44);
}

TEST_CASE("load/store round-trip through memory", "[vec][load][store]")
{
    alignas(16) const float src[4] = {1.5f, 2.5f, 3.5f, 4.5f};
    f32x4 v                  = f32x4::load(src);
    CHECK(v[0] == 1.5f);
    CHECK(v[3] == 4.5f);

    alignas(16) float dst[4] = {};
    v.store(dst);
    CHECK(dst[0] == 1.5f);
    CHECK(dst[3] == 4.5f);
}

TEST_CASE("min / max", "[ops]")
{
    f32x4 a{1.0f, 5.0f, -3.0f, 8.0f};
    f32x4 b{4.0f, 2.0f, 3.0f, 1.0f};

    auto mn = min(a, b);
    CHECK(mn[0] == 1.0f);
    CHECK(mn[1] == 2.0f);
    CHECK(mn[2] == -3.0f);
    CHECK(mn[3] == 1.0f);

    auto mx = max(a, b);
    CHECK(mx[0] == 4.0f);
    CHECK(mx[1] == 5.0f);
    CHECK(mx[2] == 3.0f);
    CHECK(mx[3] == 8.0f);
}

TEST_CASE("clamp restricts values to [lo, hi] per lane", "[ops]")
{
    const f32x4 x{1.0f, 5.0f, -3.0f, 8.0f};
    auto r = clamp(x, f32x4::splat(0.0f), f32x4::splat(4.0f));
    CHECK(r[0] == 1.0f);
    CHECK(r[1] == 4.0f);
    CHECK(r[2] == 0.0f);
    CHECK(r[3] == 4.0f);
}

TEST_CASE("lerp interpolates linearly per lane", "[ops]")
{
    const f32x4 a{0.0f, 0.0f, 0.0f, 0.0f};
    const f32x4 b{10.0f, 20.0f, 30.0f, 40.0f};

    CHECK(lerp(a, b, f32x4::splat(0.0f))[0] == Catch::Approx(0.0f));
    CHECK(lerp(a, b, f32x4::splat(1.0f))[1] == Catch::Approx(20.0f));
    CHECK(lerp(a, b, f32x4::splat(0.5f))[2] == Catch::Approx(15.0f));
}

TEST_CASE("fma computes a*b+c exactly", "[ops]")
{
    const f32x4 a{1.0f, 5.0f, -3.0f, 8.0f};
    const f32x4 b{4.0f, 2.0f, 3.0f, 1.0f};
    const f32x4 c = f32x4::splat(1.0f);
    auto r  = fma(a, b, c);
    CHECK(r[0] == 5.0f);
    CHECK(r[2] == -8.0f);
}

TEST_CASE("abs clears the sign bit per lane", "[ops]")
{
    SECTION("f32x4")
    {
        f32x4 a{-1.0f, 5.0f, -3.0f, -8.0f};
        auto r = abs(a);
        CHECK(r[0] == 1.0f);
        CHECK(r[1] == 5.0f);
        CHECK(r[2] == 3.0f);
        CHECK(r[3] == 8.0f);
    }
    SECTION("f32x8")
    {
        f32x8 a{-1, 2, -3, 4, -5, 6, -7, 8};
        auto r = abs(a);
        for (int i = 0; i < 8; ++i)
            CHECK(r[i] > 0.0f);
    }
}

TEST_CASE("sqrt is exact (IEEE correctly-rounded)", "[ops][sqrt]")
{
    const f32x4 a{4.0f, 9.0f, 16.0f, 25.0f};
    auto r = sqrt(a);
    CHECK(r[0] == Catch::Approx(2.0f));
    CHECK(r[1] == Catch::Approx(3.0f));
    CHECK(r[2] == Catch::Approx(4.0f));
    CHECK(r[3] == Catch::Approx(5.0f));
}

TEST_CASE("rsqrt approximation is within expected tolerance", "[ops][sqrt]")
{
    const f32x4 a{4.0f, 9.0f, 16.0f, 25.0f};
    auto r = rsqrt(a);
    CHECK(r[0] == Catch::Approx(0.5f).epsilon(0.01));
    CHECK(r[1] == Catch::Approx(1.0f / 3.0f).epsilon(0.01));
    CHECK(r[2] == Catch::Approx(0.25f).epsilon(0.01));
    CHECK(r[3] == Catch::Approx(0.2f).epsilon(0.01));
}

TEST_CASE("rsqrt_refine improves on the raw rsqrt approximation", "[ops][sqrt]")
{
    f32x4 a{4.0f, 9.0f, 16.0f, 25.0f};
    auto raw     = rsqrt(a);
    auto refined = rsqrt_refine(a, raw);

    for (int i = 0; i < 4; ++i)
    {
        float true_val    = 1.0f / std::sqrt(a[i]);
        const float raw_err     = std::abs(raw[i] - true_val);
        const float refined_err = std::abs(refined[i] - true_val);
        CHECK(refined_err < raw_err);
        CHECK(refined[i] == Catch::Approx(true_val).epsilon(0.001));
    }
}

TEST_CASE("dot product matches scalar reference", "[ops][dot]")
{
    const f32x4 a{1.0f, 5.0f, -3.0f, 8.0f};
    const f32x4 b{4.0f, 2.0f, 3.0f, 1.0f};
    const float expected = 1.0f * 4.0f + 5.0f * 2.0f + -3.0f * 3.0f + 8.0f * 1.0f;
    CHECK(dot(a, b) == Catch::Approx(expected));
}

TEST_CASE("length / length_squared are consistent", "[ops][length]")
{
    const f32x4 v{3.0f, 4.0f, 0.0f, 0.0f};
    CHECK(length_squared(v) == Catch::Approx(25.0f));
    CHECK(length(v) == Catch::Approx(5.0f));
}

TEST_CASE("normalize produces a unit-length vector", "[ops][normalize]")
{
    const f32x4 v{3.0f, 4.0f, 0.0f, 0.0f};
    auto n = normalize(v);
    CHECK(length(n) == Catch::Approx(1.0f).epsilon(0.0001));
    CHECK(n[0] == Catch::Approx(0.6f));
    CHECK(n[1] == Catch::Approx(0.8f));
}
