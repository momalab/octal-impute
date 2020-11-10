#pragma once

#include <string>
#include "number.hpp"

using std::string;

class SecretKey;

class Ciphertext
{
    private:
        Number c;
        Number * n2;
        Ciphertext * zero;
        const static size_t DEFAULT_BASE = 62;
        friend class SecretKey;

    public:
        Ciphertext()=default;
        Ciphertext(const Ciphertext & a) : Ciphertext( a.c, *(a.n2), *(a.zero) ) { }
        Ciphertext(const Number & a, Number & mod, Ciphertext & zero) { c=a; n2 = &mod; this->zero = &zero; }
        Ciphertext(const string & a, Number & mod, Ciphertext & zero) { c=Number(a,DEFAULT_BASE); n2 = &mod; this->zero = &zero; }

        inline explicit operator string() const { return c.str(DEFAULT_BASE); }
        inline Ciphertext & operator+=(const Ciphertext & a);
        inline Ciphertext & operator-=(const Ciphertext & a);
        inline Ciphertext & operator*=(const Number & a);
        inline Ciphertext & invert() { c.invert(*n2); return *this; }

        inline Ciphertext operator+(const Ciphertext & a) const;
        inline Ciphertext operator-(const Ciphertext & a) const;
        inline Ciphertext operator*(const Number & a) const;
        static inline Ciphertext invert(const Ciphertext & a);

        friend inline Ciphertext operator*(uint64_t a1, const Ciphertext & a2) { return a2*a1; }
        friend inline ostream & operator<<(ostream & os, const Ciphertext & c) { return os << string(c); }
};

inline Ciphertext & Ciphertext::operator+=(const Ciphertext & a)
{
    c *= a.c;
    c %= (*n2);
    return *this;
}

inline Ciphertext & Ciphertext::operator-=(const Ciphertext & a)
{
    Ciphertext tmp(a);
    tmp.invert();
    *this += tmp;
    return *this;
}

inline Ciphertext & Ciphertext::operator*=(const Number & a)
{
    Ciphertext r = *zero;
    Number n = a, msb = 0;
    while (n)
    {
        msb += 1;
        n >>= 1;
    }
    while (msb)
    {
        msb -= 1;
        r += r;
        if ( a[msb] ) r += *this;
    }
    *this = r;
    return *this;
}

inline Ciphertext Ciphertext::operator+(const Ciphertext & a) const
{
    Ciphertext r(*this);
    r += a;
    return r;
}

inline Ciphertext Ciphertext::operator-(const Ciphertext & a) const
{
    Ciphertext r(*this);
    r -= a;
    return r;
}

inline Ciphertext Ciphertext::operator*(const Number & a) const
{
    Ciphertext r(*this);
    r *= a;
    return r;
}

inline Ciphertext Ciphertext::invert(const Ciphertext & a)
{
    Ciphertext r(a);
    r.invert();
    return r;
}
