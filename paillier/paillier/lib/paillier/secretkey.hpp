#pragma once

#include <iostream>
#include <string>
#include "ciphertext.hpp"
#include "publickey.hpp"
#include "number.hpp"

using std::istream;
using std::string;

class SecretKey
{
    private:
        Number l, u;
        PublicKey pubkey;

        static Number L(const Number & x, const Number & n) { return (x-1) / n; };

    public:
        SecretKey() { }
        SecretKey(istream & in);
        SecretKey(const Number & bitsize);
        SecretKey(const Number & p, const Number & q);
        SecretKey(const Number & l, const Number & u, const PublicKey & pk);
        SecretKey(const SecretKey & sk) : SecretKey(sk.l, sk.u, sk.pubkey) { }

        inline operator string() const { return string(l)+" "+string(u)+" "+string(pubkey); }
        Number decrypt(const Ciphertext & a) const;
        bool generateKeys(const Number & bitsize);
        bool generateKeys(const Number & p, const Number & q);
        Number getL() const { return l; }
        PublicKey getPublicKey() const { return pubkey; }
        Number getU() const { return u; }

        friend inline ostream & operator<<(ostream & os, const SecretKey & sk) { return os << string(sk); }
};


SecretKey::SecretKey(istream & in)
{
    string s;
    in >> s; l = Number(s);
    in >> s; u = Number(s);
    in >> s; Number g(s);
    in >> s; Number n(s);
    pubkey = PublicKey(g, n);
}

SecretKey::SecretKey(const Number & bitsize)
{
    if ( !generateKeys(bitsize) ) throw "Invalid primes";
}

SecretKey::SecretKey(const Number & p, const Number & q)
{
    if ( !generateKeys(p, q) ) throw "Invalid primes";
}

Number SecretKey::decrypt(const Ciphertext & a) const
{
    auto m = Number::pow(a.c, l, pubkey.n2);
    m = L(m, pubkey.n);
    m *= u;
    m %= pubkey.n;
    return m;
}

bool SecretKey::generateKeys(const Number & bitsize)
{
    auto primesize = bitsize >> 1;
    Number p, q;
    do
    {
        p = Number::prime(primesize);
        q = Number::prime(primesize);
    } while ( Number::gcd( p*q, (p-1)*(q-1) ) != 1 );
    return generateKeys(p, q);
}

bool SecretKey::generateKeys(const Number & p, const Number & q)
{
    auto n = p * q;
    auto pm1 = p-1;
    auto qm1 = q-1;
    auto gcd = Number::gcd( n, pm1*qm1 );
    if ( gcd != 1 ) return false;
    l = Number::lcm(pm1, qm1);
    auto n2 = n*n;
    Number g = 1;
    do
    {
        // g = Number::random(n2);
        g += 1;
        u = Number::pow(g, l, n2);
        u = L(u, n);
    } while ( Number::gcd(u, n) != 1 );
    u.invert(n);
    pubkey = PublicKey(g, n);
    return true;
}
