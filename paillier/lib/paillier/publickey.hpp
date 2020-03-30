#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "ciphertext.hpp"
#include "number.hpp"

using std::istream;
using std::ostream;
using std::string;
using std::stringstream;

class SecretKey;

class PublicKey
{
    private:
        Number g, n, rN;
        friend class SecretKey;
        void init(const Number & g, const Number & n);

    public:
        Number n2;
        Ciphertext zero;

        PublicKey() { }
        PublicKey(istream & in);
        PublicKey(const Number & g, const Number & n) { init(g, n); }
        PublicKey(const PublicKey & pk) : PublicKey(pk.g, pk.n) { }
        // PublicKey(const PublicKey & pk) { init(pk.g, pk.n); }

        inline operator string() const { return string(g)+" "+string(n); }
        inline PublicKey & operator=(const PublicKey & pk) { init(pk.g, pk.n); }
        Ciphertext encrypt(const Number & m);
        Number getG() const { return g; }
        Number getN() const { return n; }
        Number getN2() const { return n2; }
        Number random();

        friend inline ostream & operator<<(ostream & os, const PublicKey & pk) { return os << string(pk); }
};

PublicKey::PublicKey(istream & in)
{
    string s;
    in >> s; g = s;
    in >> s; n = s;
    init(g, n);
}

void PublicKey::init(const Number & g, const Number & n)
{
    this->g = g;
    this->n = n;
    n2 = n*n;
    do { rN = Number::random(n2); }
    while ( Number::gcd(rN, n) != 1 );
    rN.pow(n, n2);
    zero = encrypt(0);
}

Ciphertext PublicKey::encrypt(const Number & m)
{
    random();
    auto gm = Number::pow(g, m, n2);
    auto c = gm*rN;
    c %= n2;
    return Ciphertext(c, n2, zero);
}

Number PublicKey::random()
{
    rN *= rN;
    rN %= n2;
    return Number(rN);
}
