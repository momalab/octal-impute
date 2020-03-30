#pragma once

#include <iostream>
#include <gmp.h>
#include <string>

using std::ostream;
using std::string;
#include <iostream>
class Number
{
    private:
        mpz_t n;
        static gmp_randstate_t state;
        static bool initialized;
        static size_t DEFAULT_BASE;

        inline void init() { if (!initialized) { initialized=true; gmp_randinit_default(state); } }

    public:
        Number() { init(); mpz_init(n); }
        Number(const mpz_t & a) { init(); mpz_init_set(n, a); }
        Number(const Number & a) : Number(a.n) { }
        Number(const string & s, size_t b=DEFAULT_BASE) { init(); mpz_init_set_str(n, s.c_str(), b); }
        Number(uint64_t a) { init(); mpz_init_set_ui(n, a); }
        ~Number() { mpz_clear(n); }

        inline explicit operator bool() const { return bool(unsigned(*this)); }
        inline explicit operator int() const { return unsigned(*this); }
        inline explicit operator unsigned() const { return mpz_get_ui(n); }
        inline explicit operator string() const { return str(); }
        inline Number & operator=(const Number & a) { mpz_set(n, a.n); }
        inline Number & operator=(const mpz_t & a) { mpz_set(n, a); }
        inline Number & operator=(const string & a) { mpz_set_str(n, a.c_str(), DEFAULT_BASE); }
        inline Number & operator=(uint64_t a) { mpz_set_ui(n, a); }
        inline Number & operator+=(const Number & a) { mpz_add(n, n, a.n); }
        inline Number & operator+=(uint64_t a) { mpz_add_ui(n, n, a); }
        inline Number & operator-=(const Number & a) { mpz_sub(n, n, a.n); }
        inline Number & operator-=(uint64_t a) { mpz_sub_ui(n, n, a); }
        inline Number & operator*=(const Number & a) { mpz_mul(n, n, a.n); }
        inline Number & operator*=(uint64_t a) { mpz_mul_ui(n, n, a); }
        inline Number & operator/=(const Number & a) { mpz_tdiv_q(n, n, a.n); }
        inline Number & operator/=(uint64_t a) { mpz_tdiv_q_ui(n, n, a); }
        inline Number & operator%=(const Number & a) { mpz_mod(n, n, a.n); }
        inline Number & operator%=(uint64_t a) { mpz_mod_ui(n, n, a); }
        inline Number & operator<<=(const Number & a) { *this <<= unsigned(a); }
        inline Number & operator<<=(uint64_t a) { mpz_mul_2exp(n, n, a); }
        inline Number & operator>>=(const Number & a) { *this >>= unsigned(a); }
        inline Number & operator>>=(uint64_t a) { mpz_tdiv_q_2exp(n, n, a); }
        inline Number & operator&=(const Number & a) { mpz_and(n, n, a.n); }
        inline Number & operator|=(const Number & a) { mpz_ior(n, n, a.n); }
        inline Number & operator^=(const Number & a) { mpz_xor(n, n, a.n); }
        inline Number & gcd(const Number & a) { mpz_gcd(n, n, a.n); }
        inline Number & invert(const Number & a) { mpz_invert(n, n, a.n); }
        inline Number & lcm(const Number & a) { mpz_lcm(n, n, a.n); }
        inline Number & pow(const Number & e, const Number & m) { mpz_powm(n, n, e.n, m.n); }
        inline Number & pow(uint64_t e, const Number & m) { mpz_powm_ui(n, n, e, m.n); }
        inline Number & prime() { randombits(); mpz_nextprime(n, n); }
        inline Number & random() { mpz_urandomm(n, state, n); }
        inline Number & randombits() { mpz_urandomb(n, state, unsigned(*this)); }

