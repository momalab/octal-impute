#pragma once

#include <vector>

namespace math
{
    using std::vector;

    const size_t decimals = 1<<3;
    const size_t bitsize = decimals<<1;
    const size_t limit = 9;
    const uint64_t one = 1<<decimals;

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

    template <class T>
    inline T divide(const T & a, const T & b)
    {
        return (a << decimals) / b;
    }

    template <class T>
    inline T factorial(const T & a)
    {
        T f = one;
        for ( T i=integer(a); i>0; i-- ) f *= i;
        return f;
    }

    template <class T>
    inline T exponentiate(T b, uint64_t e)
    {
        vector<T> squares(1, b);
        for ( size_t idx=1; idx<bitsize; idx++ ) squares.push_back( multiply(squares.back(), squares.back()) );
        T result = one;
        for ( size_t idx=0; idx<squares.size(); idx++ ) result = multiply(result, index(e, idx) ? squares[idx] : one);
        if (e < 0) result = divide(one, result);
        return result;
    }

    template <class T>
    inline T taylor(const T & x)
    {
        T res = one;
        for ( uint64_t e=1; e<limit; e++ ) res += divide( exponentiate(x, e), factorial( real(e) ) );
        return res;
    }
}
