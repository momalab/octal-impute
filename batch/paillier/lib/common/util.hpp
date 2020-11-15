#pragma once

namespace util
{ // begin of namespace

template <typename T>
T expt(T p, unsigned q)
{
    T r = 1;
    while (q)
    {
        if (q & 1)
        {
            q--;
            r *= p;
        }
        p *= p;
        q >>= 1;
    }
    return r;
}

template <class T> size_t sizeCeilPowerOfTwo( const T & a )
{
    auto n = a;
    size_t counter=0;
    while (n)
    {
        counter++;
        n>>=1;
    }
    return counter;
}

} // end of namespace
