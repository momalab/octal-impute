#pragma once

#include "definitions.hpp"
#include <vector>

namespace math
{
    using std::vector;

    const size_t bitsize = definitions::SZ; //decimals<<1;
    const size_t decimals = 4; //bitsize >> 1;
    const size_t limit = 4;
    // const uint64_t one = 1<<decimals;

    template <class T>
    inline T real(T n) { return n<<decimals; }

    template <class T>
    inline T real(float n) { return T(n*(1<<decimals)); }

    template <class T>
    inline float frac(T n) { return float(n)/(1<<decimals); }

    template <class T>
    inline T integer(T n) { return n>>decimals; }

    template <class T>
    inline bool index(const T & n, size_t i)
    {
        return (n >> i) & 1;
    }

    template <class T>
    inline T multiply(const T & a, const T & b)
    {
        return (a * b) >> decimals;
    }

    template <class T1, class T2>
    inline T1 multiply(const T1 & a, const T2 & b)
    {
        return (a * b) >> decimals;
    }

    template <class T>
    inline T divide(const T & a, const T & b)
    {
        return (a << decimals) / b;
    }

    // template <class T>
    // inline T factorial(const T & a)
    // {
    //     T f = one;
    //     for ( T i=integer(a); i>0; i-- ) f *= i;
    //     return f;
    // }

    template <class T>
    inline T exponentiate(const T & b, uint64_t e, const T & one)
    {
        vector<T> squares(1, b);
        for ( size_t idx=1; idx<bitsize; idx++ ) squares.push_back( multiply(squares.back(), squares.back()) );
        T result = one;
        for ( size_t idx=0; idx<squares.size(); idx++ ) result = multiply(result, index(e, idx) ? squares[idx] : one);
        return result;
    }

    template <class T>
    inline T taylor(const T & x, const vector<T> & fact)
    {
        T res = fact[0];
        for ( uint64_t e=1; e<limit; e++ ) res += divide( exponentiate(x, e, fact[0]), fact[e] );
        return res;
    }
}