        template <class T> inline Number operator+(const T & a) const;
        template <class T> inline Number operator-(const T & a) const;
        template <class T> inline Number operator*(const T & a) const;
        template <class T> inline Number operator/(const T & a) const;
        template <class T> inline Number operator%(const T & a) const;
        template <class T> inline Number operator<<(const T & a) const;
        template <class T> inline Number operator>>(const T & a) const;
        template <class T> inline Number operator&(const T & a) const;
        template <class T> inline Number operator|(const T & a) const;
        template <class T> inline Number operator^(const T & a) const;
        template <class T> inline Number operator[](const T & a) const { return mpz_tstbit(n, unsigned(a)); };
        inline bool operator==(const Number & a) const { return mpz_cmp(n, a.n)==0; }
        inline bool operator!=(const Number & a) const { return mpz_cmp(n, a.n)!=0; }
        inline bool operator>=(const Number & a) const { return mpz_cmp(n, a.n)>=0; }
        inline bool operator> (const Number & a) const { return mpz_cmp(n, a.n)> 0; }
        inline bool operator<=(const Number & a) const { return mpz_cmp(n, a.n)<=0; }
        inline bool operator< (const Number & a) const { return mpz_cmp(n, a.n)< 0; }
        inline bool operator==(uint64_t a) const { return mpz_cmp_ui(n, a)==0; }
        inline bool operator!=(uint64_t a) const { return mpz_cmp_ui(n, a)!=0; }
        inline bool operator>=(uint64_t a) const { return mpz_cmp_ui(n, a)>=0; }
        inline bool operator> (uint64_t a) const { return mpz_cmp_ui(n, a)> 0; }
        inline bool operator<=(uint64_t a) const { return mpz_cmp_ui(n, a)<=0; }
        inline bool operator< (uint64_t a) const { return mpz_cmp_ui(n, a)< 0; }

        static inline Number gcd(const Number & a1, const Number & a2);
        static inline Number invert(const Number & a1, const Number & a2);
        static inline Number lcm(const Number & a1, const Number & a2);
        static inline Number pow(const Number & b, const Number & e, const Number & m);
        static inline Number prime(const Number & bitsize);
        static inline Number random(const Number & mod);
        static inline Number randombits(const Number & bitsize);

        template <class T> friend inline Number operator&(const T & a1, const Number & a2) { return a2&a1; }
        // inline size_t size() { return mpz_size(n); }
        inline string str(size_t b=DEFAULT_BASE) const { return mpz_get_str(NULL, b, n); }
        friend inline ostream & operator<<(ostream & os, const Number & a) { return os << a.str(); }
};

bool Number::initialized = false;
gmp_randstate_t Number::state;
size_t Number::DEFAULT_BASE = 10;

template <class T>
inline Number Number::operator+(const T & a) const
{
    Number r(n);
    r += a;
    return r;
}

template <class T>
inline Number Number::operator-(const T & a) const
{
    Number r(n);
    r -= a;
    return r;
}

template <class T>
inline Number Number::operator*(const T & a) const
{
    Number r(n);
    r *= a;
    return r;
}

template <class T>
inline Number Number::operator/(const T & a) const
{
    Number r(n);
    r /= a;
    return r;
}

template <class T>
inline Number Number::operator%(const T & a) const
{
    Number r(n);
    r %= a;
    return r;
}

template <class T>
inline Number Number::operator<<(const T & a) const
{
    Number r(n);
    r <<= a;
    return r;
}

template <class T>
inline Number Number::operator>>(const T & a) const
{
    Number r(n);
    r >>= a;
    return r;
}

template <class T>
inline Number Number::operator&(const T & a) const
{
    Number r(a);
    r &= *this;
    return r;
}

template <class T>
inline Number Number::operator|(const T & a) const
{
    Number r(a);
    r |= *this;
    return r;
}

template <class T>
inline Number Number::operator^(const T & a) const
{
    Number r(a);
    r ^= *this;
    return r;
}

inline Number Number::gcd(const Number & a1, const Number & a2)
{
    Number r(a1);
    r.gcd(a2);
    return r;
}

inline Number Number::invert(const Number &a1, const Number & a2)
{
    Number r(a1);
    r.invert(a2);
    return r;
}

inline Number Number::lcm(const Number & a1, const Number & a2)
{
    Number r(a1);
    r.lcm(a2);
    return r;
}

inline Number Number::pow(const Number & b, const Number & e, const Number & m)
{
    Number r(b);
    r.pow(e, m);
    return r;
}

inline Number Number::prime(const Number & bitsize)
{
    Number r(bitsize);
    r.prime();
    return r;
}

inline Number Number::random(const Number & mod)
{
    Number r(mod);
    r.random();
    return r;
}

inline Number Number::randombits(const Number & bitsize)
{
    Number r(bitsize);
    r.randombits();
    return r;
}
