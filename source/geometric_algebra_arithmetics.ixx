//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <cmath>
#include <complex>
#include <ostream>

export module FawnAlgebra.GeometricArithmetics;

namespace FawnAlgebra::GeometricAlgebra
{
template<typename T>
    requires std::is_arithmetic_v<T>
struct Quaternion
{
    T w {};
    T x {};
    T y {};
    T z {};

    Quaternion() = default;

    constexpr explicit Quaternion( const T w_val ) noexcept : w { w_val }
    {
    }

    constexpr explicit Quaternion( const T w_val, const T x_val, const T y_val, const T z_val ) noexcept
        : w { w_val }
        , x { x_val }
        , y { y_val }
        , z { z_val }
    {
    }

    ~Quaternion()                                        = default;
    Quaternion( const Quaternion& other )                = default;
    Quaternion( Quaternion&& other ) noexcept            = default;
    Quaternion& operator=( const Quaternion& other )     = default;
    Quaternion& operator=( Quaternion&& other ) noexcept = default;

    static constexpr Quaternion zero {};
    static constexpr Quaternion one { 1 };

    friend bool operator==( const Quaternion& lhs, const Quaternion& rhs )
    {
        return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

    friend bool operator!=( const Quaternion& lhs, const Quaternion& rhs )
    {
        return !( lhs == rhs );
    }

    friend std::ostream& operator<<( std::ostream& os, const Quaternion& obj )
    {
        return os << "w: " << obj.w << " x: " << obj.x << " y: " << obj.y << " z: " << obj.z;
    }

    constexpr void swap( Quaternion& other ) noexcept
    {
        using std::swap;
        swap( w, other.w );
        swap( x, other.x );
        swap( y, other.y );
        swap( z, other.z );
    }

    friend constexpr void swap( Quaternion& lhs, Quaternion& rhs ) noexcept
    {
        lhs.swap( rhs );
    }

    constexpr Quaternion operator-() const noexcept
    {
        return { -w, -x, -y, -z };
    }

    constexpr Quaternion& operator+=( const Quaternion& other ) const noexcept
    {
        w += other.w;
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Quaternion& operator-=( const Quaternion& other ) const noexcept
    {
        *this += -other;
        return *this;
    }

    constexpr Quaternion& operator*=( const Quaternion& other ) const noexcept
    {
        const T tempW { w * other.w - x * other.x - y * other.y - z * other.z };
        const T tempX { w * other.x + x * other.w + y * other.z - z * other.y };
        const T tempY { w * other.y + y * other.w + z * other.x - x * other.z };
        const T tempZ { w * other.z + z * other.w + x * other.y - y * other.x };
        w = tempW;
        x = tempX;
        y = tempY;
        z = tempZ;
        return *this;
    }

    friend constexpr Quaternion operator+( Quaternion lhs, const Quaternion& rhs ) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator+( TT lhs, const Quaternion& rhs ) noexcept
    {
        Quaternion temp { lhs };
        temp += rhs;
        return temp;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator+( Quaternion lhs, TT rhs ) noexcept
    {
        lhs += Quaternion { rhs };
        return lhs;
    }

    friend constexpr Quaternion operator-( Quaternion lhs, const Quaternion& rhs ) noexcept
    {
        lhs -= rhs;
        return lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator-( TT lhs, const Quaternion& rhs ) noexcept
    {
        Quaternion temp { lhs };
        temp -= rhs;
        return temp;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator-( Quaternion lhs, TT rhs ) noexcept
    {
        lhs -= Quaternion { rhs };
        return lhs;
    }

    friend constexpr Quaternion operator*( Quaternion lhs, const Quaternion& rhs ) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Quaternion operator*=( TT rhs ) noexcept
    {
        w *= rhs;
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator*( Quaternion lhs, const TT rhs ) noexcept
    {
        lhs *= rhs;
        return lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator*( TT lhs, const Quaternion& rhs ) noexcept
    {
        return rhs * lhs;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    constexpr Quaternion operator/=( const TT rhs ) noexcept
    {
        const T inv { static_cast<T>( 1 ) / rhs };
        *this *= inv;
        return *this;
    }

    template<typename TT>
        requires std::is_arithmetic_v<TT>
    friend constexpr Quaternion operator/( Quaternion lhs, const TT rhs ) noexcept
    {
        lhs /= rhs;
        return lhs;
    }

    friend constexpr T operator|( const Quaternion& lhs, const Quaternion& rhs ) noexcept
    {
        return lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
    }

    constexpr Quaternion Conjugate() noexcept
    {
        return { w, -x, -y, -z };
    }

    constexpr Quaternion& operator/=( const Quaternion& other ) noexcept
    {
        *this *= conjugate( other ) / ( other | Conjugate( other ) );
        return *this;
    }

    friend constexpr Quaternion operator/( Quaternion lhs, const Quaternion& rhs ) noexcept
    {
        lhs += rhs;
        return lhs;
    }

    constexpr T Norm() noexcept
    {
        return static_cast<T>( std::sqrt( static_cast<double>( *this | *this ) ) );
    }

    static constexpr T Norm( const Quaternion& quat ) noexcept
    {
        return quat.Norm();
    }

    constexpr T& Real() noexcept
    {
        return w;
    }

    constexpr const T& Real() const noexcept
    {
        return w;
    }

    static constexpr T& Real( Quaternion& quat ) noexcept
    {
        return quat.Real();
    }

    static constexpr const T& Real( const Quaternion& quat ) noexcept
    {
        return quat.Real();
    }

    constexpr Quaternion RealPart() const noexcept
    {
        return { Real(), 0, 0, 0 };
    }

    static constexpr Quaternion RealPart( const Quaternion& quat ) noexcept
    {
        return quat.Real();
    }

    constexpr Quaternion ImaginaryPart() const noexcept
    {
        return { 0, x, y, z };
    }

    static constexpr Quaternion ImaginaryPart( const Quaternion& quat ) noexcept
    {
        return quat.ImaginaryPart();
    }

    constexpr Quaternion BivectorExponent() const noexcept
    {
        const Quaternion imag { ImaginaryPart() };
        const T norm { Norm( imag ) };
        return norm == 0 ? Quaternion { 1, 0, 0, 0 } : std::cos( norm ) + std::sin( norm ) * imag / norm;
    }

    static constexpr Quaternion BivectorExponent( const Quaternion& quat ) noexcept
    {
        return quat.BivectorExponent();
    }

    constexpr Quaternion Exp() const noexcept
    {
        const Quaternion bi { BivectorExponent() };
        return w == 0 ? bi : std::exp( w ) * bi;
    }

    static constexpr Quaternion Exp( const Quaternion& quat ) noexcept
    {
        return quat.Exp();
    }

    [[nodiscard]] constexpr bool IsApproximate( const Quaternion& other ) const noexcept
    {
        if constexpr ( std::is_floating_point_v<T> )
        {
            static constexpr T EPSILON = T( 1e-6 );
            const T dw { w - other.w };
            const T dx { w - other.x };
            const T dy { w - other.y };
            const T dz { w - other.z };
            return ( dw > -EPSILON && dw < EPSILON ) && ( dx > -EPSILON && dx < EPSILON ) && ( dy > -EPSILON && dy < EPSILON )
                && ( dz > -EPSILON && dz < EPSILON );
        }
        else if constexpr ( std::is_integral_v<T> )
        {
            return *this == other;
        }
        return false;
    }

    [[nodiscard]] static constexpr bool IsApproximate( const Quaternion& lhs, const Quaternion& rhs ) noexcept
    {
        return lhs.IsApproximate( rhs );
    }
};

namespace GA20
{
    template<typename T>
    struct Odd;

    template<typename T>
    struct Even
    {
        std::complex<T> c1 {};
        static constexpr Even zero {};
        static constexpr Even one { 1, 1, 1, 1 };

        constexpr Even operator-()
        {
            return -c1;
        }

        constexpr Even& operator+=( const Even& other ) const noexcept
        {
            c1 += other.c1;
            return *this;
        }

        constexpr Even& operator-=( const Even& other ) const noexcept
        {
            *this += -other;
            return *this;
        }

        friend constexpr Even operator+( Even lhs, const Even& rhs ) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        friend constexpr Even operator-( Even lhs, const Even& rhs ) noexcept
        {
            lhs -= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator+( const TT lhs, const Even& rhs )
        {
            return { lhs. };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator-( const TT lhs, const Even& rhs )
        {
            return { lhs - rhs.w, -rhs.x, -rhs.y, -rhs.z };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator*( const TT lhs, const Even& rhs )
        {
            return { lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Even& operator*=( const Even<TT>& other )
        {
            T newW { w * other.w - x * other.x - y * other.y - z * other.z };
            T newX { w * other.x + x * other.w + y * other.z - z * other.y };
            T newY { w * other.y + y * other.w + z * other.x - x * other.z };
            T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
            w = newW;
            x = newX;
            y = newY;
            z = newZ;
            return *this;
        }

        template<typename TT, typename VV>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> && std::is_arithmetic_v<VV> && std::is_convertible_v<VV, T> )
        friend constexpr Even operator*( Even<TT> lhs, const Even<VV>& rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd<T>& operator*=( const Odd<TT>& other );

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd<T> operator*( Even lhs, const Odd<TT>& rhs );
    };

    template<typename T>
    struct Odd
    {
        std::complex<T> c1 {};
        static constexpr Odd zero {};

        constexpr Odd operator-()
        {
            return { -c1.real(), c1.imag() };
        }

        constexpr Odd& operator+=( const Odd& other ) const noexcept
        {
            c1 += other.c1;
            return *this;
        }

        constexpr Odd& operator-=( const Odd& other ) const noexcept
        {
            *this += -other;
            return *this;
        }

        friend constexpr Odd operator+( Odd lhs, const Odd& rhs ) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        friend constexpr Odd operator-( Odd lhs, const Odd& rhs ) noexcept
        {
            lhs -= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Odd operator*( const TT lhs, const Odd& rhs )
        {
            return rhs.c1 * lhs;
        }

        template<typename TT, typename VV>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> && std::is_arithmetic_v<VV> && std::is_convertible_v<VV, T> )
        friend constexpr Even<TT> operator*( const Odd<TT>& lhs, const Odd<VV>& rhs )
        {
            lhs.c1.return Even<TT> { -lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z,
                                     -lhs.w * rhs.x - lhs.x * rhs.w - lhs.y * rhs.z + lhs.z * rhs.y,
                                     -lhs.w * rhs.y - lhs.y * rhs.w - lhs.z * rhs.x + lhs.x * rhs.z,
                                     -lhs.w * rhs.z - lhs.z * rhs.w - lhs.x * rhs.y + lhs.y * rhs.x };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd& operator*=( const Even<TT>& other )
        {
            const T newW { w * other.w - x * other.x - y * other.y - z * other.z };
            const T newX { w * other.x + x * other.w + y * other.z - z * other.y };
            const T newY { w * other.y + y * other.w + z * other.x - x * other.z };
            const T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
            w = newW;
            x = newX;
            y = newY;
            z = newZ;
            return *this;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Odd operator*( Odd lhs, const Even<TT>& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
    };

    template<typename T>
    template<typename TT>
        requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
    constexpr Odd<T>& Even<T>::operator*=( const Odd<TT>& other )
    {
        const T newW { w * other.w - x * other.x - y * other.y - z * other.z };
        const T newX { w * other.x + x * other.w + y * other.z - z * other.y };
        const T newY { w * other.y + y * other.w + z * other.x - x * other.z };
        const T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
        w = newW;
        x = newX;
        y = newY;
        z = newZ;
        return *this;
    }

    template<typename T>
    template<typename TT>
        requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
    constexpr Odd<T> Even<T>::operator*( Even lhs, const Odd<TT>& rhs )
    {
        lhs *= rhs;
        return lhs;
    }

    static constexpr auto e1 { Odd<int8_t> { 0, 1 } };
    static constexpr auto e2 { Odd<int8_t> { 0, 0 } };
    static constexpr auto I2 { Odd<int8_t> { 1, 0 } };
    static constexpr auto basis { e1, e2, e3 };
}// namespace GA20
namespace GA30
{
    template<typename T>
    struct Odd;

    template<typename T>
    struct Even
    {
        T w {};
        T x {};
        T y {};
        T z {};
        static constexpr Even zero {};
        static constexpr Even one { 1, 1, 1, 1 };

        constexpr Even operator-()
        {
            return { -w, -x, -y, -z };
        }

        constexpr Even& operator+=( const Even& other ) const noexcept
        {
            w += other.w;
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Even& operator-=( const Even& other ) const noexcept
        {
            *this += -other;
            return *this;
        }

        friend constexpr Even operator+( Even lhs, const Even& rhs ) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        friend constexpr Even operator-( Even lhs, const Even& rhs ) noexcept
        {
            lhs -= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator+( const TT lhs, const Even& rhs )
        {
            return { lhs + rhs.w, rhs.x, rhs.y, rhs.z };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator-( const TT lhs, const Even& rhs )
        {
            return { lhs - rhs.w, -rhs.x, -rhs.y, -rhs.z };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Even operator*( const TT lhs, const Even& rhs )
        {
            return { lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Even& operator*=( const Even<TT>& other )
        {
            T newW { w * other.w - x * other.x - y * other.y - z * other.z };
            T newX { w * other.x + x * other.w + y * other.z - z * other.y };
            T newY { w * other.y + y * other.w + z * other.x - x * other.z };
            T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
            w = newW;
            x = newX;
            y = newY;
            z = newZ;
            return *this;
        }

        template<typename TT, typename VV>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> && std::is_arithmetic_v<VV> && std::is_convertible_v<VV, T> )
        friend constexpr Even operator*( Even<TT> lhs, const Even<VV>& rhs )
        {
            lhs *= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd<T>& operator*=( const Odd<TT>& other );

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd<T> operator*( Even lhs, const Odd<TT>& rhs );
    };

    template<typename T>
    struct Odd
    {
        T w {};
        T x {};
        T y {};
        T z {};
        static constexpr Odd zero {};
        static constexpr Odd one { 1, 1, 1, 1 };

        constexpr Odd operator-()
        {
            return { -w, -x, -y, -z };
        }

        constexpr Odd& operator+=( const Odd& other ) const noexcept
        {
            w += other.w;
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Odd& operator-=( const Odd& other ) const noexcept
        {
            *this += -other;
            return *this;
        }

        friend constexpr Odd operator+( Odd lhs, const Odd& rhs ) noexcept
        {
            lhs += rhs;
            return lhs;
        }

        friend constexpr Odd operator-( Odd lhs, const Odd& rhs ) noexcept
        {
            lhs -= rhs;
            return lhs;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Odd operator*( const TT lhs, const Odd& rhs )
        {
            return { lhs * rhs.w, lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
        }

        template<typename TT, typename VV>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> && std::is_arithmetic_v<VV> && std::is_convertible_v<VV, T> )
        friend constexpr Even<TT> operator*( const Odd<TT>& lhs, const Odd<VV>& rhs )
        {
            return Even<TT> { -lhs.w * rhs.w + lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z,
                              -lhs.w * rhs.x - lhs.x * rhs.w - lhs.y * rhs.z + lhs.z * rhs.y,
                              -lhs.w * rhs.y - lhs.y * rhs.w - lhs.z * rhs.x + lhs.x * rhs.z,
                              -lhs.w * rhs.z - lhs.z * rhs.w - lhs.x * rhs.y + lhs.y * rhs.x };
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        constexpr Odd& operator*=( const Even<TT>& other )
        {
            const T newW { w * other.w - x * other.x - y * other.y - z * other.z };
            const T newX { w * other.x + x * other.w + y * other.z - z * other.y };
            const T newY { w * other.y + y * other.w + z * other.x - x * other.z };
            const T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
            w = newW;
            x = newX;
            y = newY;
            z = newZ;
            return *this;
        }

        template<typename TT>
            requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
        friend constexpr Odd operator*( Odd lhs, const Even<TT>& rhs )
        {
            lhs *= rhs;
            return lhs;
        }
    };

    template<typename T>
    template<typename TT>
        requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
    constexpr Odd<T>& Even<T>::operator*=( const Odd<TT>& other )
    {
        const T newW { w * other.w - x * other.x - y * other.y - z * other.z };
        const T newX { w * other.x + x * other.w + y * other.z - z * other.y };
        const T newY { w * other.y + y * other.w + z * other.x - x * other.z };
        const T newZ { w * other.z + z * other.w + x * other.y - y * other.x };
        w = newW;
        x = newX;
        y = newY;
        z = newZ;
        return *this;
    }

    template<typename T>
    template<typename TT>
        requires( std::is_arithmetic_v<TT> && std::is_convertible_v<TT, T> )
    constexpr Odd<T> Even<T>::operator*( Even lhs, const Odd<TT>& rhs )
    {
        lhs *= rhs;
        return lhs;
    }

    static constexpr auto e1 { Odd<int8_t> { 0, 1, 0, 0 } };
    static constexpr auto e2 { Odd<int8_t> { 0, 0, 1, 0 } };
    static constexpr auto e3 { Odd<int8_t> { 0, 0, 0, 1 } };
    static constexpr auto I3 { Odd<int8_t> { 1, 0, 0, 0 } };
    static constexpr auto basis { e1, e2, e3 };
}// namespace GA30
}// namespace FawnAlgebra::GeometricAlgebra
