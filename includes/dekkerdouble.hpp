//
// Copyright (c) ${Year}.
// Author: Joran Vandenbroucke.
//

#pragma once

#include <cmath>
#include <ostream>

class dekker_double
{
public:
    constexpr dekker_double() = default;

    constexpr explicit dekker_double( const double A, const double a )
        : A{A}
        , a{a}
    {
    }

    constexpr explicit dekker_double( const double value )
    {
        *this = Split( value );
    }

    constexpr ~dekker_double() = default;
    constexpr dekker_double( const dekker_double& ) = default;
    constexpr dekker_double( dekker_double&& ) = default;
    constexpr dekker_double& operator=( const dekker_double& ) = default;
    constexpr dekker_double& operator=( dekker_double&& ) = default;

    constexpr bool operator==( const dekker_double& rhs ) const
    {
        return std::tie( A, a ) == std::tie( rhs.A, rhs.a );
    }

    constexpr bool operator!=( const dekker_double& rhs ) const
    {
        return !( rhs == *this );
    }

    constexpr bool operator<( const dekker_double& rhs ) const
    {
        return std::tie( A, a ) < std::tie( rhs.A, rhs.a );
    }

    constexpr bool operator>( const dekker_double& rhs ) const
    {
        return rhs < *this;
    }

    constexpr bool operator>=( const dekker_double& rhs ) const
    {
        return !( *this < rhs );
    }

    constexpr bool operator<=( const dekker_double& rhs ) const
    {
        return rhs >= *this;
    }

    friend constexpr std::ostream& operator<<( std::ostream& os, const dekker_double& value )
    {
        int whole{value};
        dekker_double copy{value - static_cast<dekker_double>(whole)};
        os << whole << '.';
        if ( copy.A < 0.0 )
        {
            copy.A = -copy.A;
            copy.a = -copy.a;
        }
        for ( int i = 0; i < os.precision(); ++i )
        {
            copy = copy * static_cast<dekker_double>(10);
            whole = static_cast<int>(copy);
            os << whole;
            copy = copy - static_cast<dekker_double>(whole);
        }

        return os;
    }

    constexpr dekker_double& operator+=( const dekker_double& rhs )
    {
        const double R{A + rhs.A};
        double r{rhs.A - R + A + a + rhs.a};
        if ( std::abs( A ) < std::abs( rhs.A ) )
        {
            r = A - R + rhs.A + rhs.a + a;
        }

        A = R + r;
        a = R - A + r;
        return *this;
    }

    constexpr dekker_double& operator-=( const dekker_double& rhs )
    {
        const double R = A - rhs.A;
        double r{-rhs.A - R + A + a - rhs.a};
        if ( std::abs( A ) < std::abs( rhs.A ) )
        {
            r = A - R - rhs.A - rhs.a + a;
        }

        A = R + r;
        a = R - A + r;
        return *this;
    }

    constexpr dekker_double& operator*=( const dekker_double& rhs )
    {
        const dekker_double t{Mul12( A, rhs.A )};
        const double c{A * rhs.a + a * rhs.A + t.a};

        A = t.A + c;
        a = t.A - A + c;
        return *this;
    }

    constexpr dekker_double& operator/=( const dekker_double& rhs )
    {
        const double u{A / rhs.A};
        const dekker_double t{Mul12( u, rhs.A )};
        const double l{( A - t.A - t.a + a - u * rhs.a ) / rhs.A};

        A = u + l;
        a = u - A + l;
        return *this;
    }

    friend constexpr dekker_double operator+( dekker_double lhs, const dekker_double& rhs )
    {
        return lhs += rhs;
    }

    friend constexpr dekker_double operator-( dekker_double lhs, const dekker_double& rhs )
    {
        return lhs -= rhs;
    }

    friend constexpr dekker_double operator*( dekker_double lhs, const dekker_double& rhs )
    {
        return lhs *= rhs;
    }

    friend constexpr dekker_double operator/( dekker_double lhs, const dekker_double& rhs )
    {
        return lhs /= rhs;
    }

    constexpr explicit operator int() const
    {
        return static_cast<int>(A);
    }

    constexpr static dekker_double&& Sqrt( const dekker_double& value )
    {
        if ( value.A <= 0 )
        {
            return {};
        }
        const double c{std::sqrt( value.A )};
        const dekker_double u{Mul12( c, c )};
        const double cc{( value.A - u.A - u.a + value.a ) * 0.5 / c};
        return dekker_double{c + cc, c - value.a + cc};
    }

private:
    double A{};
    double a{};

    [[nodiscard]] static constexpr dekker_double&& Split( const double value )
    {
        constexpr double SCALE{134217729};
        const double p{value * SCALE};
        const double A{value - p + p};
        return dekker_double{A, value - A};
    }

    // masking mantisa only works in IEEE745 floating points
    [[nodiscard]] static constexpr dekker_double&& SplitFast( const double value )
    {
        constexpr unsigned long long MASK { ~0x3ffffffU };
        const double z { std::bit_cast<double>( std::bit_cast<unsigned long long>( value ) & MASK ) };
        return dekker_double { z, value - z };
    }
    static constexpr dekker_double&& Mul12( const double valueA, const double valueB )
    {
        const dekker_double doubleA{Split( valueA )};
        const dekker_double doubleB{Split( valueB )};

        const double p{doubleA.A * doubleB.A};
        const double q{doubleA.A * doubleB.a + doubleA.a * doubleB.A};

        const double r{p + q};
        return dekker_double{r, p - r + q + doubleA.a * doubleB.a};
    }
};
